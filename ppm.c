#include "ppm.h"
#include "scene.h"
#include <stdio.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

bool write_ppm(const char *filename, Color *image, int width, int height) {

    // open file
    FILE *fd = fopen(filename, "w+");
    if (fd == NULL) {
        perror("Error opening file");
        return false;
    }
    int fd_num = fileno(fd);
    if (fd_num == -1) {
        perror("Error getting file descriptor");
        fclose(fd);
        return false;
    }

    // calculate content size and resize file
    size_t header_size = snprintf(NULL, 0, "P6\n%d %d\n255\n", width, height);
    size_t total_size = header_size + width * height * sizeof(Color);
    if (ftruncate(fd_num, total_size) == -1) {
        perror("Error ridimensioning file");
        fclose(fd);
        return false;
    }

    // map file to memory
    char *data = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_num, 0);
    if (data == MAP_FAILED) {
        perror("mmap failed");
        fclose(fd);
        return false;
    }

    // write header and pixels, then sync
    snprintf(data, header_size + 1, "P6\n%d %d\n255\n", width, height);
    memcpy(data + header_size, image, width * height * sizeof(Color));

    if (msync(data, total_size, MS_SYNC) < 0) {
        perror("msync failed");
        munmap(data, total_size);
        return false;
    }

    // check if everything was written
    fseek(fd, 0, SEEK_END);
    long file_size = ftell(fd);
    if (file_size != (long)total_size) {
        fprintf(stderr, "Error: PPM file size mismatch");
        munmap(data, total_size);
        fclose(fd);
        return false;
    }

    // unmap and close file
    munmap(data, total_size);
    if (fclose(fd) != 0) {
        perror("fclose failed");
        return false;
    }

    return true;
}
