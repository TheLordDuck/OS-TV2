#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

int main() {
   int *shared = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
   int *num_code = mmap(NULL, 3*sizeof(char), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
   int *num_readers = mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
   sem_t mutex, db;
   int ret = sem_init(&mutex, 1, 1);
   int ret2 = sem_init(&db, 1, 1);

   pid_t pid;

   printf("Value of Shared int %d\n", *shared);

   int i;
   for(i=0;i<4;i++){
      pid=fork();
      if(pid==0){
         while(1){
            sem_wait(&mutex);
            *num_readers++;
            if(*num_readers==1){
               sem_wait(&db);
            }
            sem_post(&mutex);
            //read
	    sleep(1);
            printf("Child (%d) reads counter (%d) code (%d)\n",getpid(),*shared,*num_code);
            //
            sem_wait(&mutex);
            *num_readers--;
            if(*num_readers==0){
               sem_post(&db);
            }
            sem_post(&mutex);
         }
	 exit(0);
      }
   }

   int x = 0;

   while(1){
      sem_wait(&db);
      //write
      sleep(1);
      *shared = *shared + 1;
      if(x==0){
         *num_code = 200;
      } else if(x==1){
         *num_code = 300;
      } else if(x==2){
         *num_code = 400;
      }
      x++;
      if(x==3){
         x=0;
      }
      printf("Father (%d) modify counter (%d) modify code (%d)\n",getpid(), *shared, *num_code);
      *shared = 0;
      printf("Father (%d) resets counter (%d)\n", getpid(),*shared);
      //
      sem_post(&db);
   }

   for(i=0;i<4;i++){
      wait(NULL);
   }

   sem_destroy(&mutex);
   sem_destroy(&db);
}
