#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "filehandling.h"

/* Returns a file descriptor for a readable directory */
int get_dir_fd(const char *path) {
    return open(path, O_RDONLY | O_DIRECTORY);
}

/* Returns a file descriptor to a readable file */
int check_and_open(char *url, const char *document_root) {
    int dir_fd;
    int fd;
    struct stat file_stat;

    if (document_root != NULL) {
        if ((dir_fd = get_dir_fd(document_root)) == -1)
            return -1;

        if ((fd = openat(dir_fd, url, 0)) == -1)
            return -1;

    } else {
        if ((fd = open(url, O_RDONLY)) == -1)
            return -1;
    }

    if (fstat(fd, &file_stat) == -1)
        return -1;

    if (!S_ISREG(file_stat.st_mode))
        return -1;

    return fd;
}

int get_file_size(int fd) {
    struct stat file_stat;

    if (fstat(fd, &file_stat) == -1)
        return EFSTAT;

    return file_stat.st_size;
}

int copy(int in, int out) {
    const int buf_len = 4096;
    char buf[buf_len];
    int read_len;

    while ((read_len = read(in, buf, buf_len)) > 0) {
        if (write(out, buf, read_len) == -1)
            return -1;
    }

    return 0;
}
