
# Ray Tracer

## Overview
This project implements a basic ray tracer that reads a scene description from a text file, performs ray-sphere intersection calculations, and outputs the rendered image as a PPM file. The program uses OpenMP for parallel processing to improve performance.

## Features
- Reads a scene description from a text file.
- Performs ray tracing to determine pixel colors.
- Supports multiple spheres with individual colors.
- Uses OpenMP for parallel processing.
- Outputs an image in PPM format.

## File Structure and Function Implementation Details
```
├── main.c          # Entry point of the program
├── scene.h         # Scene data structures and function declarations
├── scene.c         # Scene parsing and ray tracing logic
├── ppm.h           # PPM image handling function declarations
├── ppm.c           # PPM image writing implementation
├── makefile        # Compilation and execution instructions
└── scene.txt       # Sample scene description file
```

### `scene.c`
- `retrive_scene()`: Reads and parses the scene file.
- `evaluate_image_color()`: Iterates over pixels and determines colors using ray tracing.
- `distance_to_sphere()`: Computes ray-sphere intersections.

### `ppm.c`
- `write_ppm()`: Writes pixel data to a `.ppm` image file using memory-mapped I/O.


## Usage

### Prerequisites
Ensure you have gcc installed with `OpenMP` support.

### Compile
To compile the project:
```sh
make
```

### Run
To execute the ray tracer:
```sh
make run
```
This will generate an image file (`image.ppm`) from `scene.txt`.

Alternatively, run manually:
```sh
./main scene.txt output.ppm 1920 1080
```

## Scene File Format
The scene description file (`scene.txt`) follows this format:
```
VP x y z            # Viewport size and distance
BG r g b            # Background color
OBJ_N n             # Number of spheres
S x y z r r g b     # Sphere (position, radius, color)
```
Example:
```
VP 1 1 1
BG 255 255 255
OBJ_N 2
S 0 0 -5 1 255 0 0
S 1 1 -3 0.5 0 255 0
```


## Performance Optimization
- OpenMP parallelization in `evaluate_image_color()` improves rendering speed.
- Memory-mapped file writing (`mmap`) optimizes PPM file output.
- Padding removing when allocating the pixels using `__attribute__((packed))` .

## Cleanup
To remove generated files, run:
```sh
make clean
```
