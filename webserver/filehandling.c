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

    if (document_root != NULL) {
        if ((dir_fd = get_dir_fd(document_root)) == -1)
            return -1;

        if ((fd = openat(dir_fd, url, 0)) == -1)
            return -1;

    } else {
        if ((fd = open(url, O_RDONLY)) == -1)
            return -1;
    }

    return fd;
}

int get_file_content(int fd, void *file_content) {
    struct stat file_stat;

    if (fstat(fd, &file_stat) == -1)
        return EFSTAT;

    /* The resource to serve must be a regular file */
    if (!S_ISREG(file_stat.st_mode))
        return ENOTAFILE;

    file_content = realloc(file_content, file_stat.st_size);

    /* Put the content of the file in *file_content */
    if (read(fd, file_content, file_stat.st_size) == -1)
        return EREAD;

    close(fd);

    return 0;
}
