/********************************
 * File countered.c block signal*
 ********************************/
#include <stdio.h>		// fprintf
#include <unistd.h>		// pause,sleep
#include <stdlib.h>		// exit
#include <signal.h>             // signal
#include <string.h>             // strsignal
void new_handler(int);

int main(void)
{
  int i;

  // return value for signal
  typedef void(*sighandler_t) (int);
  sighandler_t return_value_signal;
  return_value_signal= signal(SIGINT,new_handler);
  if (return_value_signal == SIG_ERR){
    fprintf(stderr, "Signal SIGINT not captured\n");
  }

    
  for (i = 1; i < 31; i++) {
		printf("Count %d\n", i);
		sleep(1);
	}
	exit(EXIT_SUCCESS);
}

//New signal Handler Definition
void new_handler(int signal_number){
  if (signal_number == SIGINT) {
    printf ("Countered! SIGINT signal received and blocked.\n");
  }
}
