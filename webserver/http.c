#include <string.h>
#include <stdio.h>

int parser(const char *request) {
    if (strstr(request, "GET ") != request) return -1;

    unsigned int i = 3;
    int words = 1;

    while (i < strlen(request)) {
        if (strcmp(request, " ")) {
            ++words;
        }
        ++i;
    }
    printf("%d mots\ntaille : %ld", words, strlen(request));

    if (words == 3) {
        return 0;
    }

    return -1;
}
