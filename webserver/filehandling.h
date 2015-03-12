#ifndef __FILEHANDLING_H__
#define __FILEHANDLING_H__

#define EROOTD -2

int get_dir_fd(const char *path);
int check_and_open(char *url, const char *document_root);
int open_file(int root_fd, char *url);
int get_file_size(int fd);
const char *get_file_type(char *file);
int copy(int in, int out);

#endif
