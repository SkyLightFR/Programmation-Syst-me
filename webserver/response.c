#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filehandling.h"

/* Sends only the status line */
void send_status(FILE *client, int code, const char *reason_phrase) {
    if (fprintf(client, "HTTP/1.0 %d %s\r\n", code, reason_phrase) < 0)
        exit(-1);
}

/* Sends the full response */
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body) {
    send_status(client, code, reason_phrase);
    if (fprintf(client, "Content-Length: %ld\r\n\r\n%s", strlen(message_body), message_body) < 0)
        exit(-1);
}

/* Send the full response using a file descriptor as content */
void send_response_fd(FILE *client, int fd, int code, const char *reason_phrase) {
    send_status(client, code, reason_phrase);
    if (fprintf(client, "Content-Length: %d\r\n\r\n", get_file_size(fd)) < 0)
        exit(-1);
    fflush(client);
    if (copy(fd, fileno(client)) < 0)
        exit(-1);
}
