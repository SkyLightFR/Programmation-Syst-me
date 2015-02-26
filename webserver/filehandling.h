#define EFSTAT -2

int get_dir_fd(const char *path);
int check_and_open(char *url, const char *document_root);
int open_file(int root_fd, char *url);
int get_file_size(int fd);
int copy(int in, int out);
