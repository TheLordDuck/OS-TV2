/********************************
 * Fichier signal_processor.c
  ********************************/
#include <stdio.h>		// fprintf
#include <signal.h>		// signal
#include <unistd.h>		// pause
#include <string.h>		// strsignal

void my_SignalHandler(int);

int main(void)
{
  int Signal_nb;

  // to get the return value of signal primitive
  typedef void (*sighandler_t) (int);
  sighandler_t Signal_return_value;

  for (Signal_nb = 1; Signal_nb < 32; Signal_nb++)
    {
      fprintf(stdout,"Signal %d (%s) ready \n",Signal_nb, strsignal(Signal_nb));
      Signal_return_value = signal(Signal_nb, my_SignalHandler);
      if (Signal_return_value == SIG_ERR) {
	fprintf(stderr, "UPSSS !! Signal %d (%s) not captured \n",
		Signal_nb, strsignal(Signal_nb));
      }
    }
  while (1) // Busy waiting on signal recpetion
    pause();
}


// Signal management - handler definition
void my_SignalHandler(int Signal_nb)
{
  fprintf(stderr,"(%d) I recieved signal %d (%s), reaction aborted. \n",
		getpid(), Signal_nb, strsignal(Signal_nb));
}
