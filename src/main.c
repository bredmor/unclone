#include "gd.h"
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

/* Enable Debug Messages */
const bool DEBUG = true;
/* Fidelity of comparisons (smaller is cheaper but more likely to clash) */
const int TRANSFORMED_SIZE = 64;

gdImagePtr readFileAsImage(char * path);
gdImagePtr resizeImage(gdImagePtr image, int w, int h);
double * oneDimensionalDCT(int row[]);
int * imageHash(gdImagePtr image);
double getMeanColorFromPixelSet(const double * pixels);
int hammingDistance(int one[], int two[]);

        int main (int argc, char *argv[])
{
    /* Allocate vars */
    gdImagePtr sourceImage;
    gdImagePtr compImage;
    int *compHash;
    int *sourceHash;

    if(argc < 3) {
        fprintf(stderr, "Error: Not enough arguments.\n");
        return 0;
    }

    /* Source file to be compared against */
    char *sourceFilePath = argv[1];

    /* New file to compare against the source file for a match */
    char *compFilePath = argv[2];

    if(DEBUG) {
        fprintf(stdout, "Source File Path: %s\n", sourceFilePath);
        fprintf(stdout, "Comparison File Path: %s\n", compFilePath);
    }

    sourceImage = gdImageCreateFromFile(sourceFilePath);
    compImage = gdImageCreateFromFile(compFilePath);

    /* Handle invalid file paths */
    if(sourceImage == NULL) {
        fprintf(stderr, "Could not load source image: %s\n", sourceFilePath);
        return -1;
    }
    if(compImage == NULL) {
        fprintf(stderr, "Could not load comparison image: %s\n", compFilePath);
        return -1;
    }

    /* Resize images to 64x64 */
    sourceImage = resizeImage(sourceImage, TRANSFORMED_SIZE, TRANSFORMED_SIZE);
    compImage = resizeImage(compImage, TRANSFORMED_SIZE, TRANSFORMED_SIZE);

    /* Convert images to greyscale */
    if(!gdImageGrayScale(sourceImage)) {
        fprintf(stderr, "Could not convert source to greyscale.\n");
    }
    if(!gdImageGrayScale(compImage)) {
        fprintf(stderr, "Could not convert comparison to greyscale.\n");
    }

    compHash = imageHash(compImage);
    sourceHash = imageHash(sourceImage);

    int distance = hammingDistance(compHash, sourceHash);

    fprintf(stdout, "%d", distance);

    /* Cleanup */
    gdImageDestroy(sourceImage);
    gdImageDestroy(compImage);
}

int * imageHash(gdImagePtr image)
{
    double *map[TRANSFORMED_SIZE];
    double *rows[TRANSFORMED_SIZE];

    for (int y = 0; y < TRANSFORMED_SIZE; y++) {
        int row[TRANSFORMED_SIZE];
        for (int x = 0; x < TRANSFORMED_SIZE; x++) {
            int rgb = gdImageGetPixel(image, x, y);
            row[x] = rgb;
        }
        rows[y] = oneDimensionalDCT(row);
    }

    for (int x = 0; x < TRANSFORMED_SIZE; x++) {
        double col[TRANSFORMED_SIZE];
        for (int y = 0; y < TRANSFORMED_SIZE; y++) {
            col[y] = rows[y][x];
        }
        map[x] = oneDimensionalDCT(col);
    }

    double topPixels[TRANSFORMED_SIZE];
    int pixelCount = 0;
    for (int y = 0; y < sqrt(TRANSFORMED_SIZE); y++) {
        for (int x = 0; x < sqrt(TRANSFORMED_SIZE); x++) {
            topPixels[pixelCount] = *map[y][x];
        }
    }

    double mean = getMeanColorFromPixelSet(topPixels);

    int bits[TRANSFORMED_SIZE];
    for(int p = 0; p < TRANSFORMED_SIZE; p++) {
        bits[p] = (topPixels[p] > mean);
    }

    return bits;
}

int hammingDistance(int one[], int two[])
{
    int i,length,count = 0;

    for(i=0; i<length; i++)
    {
        if(one[i] != two[i])
        {
            count++;
        }
    }

    return count;
}

double getMeanColorFromPixelSet(const double * pixels)
{
    int n = TRANSFORMED_SIZE - 1;
    double sum = 0;
    for(int i = 1; i < n; i++) {
        sum += pixels[i];
    }
    return sum/n;
}

double * oneDimensionalDCT(int row[])
{
    double dct[TRANSFORMED_SIZE];

    for(int i = 0; i < TRANSFORMED_SIZE; i++) {
        double sum = 0;

        for(int j = 0; j < TRANSFORMED_SIZE; j++) {
            sum += row[j] * cos(i * M_PI * (j + 0.5) / TRANSFORMED_SIZE);
        }

        sum *= sqrt(2 / TRANSFORMED_SIZE);
        if(i == 0) {
            sum *= 1 / sqrt(2);
        }
        dct[i] = sum;
    }

    return dct;
}

gdImagePtr resizeImage(gdImagePtr image, int w, int h)
{
    gdImagePtr output;

    /* Don't allow resize to 0 */
    if (w<=0 || h<=0) {
        return NULL;
    }

    /* Force square */
    if(w != h) {
        return NULL;
    }

    gdImageSetInterpolationMethod(image, GD_BILINEAR_FIXED);
    return gdImageScale(image, w, h);
}

bool fileExists(char * path)
{
    FILE *file;
    file = fopen(path, "r");
    if (file){
        fclose(file);
        return true;
    }
    return false;
}

void test() {
    /* Image Handle*/
    gdImagePtr image;
    /* Output files */
    FILE *pngout;
    /* Color indexes */
    int black, white;

    /* Allocate image memory */
    image = gdImageCreate(64, 64);

    /* Allocate colors */
    black = gdImageColorAllocate(image, 0, 0 ,0);
    white = gdImageColorAllocate(image, 255, 255, 255);

    /* Draw a line from the upper left to the lower right,
    using white color index. */
    gdImageLine(image, 0, 0, 63, 63, white);

    /* Open file for writing */
    pngout = fopen("test.png", "wb");

    /* Output file */
    gdImagePng(image, pngout);

    /* Close file handle */
    fclose(pngout);

    /* Free Memory */
    gdImageDestroy(image);

    printf("Image created!\n");
    return;
}