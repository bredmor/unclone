# unclone
This is an attempt to start an image comparison tool to detect duplicates.

My first thought was to convert images to a small binary hash with a discrete cosine transform and then compute the hamming distance.

Forcing a square aspect ratio prevents distortion tricking the algorithm and gives us a square array every time.

The `topPixels` bit in `imageHash()` is an attempt to deal with cropping/padding.