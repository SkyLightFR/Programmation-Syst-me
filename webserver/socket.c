#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>

int creer_serveur(int port) {
 
  /* Creation of the Socket */

  int server_socket;
 
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1) {
    perror("server_socket");
  }

  /* Binding Socket */

  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(8000);
  saddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
    perror("bind server_socket");
  }

  return 0;
} 
