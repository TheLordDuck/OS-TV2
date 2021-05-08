
/********************************
 * File : threads.c
 * Thread creation with pthread()
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 5



void *Hello_thread(void *thread_nb) {
  long t_nb = (long) thread_nb;

  printf("--Hello !!-- I'm thread #%ld, my p_id is %d ; my id is %lu\n"
	 , t_nb,  getpid(), pthread_self());
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t my_threads[NUM_THREADS];
  long l;int err_code;
    
  for(l = 0 ; l < NUM_THREADS; l++){
    printf ("in main, creating thread #%ld \n", l);
    err_code = pthread_create(&my_threads[l], NULL, Hello_thread, (void *) l);
    if (err_code) {
      fprintf(stdout, "ERROR: creating thread #%ld :%d", l,  err_code);
	exit(-1);
    }
  }
  printf(" I'm initial thread, pid %d. %lu\n", getpid(), pthread_self());
  pthread_exit(NULL);
}
