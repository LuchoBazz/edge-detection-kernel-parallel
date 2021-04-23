#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#include "sod/sod.h"

char* IMG_INPUT;
char* IMG_OUTPUT;
int ARG;
int THREADS;

const int MAX_THREADS = 512;

float kernel[3][3] = {{-1,-1,-1}, 
                      {-1, 8,-1},
                      {-1,-1,-1}};

int range[MAX_THREADS][2]; // The i-th thread goes from range range[i][0] to range[i][1]
 
sod_img imgIn;
sod_img imgOut;

void * filter(void * arg) {
    // Id of the Thread
    int threadId = *(int*) arg;
    // Get range, from-to
    int from = range[threadId][0];
    int to = range[threadId][1];

    for(int y = from; y <= to; ++y) {
        for(int x = 1; x < imgIn.w-1; ++x) {

            float sum = 0.0;
            // Iterate the matrix 3x3 kernel
            for(int ky = -1; ky <= 1; ++ky){
                for(int kx = -1; kx <= 1; ++kx){
                    // Get pixel (Red) at coordinate (x + kx, y + ky)
                    float val = sod_img_get_pixel(imgIn, x+kx, y+ky, 0); // R
                    sum += kernel[ky+1][kx+1] * val;

                    //  kernel       Image pixels, px=Pixel

                    // [k1 k2 k3]    [px1 px2 px3]
                    // [k4 k5 k6]    [px4 px5 px6]
                    // [k7 k8 k9]    [px7 px8 px9]

                    // sum = k1*px1 + k2*px2 + ... + k9*px9
                }
            }

            // Change pixel (Red) of coordinate (x, y)
            sod_img_set_pixel(imgOut, x, y, 0, abs(sum)); // R

            // Change pixel (Green) of coordinate (x, y)
            sod_img_set_pixel(imgOut, x, y, 1, abs(sum)); // G

            // Change pixel (Blue) of coordinate (x, y)
            sod_img_set_pixel(imgOut, x, y, 2, abs(sum)); // B
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc < 5) {
        printf("You must provide 4 arguments: [input image] [output image] [filter argument] [number of threads]");
        // Sample: ./filtro.o input.png output.png 8 16
        exit(0);
    }
    // Input image path: Sample: img/input1.png img/input1.png
    IMG_INPUT = argv[1];

    // Output image path: Sample: img/output1.png
    IMG_OUTPUT = argv[2];

    // Filter argument: Ej 8
    ARG = atof(argv[3]);
    kernel[1][1] = ARG;

    // Number of threads used
    THREADS = atoi(argv[4]);

    // Load image into dynamic memory
    imgIn = sod_img_load_from_file(IMG_INPUT, SOD_IMG_COLOR);
    imgOut = sod_img_load_from_file(IMG_INPUT, SOD_IMG_COLOR);

    if (imgIn.data == 0) {
        // Verify that there is an image
        printf("No pudo cargar la imagen %s\n", IMG_INPUT);
        return 0;
    }

    // Define ranges for the number of threads
    int factor = imgIn.h / THREADS;
    int last = 1;
    for(int i = 0; i < THREADS; ++i) {
        range[i][0] = last;
        if(i != (THREADS-1)) {
            range[i][1] = last + factor-1;
        } else {
            range[i][1] = imgIn.h - 1;
        }
        last = range[i][1] + 1;
    }

    // Create Threads
    int threadId[THREADS];
    pthread_t thread[THREADS];

    // Define variables to measure execution time
    struct timeval tval_before, tval_after, tval_result;
    gettimeofday(&tval_before, NULL);
    
    // Execute Threads
    for(int i = 0; i < THREADS; i++){
        threadId[i] = i;
        pthread_create(&thread[i], NULL, (void *)filter, &threadId[i]);
    }

    // Join Threads
    int *retval;
    for(int i = 0; i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }

    // Measure the time
    gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);
    
    // Save Image
    sod_img_save_as_png(imgOut, IMG_OUTPUT);

    // Free the Memory
    sod_free_image(imgIn);
    sod_free_image(imgOut);

    // Show execution time
    printf("Time elapsed: %ld.%06ld\n", (long int) tval_result.tv_sec, (long int) tval_result.tv_usec);

    return 0;
}