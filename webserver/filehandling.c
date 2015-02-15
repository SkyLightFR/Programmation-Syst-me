#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "filehandling.h"

/* Returns a file descriptor for the requested file */
int open_file(char *url) {
    /* Reset errno to check its value after an error from open() (if any) */
    errno = 0;

    return open(url, O_RDONLY);
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
