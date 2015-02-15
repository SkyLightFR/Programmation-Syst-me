#define EFSTAT -1
#define ENOTAFILE -2
#define EREAD -3

int open_file(char *url);
int get_file_content(int fd, void *file_content);
