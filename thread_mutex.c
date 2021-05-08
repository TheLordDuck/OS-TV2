
/********************************
 * File : thread_mutex.c
 * Threads share variables but lock updates / several outcomes
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 3
pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;
int my_counter; 


void *addfunction(void *thread_nb) {
  long t_nb = (long) thread_nb;

  //lock mutex and update variable
  pthread_mutex_lock(&my_lock);
  my_counter += 10;
  printf("\n--Thread #%ld; id  %ld ; adding 10 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  my_counter += 20;
  printf("--Thread #%ld; id  %ld ; adding 20 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  pthread_mutex_unlock(&my_lock);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t my_threads[NUM_THREADS];
  long l=0;
    
  for(l = 0 ; l < NUM_THREADS; l++){
    if (pthread_create(&my_threads[l], NULL, addfunction, (void *) l) == -1)
      perror("ERROR : pthread create");
  }

  
  //lock parent mutex and update variable
  pthread_mutex_lock(&my_lock);
  my_counter+= 1000;
  printf("\n--Thread #%ld; id  %ld ; adding 1000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  my_counter += 2000;
  printf("--Thread #%ld; id  %ld ; adding 2000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  pthread_mutex_unlock(&my_lock);

  for(l = 0 ; l < NUM_THREADS; l++){
    pthread_join(my_threads[l], NULL);
  }
  //
  printf("\n Children join father. now Thread #%ld; id %ld finishing \n",
 		 l , pthread_self());
  pthread_mutex_destroy(&my_lock);
}
