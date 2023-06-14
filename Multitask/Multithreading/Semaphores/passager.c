/*
  Programme passager à compléter : les zones à compléter sont indiquées et il n'est pas nécessaire d'ajouter de nouvelles traces d'exécution.
   
  Rappel pour le dépôt : sur Moodle, donner les instructions pour la création et l'initialisation du tableau de sémaphores nécessaires au lancement de ./bin/semInit (voir le sujet)   
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include "simulation.h"

int main(int argc, char * argv[]){

  initDefault(10); // ne pas modifier ni déplacer cette ligne.
  
  if (argc!=5) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s destination aller_ou_retour fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
    
  int cle =ftok(argv[3], atoi(argv[4]));

  int idSem = semget(cle, 1, 0666);

  // j'utilise semget de sorte a s'assurer que le tableau existe.
  if (idSem == -1){
    perror("erreur  semget");
    exit(-1);
  }
  
  char * destination = argv[1];
  int aller_ou_retour = atoi(argv[2]); // 0 si je suis un passager du voyage aller, sinon 1 (pour un vol retour) 
  
  char * couleurTrace = "\x1b[38;5;226m";  // ne pas modifier ni déplacer
  if (aller_ou_retour) couleurTrace="\x1b[38;5;46m";  // ces deux lignes.

  struct sembuf op[] = {
    {aller_ou_retour, -1, 0}, //entrée/sortie de l'avion
    {aller_ou_retour, 0, 0}, //attente
  };

  afficher('p', "Yoopi, je vais prendre l'avion !", destination, couleurTrace);
   
  afficher('p', "je patiente en salle d'embarquement", destination, couleurTrace);

  

  if (semop(idSem, op, 1) == -1) {
    perror("Erreur embarquement semop()");
    exit(1);
  }

  if (semop(idSem, op+1, 1) == -1) {
    perror("Erreur synchro semop()");
    exit(1);
  }
  
  afficher('p', "j'embarque ...", destination, couleurTrace);
  

  action(); // simulation de l'action embarquer.
 
   
  // ici se produit la simulaion du vol. je n'ai rien à faire
   
  afficher('p', "maintenant, je patiente jusqu'à destination", destination, couleurTrace);
 
  if (semop(idSem, op, 1) == -1) {
    perror("Erreur débarquement semop()");
    exit(1);
  }
  
  afficher('p', "je descends de l'avion", destination, couleurTrace);

  action(); // simulation de l'action débarquer.

  if (semop(idSem, op+1, 1) == -1) {
    perror("Erreur synchro2 semop()");
    exit(1);
  }
    
  afficher('p', "je quitte l'aéroport !", destination, couleurTrace); 
 
  return 0;
}


 
