#include "gd.h"

#include <stdio.h>

int main() {
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
    return 0;
}