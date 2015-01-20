#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

int main(int argc, char **argv) {
  int client_socket;
  int welcome_fd;
  struct stat welcome_stat;
  char *welcome_msg = "";
  char client_message[100];

  if(argc == 0) {
    perror("No argument");
  }

  int server_socket = creer_serveur(atoi(argv[1]));
  while(1) {

    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
      perror("accept");
    }
    sleep(1);

    welcome_fd = open("resource/welcome_message", O_RDONLY);
    fstat(welcome_fd, &welcome_stat);
    read(welcome_fd, (void *)welcome_msg, welcome_stat.st_size);
    write(client_socket, welcome_msg, strlen(welcome_msg));
  
    while(1) {
        int lg = read(client_socket, client_message,100);
        write(client_socket, client_message,lg);
    }
  }
  return 0;
}
