/*
  Programme avion à compléter. Les zones à compléter sont indiquées et il n'est pas nécessaire d'ajouter de nouvelles traces d'exécution.
  
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

  initDefault(10); // ne pas modifier ni déplacer ces
  char * couleurTrace = "\x1b[38;5;39m"; // deux lignes.
    
  if (argc!=7) {
    printf("Nbre d'args invalide, utilisation :\n");
    printf("%s destination_aller nombre_passagers_aller destination_retour nombre_passagers_retour fichier-pour-cle-ipc entier-pour-cle-ipc\n", argv[0]);
    exit(0);
  }
	
  int nbSem = 2;

  char * dest_aller = argv[1];
  int nbPassagers_Aller = atoi(argv[2]);
    
  char * dest_retour = argv[3];
  int nbPassagers_Retour = atoi(argv[4]);
  
  int cle=ftok(argv[5], atoi(argv[6]));

  int idSem = semget(cle, nbSem, IPC_CREAT | IPC_EXCL | 0600);
    if(idSem == -1){
      perror("semget()");
      exit(-1);
    }
   
printf("Semaphore cree. Id: %d\n", idSem);



  // INITIALISATION DE LA SEMAPHORE 2 CASES: [ places_aller ] [ places_retour ]
  // Au debut la case retour est vide, pour ne pas embarquer trop tot
  ushort tabinit[2] = {atoi(argv[1]), 0};

  union semun {
    int val;
    struct semid_ds * buf;
    ushort * array;
  } valinit;
  
  valinit.array = tabinit;

  if (semctl(idSem, nbSem, SETALL, valinit) == -1){
    perror("initialization semaphore semctl()");
    exit(1);
  }


  struct sembuf op[] = {
    {0, 0, 0},              // attente embarquement (case 0)
    {0, nbPassagers_Aller, 0},  // reset nbr places aller
    {1, 0, 0},              // attente debarquement (case 1)
    {1, nbPassagers_Retour, 0},  // reset nbr places retour
  };


  
  // vol aller 
  afficher('a', "embarquement immediat !", dest_aller, couleurTrace);
   
  if (semop(idSem, op+1, 1) == -1) {
    perror("Erreur annonce embarquement aller semop()");
    exit(1);
  }
  
  afficher('a', "attente fin embarquement", dest_aller, couleurTrace);    

  if (semop(idSem, op, 1) == -1) {
    perror("Erreur attente embarquement aller semop()");
    exit(1);
  }
    
  afficher('a',"décolage imminent! Durée du vol estimée à quelques secondes", dest_aller, couleurTrace); 
 
  vol(4); // simulation d'une durée de vol (vous pouvez tester avec d'autres valeurs. Le code rendu doit toutefois être vol(4); 
  
  afficher('a',"arrivé à destination", dest_aller, couleurTrace);


  if (semop(idSem, op+1, 1) == -1) {
    perror("Erreur annonce débarquement aller semop()");
    exit(1);
  }

    
  afficher('a', "attente que tout le monde soit descendu", dest_aller, couleurTrace); 
  
  if (semop(idSem, op, 1) == -1) {
    perror("Erreur attente débarquement aller semop()");
    exit(1);
  }

  afficher('a', "vol aller terminé", dest_aller, couleurTrace); 
 
  // vol retour 
  afficher('a', "embarquement immediat !", dest_retour, couleurTrace);

  if (semop(idSem, op+3, 1) == -1) {
    perror("Erreur annonce embarquement retour semop()");
    exit(1);
  }
  
  afficher('a', "attente fin embarquement", dest_retour, couleurTrace);    

  if (semop(idSem, op+2, 1) == -1) {
    perror("Erreur attente embarquement retour semop()");
    exit(1);
  }
    
  afficher('a',"décolage imminent! Durée du vol estimée à quelques secondes", dest_retour, couleurTrace); 
 
  vol(4); // simulation d'une durée de vol (vous pouvez tester avec d'autres valeurs. Le code rendu doit toutefois être vol(4); 
  
  afficher('a',"arrivé à destination", dest_retour, couleurTrace);


  if (semop(idSem, op+3, 1) == -1) {
    perror("Erreur annonce débarquement retour semop()");
    exit(1);
  }

    
  afficher('a', "attente que tout le monde soit descendu", dest_retour, couleurTrace); 
  
  if (semop(idSem, op+2, 1) == -1) {
    perror("Erreur attente débarquement retour semop()");
    exit(1);
  }


  afficher('a', "vol retour terminé", dest_retour, couleurTrace); 
  return 0;
      
}

