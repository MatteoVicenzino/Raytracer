#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <sys/mman.h>
#include <omp.h>
#include <stdbool.h>
#include "scene.h"


Scene retrive_scene(const char *scene_path) {

    Scene scene; // craete new scene struct

    // open file
    FILE *file = fopen(scene_path, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    // read viewport, object number and background color using fscanf
    int read_VP = fscanf(file, "VP %f %f %f\n", &scene.viewport.x, &scene.viewport.y, &scene.viewport.z);
    int read_BG = fscanf(file, "BG %hhu %hhu %hhu\n", &scene.background.r, &scene.background.g, &scene.background.b);
    int read_OBJ_N = fscanf(file, "OBJ_N %d\n", &scene.n_obj);

    // check correct formatting
    if (read_VP != 3 || read_BG != 3 || read_OBJ_N != 1 || scene.n_obj < 0) {
        perror("Error in the file formatting");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // allocate memory for the spheres
    scene.spheres = malloc(scene.n_obj * sizeof(Sphere));
    if (!scene.spheres) {
        perror("Error allocating memory");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // read the spheres
    for (int i = 0; i < scene.n_obj; i++) {
        fscanf(file, "S %f %f %f %f %hhu %hhu %hhu\n",
            &scene.spheres[i].center.x, &scene.spheres[i].center.y, &scene.spheres[i].center.z,
            &scene.spheres[i].radius,
            &scene.spheres[i].color.r, &scene.spheres[i].color.g, &scene.spheres[i].color.b);
    }

    fclose(file);
    return scene;
}


Coordinate normalize(Coordinate v) {
    float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return (Coordinate){v.x / norm, v.y / norm, v.z / norm};
}

/* function distance_to_sphere takes in input:
    - camera:       coordinates of the camera position
    - directionV:    the direction of the pixel's vector
    - sphere:       the k_th sphere struct
and return the min distance of the intercepting vector with that sphere
if sphere is not intercepting return large number
*/
float distance_to_sphere(Coordinate camera, Coordinate directionV, Sphere *sphere) {

    Coordinate C_s = (Coordinate){camera.x - sphere->center.x, camera.y - sphere->center.y, camera.z - sphere->center.z};

    float a = directionV.x * directionV.x + directionV.y * directionV.y + directionV.z * directionV.z;
    float b = 2.0f * (C_s.x * directionV.x + C_s.y * directionV.y + C_s.z * directionV.z);
    float c = (C_s.x * C_s.x + C_s.y * C_s.y + C_s.z * C_s.z) - sphere->radius * sphere->radius;

    if((b*b) -4.0f *a*c < 0){ // doesn't intecept => return large number
        return INFINITY;
    }
    else { // intercepts
        float x_1 = (-b + sqrt((b*b) - 4.0f *a*c)) / (2.0f * a);
        float x_2 = (-b - sqrt((b*b) - 4.0f *a*c)) / (2.0f * a);
        return fmin(abs(x_1), abs(x_2)); // return the min distance
    }
}


void evaluate_image_color(const Scene *scene, Color *image, int width, int height) {

    Coordinate camera = {0, 0, 0}; // assume camera position is always the origin
    float viewport_width = scene->viewport.x; //save viewport dimensions to calculate the pixel direction of image
    float viewport_height = scene->viewport.y;
    float viewport_depth = scene->viewport.z;

    #pragma omp parallel for collapse(2)
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            float pixel_i_position = ((viewport_width / (width - 1)) * i) - (viewport_width / 2);
            float pixel_j_position = ((viewport_height / (height - 1)) * j) - (viewport_height / 2);
            Coordinate directionV = {pixel_i_position, pixel_j_position, -viewport_depth}; // -viewport_depth because the camera is at the origin
            directionV = normalize(directionV);

            // initialize pixel with background color
            Color pixel = {scene->background.r,
                scene->background.g, 
                scene->background.b};

            // check for sphere intersections:
            float min_distance = INFINITY; // set intercept distance to large number

            for (int k = 0; k < scene->n_obj; k++) {
                Sphere *k_sphere = &scene->spheres[k]; // k-th sphere
                float distance = distance_to_sphere(camera, directionV, k_sphere);
                if (distance < min_distance) {
                    min_distance = distance;
                    pixel = k_sphere->color;
                }
            }

            // save pixel color to image
            image[j * width + (width - 1 - i)] = pixel;
        }
    }
}
