#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "socket.h"
#include "client.h"
#include "http.h"
#include "filehandling.h"
#include "stats.h"

int main(int argc, char **argv) {
    int port;

    const char *paramerr = " : invalid parameters\nusage : TTS-Server [-d root_dir] <listen_port>\n";

    /* Check parameters */
    if (argc >= 2) {
        port = atoi(argv[argc-1]);

        if (port < 0 || port > 65535) {
            printf("%s%s", argv[0], paramerr);
            return -1;
        }

        if (argc == 4) {
            if (strcmp(argv[1], "-d")) {
                printf("%s%s", argv[0], paramerr);
                return -1;
            }

            if (get_dir_fd(argv[2]) == -1) {
                perror("TTS-Server : root directory");
                return -1;
            }


        } else if (argc == 3 || argc > 4) {
            printf("%s%s", argv[0], paramerr);
            return -1;
        }

    } else {
        printf("%s%s", argv[0], paramerr);
        return -1;
    }

    /* Create socket and start listening */
    int sockets[2];
    create_server(sockets, port);

    if (sockets[0] < 0 || sockets[1] < 0) {
        printf("%s : unable to start : failed to create server socket\n", argv[0]);
        return -1;
    }

    if (init_stats() < 0)
        return -1;

    signal_init();

    /* Wait for connections forever */
    while (1) {
        create_client_socket(sockets, argv[2]);
    }

    return 0;
}
