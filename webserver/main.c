#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

int main(int argc, char **argv) {
  if(argc == 0) {
    perror("No argument");
  }


  int server_socket = creer_serveur(atoi(argv[1]));
  int client_socket;
  /*char client_message[100];*/
  while(1) {

    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1) {
      perror("accept");
    }
    sleep(1);
    const char *welcome_message = "Hello ! Welcome on my server! \n Ceci est un nouveau serveur créer par Crosnier Florian et Chevalier Benjamin. \n Il est distribué sous la license GNU GPL V2.0 et a été développé dans le cadre du cours ASR4-Programmation Système.\n\n";
    write(client_socket, welcome_message, strlen(welcome_message));
  
  /* while(1) {
    read(client_socket, client_message,100);
    write(client_socket, client_message,100);
    }*/
  }
  return 0;
}
