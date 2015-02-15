#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include "acquire.h"
#include "http.h"
#include "response.h"
#include "filehandling.h"

int client_treatment(int client_socket) {
    FILE *client = fdopen(client_socket, "w+");
    char client_message[MAX_MSG_LENGTH];
    http_request request;
    int errval;

    int url_fd;
    char *file_content = malloc(1);

    /* Fetch everything the client sends until \r\n or \n */
    fgets_or_exit(client_message, MAX_MSG_LENGTH, client);
    errval = parse_http_request(client_message, &request);
    skip_headers(client);

    /* Send response according to request state */
    if (errval < 0)
        send_response(client, 400, "Bad Request", "Bad request\r\n");

    else if (request.method == HTTP_UNSUPPORTED)
        send_response(client, 405, "Method Not Allowed", "Method not allowed\r\n");

    else if (request.major_version == -1 || request.minor_version == -1)
        send_response(client, 505, "HTTP Version Not Supported", "HTTP version not supported\r\n");

    else if ((url_fd = open_file(request.url)) == -1) {
        if (errno == ENOENT)
            send_response(client, 404, "Not Found", "Not found\r\n");
        else if (errno == EACCES)
            send_response(client, 403, "Forbidden", "Forbidden\r\n");

    } else if ((errval = get_file_content(url_fd, file_content)) < 0) {
        if (errval == EFSTAT || errval == EREAD)
            send_response(client, 500, "Internal Server Error", "Internal server error\r\n");
        else if (errval == ENOTAFILE)
            send_response(client, 404, "Not Found", "Not found\r\n");

    } else
        send_response(client, 200, "OK", file_content);

    fclose(client);
    exit(0);
}

int create_client_socket(int server_socket) {
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
        if (client_treatment(client_socket) == -1) {
            printf("Could not start client treatment");
        }
    }

    close(client_socket);

    return 0;
}
