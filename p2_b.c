#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>

int shared, num_code, num_readers, x;
pthread_mutex_t mutex;
sem_t db;

void* writer(void* id){
   int c = 0;
   int i = *(int*)id;
   while(c < 5){
      sem_wait(&db);
      //write
      sleep(1);
      shared++;
      if(x==0){
         num_code = 200;
      } else if(x==1){
         num_code = 300;
      } else if(x==2){
         num_code = 400;
      }
      x++;
      if(x==3){
         x=0;
      }
      printf("Father thread (%d) modify counter (%d) modify code (%d)\n", i, shared, num_code);
      shared = 0;
      printf("Father thread (%d) resets counter (%d)\n", i, shared);
      //
      sem_post(&db);
      c++;
   }
}

void* reader(void* id){
   int c = 0;
   int i = *(int*)id;
   while(c < 5){
      pthread_mutex_lock(&mutex);
      num_readers++;
      if(num_readers==1){
         sem_wait(&db);
      }
      pthread_mutex_unlock(&mutex);
      //read
      sleep(1);
      printf("Thread (%d) reads counter (%d) code (%d)\n", i, shared, num_code);
      //
      pthread_mutex_lock(&mutex);
      num_readers--;
      if(num_readers==0){
         sem_post(&db);
      }
      pthread_mutex_unlock(&mutex);
      c++;
   }
}

int main(){
   int ret = sem_init(&db, 1, 1);
   pthread_mutex_init(&mutex, NULL);
   pthread_t r[4], w;

   int i;

   int id[5];

   for(i=0;i<4;i++){
      id[i] = i;
      pthread_create(&r[i], NULL, &reader, &id[i]);
   }

   id[4] = 4;

   pthread_create(&w, NULL, &writer, &id[4]);

   for(i=0;i<4;i++){
      pthread_join(r[i],NULL);
   }

   pthread_join(w, NULL);

   sem_destroy(&db);
}
