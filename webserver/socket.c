#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

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
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
    perror("bind server_socket");
  }

  /* Enabled listening */

  if (listen(server_socket, 10) == -1) {
    perror("listen server_socket");
  }

 
  return server_socket;
} 
