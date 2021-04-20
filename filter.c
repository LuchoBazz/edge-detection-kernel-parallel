#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sod/sod.h"

char* IMG_INPUT;
char* IMG_OUTPUT;
float ARG;
int THREADS;

int main(int argc, char *argv[]) {

    if(argc < 5) {
        printf("You must provide 4 arguments: [input image] [output image] [filter argument] [number of threads]");
        // Sample: ./filtro.o input.png output.png 8 16
        exit(0);
    }
    IMG_INPUT = argv[1];
    IMG_OUTPUT = argv[2];
    ARG = atof(argv[3]);
    THREADS = atoi(argv[4]);
    printf("%s %s %i %i\n", IMG_INPUT, IMG_OUTPUT, ARG, THREADS);

    // TODO:

    sod_img imgIn = sod_img_load_from_file(IMG_INPUT, SOD_IMG_COLOR);

    if (imgIn.data == 0) {
        // Validate that the image exists
        printf("Could not load the image %s\n", IMG_INPUT);
        return 0;
    }

    // Modify the image
    const int TAM_COLORES = 9;
    float colores[9];

    printf("Image size %dx%d\n", imgIn.w, imgIn.h);

    for(int i = 0; i < imgIn.w/2; ++i) {
        for(int j = 0; j < imgIn.h; ++j) {
            float R = sod_img_get_pixel(imgIn, i, j, 0);
            float G = sod_img_get_pixel(imgIn, i, j, 1);
            float B = sod_img_get_pixel(imgIn, i, j, 2);

            float mean = (R+G+B) / 3.0;
            sod_img_set_pixel(imgIn, i, j, 0, mean);
            sod_img_set_pixel(imgIn, i, j, 1, mean);
            sod_img_set_pixel(imgIn, i, j, 2, mean);
        }
    }

    sod_img_save_as_png(imgIn, IMG_OUTPUT);

    // Free the Memory
    sod_free_image(imgIn);

    return 0;
}