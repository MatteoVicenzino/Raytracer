#ifndef _PPM_H 
#define _PPM_H

#include "scene.h"
#include <stdbool.h>

/* function write_ppm
takes in input:
    - filename:     output file name
    - image:        the object image containing the pixels
    - width:        the width of the image
    - height:       the height of the image

the function writes the pixel into the ppm file with the given name, 
then returns true if the operation is successful, false otherwise.
*/

bool write_ppm(const char *filename, Color *image, int width, int height);

#endif
