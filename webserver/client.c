#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "acquire.h"
#include "http.h"
#include "response.h"
#include "stats.h"
#include "filehandling.h"

int client_treatment(int client_socket, char *document_root) {
    FILE *client = fdopen(client_socket, "w+");
    char client_message[MAX_MSG_LENGTH];
    http_request request;
    int errval;
    int url_fd;
    web_stats *stats = get_stats();

    ++stats->served_connections;

    /* Fetch everything the client sends until \r\n or \n */
    fgets_or_exit(client_message, MAX_MSG_LENGTH, client);
    errval = parse_http_request(client_message, &request);
    skip_headers(client);

    /* Send response according to request state */
    if (errval < 0) {
        send_response(client, 400, "Bad Request", "Bad request\r\n");
        ++stats->ko_400;
    }

    else if (request.method == HTTP_UNSUPPORTED)
        send_response(client, 405, "Method Not Allowed", "Method not allowed\r\n");

    else if (request.major_version == -1 || request.minor_version == -1)
        send_response(client, 505, "HTTP Version Not Supported", "HTTP version not supported\r\n");

    else if (!strcmp(request.url, "stats")) {
        send_stats(client);
        ++stats->ok_200;
    }

    else if ((url_fd = check_and_open(request.url, document_root)) < 0) {
        if (url_fd == EROOTD) {
            send_response(client, 403, "Forbidden", "Forbidden\r\n");
            ++stats->ko_403;
        } else {
            send_response(client, 404, "Not Found", "Not found\r\n");
            ++stats->ko_404;
        }

    } else {
        send_response_file(client, url_fd, request.url, 200, "OK");
        ++stats->ok_200;
    }

    ++stats->served_requests;
    close(url_fd);
    fclose(client);
    exit(0);
}

int create_client_socket(int server_socket, char *document_root) {
    int client_socket;
    int pid;

    /* Wait for connections in the main process */
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
        perror("accept");
        return -1;
    }

    /* Create a forked process to handle each connection */
    if ((pid = fork()) == -1) {
        perror("fork");
        return -1;

    } else if (pid == 0) {
        sleep(1);
        if (client_treatment(client_socket, document_root) == -1) {
            printf("Could not start client treatment");
        }
    }

    close(client_socket);

    return 0;
}
