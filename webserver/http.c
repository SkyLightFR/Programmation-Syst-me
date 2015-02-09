#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <regex.h>
#include "http.h"

regex_t method_regex;

int compile_regex(void) {
    if (regcomp(&method_regex, "^GET /.* HTTP/1\\.[01]\r\n$", 0)) {
        return -1;
    }

    return 0;
}

int method_parser(const char *request) {
    if (regexec(&method_regex, request, 0, NULL, 0) == REG_NOMATCH) {
        return -1;
    }

    if (strncmp(request, "GET / ", 6) != 0) return RSC_NOT_FOUND;

    return 0;
}
