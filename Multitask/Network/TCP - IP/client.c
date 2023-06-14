#include <netinet/in.h>
#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<string.h>
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 146980

int main(int argc, char *argv[]) {

  if (argc != 4){
    printf("Usage:%s IP PORT FILE\n", argv[0]);
    exit(0);
  }

  /* etape 1 : créer une socket */
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  sockfd <= 0
    ? perror("socket()"), exit(1)
    : printf("[-] Socket created\n");

  /* etape 2 : designer la socket du serveur */
  struct sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  server.sin_port = htons(atoi(argv[2]));

  /* etape 3 : demander une connexion */
  socklen_t len = sizeof(struct sockaddr_in);
  connect(sockfd, (struct sockaddr *) &server, len) < 0
    ? perror("connect()"), exit(1)
    : printf("[-] Connected to server\n");

  /* etape 4 : envoi de fichier : attention la question est générale Il faut creuser pour définir un protocole d'échange entre le client et le serveur pour envoyer correctement un fichier */

  // construction du nom du chemin vers le fichier
  char* filepath = malloc(strlen(argv[3]) + 16); // ./emission/+nom fichier +\0
  filepath[0] = '\0';
  strcat(filepath, "./emission/");
  strcat(filepath, argv[3]);

  printf("[-] Selected file: %s\n", filepath);

  // obtenir la taille du fichier
  struct stat attributes;
  if(stat(filepath, &attributes) == -1){
    perror("stat()");
    free(filepath);
    exit(1);
  }


  // SEND FILE NAME SIZE + FILE NAME
  int filename_size = strlen(argv[3]);

  if (send(sockfd, &filename_size, sizeof(filename_size), 0) < 0)
    perror("send() filename size"), exit(1);
  if (send(sockfd, argv[3], strlen(argv[3]), 0) < 0)
    perror("send() filename"), exit(1);

  // SEND FILE SIZE
  int file_size = attributes.st_size;
  printf("[-] File size: %d octets\n", file_size);
  if (send(sockfd, &file_size, 4, 0) < 0)
    perror("send() file size"), exit(1);


  // lecture du contenu d'un fichier
  FILE* file = fopen(filepath, "rb");
  if (file == NULL){
    perror("fopen()");
    free(filepath);
    exit(1);
  }
  free(filepath);

  int total_sent = 0;
  char buffer[MAX_BUFFER_SIZE];

  while(total_sent < file_size){
    
    int read = fread(buffer, sizeof(char), MAX_BUFFER_SIZE, file);
    if(read == 0) {

      ferror(file) != 0
        ? perror("read()"),
          fclose(file),
          exit(1)

        : printf("[-] Reached end of file\n");
          break;
    }

    int sent = send(sockfd, buffer, read, 0);
    if (send < 0) {
      perror("send()"); exit(1);
    } else {
      printf("[-] Sent block of %d octets\n", sent);
    }

    total_sent += read;
  }

  fclose(file); close(sockfd);

  printf("[-] Sent %d octets in total\n", total_sent);  
  printf("[-] Client exiting\n");

  return 0;
}