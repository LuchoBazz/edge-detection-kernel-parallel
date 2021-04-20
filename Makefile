COMPILER = gcc

CFLAGS= -lm -Ofast -march=native -std=c11


app: clean sod.o

sod.o:
	$(COMPILER) -c sod/sod.c -o sod.o

clean: 
	$(RM) *.o