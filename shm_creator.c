/********************************/
// File: shm_creator.c
// Creator process of the Shared memory region and writer

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* getpid */
#include <string.h>             /* strcpy */

#include <sys/shm.h>		/* shm system calls*/ 
#define KEY 256

int main(void)
{
  int shm_id;
  char *memory_address;
  
  // process creare a shared memory creation associated to its adress space
  if ((shm_id = shmget((key_t)KEY, 1000, 0750 | IPC_CREAT | IPC_EXCL)) == -1) {
    perror("shmget");
    exit(1);
  }

  // attachment to current process
  if ((memory_address = shmat(shm_id, NULL, 0)) == (char*)-1)  {
    perror("shmat");
    exit(2);
  }

  // writes in the region
  strcpy(memory_address, "First written cahracters in the shared region");
  exit(0);
}

    

     
 
