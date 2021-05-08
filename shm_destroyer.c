/********************************/
// File: shm_cdestroyer.c
// Reader process of the Shared memory region and destroyer

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <sys/shm.h>		/* shm system calls*/ 
#define KEY 256

int main(void)
{
  int shm_id;
  char *memory_address;
  
  // get the internal id fo the sahred region
  if ((shm_id = shmget((key_t)KEY, 0,0 )) == -1) {
    perror("shmget");
    exit(1);
  }

  // region attachment to the process
  if ((memory_address = shmat(shm_id, NULL, 0)) == (char*)-1)  {
    perror("shmat");
    exit(2);
  }
  printf(" reading region : %s\n", memory_address);
  
  // region detachment
  if (shmdt(memory_address)) {
      perror("shmdt");
      exit(3);
  }

  // region destruction
  shmctl( shm_id, IPC_RMID, NULL);
  
  exit(0);
}

    

     
 
