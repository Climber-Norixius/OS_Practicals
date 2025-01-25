#include <stdint.h>     /* uint8_t */
#include <stdio.h>      /* fprintf */
#include <unistd.h>     /* read, write, ssize_t */
#include <fcntl.h>      /* open */
#include <stdlib.h>     /* EXIT_* */
#include <err.h>        /* warn */

/*
* File Copier with Byte-Specific Filtering
*
* Simple file copier program reads a file byte by byte,
* performs a filtering operation (to include only specific
* bytes based on a condition), and writes the filtered bytes
* to another file. Your program should handle both reading
* and writing efficiently using system calls.
*
*/

#define BUFFER_SIZE 2048

int copy_filter(int source_fd, int destination_fd, uint8_t byte) {
    //  0 -> Everything went without error
    // -1 -> Program ran into an unexpected error
    char buffer[BUFFER_SIZE];
    char filtered[BUFFER_SIZE];
    ssize_t bytes_read = 0, bytes_to_write = 0;

    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] != byte) {
                filtered[bytes_to_write++] = buffer[i];
            }
        }

        if (write(destination_fd, filtered, bytes_to_write) < 0) {
            return -1;
        }
        bytes_to_write = 0;
    }

    if (bytes_read == -1) {
        return -1;
    }
    return 0;
}


void close_or_warn(int fd) {
    if (close(fd) == -1) {
        warn("Closing fd %d", fd);
    }
}


int main(int argc, char *argv[]) {
    // Usage: ./copy_filter name_of_src_file name_dst_file byte_to_filter
    if (argc != 4) {
        fprintf(stderr, "Wrong number of arguments!\n");
        return -1;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        return EXIT_FAILURE;
    }

    int dst_fd = open(argv[2], O_WRONLY);
    if (dst_fd == -1) {
        close_or_warn(src_fd);
        return EXIT_FAILURE;
    }

    if (copy_filter(src_fd, dst_fd, argv[3][0]) == -1) {
        close_or_warn(src_fd);
        close_or_warn(dst_fd);
        return EXIT_FAILURE;
    }

    close_or_warn(src_fd);
    close_or_warn(dst_fd);
    return EXIT_SUCCESS;
}