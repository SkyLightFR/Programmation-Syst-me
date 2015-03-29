#include <stdlib.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <stdio.h>

int gen_socket4(const int port ) {
    int server_socket4;

    /* Creation of the IPv4 socket */
    server_socket4 = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket4 == -1) {
        perror("socket");
        exit(-1);
    }


    /* Enable SO_REUSEADDR */
    int optval = 1;

    if (setsockopt(server_socket4, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        perror("Can not set SO_REUSEADR option on IPv4 socket");
        exit(-1);
    }

    
    /* Bind IPv4 socket */
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket4, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        perror("bind server IPv4 socket");
        exit(-1);
    }


    return server_socket4;
}

int gen_socket6(const int port) {
    int server_socket6;

    /* Creation of the IPv6 socket */
    server_socket6 = socket(AF_INET6, SOCK_STREAM, 0);
    if (server_socket6 == -1) {
        perror("socketv6");
        exit(-1);
    }


    /* Enable SO_REUSADDR */
    int optval = 1;

    if (setsockopt(server_socket6, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1) {
        perror("Can not set SO_REUSEADR option on IPv6 socket");
        exit(-1);
    }


    /* Bind IPv6 socket */
    struct sockaddr_in6 saddr6;
    saddr6.sin6_family = AF_INET6;
    saddr6.sin6_port = htons(port);
    saddr6.sin6_addr = in6addr_any;

    if (bind(server_socket6, (struct sockaddr *)&saddr6, sizeof(saddr6)) == -1) {
        perror("bind server IPv6 socket");
        exit(-1);
    }


    return server_socket6;
}

/*
 * TODO: USE SELECT
*/
void enable_listening(const int socket) {
    if (listen(socket, 10) == -1) {
        perror("listen server socket");
        exit(-1);
    }
}

void create_server(int *sockets, const int port) {
    sockets[0] = gen_socket4(port);
    sockets[1] = gen_socket6(port);

    enable_listening(sockets[0]);
    enable_listening(sockets[1]);
}

void signal_treatment(const int sig) {
    if (sig == SIGCHLD) {
        int status;

        if (waitpid(-1, &status, WNOHANG) == -1)
            perror("waitpid");

        if (WIFSIGNALED(status))
            printf("killed by %d\n", WTERMSIG(status));
    }
}

/* Set which signals the program has to compute differently */
void signal_init(void) {
    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
        perror("signal");

    struct sigaction sa;
    sa.sa_handler = signal_treatment;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
        perror ("sigaction(SIGCHLD)");
}
