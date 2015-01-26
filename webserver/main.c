#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket.h"
#include "client.h"

int main(int argc, char **argv) {
    int client_socket;
    int port;
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

    signal_init();

    while (1) {
        client_socket = create_client_socket(server_socket);
        client_treatment(client_socket);
        close(client_socket);
    }

    return 0;
}
