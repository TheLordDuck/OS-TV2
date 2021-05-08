/********************************
 * File my_shell.c
 *********************************/
#include <stdio.h>		// fprintf
#include <unistd.h>		// sleep
#include <stdlib.h>		// exit

int main(void)
{

  int fd;
  int pid;
  int current_output;

  if ( pid == 0) {
    int fd0, fd1, input=o, output=0;
    char input[64], output[64];

    for (i=0, argv[i]!="\0"; i++)
      {
	if (strcomp(argv[i],"<")==0)
	  {
	    argv[i]=NULL;
	    strcpy(input, argv[i+1]);
	    in=2;
	  }
	if (strcomp(argv[i],"<")==0)
	  {
	    argv[i]=NULL;
	    strcpy(input, argv[i+1]);
	    in=2;
	  }
      }
    
  
    if (in)  //if < char was found
      {
	int fd0;
	if (fd0 = open(input, O_RDONLY,0) < 0) {
	  perror("Coulddn't open input file");
	  exit(EXIT_FAILURE);
	}
	// copies fdo content on input of precedin file 
	dup2(fd0,STDIN_FILENO);
	close(fd0);
      }

     if (out)  //if > char was found
      {
	int fd1;
	if (fd1 = creat(output, 0640, 0) < 0) {
	  perror("Coulddn't open input file");
	  exit(EXIT_FAILURE);
	}
	// copies fdo content on input of precedin file 
	dup2(fd0,STDIN_FILENO);
	close(fd0);
      }

    
    
