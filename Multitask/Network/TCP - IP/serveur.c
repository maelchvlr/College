#include <netinet/in.h>
#include <stdio.h>//perror
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>//close
#include <stdlib.h>
#include <string.h>


#define MAX_BUFFER_SIZE 146980


int main(int argc, char *argv[])
{
  /* etape 0 : gestion des paramètres si vous souhaitez en passer */

  /* etape 1 : creer une socket d'écoute des demandes de connexions*/
  int sockfd = socket(PF_INET, SOCK_STREAM, 0);
  sockfd <= 0
    ? perror("socket()"), exit(1)
    : printf("[-] Socket created\n");

  /* etape 2 : nommage de la socket */
  socklen_t len = sizeof(struct sockaddr_in);
  struct sockaddr_in me, client;
  me.sin_family = AF_INET;
  me.sin_addr.s_addr = htonl(INADDR_ANY);
  me.sin_port = htons(4443);

  bind(sockfd, (struct sockaddr*) &me, len) < 0
    ? perror("bind()"), exit(1)
    : printf("[-] Socket binded\n");

  getsockname(sockfd, (struct sockaddr*) &me, &len) < 0
    ? perror("getsockname()"), exit(1)
    : printf("[-] Port Assigned to %d\n", ntohs(me.sin_port));

  /* etape 3 : mise en ecoute des demandes de connexions */
  listen(sockfd, 3) < 0
    ? perror("listen()"), exit(1)
    : printf("[-] Server listening...\n");

  /* etape 4 : plus qu'a attendre la demadne d'un client */
  int filename_size, file_size;
  char filename[256];

  while(1) {

    int sessionfd = accept(sockfd, (struct sockaddr*) &client, &len);
    printf("[-] Client %s:%d connected\n", inet_ntoa(client.sin_addr), htons(client.sin_port));


    /* le protocol d'echange avec un client pour recevoir un fichier est à définir. Ici seul un exemple de code pour l'écriture dans un fichier est founi*/

    // FILE NAME SIZE
    recv(sessionfd, &filename_size, sizeof(filename_size), 0) < 0
      ? perror("recv() filename size"), exit(1)
      : printf("[-] -- File name size: %d\n", filename_size);

    // FILE NAME
    memset(&filename, 0, sizeof(filename));
    recv(sessionfd, filename, filename_size, 0) < 0
      ? perror("recv() file name"), exit(1)
      : printf("[-] -- File name: %s\n", filename);

    // FILE SIZE
    recv(sessionfd, &file_size, sizeof(file_size), 0) < 0
      ? perror("recv() file size"), exit(1)
      : printf("[-] -- File size: %d octets\n", file_size);

    // PREPARE RECEIVING FILE
    char filepath[256] = "./reception/"; // cette ligne n'est bien-sur qu'un exemple et doit être modifiée : le nom du fichier doit être reçu.
    strcat(filepath, filename);

    FILE* file = fopen(filepath, "wb");
    if(file == NULL){
      perror("fopen()\n"); exit(1);
    }

    int total_received = 0;

    while(total_received < file_size) {

      // WRITE TO FILE
      char buffer[MAX_BUFFER_SIZE];

      int block = recv(sessionfd, buffer, MAX_BUFFER_SIZE, 0);
      block < 0
        ? perror("recv() file data"), exit(1)
        : printf("[-] Received block of %d octet\n", block);

      total_received += block;

      size_t written = fwrite(buffer, sizeof(char), block, file);
      if (written < strlen(buffer)+1) {
        perror("fwrite()"); fclose(file); exit(1);
      } else {
        printf("[-] Wrote %zd octets to file\n", written);
      }
    }

    fclose(file);
    close(sessionfd);

    printf("[-] Connection closed with %s (total %d octets)\n\n", inet_ntoa(client.sin_addr), total_received);
  }

  close(sockfd); printf("[-] Server exiting\n");
}