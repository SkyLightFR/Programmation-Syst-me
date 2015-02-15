#include <string.h>
#include <stdio.h>
#include "acquire.h"
#include "http.h"

char *rewrite_url(char *url);

/* Fills *request according to *request_line */
int parse_http_request(const char *request_line, http_request *request) {
    char rq_method[7];
    char rq_url[2000];
    char rq_version[8];
    char devnull;

    /* request_line must contain three non-empty strings */
    if (sscanf(request_line, "%s %s %s %c", rq_method, rq_url, rq_version, &devnull) != 3) return -1;

    request->url = rewrite_url(rq_url);

    if (!strcmp(rq_method, "GET")) request->method = HTTP_GET;
    else request->method = HTTP_UNSUPPORTED;

    if (!strcmp(rq_version, "HTTP/1.0")) {
        request->major_version=1;
        request->minor_version=0;

    } else if (!strcmp(rq_version, "HTTP/1.1")) {
        request->major_version=1;
        request->minor_version=0;

    } else {
        request->major_version=-1;
        request->minor_version=-1;
    }

    return 0;
}

/* Ignores the headers in the client request */
void skip_headers(FILE *client) {
    char client_line[MAX_MSG_LENGTH];

    while (1) {
        fgets_or_exit(client_line, MAX_MSG_LENGTH, client);
        /* Wait for the end... just wait */
        if (!strcmp(client_line, "\r\n") || !strcmp(client_line, "\n")) break;
    }
}

/* Removes the query part of the URL */
char *rewrite_url(char *url) {
    char formatted_url[2000];
    unsigned int i;

    /* Find the first position of the '?' symbol in the URL */
    for (i = 0 ; i < strlen(url) && url[i] != '?' ; ++i);

    /* Re-create the URL if '?' was found */
    if (i != strlen(url)) {
        strncpy(formatted_url, url, i);
        strcpy(url, formatted_url);
    }

    return url;
}
