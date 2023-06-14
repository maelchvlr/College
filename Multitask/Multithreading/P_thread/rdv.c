#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "obj/calcul.h"


struct shared_variables {

  int waitingthread;
  pthread_mutex_t lock;
  pthread_cond_t all_threads_done;

};

struct params {

  // structure pour regrouper les parametres d'un thread. 
  int idThread; // un identifiant de thread, de 1 a N (N le nombre total de theads secondaires
  int num;
  struct shared_variables *shared_variables;
  
};

// thread function for every participant
void *participant (void *arguments){ 

  struct params *local_variables = (struct params *) arguments;
  struct shared_variables *shared_variables = local_variables -> shared_variables;

  // simulation d'un long calcul pour le travail avant RdV
  
  calcul(1); // c'est un exemple.

  shared_variables -> waitingthread++;

  printf("[-] Locking\n");
  if(pthread_mutex_lock(&shared_variables -> lock) != 0){
    perror("[X] lock_mutex()"); exit(1);
  }
  
  if (shared_variables -> waitingthread == local_variables -> num) {
    printf("[-] Everyone is done i'm working again (idThread: %d)\n", local_variables -> idThread);
  }
  
  while (shared_variables -> waitingthread <  local_variables -> num) {  // attention : le dernier arriv� ne doit pas attendre. Il doit r�veiller tous les autres.
    printf("[-] idThread: %d, waiting: %d\n", local_variables -> idThread, (local_variables -> num - shared_variables -> waitingthread));
    pthread_cond_wait(&shared_variables -> all_threads_done, &shared_variables->lock);
  }


  if(pthread_cond_broadcast(&shared_variables -> all_threads_done) != 0){
    perror("[X] broadcast()"); exit(1);
  }
  
  calcul(1);

  printf("[-] Unlocking\n");
  if(pthread_mutex_unlock(&shared_variables -> lock) != 0){
    perror("[X] unlock_mutex()"); exit(1);
  }

  pthread_exit(NULL); // fortement recommand�.
}

int main(int argc, char * argv[]){
  
  if (argc!=2) {
    printf("Usage: %s thread_number\n", argv[0]); exit(1);
  }

  struct shared_variables shared_variables = { 
    .waitingthread = 0
  };

  if(pthread_mutex_init(&shared_variables.lock, NULL) != 0){
    perror("[X] lock mutex_init()"); exit(1);
  }
  if(pthread_cond_init(&shared_variables.all_threads_done, NULL) != 0){
    perror("[X] cond_init()"); exit(1);
  }
  

  // initialize 
  pthread_t threads[atoi(argv[1])];
  struct params params[atoi(argv[1])];

  srand(atoi(argv[1]));  // initialisation de rand pour la simulation de longs calculs

  // threads creation
  for (int i = 0; i < atoi(argv[1]); i++){
    params[i].idThread = i;
    params[i].shared_variables = &shared_variables;
    params[i].num = atoi(argv[1]);

    if (pthread_create(&threads[i], NULL, &participant, &params[i]) != 0){
      perror("[X] thread create()");
      exit(1);
    }
  }
  // wait for other threads to end 
  for (int i = 0; i < atoi(argv[1]); i++){
    if(pthread_join(threads[i], NULL) != 0){
      perror("[X] thread join)"); exit(1);
    }
  }
  printf("thread principal : fin de tous les threads secondaires\n");

  // terminer "proprement". 
  
  if(pthread_mutex_destroy(&shared_variables.lock) != 0){
    perror("[X] mutex_destroy()"); exit(1);
  }

  if(pthread_cond_destroy(&shared_variables.all_threads_done) != 0){
    perror("[X] cond_destroy()"); exit(1);
  }
  
  
  
}

