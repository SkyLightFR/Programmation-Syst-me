#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"

int main(int argc ,char **argv) {
  if(argc == 0) {
    perror("No argument");
  } else {
    creer_serveur(atoi(argv[1]));
  }
  return 0;
}
