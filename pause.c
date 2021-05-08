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

int main(void)
{
  char string[10];
  int fd;
  
  // process prepares signal and waits for it
  if( signal(SIGINT, waiting_ends) == SIG_ERR ) {
    perror ("!error signal:");
    exit(1);
  }

  pause(); //pause
  printf("now i'm gonna work!\n");
  printf("give me a string: ");
  scanf("%s", string);

  fd = open("pausingfile.txt", O_WRONLY);
  write(fd , string, strlen(string));
  close(fd);
  exit(0);
}

void waiting_ends() {
  printf(" Received signal %s, end of the delay.\n",strsignal(SIGINT));
}
 
