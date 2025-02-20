// Matteo Vicenzino SM3201397

#ifndef _SCENE_H
#define _SCENE_H

#include <stdint.h>

// struct to store a 3D coordinate (or a 3D vector)
typedef struct {
    float x;
    float y;
    float z;
} Coordinate;

// struct to store a pixel color
typedef struct __attribute__((packed)) {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

// struct to store data of a sphere
typedef struct {
    Coordinate center;
    float radius;
    Color color;
} Sphere;

// struct to store the scene data
typedef struct {
    Coordinate viewport;
    Color background;
    int n_obj;
    Sphere *spheres;
} Scene;

/* function retrive_scene
takes in input the path of the scene.txt file
the function creates and return a scene struct with the information of the scene.txt file
*/
Scene retrive_scene(const char *scene_path);

/* function evaluate_image_color
takes in input:
    - scene:    scene structur created in the retrive_scene function
    - image:    the pixel's color array that will be filled with the color of the image
    - width:    the width of the image
    - height:   the height of the image

the function iterates over the image pixel array using OpenMP,
and for each pixel calculates the color of the closest intercepting sphere
*/
void evaluate_image_color(const Scene *scene, Color *image, int width, int height);


#endif