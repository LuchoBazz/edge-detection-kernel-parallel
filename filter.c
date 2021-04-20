#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sod/sod.h"

char* IMG_INPUT;
char* IMG_OUTPUT;
float ARG;
int THREADS;

float kernel[3][3] = {{-1,-1,-1}, 
                      {-1, 8,-1},
                      {-1,-1,-1}};

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
    sod_img imgOut = sod_img_load_from_file(IMG_INPUT, SOD_IMG_COLOR);

    if (imgIn.data == 0) {
        // Validate that the image exists
        printf("Could not load the image %s\n", IMG_INPUT);
        return 0;
    }

    // Modificar la imagen
    const int TAM_COLORES = 9;
    float colores[9];

    printf("Image size %dx%d\n", imgIn.w, imgIn.h);

    for(int y = 1; y < imgIn.h-1; ++y) {
        for(int x = 1; x < imgIn.w-1; ++x) {
            float sum = 0.0;
            for(int ky = -1; ky <= 1; ++ky){
                for(int kx = -1; kx <= 1; ++kx){
                    float val = sod_img_get_pixel(imgIn, x+kx, y+ky, 0);
                    sum += kernel[ky+1][kx+1] * val;
                }
            }
            sod_img_set_pixel(imgOut, x, y, 0, abs(sum));            
            sod_img_set_pixel(imgOut, x, y, 1, abs(sum));            
            sod_img_set_pixel(imgOut, x, y, 2, abs(sum));            
        }
    }
    // Save image ouput
    sod_img_save_as_png(imgIn, IMG_OUTPUT);

    // Free the Memory
    sod_free_image(imgIn);
    sod_free_image(imgOut);

    return 0;
}