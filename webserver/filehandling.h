#define EFSTAT -1
#define ENOTAFILE -2
#define EREAD -3

int get_dir_fd(const char *path);
int check_and_open(char *url, const char *document_root);
int open_file(int root_fd, char *url);
int get_file_content(int fd, void *file_content);
