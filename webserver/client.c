#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
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

    update_stats(CON);

    /* Fetch everything the client sends until \r\n or \n */
    fgets_or_exit(client_message, MAX_MSG_LENGTH, client);
    errval = parse_http_request(client_message, &request);
    skip_headers(client);

    /* Send response according to request state */
    if (errval < 0) {
        send_response(client, 400, "Bad Request", "Bad request\r\n");
        update_stats(BADREQ);
    }

    else if (request.method == HTTP_UNSUPPORTED) {
        send_response(client, 405, "Method Not Allowed", "Method not allowed\r\n");
        update_stats(METHD);
    }

    else if (request.major_version == -1 || request.minor_version == -1) {
        send_response(client, 505, "HTTP Version Not Supported", "HTTP version not supported\r\n");
        update_stats(HTTPVERS);
    }

    else if (!strcmp(request.url, "stats")) {
        send_stats(client);
        update_stats(OK);
    }

    else if ((url_fd = check_and_open(request.url, document_root)) < 0) {
        if (url_fd == EROOTD || errno == EACCES) {
            send_response(client, 403, "Forbidden", "Forbidden\r\n");
            update_stats(FORB);
        } else {
            send_response(client, 404, "Not Found", "Not found\r\n");
            update_stats(NOTF);
        }

    } else {
        send_response_file(client, url_fd, request.url, 200, "OK");
        update_stats(OK);
    }

    update_stats(REQ);

    close(url_fd);
    fclose(client);
    exit(0);
}

int create_client_socket(int *sockets, char *document_root) {
    fd_set fds;
    int pid;
    int client_socket = -1;

    /* Add the two sockets to the set */
    FD_ZERO(&fds);
    FD_SET(sockets[0], &fds);
    FD_SET(sockets[1], &fds);

    /* Wait for connections in the main process */
    if (select(sockets[1] + 1, &fds, NULL, NULL, NULL) == -1) {
        perror("select");
        return -1;

    } else {
        if (FD_ISSET(sockets[0], &fds))
            client_socket = accept(sockets[0], NULL, NULL);
        else if (FD_ISSET(sockets[1], &fds))
            client_socket = accept(sockets[1], NULL, NULL);
        else
            printf("Why can I even read this?!\n");

        if (client_socket < 0) {
            perror("accept");
            return -1;
        }

        /* Create a forked process to handle each connection */
        if ((pid = fork())  == -1) {
            perror("fork");
            return -1;

        } else if (pid == 0) {
            sleep(1);
            if (client_treatment(client_socket, document_root) == -1) {
                printf("Could not start client treatment");
            }
        }

        close(client_socket);
    }

    return 0;
}
