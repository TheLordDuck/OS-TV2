/********************************
 * File : sempahore.c
 * Threads share variables with semaphores
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#include <sys/sem.h>
#define KEY  12

int sem_id;
int sh_var;
struct sembuf operation;


void *mutex_add(void *thread_nb) {
  long thr_nb = (long) thread_nb;
  
  // Operation
  operation.sem_num = 0;
  operation.sem_op = -1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  sh_var+= 10;
  printf("Thread #%ld: son, op=%d.\n", thr_nb, sh_var);
  sh_var+=20;
  printf("Thread #%ld: son, op=%d.\n", thr_nb, sh_var);
  
  
  //operation V
  operation.sem_num = 0;
  operation.sem_op = 1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  exit(EXIT_SUCCESS);
}
  


int main(int argc, char *argv[])

{
  pthread_t my_thread;
  long thread_nb=3;
  sh_var=0;
  

  // Semaphore creation and intializated @ value 1
  if ((sem_id = semget(KEY, 1, IPC_CREAT|IPC_EXCL|0600)) == -1)
    perror("Error semget");
  if ((semctl (sem_id, 0, SETVAL , 1)) == -1)
    perror("Error semctl");
  
  //thread creation
  if ((pthread_create(&my_thread, NULL, mutex_add, (void*) thread_nb)) == -1)
    perror("Error pthread_crate");

  thread_nb=1;
  // Operation P
  operation.sem_num = 0;
  operation.sem_op = -1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  sh_var+= 1000;
  printf("Thread #%ld: father, op=%d\n", thread_nb, sh_var);
  sh_var+=2000;
  printf("Thread #%ld: father, op=%d\n", thread_nb, sh_var);


  //operation V
  operation.sem_num = 0;
  operation.sem_op = 1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  pthread_join(my_thread, NULL);
  semctl(sem_id, 0 , IPC_RMID,0);
}
