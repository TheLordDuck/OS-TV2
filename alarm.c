/*********************************************
 * File pause - waits for a signal to start  *
 *********************************************/
#include <stdio.h>		// fprintf
#include <unistd.h>		// pause,sleep
#include <stdlib.h>		// exit
#include <signal.h>             // signal
#include <fcntl.h>              // open
#include <string.h>             // strlen strsignal
void  waiting_ends();
int fd; // global var now

int main(void)
{
  char string[10];
 
  // process prepares signal and waits for it
  if( signal(SIGINT, waiting_ends) == SIG_ERR ) {
    perror ("!error signal:");
    exit(1);
  }
  // process prepares signal and waits for it
  if( signal(SIGALRM, waiting_ends) == SIG_ERR ) {
    perror ("!error signal:");
    exit(1);
  }

  pause(); //pause
  printf("now i'm gonna work!\n");
  fd = open("pausingfile.txt", O_WRONLY);
  alarm(5); // timer 5 seconds
  printf("give me a string: ");
  scanf("%s", string);
  alarm(0); // cancels timer
  write(fd , string, strlen(string));
  close(fd);
  exit(0);
}

void waiting_ends(int num_sig) {
  if (num_sig == SIGINT)
    printf(" Received signal %s, end of the delay.\n",strsignal(SIGINT));
  else {
    printf("\n --> too late, i'm not waiting any longer.\n");
    close(fd);
    exit(1);
  } 
}
 
