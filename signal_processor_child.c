/********************************
 * File signal_processor_child.c
  ********************************/
#include <sys/types.h>		// getpid
#include <signal.h>		// signal, kill
#include <unistd.h>		// pause, fork, exit, getpid
#include <stdio.h>		// fprintf
#include <stdlib.h>		// srand, rand
#include <time.h>		// time
#include "my_handlers.h"        // code in my_handlers.c

int main(void)
{
  pid_t parent_pid;
  pid_t child_pid;
  int i;

  parent_pid = getpid();
  InitHandlers();   // get Handlers to write on stderr for parent process

  child_pid = fork();
  if (child_pid == 0) {	        // Child code
    ResetHandlers2Default();	// reset default behavior to handlers
    for (i = 0; i < 20; i++) {
      sleep(1);
      srand((unsigned int)time((time_t *) NULL)); 
      kill(parent_pid, (rand() % 31) + 1); //send a random signal
    }
    kill(parent_pid, SIGKILL);	// last  signal, SIGKILL for parent termination
    exit(EXIT_SUCCESS);
  } else  {		// Parent Code
    while (1) pause();  // Busy waiting in order to receive signals
  }
}
