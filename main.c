// Matteo Vicenzino SM3201397

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "scene.h"
#include "ppm.h"

int main( int argc, char *argv[] ) {

    srand(time(NULL));
    clock_t start = clock();
    
    // save input parameters in variables
    const char *scene_path = argv[1];
    const char *output_file = argv[2];
    int width = atoi(argv[3]); // convert from string to integer
    int height = atoi(argv[4]);
    
    // retrive the scene from the text file
    Scene scene = retrive_scene(scene_path);
    
    // allocate pixel's color array
    Color *image = malloc(sizeof(Color) * width * height);
    if (!image) {
        perror("Error allocating pixel matrix");
        return EXIT_FAILURE;
    }

    // evaluate the pixel color of the image
    evaluate_image_color(&scene, image, width, height);
    
    // write the image to a PPM file
    bool complete = write_ppm(output_file, image, width, height);

    clock_t end = clock();

    if (complete == true) {
        printf("Image written successfully in %f seconds\n", (double)(end - start) / CLOCKS_PER_SEC);
    } else {
        perror("Error writing the image into the ppm file");
        return EXIT_FAILURE;
    }

    // free memory
    free(image);
    free(scene.spheres);
    
    return 0;
}
