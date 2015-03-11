#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "filehandling.h"
#include "stats.h"

/* Sends only the status line */
void send_status(FILE *client, int code, const char *reason_phrase) {
    if (fprintf(client, "HTTP/1.0 %d %s\r\n", code, reason_phrase) < 0)
        exit(-1);
}

/* Sends the full response */
void send_response(FILE *client, int code, const char *reason_phrase, const char *message_body) {
    send_status(client, code, reason_phrase);
    if (fprintf(client, "Connection: close\r\nContent-Length: %ld\r\nContent-Type: text/plain\r\n\r\n%s", strlen(message_body), message_body) < 0)
        exit(-1);
}

/* Send the full response using a file descriptor as content */
void send_response_file(FILE *client, int fd, char *path, int code, const char *reason_phrase) {
    send_status(client, code, reason_phrase);
    if (fprintf(client, "Connection: close\r\nContent-Length: %d\r\nContent-Type: %s\r\n\r\n", get_file_size(fd), get_file_type(path)) < 0)
        exit(-1);

    fflush(client);

    if (copy(fd, fileno(client)) < 0)
        exit(-1);
}

void send_stats(FILE *client) {
    web_stats *stats = get_stats();
    const char *html = "<html><head><meta charset='UTF-8'><title>Statistics</title><style type='text/css'>table{border:1px solid black;}th,td{text-align:center;border:1px solid black;}</style></head><body><table><tr><th>Request or Answer type</th><th>Times served</th><tr><td>Connections</td><td>%d</td></tr><tr><td>Requests</td><td>%d</td></tr><tr><td>Code 200</td><td>%d</td></tr><tr><td>Code 400</td><td>%d</td></tr><tr><td>Code 403</td><td>%d</td></tr><tr><td>Code 404</td><td>%d</td></tr></table></body></html>";
    char formatted_stats[strlen(html) + 30];

    sprintf(formatted_stats, html, stats->served_connections, stats->served_requests, stats->ok_200, stats->ko_400, stats->ko_403, stats->ko_404);

    send_status(client, 200, "OK");
    if (fprintf(client, "Connection: close\r\nContent-Length: %ld\r\nContent-Type: text/html\r\n\r\n%s\n", strlen(formatted_stats), formatted_stats) < 0)
        exit(-1);

    fflush(client);
}
