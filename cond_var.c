/********************************
 * File : cond_var.c
 * use of condition var to synchronize threads
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 5

pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t my_condition = PTHREAD_COND_INITIALIZER;
int my_counter; 


void *addfunction(void *thread_nb) {
  long t_nb = (long) thread_nb;


  //lock mutex and update variable
  pthread_mutex_lock(&my_mutex);
  printf("child\n"); 
  
  my_counter += 10;
  printf("\n--Thread #%ld; id  %ld ; adding 10 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  my_counter += 20;
  printf("--Thread #%ld; id  %ld ; adding 20 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  pthread_cond_signal(&my_condition);
  pthread_mutex_unlock(&my_mutex);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t my_thread;
  long l=0;
  
  if (pthread_create(&my_thread, NULL, addfunction, (void*) l) == -1)
    perror("ERROR : pthread create");
  
  l++;
  printf("Parent -- \n");
  //lock parent mutex and update variable
  pthread_mutex_lock(&my_mutex);
  pthread_cond_wait(&my_condition, &my_mutex);
  
  my_counter+= 1000;
  printf("\n--Thread #%ld; id  %ld ; adding 1000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  my_counter += 2000;
  printf("--Thread #%ld; id  %ld ; adding 2000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  
 
  
  pthread_mutex_unlock(&my_mutex);
  pthread_join(my_thread, NULL);
  printf("\n Child joined. now Thread #%ld; id %ld finishing \n",
	 l , pthread_self());
  pthread_mutex_destroy(&my_mutex);
  pthread_cond_destroy(&my_condition);
}
