#include <unistd.h>     /* write, lseek */
#include <stdlib.h>

/*
 * OTP  
 * Provided file, generate key file, and encrypt file OR take key and decrypt file
 * 
 */

#define BUFFER_SIZE 1024


int generate_key(int fd, int length) {
    //  0 -> SUCCESS
    // -1 -> FATAL_ERROR
    char buffer[BUFFER_SIZE];

    // setting pointer to the start of the file
    if (lseek(fd, 0, SEEK_SET) == -1) {
        return -1;
    }

    for (int i = 0; i < length; ++i) {
        buffer[i % 1024] = rand() % 256;
        if ((i != 0 && i % BUFFER_SIZE) || (i == length - 1)) {
            if (write(fd, buffer, (i % 1024) + 1) == -1) {
                return -1;
            }
        }
    }

    return 0;
}


int encrypt(int src_fd, int dst_fd, int key_fd) {
    char buffer[BUFFER_SIZE];
    char key_buff[BUFFER_SIZE];
    ssize_t bytes_read = 0;

    while ((bytes_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {
        if ((bytes_read = read(key_fd, key_buff, bytes_read)) == -1) {
            return -1;
        }

        for (int i = 0; i < bytes_read; ++i) {
            buffer[i] ^ key_buff[i];
        }

        if (write(dst_fd, buffer, bytes_read) == -1) {
            return -1;
        }
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
    return EXIT_SUCCESS;
}