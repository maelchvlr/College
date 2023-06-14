/*
  Programme passager à compléter : les zones à compléter sont indiquées et il n'est pas nécessaire d'ajouter de nouvelles traces d'exécution. 
  
  Vous devez expliquer en commentaires : le sens donné au messages echangés et aux étiquettes.
    
*/

#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include "simulation.h"
#define MAX_BUFF_SIZE 16500

struct msg {
  long id;
  char text[MAX_BUFF_SIZE];
};

int main(int argc, char * argv[]){

  initDefault(10); // ne pas modifier ni déplacer cette ligne
 
  if (argc!=5) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s destination aller_ou_retour fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	   
  int cle =ftok(argv[3], atoi(argv[4]));
  int idFile = msgget(cle, 0666);

  // j'utilise msgget de sorte à s'assurer que la file existe avant de poursuivre
  if (idFile == -1){
    perror("erreur  msgget");
    exit(-1);
  }
  
  char * destination = argv[1];
  int aller_ou_retour = atoi(argv[2]); // 0 si je suis un passager du voyage aller, sinon 1 (pour un vol retour)  
  
  char * couleurTrace = "\x1b[38;5;226m";  // ne pas déplacer ni modifier
  if (aller_ou_retour) couleurTrace="\x1b[38;5;46m"; // ces deux lignes.
    
  struct msg msg;
  int monId;
  msg.id = 1; //on envoie le message sur le premier canal d'écoute de l'avion
  sprintf(msg.text, "%d", getpid()); //on lui donne notre pid pour le message de retour

  int send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  // en attente sur son PID afin de recevoir notre id passager
  int recv = msgrcv(idFile, &msg, sizeof(msg), getpid(), 0);
  if (recv < 0) {
      perror("msgrcv()");
      exit(1);
  }
  
  monId = atoi(msg.text); //on stocke notre id passager, qui sera l'étiquette d'écoute

  //à partir de cette ligne, chaque send et receive n'envoient aucune donnée particulière, ils servent juste à faire de la synchronisation
  // par exemple, avion indique au premier passager (d'étiquette 2 pour ne pas bloquer le canal 1) l'info qu'il peut descendre
  // le passager d'étiquette 2 descend puis propage au passager d'étiquette 3; qui propage au passager d'étiquette 4, etc...


  afficher('p', "Yoopi, je vais prendre l'avion !", destination, couleurTrace);
  afficher('p', "je patiente en salle d'embarquement", destination, couleurTrace);
  
  recv = msgrcv(idFile, &msg, sizeof(msg), monId, 0);

   
  afficher('p', "j'embarque ...", destination, couleurTrace);
  action(); // simulation de l'action embarquer.
  
  msg.id = monId + 1;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }
  
  recv = msgrcv(idFile, &msg, sizeof(msg), monId, 0);   
  // ici se produit la simulaion du vol. je n'ai rien à faire
   
  msg.id = monId + 1;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  afficher('p', "maintenant, je patiente jusqu'à destination", destination, couleurTrace);
  
  recv = msgrcv(idFile, &msg, sizeof(msg), monId, 0);   
   
  afficher('p', "je descends de l'avion", destination, couleurTrace);
 
  action(); // simulation de l'action débarquer.

  msg.id = monId + 1;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  afficher('p', "je quitte l'aéroport !", destination, couleurTrace); 
 
  return 0;
}

