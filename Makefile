# parameters
CC = gcc
CFLAGS = -O3 -fopenmp
LIB = lib

INPUT = scene.txt
OUTPUT = image.ppm
WIDTH = 1920
HEIGHT = 1080

all: main

main: main.o ppm.o scene.o
	@${CC} ${CFLAGS} -o main main.o ppm.o scene.o -lm

%.o: %.c %.h
	@${CC} ${CFLAGS} -c $<


.PHONY: clean 
clean:
	@rm *.o
	@rm *.ppm
	@rm main


.PHONY: run
run: main
	@./main ${INPUT} ${OUTPUT} ${WIDTH} ${HEIGHT}
