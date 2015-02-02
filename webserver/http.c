#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>

regex_t method_regex;

int compile_regex(void) {
    if (regcomp(&method_regex, "^GET /.* HTTP/1\\.[01]\r\n$", 0)) {
        return -1;
    }

    return 0;
}

int method_parser(const char *request) {
    /*if (strstr(request, "GET ") != request) return -1;

    regex_t http_version;
    unsigned int i = 3;
    int words = 1;

    while (i < strlen(request)) {
        if (request[i] == ' ') {
            ++words;
        }
        ++i;
    }

    if (words == 3) {
        if (regcomp(&http_version, "HTTP/1\\.[01]", 0)) {
            return -1;
        } else if (regexec(&http_version, strstr(request, "HTTP/1"), 0, NULL, 0)) {
            perror("regexec");
            return -1;
        }
    } else {
        return -1;
    }*/
    if (regexec(&method_regex, request, 0, NULL, 0) == REG_NOMATCH) {
        return -1;
    }

    return 0;
}
