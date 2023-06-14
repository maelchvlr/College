/*
  Programme avion à compléter. Les zones à compléter sont indiquées et il n'est pas nécessaire d'ajouter de nouvelles traces d'exécution.
  
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

  initDefault(10); // ne pas modifier ni déplacer 
  char * couleurTrace = "\x1b[38;5;39m"; //ces deux ligne.
     
  if (argc!=7) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s destination_aller nombre_passagers_aller destination_retour nombre_passagers_retour fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	  
  char * dest_aller = argv[1];
  int nbPassagers_Aller = atoi(argv[2]);
    
  char * dest_retour = argv[3];
  int nbPassagers_Retour = atoi(argv[4]);
  
  int cle=ftok(argv[5], atoi(argv[6]));

  int idFile = msgget(cle, IPC_CREAT | 0666);

  // j'utilise msgget de sorte à s'assurer que la file existe avant de poursuivre l'exécution.
  if (idFile == -1){
    perror("erreur  msgget");
    exit(-1);
  }
  
  struct msg msg;
 
  // dans la suite, les traces d'exécution données sont à garder inchangées.
  
  // vol aller 
  afficher('a', "embarquement immediat !", dest_aller, couleurTrace);
  
  for (int i = 2; i < nbPassagers_Aller+2; i++) {

    int recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, 1, 0);
    if (recv < 0) {
      perror("msgrcv()");
      exit(1);
    }
    
    printf("numéro passager: %s\n", msg.text);
    
    msg.id = atoi(msg.text); //on assigne l'id du client à la destination du deuxième message
    sprintf(msg.text,"%d",i); //on envoie au client l'étiquette qu'il va écouter

    int send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
    if (send < 0) {
      perror("msgsnd()");
      exit(1);
    }
  }
  
  msg.id = 2;
  int send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0); // on envoie l'information au premier passager qu'il peut monter dans l'avion
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }


  afficher('a', "attente fin embarquement", dest_aller, couleurTrace); 

  //on écoutera sur le canal nombre de passager + 2 puisque la manière dont j'ai défini les étiquettes (expliquées dans passager.c)
  //fait que le dernier passager enverra son message sur nombre de passager+2
  //à partir de cette ligne, chaque send et receive n'envoient aucune données spécifique
  //le tout est expliquer dans passager.c
  
  int recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, nbPassagers_Aller+2, 0);
  if (recv < 0) {
      perror("msgrcv()");
      exit(1);
  }

  msg.id = 2;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, nbPassagers_Aller+2, 0);
  if (recv < 0) {
      perror("msgrcv()");
      exit(1);
  }

  afficher('a',"décolage imminent! Durée du vol estimée à quelques secondes", dest_aller, couleurTrace); 
 
  vol(4);  // simulation d'une durée de vol (vous pouvez tester avec d'autres valeurs. Le code rendu doit toutefois être vol(4); 
  
  afficher('a',"arrivé à destination", dest_aller, couleurTrace);

  msg.id = 2;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  afficher('a', "attente que tout le monde soit descendu", dest_aller, couleurTrace);
  
  recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, nbPassagers_Aller+2, 0);
  if (recv < 0) {
      perror("msgrcv()");
      exit(1);
  }

  afficher('a', "vol aller terminé", dest_aller, couleurTrace);
  
  // vol retour 
  afficher('a', "embarquement immediat !", dest_retour, couleurTrace);
  
  for (int i = 2; i < nbPassagers_Retour+2; i++) {

    int recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, 1, 0);
    if (recv < 0) {
      perror("msgrcv()");
      exit(1);
    }
    printf("numéro passager: %s\n", msg.text);
    
    msg.id = atoi(msg.text); //on assigne l'id du client à la destination du deuxième message
    sprintf(msg.text,"%d",i); //on envoie au client l'étiquette qu'il va écouter

    int send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
    if (send < 0) {
      perror("msgsnd()");
      exit(1);
    }
  }
  
  msg.id = 2;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  afficher('a', "attente fin embarquement", dest_retour, couleurTrace); 

  recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, nbPassagers_Aller+2, 0);
  if (recv < 0) {
      perror("msgrcv()");
      exit(1);
  }

  msg.id = 2;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, nbPassagers_Aller+2, 0);
  if (recv < 0) {
      perror("msgrcv()");
      exit(1);
  }

  afficher('a',"décolage imminent! Durée du vol estimée à quelques secondes", dest_retour, couleurTrace); 
 
  vol(4);  // simulation d'une durée de vol (vous pouvez tester avec d'autres valeurs. Le code rendu doit toutefois être vol(4); 
  
  afficher('a',"arrivé à destination", dest_retour, couleurTrace);

  msg.id = 2;
  send = msgsnd(idFile, &msg, strlen(msg.text) + 1, 0);
  if (send < 0) {
    perror("msgsnd()");
    exit(1);
  }

  afficher('a', "attente que tout le monde soit descendu", dest_retour, couleurTrace);
  
  recv = msgrcv(idFile, &msg, MAX_BUFF_SIZE, nbPassagers_Aller+2, 0);
  if (recv < 0) {
      perror("msgrcv()");
      exit(1);
  }

  afficher('a', "vol rertour terminé", dest_retour, couleurTrace); 
  return 0;
      
}

