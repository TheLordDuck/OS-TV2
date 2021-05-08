/********************************
 * File my_handlers.c
  ********************************/
#include <sys/types.h>		// getpid
#include <signal.h>		// signal
#include <string.h>		// strsignal
#include <stdio.h>		// fprintf
#include <unistd.h>		// getpid


void NewHandler(int Signal_nb)
{
  fprintf(stderr, "(%d): Signal(%d) %s received, but reaction stopped. \n",
	  getpid(), Signal_nb, strsignal(Signal_nb));
}


void InitHandlers()
{
  int Signal_nb;

  typedef void (*sighandler_t) (int);
  sighandler_t returned_value_signal;

  for (Signal_nb = 1; Signal_nb < 32; Signal_nb++)
    returned_value_signal = signal(Signal_nb, NewHandler);

  if (returned_value_signal == SIG_ERR) {
    fprintf(stderr, "!!Signal %s (%d) not captured!!\n",
	    strsignal(Signal_nb),Signal_nb);
  }
}


void ResetHandlers2Default()
{
  int Signal_nb;
  for (Signal_nb = 1; Signal_nb < 32; Signal_nb++) {
    signal(Signal_nb, SIG_DFL);
  }
  // Failure on SIGSTOP et SIGKILL but no functional consequence here.
  }
