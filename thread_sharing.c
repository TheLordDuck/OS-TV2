
/********************************
 * File : thread_sharing.c
 * Threads share variables
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 5


long my_var=0;


void *addfunction(void *thread_nb) {
  long t_nb = (long) thread_nb;

  my_var +=10;
  printf("--Hello !!-- I'm child thread; my pid is %d my id is %ld ; my_var is %ld \n",
	 getpid(),pthread_self(), t_nb);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t my_threads[NUM_THREADS];
  long l ;int err_code;
    
  for(l = 0 ; l < NUM_THREADS; l++){
    err_code = pthread_create(&my_threads[l], NULL, addfunction, (void *) my_var);
    if (err_code) {
      fprintf(stdout, "ERROR: creating thread #%ld :%d", l,  err_code);
	exit(-1);
    }
  }
  my_var +=1000;
  printf(" I'm parent thread, pid %d. id %lu  my_var: %lu \n",
	 getpid(), pthread_self(), my_var);
  pthread_exit(NULL);
}
