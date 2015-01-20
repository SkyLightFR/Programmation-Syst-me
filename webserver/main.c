#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket.h"

#define MAX_MSG_LENGTH 255

int main(int argc, char **argv) {
    int client_socket;
    int port;
    int welcome_fd;
    struct stat welcome_stat;
    char welcome_msg[1024];
    char client_message[MAX_MSG_LENGTH];
    const char *welcomeerr = "Could not read welcome file\n";
    const char *paramerr = " : invalid parameters\nusage : TTS-Server <listen_port>\n";

    /* Check parameters */

    if (argc == 2) {
        port = atoi(argv[1]);
        if (port < 0 || port > 65535) {
            printf("%s%s", argv[0], paramerr);
            return -1;
        }
    } else {
        printf("%s%s", argv[0], paramerr);
        return -1;
    }

    int server_socket = create_server(port);
    if (server_socket == -1) {
        printf("%s : unable to start : failed to create server socket\n", argv[0]);
        return -1;
    }

    /* Client treatment */

    while(1) {
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
        }
        sleep(1);

        /* Fetch content of resource/welcom_message and print it */

        welcome_fd = open("resource/welcome_message", O_RDONLY);
        if (welcome_fd == -1) {
            perror("read resource/welcome_message");
            write(client_socket, welcomeerr, strlen(welcomeerr));
        } else {
            if (fstat(welcome_fd, &welcome_stat) == -1) {
                perror("fstat");
            }

            if (read(welcome_fd, (void *)welcome_msg, welcome_stat.st_size) == -1) {
                perror("read welcome_message");
            }

            close(welcome_fd);

            if (write(client_socket, welcome_msg, strlen(welcome_msg)) == -1) {
                perror("write welcome_message");
            }
        }

        /* Echo everything the client sends */

        while(1) {
            int lg = read(client_socket, client_message, MAX_MSG_LENGTH);
            write(client_socket, client_message, lg);
        }
    }

    return 0;
}
