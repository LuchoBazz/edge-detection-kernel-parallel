#include <stdio.h>
#include <stdlib.h>

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

    return 0;
}