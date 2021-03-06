allocation.c                                                                                        0000644 0001750 0001750 00000001667 13441510137 012034  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  allocation.c
// Program creates a file and store contact type data in the file
// File contains 4 registries for contacts

#include <stdio.h>		/* printf */
#include <unistd.h>             /* close */
#include <stdlib.h>             /* malloc */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
  struct contact {
    char name[10];
    char phone[10];
  };
 
  int fd, i;
  struct contact *my_contact;
   
  fd = open ("/home/ivan/newdirectory.txt", O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
  if (fd == -1)
    perror("Error: open file");

  i = 0;
  while (i < 4) {
    my_contact = ( struct contact* ) malloc (sizeof(struct contact));
    printf(" Enter a name: ");
    scanf("%s", my_contact->name);
    printf(" Enter a phone number: ");
    scanf("%s", my_contact->phone);
    write(fd, my_contact, sizeof(struct contact));
    i++;
  }
  close(fd);
}
 
                                                                         appendfile.c                                                                                        0000644 0001750 0001750 00000003111 13462126206 012003  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /***************************************
 *File: appendfile.c
 ***************************************/

#include <stdio.h>     
#include <stdlib.h>     // exit
#include <unistd.h>     // fork
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUF_SIZE  4096

int main( int argc, char *argv[]){
  int in_fd1, in_fd2,out_fd, write_count, read_count;
  char my_buffer[BUF_SIZE];

  if (argc != 4) perror("!! wrong arguments");
    
  in_fd1 = open(argv[1], O_RDONLY); //open source file1
  if ( in_fd1 < 0 ) perror("!! in_fd");

  in_fd2 = open(argv[2], O_RDONLY); //open source file2
  if ( in_fd2 < 0 ) perror("!! in_fd");

  out_fd = open(argv[3], O_CREAT|O_WRONLY, S_IRUSR | S_IWUSR); // create target file
  if ( out_fd < 0 ) perror("!! out_fd");

  while(1) { // Copying fd1
    read_count = read(in_fd1, my_buffer, BUF_SIZE); //read a data block
    if (read_count <=0) break;
    printf("my buffer1: %s",my_buffer);
    write_count = write(out_fd, my_buffer, read_count);
    printf("write count1: %d\n", write_count);
    if (write_count <=0) perror("writing error");
  }
   while(1) { // Copying fd1
    read_count = read(in_fd2, my_buffer, BUF_SIZE); //read a data block
    if (read_count <=0) break;
    printf("my buffer2: %s",my_buffer);
    write_count = write(out_fd, my_buffer, read_count);
    printf("write count2: %d\n", write_count);
    if (write_count <=0) perror("writing error");
  }


  
  close(in_fd1);
  close(in_fd2);
  close(out_fd);
  if (read_count == 0) exit(0); // no error on last read
  else perror("!! last read");  // error on last read	       
 
}

                                                                                                                                                                                                                                                                                                                                                                                                                                                       bi_npipes_client.c                                                                                  0000644 0001750 0001750 00000002047 13440322727 013213  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  bi_npipes_client.c
// Client send two integers to a server by pipe1
// and receive addition thougth pipe2

#include <stdio.h>		/* printf */
#include <unistd.h>             /* write read */
#include <stdlib.h>             /* atoi */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  char integers[2];
  int nb1, nb2, result;
  int n_pipe1, n_pipe2;
    
  n_pipe1 = open ("n_pipe1", O_WRONLY);    // open the FIFO Write
  n_pipe2 = open ("n_pipe2", O_RDONLY);    // open the FIFO Read

  // Client ask for numbers
  printf  ("Entrer two number to add:\n");
  scanf   ("%d %d", &nb1, &nb2);
  sprintf (integers, "%d", 2);
  write   (n_pipe1, integers, 2); // write in pipe
  sprintf (integers, "%d", 2);
  write   (n_pipe1, integers, 2); // write in pipe
  

  // read the result
  read(n_pipe2, integers, 2);
  result = atoi(integers);
  printf("The received result is %d\n", result);

  // close pipes
  close (n_pipe1);
  close (n_pipe2);
  unlink(n_pipe1);
  unlink(n_pipe2);
}
	
 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         bi_npipes_server.c                                                                                  0000644 0001750 0001750 00000001734 13440322443 013240  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  bi_npipes_server.c
// Server recieves two integers to a server by pipe1
// and send addition thougth pipe2

#include <stdio.h>		/* printf */
#include <unistd.h>             /* read, write */
#include <stdlib.h>             /* atoi */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  char integers[2]; int nb1,nb2,result;
  mode_t mode;
  int n_pipe1, n_pipe2;
  mode = S_IRUSR | S_IWUSR;

  mkfifo("n_pipe1", mode);               // named-pipe creation
  mkfifo("n_pipe2", mode);

  
  n_pipe1 = open ("n_pipe1", O_RDONLY);    // open the FIFO Read
  n_pipe2 = open ("n_pipe2", O_WRONLY);    // open the FIFO Write

  // read the tow integerss
  read(n_pipe1, integers, 2);
  nb1 = atoi(integers);
  read(n_pipe1, integers, 2);
  nb2 = atoi(integers);

  result = nb1 + nb2;
  sprintf (integers , "%d", result);
  write   (n_pipe2, integers, 2);

  // close pipes
  close (n_pipe1);
  close (n_pipe2);
}
	
 
                                    capture_signaux_SIGINT_ole.c                                                                        0000755 0001750 0001750 00000002122 13424264743 015024  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * Fichier capture_signaux_SIGINT_ole.c
 * Livre "Exercices corrig??s de d??veloppement syst??me sous Linux"
 *
 * (c) 2018 P. Alain, J. Chevelu, V. Thion
 *
 ********************************/
#include <stdio.h>		// fprintf
#include <signal.h>		// signal
#include <unistd.h>		// pause, sleep
#include <string.h>		// strsignal
#include <stdlib.h>		// exit

void nouveau_gestionnaire(int);

int main(void)
{
	int i;

	// Pour r??cup??rer la valeur retour de la primitive signal
	typedef void (*sighandler_t) (int);
	sighandler_t return_value_signal;

	// Affectation du nouveau gestionnaire
	return_value_signal = signal(SIGINT, nouveau_gestionnaire);
	if (return_value_signal == SIG_ERR) {
		fprintf(stderr, "Signal SIGINT non captur??\n");
	}
	
	// Travail courant du processus
	for (i = 1; i < 31; i++) {
	  printf("%d\n", i);
	  sleep(1);
	}
	exit(EXIT_SUCCESS);
}

// Definition du nouveau gestionnaire de signal, celui-ci a pour param??tre le signal l'ayant d??clench??
void nouveau_gestionnaire(int numSignal)
{
	if (numSignal == SIGINT) {
		printf("Ol?? !\n");
	}
}
                                                                                                                                                                                                                                                                                                                                                                                                                                              cond_var.c                                                                                          0000644 0001750 0001750 00000003346 13440236131 011474  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * File : cond_var.c
 * use of condition var to synchronize threads
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 1

pthread_mutex_t my_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t my_condition = PTHREAD_COND_INITIALIZER;
int my_counter; 


void *addfunction(void *thread_nb) {
  long t_nb = (long) thread_nb;


  //lock mutex and update variable
  pthread_mutex_lock(&my_mutex);
  printf("child\n"); 
  
  my_counter += 10;
  printf("\n--Thread #%ld; id  %ld ; adding 10 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  my_counter += 20;
  printf("--Thread #%ld; id  %ld ; adding 20 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  pthread_cond_signal(&my_condition);
  pthread_mutex_unlock(&my_mutex);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t my_thread;
  long l=0;
  
  if (pthread_create(&my_thread, NULL, addfunction, (void*) l) == -1)
    perror("ERROR : pthread create");
  
  l++;
  printf("Parent -- \n");
  //lock parent mutex and update variable
  pthread_mutex_lock(&my_mutex);
  pthread_cond_wait(&my_condition, &my_mutex);
  
  my_counter+= 1000;
  printf("\n--Thread #%ld; id  %ld ; adding 1000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  my_counter += 2000;
  printf("--Thread #%ld; id  %ld ; adding 2000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  
 
  
  pthread_mutex_unlock(&my_mutex);
  pthread_join(my_thread, NULL);
  printf("\n Child joined. now Thread #%ld; id %ld finishing \n",
	 l , pthread_self());
  pthread_mutex_destroy(&my_mutex);
  pthread_cond_destroy(&my_condition);
}
                                                                                                                                                                                                                                                                                          copyfile.c                                                                                          0000644 0001750 0001750 00000002177 13462530450 011521  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /***************************************
 *File: fd_copyfile.c
 ***************************************/

#include <stdio.h>     
#include <stdlib.h>     // exit
#include <unistd.h>     // fork
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define BUF_SIZE  4096

int main( int argc, char *argv[]){
  int in_fd, out_fd, write_count, read_count;
  char my_buffer[BUF_SIZE];

  if (argc != 3) perror("!! wrong arguments");
    
  in_fd = open(argv[1], O_RDONLY); //open source file
  if ( in_fd < 0 ) perror("!! in_fd");

  out_fd = open(argv[2], O_CREAT|O_WRONLY, S_IRUSR | S_IWUSR); // create target file
  if ( out_fd < 0 ) perror("!! out_fd");

  while(1) { // Copying
    read_count = read(in_fd, my_buffer, BUF_SIZE); //read a data block
    if (read_count <=0) break;
    printf("my buffer: %s",my_buffer);
    write_count = write(out_fd, my_buffer, read_count);
    printf("write count: %d\n", write_count);
    if (write_count <=0) perror("writing error");
    
  }
  
  close(in_fd);
  close(out_fd);
  if (read_count == 0) exit(0); // no error on last read
  else perror("!! last read");  // error on last read	       
 
}

                                                                                                                                                                                                                                                                                                                                                                                                 directory.c                                                                                         0000644 0001750 0001750 00000001511 13444705546 011714  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File: filestat.c
// print inode data

#include <stdio.h>		/* printf */
#include <unistd.h>		/* fork */
#include <sys/stat.h>		/* stat*/
#include <sys/types.h>          /* struct stat*/
#include <fcntl.h>              /* open, write */
#include <dirent.h>             /* struc dirent */

int main(void)
{
  char dir_name[20];
  struct dirent *my_dir;   // directory entry 3 fields
  DIR *dir_fd;             // type DIR din <dirent.h> open directory descripor

  getcwd(dir_name, 20);
  printf ("My current directory is %s \n", dir_name);
  
  dir_fd = opendir(dir_name);
  my_dir= readdir(dir_fd);

  while (my_dir != NULL){
    printf(" Directory entry %s, i-node %d, size %d \n
",
	   my_dir->d_name, (int)my_dir->d_ino, my_dir->d_reclen);
    my_dir=readdir(dir_fd);
  }
  closedir(dir_fd);
}
		  
 
                                                                                                                                                                                       dup.c                                                                                               0000644 0001750 0001750 00000002242 13464104470 010471  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  mydouble pipe
// command ls -l | head | tail -4

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */
#include <wait.h>		/* wait */ 
#define MAX_ARGS 10
#define MAX_CMDS 2014

struct commands[MAX_CMDS]{
  const char *args[MAX_ARGS];
};

void getcmd() {
  commands[0].argv[0] = "ls";
  commands[0].argv[1] = "-l";
  commands[1].argv[0] = "head";
  commands[2].argv[0] = "tail";
  commands[2].argv[1] = "-4";
}
  

int main(void)
{
  int my_pipefd[2];

  if (pipe(my_pipefd)) {
    perror("Error: pipe");
    exit(1);
  }

  switch (fork()) {
  case -1: // error fork
    perror("Error: fork");
    exit(2);
  case 0: // child process execute ls -l
    close (STDOUT_FILENO);   // stdout is redirected to the pipe
    (void) dup(my_pipefd[1]);
    close (my_pipefd[1]);

    close(my_pipefd[0]);       // process do not read the tube
    execlp("ls", "ls", "-l", NULL);
  default:  // parent process execute wc
    close (STDIN_FILENO);  //STDIN redirected to pipe reader
    (void)dup(my_pipefd[0]);
    close(my_pipefd[0]);
    
    close(my_pipefd[1]);
    execlp("wc", "wc", "-l", NULL);
  }
} 
                                                                                                                                                                                                                                                                                                                                                              fd_father.c                                                                                         0000644 0001750 0001750 00000001513 13453123474 011626  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /***************************************
 *File: fd_father.c
 ***************************************/

#include <stdio.h>     
#include <stdlib.h>     // exit
#include <unistd.h>     // fork
#include <sys/wait.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
  int pid, fd, j;
  char message[4], long_msg[24];

  fd = open("StExupery.txt", O_RDONLY);
  if ( fd == -1 ) {
    perror("!Error open:");
    exit(1);
  }
  printf("fd value at opening , %d.\n", fd);

  pid = fork();
  if (pid == 0) { // child code
    sprintf(long_msg, "%d", fd);
    execlp("/home/ivan/read-file","read-file", long_msg, NULL);
    perror("!error exec"); 
  }
  else {
    for (j = 0 ; j < 5; j++){
      read(fd, message, 4);
      printf(" Parent process, reading : %s \n", message);
    }
    wait(NULL);
    close(fd);
  }
}
                                                                                                                                                                                     fd_inheritance.c                                                                                    0000644 0001750 0001750 00000001515 13452732756 012660  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /***************************************
 *File: fd_inheritance.c
 ***************************************/

#include <stdio.h>     
#include <stdlib.h>     // exit
#include <unistd.h>     // fork
#include <sys/wait.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
  int pid, fd, j;
  char message[4];

  fd = open("StExupery.txt", O_RDONLY);
  if ( fd == -1 ) {
    perror("!Error open:");
    exit(1);
  }
  printf("fd value at opening , %d.\n", fd);

  pid = fork();
  if (pid == 0) { // child code
    for (j = 0 ; j < 5; j++){
      read(fd,message, 4);
      printf(" Child process, reading : %s \n", message);
    }
    close(fd);
  }
  else {
    for (j = 0 ; j < 5; j++){
      read(fd, message, 4);
      printf(" Parent process, reading : %s \n", message);
    }
    wait(NULL);
    close(fd);
  }
}
                                                                                                                                                                                   filestat.c                                                                                          0000644 0001750 0001750 00000004225 13453343013 011513  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File: filestat.c
// print inode data

#include <stdio.h>		/* printf */
#include <unistd.h>		/* fork */
#include <sys/stat.h>		/* stat*/
#include <sys/types.h>          /* struct stat*/
#include <fcntl.h>              /* open, write */

int main(void)
{

  struct student {
    char name[10];
    int  grade;
  };

  int fd, my_return; int i = 0;
  struct student my_student;

  fd = open("/home/ivan/my_grades.txt", O_RDWR | O_CREAT, S_IRUSR |S_IWUSR);
  printf("my_fd %d",fd);
  if (fd == -1)
    perror("!error open :");

  // stat file
  struct stat finfo;
  
  fstat(fd, &finfo);
  printf( "File stat. \n");
  printf( "Logical disk. %d \n", (int) finfo.st_dev);
  printf( "i-node. %d \n", (int)finfo.st_ino);
  printf( "Filetype and access rigths. %d \n", (int) finfo.st_mode);
  printf( "Nber of links. %d \n", (int) finfo.st_nlink);
  printf( "File owner. %d \n", (int) finfo.st_uid);
  printf( "File Size. %d \n",(int) finfo.st_size);
  printf( "Last access date. %d \n",(int) finfo.st_atime);
  printf( "Last modification date. %d \n", (int) finfo.st_mtime);
  printf( "Last node modification. %d \n",(int) finfo.st_ctime);
  printf( "Block size. %d \n", (int) finfo.st_blksize);
  printf( "Blocks allocated to file. %d \n\n", (int) finfo.st_blocks);

  if (S_ISBLK(finfo.st_mode)) printf ("block file\n");
  if (S_ISCHR(finfo.st_mode))  printf ("Char file\n");
  if (S_ISDIR(finfo.st_mode))  printf ("Directory file\n");
  if (S_ISFIFO(finfo.st_mode))  printf ("Pipe file\n");
  if (S_ISREG(finfo.st_mode))  printf ("Regular file\n");
 
  
  while (i < 4) {
    printf("Enter a student name: \n");
    scanf ("%s", my_student.name);
    printf("Enter a student grade: \n");
    scanf ("%d", &my_student.grade);
    write (fd, &my_student, sizeof(my_student));
    i++;
  }

  // repositions in filedescriptor
  my_return = lseek (fd, sizeof(my_student), SEEK_SET);
  if (my_return == -1)
    perror("!error lseek");
  printf(" new position is %d\n", my_return);

  i = 0;
  while ( i < 4) {
    read (fd, &my_student, sizeof(my_student));
    printf ("name and grade %s, %d \n", my_student.name, my_student.grade);
    i++;
  }
  close(fd);
}
		  
 
                                                                                                                                                                                                                                                                                                                                                                           firstpipe.c                                                                                         0000644 0001750 0001750 00000001740 13434027066 011712  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  mypipe.c
// retraces command ls -l | wc -l

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */
#include <wait.h>		/* wait */ 

int main(void)
{
  int my_pipe[2], status;
  pid_t child_pid;
  char message[10];

  //opening pipe
  if (pipe(my_pipe)) {
    perror("pipe");
    exit(1);
  }

  switch(fork()) {
  case -1 : /*error*/
    perror(" pb fork");
    exit();
  case 0: /*child process execute Ls-l */
    close(STDOUT_FILENO);   // stdout redirected to the pipe
    (void)dup(my_pipe[1]);     // stdout duplicated into p[1]
    close(my_pipe[1]);
    close(my_pipe[0]);         // child do not read
    execlp("ls", "ls", "-l", NULL);
  default:
    close(STDIN_FILENO);   // stdinput redirected to the pipe
    (void)dup(my_pipe[0]);     // p[1] becomes stdinput of the proces
    close(my_pipe[0]);
    close(my_pipe[1]);         // child do not read
    execlp("wc", "wc", "-l", NULL);
  }
}
                                msq_client.c                                                                                        0000644 0001750 0001750 00000002251 13435303066 012037  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File: msq_server.c
// Client process sends in the MSQ 314

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* getpid */
#include <sys/msg.h>		/* msq system calls*/ 
#define KEY 314

struct request {
  long type;
  int nb1;int nb2;
  pid_t pid;
};

struct answer {
  long type;
  int result;
};

 

int main(void)
{
  int msq_id, l, nb1, nb2;
  struct request my_request;
  struct answer my_answer;

  // gets the MSQ internal id
  if ((msq_id = msgget((key_t)KEY, 0)) == -1) {
    perror("msgget");
    exit(1);
  }

  // ask for two numbers
  printf(" Please, enter two numbers: ");
  scanf("%d %d", &nb1, &nb2);
  
  //prepares the message and send the request
  my_request.type = 1;
  my_request.nb1 = nb1;
  my_request.nb2 = nb2;
  my_request.pid = getpid();

  if (msgsnd(msq_id, &my_request, sizeof(struct request)-4,0) == -1)  {
    perror("msgsnd");
    exit(2);
  }

  // receive answer from server
  if ((l = msgrcv(msq_id, &my_answer, sizeof(struct answer)-4, getpid(), 0)) ==-1) {
    perror("msgrcv");
    exit(2);
  }
  printf(" addtion of two number is : %d \n", my_answer.result);
  exit(0);
}
    
 
                                                                                                                                                                                                                                                                                                                                                       msq_server.c                                                                                        0000644 0001750 0001750 00000002067 13435275432 012101  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File: msq_server.c
// Server process creates MSQ and adds two number from the message

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <sys/msg.h>		/* msq system calls*/ 
#define KEY 314

struct request {
  long type;
  int nb1;int nb2;
  pid_t pid;
};

struct answer {
  long type;
  int result;
};

 

int main(void)
{
  int msq_id, l;
  struct request my_request;
  struct answer my_answer;

  // message queue creation
  if ((msq_id = msgget((key_t)KEY, 0750|IPC_CREAT|IPC_EXCL)) == -1) {
    perror("msgget");
    exit(1);
  }

  while(1) { // busy waiting
    // read the request of type 1  with no options
    if ((l = msgrcv(msq_id, &my_request, sizeof(struct request)-4, 1, 0)) == -1) {
      perror("msgrcv");
      exit(2);
    }
    my_answer.result = my_request.nb1 + my_request.nb2;
    my_answer.type = my_request.pid;  // message type is the client pid
    
    if (msgsnd(msq_id, &my_answer, sizeof(struct answer)-4,0) == -1)  {
      perror("msgsnd");
      exit(3);
    }
  }
  exit(0);
}
    
 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                         my_pipe.c                                                                                           0000644 0001750 0001750 00000001660 13434027757 011357  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /******************************/
// File:  mypipe.c
// retraces command ls -l | wc -l

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */
#include <wait.h>		/* wait */ 

int main(void)
{
  int my_pipe[2];

  //opening pipe
  if (pipe(my_pipe)) {
    perror("pipe");
    exit(1);
  }

  switch(fork()) {
  case -1 : /*error*/
    perror(" pb fork");
    exit(2);
  case 0: /*child process execute Ls-l */
    close(STDOUT_FILENO);   // stdout redirected to the pipe
    (void)dup(my_pipe[1]);     // stdout duplicated into p[1]
    close(my_pipe[1]);
    close(my_pipe[0]);         // child do not read
    execlp("ls", "ls", "-l", NULL);
  default:
    close(STDIN_FILENO);   // stdinput redirected to the pipe
    (void)dup(my_pipe[0]);     // p[1] becomes stdinput of the proces
    close(my_pipe[0]);
    close(my_pipe[1]);         // child do not read
    execlp("wc", "wc", "-l", NULL);
  }
}
                                                                                my_piper.c                                                                                          0000644 0001750 0001750 00000002747 13464112043 011532  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  my_piper
// command ls -l | head | tail -4

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */
#include <wait.h>		/* wait */ 


struct command{
  const char **argv;
};

  

int do_pipe(int in, int out, struct command *cmd){
  pid_t my_child;

  my_child= fork();
  if (my_child == 0) { // child code
    if (in !=0 ){
      dup2(in, STDIN_FILENO);
      close(in);
    }
    if (out !=0){
      dup2(out ,STDOUT_FILENO);
      close(out);
    }
    return execvp(cmd->argv[0], (char * const *)cmd->argv);
  }
  return my_child;
}



int fork_pipes (int n, struct command *cmd){
  int i;
  int my_pipefd[2];

  int in = 0; // first process should fet its input rom the original fd 0
  
  for (i=0; i< n -1 ; i++) { // let the last iteration out
    if (pipe(my_pipefd)) {
      perror("Error: pipe");
      exit(1);
    }
    do_pipe( in, my_pipefd[1], cmd + i );
    // fd[1] is the write end, we carry input form the rpevious iteration
    close(my_pipefd[1]); // no need to write, child will do it
    in = my_pipefd[0];
  }

  // last iteration
  if (in !=0) dup2(in, STDIN_FILENO);
  return execvp( cmd[i].argv[0], (char * const *) cmd[i].argv);
} 


int main () {
  const char *ls[] = { "ls", "-l", 0};
  const char *awk[] = { "mawk", "{print $1}", 0};
  const char *sort[] = { "sort", 0};
  const char *uniq[] = { "uniq", 0};

  struct command cmd[] = { {ls},{awk},{sort},{uniq}};
  return fork_pipes(4, cmd);
}
7
                         my_shell2.c                                                                                         0000644 0001750 0001750 00000007367 13462637757 011635  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   // file: my shell
//

#include <stdio.h>
#include <stdlib.h>   // exit
#include <string.h>   // strtok strcmp
#include <unistd.h>   //pipe
#include <wait.h>
#include <fcntl.h>
#define MAX_ARGS 10
#define BUFFER_SIZE 256
void execute(char* cmdline);
int get_cmds(char* cmdline);
int _in,_out,_ap;
char* output_file;
char* input_file;
char* my_args[MAX_ARGS];

int main(int argc, char*argv[]) {
  char cmdline[BUFFER_SIZE];

  for(;;) {
    printf("$ ");
    if(fgets(cmdline, BUFFER_SIZE, stdin) == NULL) {
      perror("fgets failed");
      exit(1);
    }
    execute(cmdline);
  }
  return(EXIT_SUCCESS);
}


int get_cmds(char* cmdline){
  int i = 0;
  char * pch;

  pch = strtok(cmdline,"\n\t ");
  while (pch != NULL) {
    my_args[i++] = pch;
    //printf (" my_arg %d %s \n ", i, my_args[i-1]);
    pch = strtok(NULL , "\n\t ");
    
    if (i >= MAX_ARGS) {
      printf("Too many commands!\n");
      exit(1);
    }
  }
  return i-1;
}

int pipehandler(int nb_args){

  int cmd_nb = 0;
  char buffer[1024];
  char command[20][1024];
  
  
  int fd1[2], fd2[2]; //even output odd input
  int child_pid= -1;
  
  
  
  // get number of commands to execute
  
  strcpy(buffer, "");
  for(int i = 0; i < nb_args; i++) {
    if (strcmp(my_args[i], "|") == 0) {
      strcpy(command[cmd_nb], buffer);
      //printf (" command[%d]:  %s  \n",cmd_nb,  command[cmd_nb]);
      strcpy(buffer, "");
      cmd_nb++;
    }
    else{
      strcat(buffer, my_args[i]);
      strcat(buffer, " ");
    }
  }
  strcpy(command[cmd_nb],buffer);
  
  
  //printf ("how many pipes: %d \n", cmd_nb);
  for (int i = 0; i < cmd_nb+1 ; i++){
    printf("command [%d]; %s \n", i,  command[i]);
  }

  
  if(cmd_nb <= 0) return 0; // no pipes to execute
  
  for (int i = 0; i < cmd_nb+1; i++){
    
    if (i%2!=0) pipe(fd1);     // for odd i
    else pipe(fd2);            // for even i
   
    
    
    child_pid = fork();
    if (child_pid == 0) { // child code
      
      if (i==0)  // first command
	dup2(fd2[1], STDOUT_FILENO);
      else if (i == cmd_nb) // last command
	if(cmd_nb % 2 != 0)
	  dup2(fd2[0], STDIN_FILENO);
	else
	  dup2(fd1[0],STDIN_FILENO);
      else { //command in the middle
	if(cmd_nb % 2 != 0) {
	  dup2(fd2[0], STDIN_FILENO);
	  dup2(fd1[1], STDOUT_FILENO);
	} else {
	  dup2(fd1[0],STDIN_FILENO);
	  dup2(fd2[1], STDOUT_FILENO);
	}
      }

      char my_command[1024];
      char *parameters[10];
      char *token;

      token = strtok(command[i], " ");
      strncpy( my_command, token, 1024);
      parameters[0] = my_command;
      while ( (token = strtok(NULL, " ")) && i < 10) {
	parameters[i] = token;
	i++;
      }
      execvp( my_command, parameters);
      perror("PIPE!!  exec failed"); //replacement only if exec fails
      exit(-1);
      
      
    }
    else { //closing descriptors for parent
      if (i == 0) close(fd2[0]); // first command
      else if ( i == cmd_nb) {  // last command
	if (cmd_nb % 2 != 0) close(fd2[0]);
	else close(fd1[0]);
      } else {  // command in the middle
	if (i % 2 != 0) {
	  close(fd2[0]);
	  close(fd1[1]);
	} else {
	  close(fd1[0]);
	  close(fd2[1]);
	}
      }
      waitpid(child_pid, NULL, 0);      
    }
  }
  return cmd_nb;
}

  



void execute(char* cmdline) {
  int pid;
  
  int nb_args=get_cmds(cmdline)+1;
  //printf("Number of arguments tokenized : %d \n", nb_args);
  if (!strcmp(my_args[0],"quit") || !strcmp(my_args[0],"exit")) exit(EXIT_SUCCESS);

  int cmmd_nb = pipehandler(nb_args);
  printf("-->  no pipes \n");
  if (nb_args <= 0 || cmmd_nb>=1) return ;
    
  pid = fork();
  if (pid == 0) { // child process
    execvp(my_args[0], my_args);
    perror("Exec failed"); //replacemetn only if exec fails
    exit(-1);
  }
  else if (pid>0) // parent process
    waitpid(pid, NULL,0);
  else
    perror("!!! fork failed");
}



    
                                                                                                                                                                                                                                                                         my_shell3.c                                                                                         0000644 0001750 0001750 00000000200 13462531310 011563  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   CC=gcc
CFLAGS=  -Wall -Werror -g -lpthread
TARGET= my_shell2


$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c
                                                                                                                                                                                                                                                                                                                                                                                                my_shell.c                                                                                          0000644 0001750 0001750 00000006527 13462532435 011533  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   // file: my shell
//

#include <stdio.h>
#include <stdlib.h>   // exit
#include <string.h>   // strtok strcmp
#include <unistd.h>   //pipe
#include <wait.h>
#include <fcntl.h>
#define MAX_ARGS 10
#define BUFFER_SIZE 256
void execute(char* cmdline);
int get_cmds(char* cmdline);
char *concat(const char *s1, const char *s2);
char* my_args[MAX_ARGS];

int main(int argc, char*argv[]) {
  char cmdline[BUFFER_SIZE];

  for(;;) {
    printf("$ ");
    if(fgets(cmdline, BUFFER_SIZE, stdin) == NULL) {
      perror("fgets failed");
      exit(1);
    }
    execute(cmdline);
  }
  return(EXIT_SUCCESS);
}


int get_cmds(char* cmdline){
  int i = 0;
  char * pch;

  pch = strtok(cmdline,"\n\t ");
  while (pch != NULL) {
    my_args[i++] = pch;
    //printf (" my_arg %d %s \n ", i, my_args[i-1]);
    pch = strtok(NULL , "\n\t ");
    
    if (i >= MAX_ARGS) {
      printf("Too many commands!\n");
      exit(1);
    }
  }
  return i-1;
}


void pipehandler(int nb_args){

  int command_nb = 0;
  char * command[10];
  
  int fd1[2], fd2[2]; //even output odd input
  int child_pid= -1;
  
  
  // get number of commands to execute
  for(int i = 0; i < nb_args+1; i++) {
    if (strcmp(my_args[i], "|") == 0) {
      command_nb++; 
    }
    else
      command[command_nb] = concat (command[command_nb], my_args[i]);
  }

  
  printf ("how many pipes: %d \n", command_nb);
  for (int i = 0; i < command_nb+1 ; i++){
    printf("command [%d]; %s \n", i,  command[i]);
  }

  if(command_nb <= 0) {
    printf (" no pipes- execution \n");
    return; // no pipes to execute
  }

  for (int i = 0; i < command_nb; i++){
    if (i%2!=0) pipe(fd1);     // for odd i
    else pipe(fd2);            // for even i
    
    child_pid = fork();
    if (child_pid == 0) { // child code
      if (i==0)  // first command
	dup2(fd2[1], STDOUT_FILENO);
      else if (i == command_nb -1) // last command
	if(command_nb % 2 != 0)
	  dup2(fd1[0], STDIN_FILENO);
	else
	  dup2(fd2[0],STDIN_FILENO);
      else { //command in the middle
	if(command_nb % 2 != 0) {
	  dup2(fd2[0], STDIN_FILENO);
	  dup2(fd1[1], STDOUT_FILENO);
	} else {
	  dup2(fd1[0],STDIN_FILENO);
	  dup2(fd2[1], STDOUT_FILENO);
	}
      }
      fprintf(stdout,"to execute :: %s \n", command[i]);
      execvp(command[i], &command[i]);
    }
    else { //closing descriptors for parent
    if (i == 0) close(fd1[0]); // fisrt command
      else if ( i == command_nb - 1) {  // last command
	if (command_nb % 2 != 0) close(fd1[0]);
	else close(fd2[0]);
      } else {  // command in the middle
	if (i % 2 != 0) {
	  close(fd2[0]);
	  close(fd1[1]);
	} else {
	  close(fd1[0]);
	  close(fd2[1]);
	}
      }
      waitpid(child_pid, NULL, 0);
    }
  }
}

void execute(char* cmdline) {
  //int pid;
  
  int nb_args=get_cmds(cmdline);
  //printf("Number of arguments tokenized : %d \n", nb_args+1);
  if (!strcmp(my_args[0],"quit") || !strcmp(my_args[0],"exit")) exit(EXIT_SUCCESS);

  pipehandler(nb_args);
  if (nb_args <= 0) return ;
  
  //pid = fork();
  //if (pid == 0) ; // child process
  execvp(my_args[0], my_args);
  perror("exec failed"); //replacemetn only if exec fails
  exit(-1);
}
 



char *concat(const char *s1, const char *s2){
  const size_t len1 = strlen(s1);
  const size_t len2 = strlen(s2);
  int my_size = len1 + len2 + 1;
  char *result = malloc(my_size);
  memcpy(result, s1, len1);
  memcpy(result + len1, s2, len2 + 1);
  return result;
}
                                                                                                                                                                         my_system.c                                                                                         0000755 0001750 0001750 00000001265 13424073603 011740  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * Fichier my_system.c
 ********************************/
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int my_system(char *cmd)
{
  int status;
  
  if (fork() == 0) {
    char *tab[10];
    char *token = strtok(cmd, " \n");
    int i = 0;
    while (token != NULL) {
      tab[i] = token;
      token = strtok(NULL, " \n");
      i = i + 1;
    }
    tab[i] = NULL;
    
    execvp(tab[0], tab);
  } else {
    wait(&status);
  }
  return status;
}


int main()
{
  char cmd[150];
  
  fgets(cmd, sizeof(cmd) / sizeof(char), stdin);
  my_system(cmd);
  
  exit(EXIT_SUCCESS);
}
                                                                                                                                                                                                                                                                                                                                           namedpipe.c                                                                                         0000644 0001750 0001750 00000001247 13434030260 011637  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  firstpipe.c
// One direction comunication between father and child

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */
#include <wait.h>		/* wait */ 

int main(void)
{
  int pipefd[2], status;
  pid_t child_pid;
  char message[10];
  
  pipe(pipefd);
  child_pid= fork();
  if (child_pid == 0) { // child code
    close(pipefd[0]);
    write(pipefd[1], "hello in the pipe", 10);
    close(pipefd[1]);
    exit(EXIT_SUCCESS);
  }

  else {  // father code
    close (pipefd[1]);
    read (pipefd[0], message, 10);
    printf("Message in pipe is: s .\n", message);
    wait(&status);
  }
}
                                                                                                                                                                                                                                                                                                                                                         namedpipe_reader.c                                                                                  0000644 0001750 0001750 00000001057 13440311215 013157  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  namedpipe_reader.c
// reader process from a named tube

#include <stdio.h>		/* printf */
#include <stdlib.h>
#include <unistd.h>             /* read write */
#include <fcntl.h>		/* */ 
#include <sys/types.h>
#include <sys/stat.h>

int main(void)
{
  char message[11];
  int n_pipe;
  
  n_pipe = open("named_pipe", O_RDONLY);    // open the FIFO
  read(n_pipe, message, 11);
  message[10] = 0;
  printf("reader process from the pipe 'named_pipe':\n");
  printf("I've read: %s \n", message);
  close(n_pipe);
}
	
 
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 namedpipe_writer.c                                                                                  0000644 0001750 0001750 00000000766 13434033641 013246  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  namedpipe_writer.c
// Writer process in a named tube

#include <stdio.h>		/* printf */
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
  mode_t mode;
  int n_pipe;
  mode = S_IRUSR | S_IWUSR;

  mkfifo("named_pipe", mode);               // named-pipe creation
  n_pipe = open ("named_pipe", O_WRONLY);    // opent the FIFO
  write (n_pipe, "0123456789", 10);
  close (n_pipe);
}
	
 
          pere_attend_fils.c                                                                                  0000755 0001750 0001750 00000002221 13422631207 013205  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * Fichier pere_attend_fils.c
 * Livre "Exercices corrig??s de d??veloppement syst??me sous Linux"
 *
 * (c) 2018 P. Alain, J. Chevelu, V. Thion
 *
 ********************************/
#include <stdio.h>		/* stderr, stdout, fprintf, perror */
#include <unistd.h>		/* fork */
#include <sys/wait.h>		/* wait */
#include <stdlib.h>		/* exit */

int main(void)
{
	pid_t pid_fils = -1;	/* Pour r??cup??rer la valeur de retour de l'execution du fork */
	pid_t wait_returned_value = -1;
	pid_fils = fork();	/* Cr??ation du processus fils */
	int i;

	if (pid_fils == 0) {
		/* Code affect?? au fils */
		printf("(%d) Je suis le fils, j'effectue mon traitement \n", getpid());
		for (i = 0; i < 10; i++) {
			printf("(%d) ..\n", getpid());
			sleep(2);
		}
		printf("(%d) Je suis le fils, j'ai fini mon traitement \n", getpid());
		exit(EXIT_SUCCESS);
	} else {		/* Code affect?? au p??re */
		printf("(%d) Je suis le p??re, j'attends mon fils. \n", getpid());
		wait_returned_value = wait(NULL);
		printf("(%d) Je suis le p??re, mon fils %d est termin??, et moi aussi... \n",
		       getpid(), wait_returned_value);
		exit(EXIT_SUCCESS);
	}
}
                                                                                                                                                                                                                                                                                                                                                                               perfect_mutex.c                                                                                     0000644 0001750 0001750 00000003331 13431302556 012552  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   
/********************************
 * File : perfectnb.c
 * N Threads to find perfect numbers 
 ********************************/
#include <stdio.h>	   /* stderr, stdoit, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 5
#define NUM_MAX     10000
static pthread_mutex_t mtx= PTHREAD_MUTEX_INITIALIZER;
int perfect_list[1];

static void *Thread_code(void *thread_nb) {
  long end = ((long)thread_nb+1) * NUM_MAX / NUM_THREADS-1;
  long start =(long)thread_nb * NUM_MAX / NUM_THREADS;
  int nb, i, mod;
  int j=0;
  
  printf("Thread %ld : scaning from %ld - %ld \n", (long)thread_nb, start, end);

  for (nb=start ;nb<end; nb++){
    int sum=0;
    
    for (i=1; i<nb; i++){ // look thougth divisor and sum them up
      mod = nb % i;
      if (mod ==0) // i is divisor then sum up
	sum +=i;
    }
    if (sum == nb){ //its a perfect number	
      printf("Thread: %ld - %i is a perfect number\n",(long)thread_nb, sum);
      
      // enter critical region
      pthread_mutex_lock(&mtx);
      perfect_list[j++ ] = sum;
      pthread_mutex_unlock(&mtx);
    }
  }
  pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
  pthread_t my_threads[NUM_THREADS];
  int err_code;
  long l;

 
  for(l = 0 ; l < NUM_THREADS; l++){
    err_code = pthread_create(&my_threads[l], NULL, Thread_code, (void *)l);
    if (err_code) {
      fprintf(stdout, "ERROR: creating thread #%i :%d", (int)l,  err_code);
	exit(-1);
    }
  }

  for(l = 0 ; l < NUM_THREADS;l++)
    pthread_join(my_threads[l], NULL);

  printf("All threads finished. Perfect numbers are ");
  for (l = 0; l < sizeof(perfect_list); l++)
    printf ("%i ,", perfect_list[l]);
  printf("\n");
  return(EXIT_SUCCESS);
}
                                                                                                                                                                                                                                                                                                       perfectnb.c                                                                                         0000644 0001750 0001750 00000002523 13431274424 011654  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   
/********************************
 * File : perfectnb.c
 * Thread creation with pthread()
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 5
#define NUM_MAX     10000


static void *Thread_code(void *thread_nb) {
  long end = ((long)thread_nb+1) * NUM_MAX / NUM_THREADS-1;
  long start =(long)thread_nb * NUM_MAX / NUM_THREADS;
  int nb,i, mod;
  
  printf("Thread %ld : scaning from %ld - %ld \n", (long)thread_nb, start, end);

  for (nb=start ;nb<end; nb++){
    int sum=0;
    
    for (i=1; i<nb; i++){ // look thougth divisor and sum them up
      mod = nb % i;
      if (mod ==0) // i is divisor then sum up
	sum +=i;
    }
    if (sum == nb) //its a perfect number	
      printf("Thread: %ld - %i is a perfect number\n",(long)thread_nb, sum);
  }
  pthread_exit(NULL);
}


int main(int argc, char *argv[])
{
  pthread_t my_threads[NUM_THREADS];
  int err_code;
  long l;

 
  for(l = 0 ; l < NUM_THREADS; l++){
    err_code = pthread_create(&my_threads[l], NULL, Thread_code, (void *)l);
    if (err_code) {
      fprintf(stdout, "ERROR: creating thread #%i :%d", (int)l,  err_code);
	exit(-1);
    }
  }

  for(l = 0 ; l < NUM_THREADS;l++)
    pthread_join(my_threads[l], NULL);
  return(EXIT_SUCCESS);
}
                                                                                                                                                                             pipe2directions.c                                                                                   0000644 0001750 0001750 00000003022 13434025530 012775  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File: pipe2 directions.c
// two direction comunication between father and child

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* fork */
#include <wait.h>		/* wait */ 
int pipe1[2];int pipe2[2];
int status;

int main(void)
{
 pid_t child_pid;
 char reception[7], message[12];
 
 //opening pipes
 if (pipe(pipe1)) {
   perror("pipe 1 fails");
   exit(1);
 }
 if (pipe(pipe2)){
   perror("pipe 2 fails");
   exit(2);
 }

 //process fork
 if( (child_pid = fork()) == -1){
   perror ("fork");
   exit(3);
 }
 
 if (child_pid) { // father code
   close(pipe1[0]);   // first  pipe father writes --> son reads
   close(pipe2[1]);   // second pipe son writes --> father readsa

   if (write(pipe1[1], "hello in the pipe", 12) != 12) {
     fprintf(stderr, " father: writing error\n");
     exit(4);
   }
   if (read(pipe2[0],reception ,7) != 7) {
     fprintf(stderr, " father: writing error\n");
     exit(5);
   }
   fprintf(stdout, "father: my son messade is: %s \n", reception);
   wait(&status);
  }
  else {  // child code
    close(pipe1[1]);   // first  pipe father writes --> son reads
    close(pipe2[0]);   // second pipe son writes --> father readsa

   if (write(pipe2[1], "send me money", 7) != 7) {
     fprintf(stderr, " son: writing error\n");
     exit(6);
   }
   if (read(pipe1[0], message ,7) != 7) {
     fprintf(stderr, " son: writing error\n");
     exit(7);
   }
   fprintf(stdout, "son; my father message is: %s \n", message);
   exit(EXIT_SUCCESS);
  }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              porjection.c                                                                                        0000644 0001750 0001750 00000001557 13440553563 012072  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  projection.c
// Projects  afile into memory
// File contains 4 registries for contacts

#include <stdio.h>		/* printf */
//#include <unistd.h>             /* read, write */
//#include <stdlib.h>             
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
  struct contact {
    char nom[10];
    char teleohne[10];
  };
  
  int fd, i;
  struct contact *my_contact;
  char* address, nom[10];
  
  fd = open ("/ivan/directory.txt", O_RDWR);
  if (fd == -1)
    perror("prob open");
  
  address = (char*) mmap(NULL, 4*sizeof(struct contact), PROT_READ, MAP_SHARED, fd, (off_t)0);
  close(fd);
  
  //file data is accessed on a table, first element is un fisrt by the ofthe region
  i = 0;
  WHILE (i < 4 * sizeof(struct contact)) {
    putchar (address[i]);
    i++;
  }
}

	
 
                                                                                                                                                 printmem.c                                                                                          0000644 0001750 0001750 00000002376 13445375112 011546  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  printmem.c
// Print memory regions

#include <stdio.h>          // printf
#include <unistd.h>          //
#include <linux/kernel.h>
#include <linux/module.h>     // mm_struct
#include <linux/mman.h>
#include <mm.h>
#include <sched.h>           // task_struc
//#include <linux/mm_types.h>

struct task_struct *process; //process descriptor
struct mm_struct *mm;        //memory descriptor
struct vm_area_struct *vma;  //vm descriptor


static void print_mem(struct task_struct *task)
{
  struct mm_struct *mm;
  struct vm_area_struct *vma;
  int count = 0;
  
  mm = task->mm;
  printf("This mm_struct has %d vmas.\n",  mm->map_count);
  for (vma = mm->mmap ; vma ; vma= vma->vm_next) { 
    printf(" Vma number %d: \n", ++count);
    printf(" Start at 0x%lx, Ends Ox%lx\n", vma->vm_start, vma->vm_end);
  }
  
  printf("Code segment start=0x%lX, end=0x%lx\n", mm->start_code, mm->end_code);
  printf("Data segment start=0x%lX, end=0x%lx\n", mm->start_data, mm->end_data);
  printf("Stack segment start=0x%lX, end=0x%lx\n", mm->start_stack, mm->end_stack);
}

int main(void){
  
  printf("got the process id to lookup as %d.\n", getpid());
  printf("%s[%s]\n", process->comm, process->pid);
  print_mem(process);
  return 0;
}


 
  
                                                                                                                                                                                                                                                                  productallocation.c                                                                                 0000644 0001750 0001750 00000001635 13441526076 013441  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  productallocation.c
// Program creates a file and store product type data in the file

#include <stdio.h>		/* printf */
#include <unistd.h>             /* close */
#include <stdlib.h>             /* malloc */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
  struct product {
    char id[5];
    char items[2];
  };
 
  int fd, i;
  struct product *my_product;
   
  fd = open ("/home/ivan/products.txt", O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
  if (fd == -1)
    perror("Error: open file");

  i = 0;
  while (i < 4) {
    my_product = ( struct product* ) malloc (sizeof(struct product));
    printf(" Enter a id (5numbers): ");
    scanf("%s", my_product->id);
    printf(" Enter how many items (2numbers): ");
    scanf("%s", my_product->items);
    write(fd, my_product, sizeof(struct product));
    i++;
  }
  close(fd);
}
 
                                                                                                   productmanager.c                                                                                    0000644 0001750 0001750 00000003371 13445371071 012722  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  productmanager.c
// Projects a file into memory
// File contains 4 registries for contacts

#include <stdio.h>		/* printf */
#include <unistd.h>             /* close */
#include <stdlib.h>             /* malloc */
#include <string.h>             /* strncpy */ 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
  struct product {
    char id[5];
    char items[2];
    struct product *next;
  };
 
  int fd, i, j;
  struct stat st;
  struct product *my_product, *current_product, *product_list;
  char *mem_address;

 
  // get filesize and get file
  if (stat ("/home/ivan/products.txt", &st))
    perror("Error stat");
  fd = open ("/home/ivan/products.txt", O_RDWR);
  if ( fd == -1)
    perror("Error: openfile");
  mem_address = (char *) mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, (off_t)0);
  close(fd);
  printf ("mmap allocation:  %s \n", mem_address);
  
    
  //Creating linked list
  current_product = NULL; i = 0;
  
  while (i < 4 * 7) {
    my_product =  malloc(sizeof(struct product));

    j=0;
    while (j < 5) {
      my_product->id[j] = mem_address[i];
      i++; j++;
      }
      
    j=0;
    while (j < 2) {
      my_product->items[j] = mem_address[i];
      i++; j++;
      }
x1
    if (current_product == NULL) {
      current_product = my_product;
      product_list = my_product; // save first product linked list
    }
    else
      {
	my_product->next = NULL;
	current_product->next = my_product;
	current_product = my_product;
      }
   }

  i=0;
  // printing product database in memory
  for (i=0; i < 4;i++){
    printf("%d. Product %.5s: %s items\n", i, product_list->id, product_list->items);
    product_list = product_list->next;
  }
}

                                                                                                                                                                                                                                                                       projection2.c                                                                                       0000644 0001750 0001750 00000003201 13445374116 012140  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  projection2.c
// Projects a file into memory
// prints it in a linked list

#include <stdio.h>		/* printf */
#include <unistd.h>             /* close */
#include <stdlib.h>             /* malloc */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
  struct contact {
    char name[10];
    char phone[10];
    struct contact *next;
  };
 
  int fd, i, j;
  struct contact *my_contact, *current_contact,*contact_list;
  char *mem_address;

  // creates projection
  fd = open ("/home/ivan/directory.txt", O_RDWR);
  if (fd == -1)
    perror("Error: openfile");
  mem_address = (char*) mmap(NULL, 4*sizeof(struct contact),
			     PROT_READ, MAP_SHARED, fd, (off_t)0);
  close(fd);
  
    
  //Creating linked list
  current_contact = NULL; i = 0;
  while (i < 4 * sizeof(struct contact)) {
    my_contact =  malloc(sizeof(struct contact));
    
    j=0;
    while (j < sizeof(my_contact->name)) {
      my_contact->name[j] = mem_address[i];
      i++; j++;
    }
    
    j=0;
    while (j < sizeof(my_contact->phone)) {
      my_contact->phone[j] = mem_address[i];
      i++; j++;
    }
    
    if (current_contact == NULL) { //first link
      current_contact = my_contact;
      contact_list = my_contact; // save first link address
    }
    else
      {
	my_contact->next = NULL;
	current_contact->next = my_contact;
	current_contact = my_contact;
      }
   }

  // printing product database in memory
  for (i=0; i < 4;i++){
    printf("%d. contact %.10s: %s \n", i,contact_list->name, contact_list->phone);
    contact_list = contact_list->next;
  }
}



  
                                                                                                                                                                                                                                                                                                                                                                                               projection.c                                                                                        0000644 0001750 0001750 00000001633 13445224064 012061  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  projection.c
// Projects a file into memory
// File contains 4 registries for contacts

#include <stdio.h>		/* printf */
#include <unistd.h>             /* close */
#include <stdlib.h>             /* malloc */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
  struct contact {
    char name[10];
    char phone[10];
  };
 
  int fd, i;
  struct contact;
  char *mem_address;
  
  fd = open ("/home/ivan/directory.txt", O_RDWR);
  if (fd == -1)
    perror("Error: openfile");
  
  mem_address = (char*) mmap(NULL, 4*sizeof(struct contact),
			     PROT_READ, MAP_SHARED, fd, (off_t)0);
  close(fd);
  
 //file data is accessed by table, first element is first byte of the region
 
 printf("\n file in memory \n"); i=0;
 while (i < 4 * sizeof(struct contact)) {
   putchar(mem_address[i]);
   i++;
 }
 printf("end. \n");
}

                                                                                                     projection_list2.c                                                                                  0000644 0001750 0001750 00000001666 13445225561 013207  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
// File:  projection_list.c
// Projects a file into a structure of a linked list
// File contains 4 registries for contacts

#include <stdio.h>		/* printf */
#include <unistd.h>             /* close */
#include <stdlib.h>             /* malloc */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(void)
{
  struct contact {
    char name[10];
    char phone[10];
  };
 
  int fd, i;
  struct contact;
  char *mem_address;
  
  fd = open ("/home/ivan/directory.txt", O_RDWR);
  if (fd == -1)
    perror("Error: openfile");
  
  mem_address = (char*) mmap(NULL, 4*sizeof(struct contact),
			     PROT_READ, MAP_SHARED, fd, (off_t)0);
  close(fd);
  
 //file data is accessed by table, first element is first byte of the region
 
 printf("\n file in memory \n"); i=0;
 while (i < 4 * sizeof(struct contact)) {
   putchar(mem_address[i]);
   i++;
 }
 printf("end. \n");
}

                                                                          read-file.c                                                                                         0000644 0001750 0001750 00000001113 13452734742 011535  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /***************************************
 *File: read_file.c
 ***************************************/

#include <stdio.h>     
#include <stdlib.h>     // exit
#include <unistd.h>     // fork
#include <sys/wait.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char * argv[]){
  int fd, i;
  char message[4];

  fd = atoi(argv[1]);
  printf("fd child value, %d.\n", fd);

  i=0;
  while ( i < 5 ) {
    printf("%d", i);
    read(fd, message, 4);
    printf(" Parent process, reading : %s \n", message);
    i++;
  }
  close(fd);
  exit(0);
}
  
 
                                                                                                                                                                                                                                                                                                                                                                                                                                                     readtaskstruct.c                                                                                    0000644 0001750 0001750 00000003554 13445374115 012757  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                    /********************************/
// File:  readtaskstruc.c
// User program to read

#include <stdio.h>        // printf
#include <sys/mman.h>     // struct
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      // open
#include <unistd.h>     // get page size
#include <stdbool.h>
#include <string.h>    // memcpy
#include <stdlib.h>   // calloc

long read_task_struct (long address)
{
  int fd;
  void *ret_ptr = NULL;

  long addr = address;
  long *addr_ptr=NULL;
  
  long next,prev;
  long val;
  int page_size = getpagesize();

  addr_ptr  = (long *) address;
  page_size = getpagesize();
  val = address & ~(page_size-1);
  long *ptr =(long*) val;
  long offset = address;

  fd = open("/dev/mem", O_RDWR | O_SYNC);
  ret_ptr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, *ptr);
  if (ret_ptr == MAP_FAILED) printf("FAILED \n");
  else {
    memcpy(&val, ret_ptr + (offset + 620), 4);  //tasks next pointer
    val=val-620; // address to task_struct
    munmap(ret_ptr, 4096);
    close(fd);
  } 
  return val;
}

const char * read_value(long address){
  int fd;
  void *ret_ptr = NULL;
  long my_address = address;
  long *my_address_ptr = NULL;
  long next,prev;
  long val;
  char *nextstr = calloc(sizeof(char),100);
  int page_size,offset;

  long *next_ptr = (long *) next;
  page_size = getpagesize();
  val = address & ~(page_size-1);
  long *ptr =(long*) val;
  offset = address;

  fd = open("/dev/mem", O_RDWR | O_SYNC);
  ret_ptr = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, *ptr);
  if (ret_ptr == MAP_FAILED) printf ("FAILED \n");
  else {
    memcpy(&val, ret_ptr + (offset + 966), 100);  //tasks--> comm
    munmap(ret_ptr, 4096);
    close(fd);
  } 
  return nextstr;
}
  
void main(void)
{
  int ret = 0xc1a8cac0;  //init_task
  for (int i=0; i<10;i++) {
    printf("Ox%x\n", ret);
    ret = read_task_struct(ret);
    printf("%s\n",read_value(ret));
  }
}
                                                                                                                                                    scheduler.c                                                                                         0000644 0001750 0001750 00000004275 13425663022 011667  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * File sheduler.c
 * ********************************/
#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* posix standard units ; fork, getpid */
#include <errno.h>		/* errno */
#include <sched.h>		/* scheduler function */
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void)
{
  pid_t my_return, pid;
  int policy, status, priority;
  struct timespec quantum;
  struct sched_param my_paramater;

  my_return = fork();
  if (my_return == 0) { /* child code */  
    pid= getpid();
    printf("I'm a child process (%d), my scheduler priority is %d \n",
	   pid, getpriority(PRIO_PROCESS, pid));

    policy=sched_getscheduler(pid);
    if (policy == SCHED_RR)
      printf("I'm child(%d), my scheduler policy is SCHED_RR\n\n",pid);
    if (policy == SCHED_FIFO)
      printf("I'm child(%d), my scheduler policy is SCHED_FIFO\n\n",pid);
    if (policy == SCHED_OTHER)
         printf("I'm child(%d), my scheduler policy is SCHED_OTHER\n\n",pid);

    my_paramater.sched_priority = 10;
    setpriority(PRIO_PROCESS, pid , 10);
    if (sched_setscheduler(pid, SCHED_FIFO, &my_paramater) == -1)
      perror ("ERROR --  setscheduler");
    priority = getpriority(PRIO_PROCESS, pid);
    printf("I'm child(%d), my NEW scheduler priority is %d\n",pid, priority);

    policy=sched_getscheduler(pid);
    if (policy == SCHED_RR)
      printf("I'm child(%d), my NEW scheduler policy is SCHED_RR\n",pid);
    if (policy == SCHED_FIFO)
      printf("I'm child(%d), my NEW scheduler policy is SCHED_FIFO\n",pid);
    if (policy == SCHED_OTHER)
      printf("I'm child(%d), my NEW scheduler policy is SCHED_OTHER\n",pid);
  }
  else { // father code
    printf("These are min and max priorities of SCHED_FIFO, %d , %d\n",
	   sched_get_priority_min(SCHED_FIFO),
	   sched_get_priority_max(SCHED_FIFO));
    printf("These are min and max priorities of SCHED_RR, %d , %d\n",
	   sched_get_priority_min(SCHED_RR),sched_get_priority_max(SCHED_RR));
    sched_rr_get_interval(getpid(), &quantum);
    printf("This is the quantum value of SCHED_RR policy, %lu seconds, %lu nanoseconds \n\n", quantum.tv_sec, quantum.tv_nsec);
    wait(&status);
  }
}
                                                                                                                                                                                                                                                                                                                                   semaphore.c                                                                                         0000644 0001750 0001750 00000003401 13440237673 011670  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * File : sempahore.c
 * Threads share variables with semaphores
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#include <sys/sem.h>
#define KEY  12

int sem_id;
int sh_var;
struct sembuf operation;


void *mutex_add(void *thread_nb) {
  long thr_nb = (long) thread_nb;
  
  // Operation
  operation.sem_num = 0;
  operation.sem_op = -1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  sh_var+= 10;
  printf("Thread #%ld: son, op=%d.\n", thr_nb, sh_var);
  sh_var+=20;
  printf("Thread #%ld: son, op=%d.\n", thr_nb, sh_var);
  
  
  //operation V
  operation.sem_num = 0;
  operation.sem_op = 1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  exit(EXIT_SUCCESS);
}
  


int main(int argc, char *argv[])

{
  pthread_t my_thread;
  long thread_nb=0;
  sh_var=0;
  

  // Semaphore creation and intializated @ value 1
  if ((sem_id = semget(KEY, 1, IPC_CREAT|IPC_EXCL|0600)) == -1)
    perror("Error semget");
  if ((semctl (sem_id, 0, SETVAL , 1)) == -1)
    perror("Error semctl");
  
  //thread creation
  if ((pthread_create(&my_thread, NULL, mutex_add, (void*) thread_nb)) == -1)
    perror("Error pthread_crate");

  thread_nb=1;
  // Operation P
  operation.sem_num = 0;
  operation.sem_op = -1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  sh_var+= 1000;
  printf("Thread #%ld: father, op=%d\n", thread_nb, sh_var);
  sh_var+=2000;
  printf("Thread #%ld: father, op=%d\n", thread_nb, sh_var);


  //operation V
  operation.sem_num = 0;
  operation.sem_op = 1;
  operation.sem_flg = 0;
  semop(sem_id, &operation, 1);
  pthread_join(my_thread, NULL);
  semctl(sem_id, 0 , IPC_RMID,0);
}
                                                                                                                                                                                                                                                               shm_creator.c                                                                                       0000644 0001750 0001750 00000001516 13435300042 012201  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
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

    

     
 
                                                                                                                                                                                  shm_destroyer.c                                                                                     0000644 0001750 0001750 00000001473 13435304131 012567  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************/
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

    

     
 
                                                                                                                                                                                                     signal_handler.c                                                                                    0000644 0001750 0001750 00000000613 13425300302 012640  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /**********************************/
/*  File Signal Handler Example        */
/**********************************/

typdef void (*sighandler_t) int  // Type definition for easy reading

void new_handler(int receivedSignal){
  fprintf(stdout, "I have just received the signal %d \n", receivedSignal);

  signalhandler_t return_value_signal;
  return_value_signal = signal(SIGINT, new_handler)
}
                                                                                                                     signal_processor.c                                                                                  0000755 0001750 0001750 00000002017 13424301526 013255  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 signal_processor_child.c                                                                            0000755 0001750 0001750 00000002034 13424326700 014420  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    stack.c                                                                                             0000644 0001750 0001750 00000006732 13446710570 011022  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /* File : stack.c  
Stack Operations using Dynamic Memory Allocation */

#include <stdio.h>
#include <stdlib.h>
#define MAX 5

struct node {
    int data;
    struct node *next;
}*top_of_stack = NULL;

// function prototypes declaration
void push();
void pop();
void empty();
void stack_full();
void stack_count();
int stack_counter();
void destroy();
void print_top();

 

int main(void)
{
  int choice;
  
  while (1){
    printf("\n\n1. push an element \n");
    printf("2. pop an element \n");
    printf("3. check if stack is empty \n");
    printf("4. check if stack is full \n");
    printf("5. count/display elements present in stack \n");
    printf("6. empty and destroy stack \n");
    printf("7. Print top of the stack \n");
    printf("8. exit \n");
    printf("Enter your choice:  ");
    scanf("%d",&choice);
    printf("-->");
    switch (choice)
      {
      case 1:    
	push();
	break;         
      case 2:    
	pop();
	break;         
      case 3:    
	empty();
	break;         
      case 4:    
	stack_full();
	break;         
      case 5:    
	stack_count();
	break;         
      case 6:    
	destroy();
	break;         
      case 7:    
	print_top();
	break;
        case 8:    
	  exit(0);
      default:
	printf("wrong choice\n");         
      }
  }
}

 
// procedure to insert elements in stack
void push()
{
  int my_data, count;
  struct node *my_stack;

  my_stack = (struct node*)malloc(sizeof(struct node));
  count = (int)stack_counter();
  
  if (count <= MAX - 1)
    {
      printf("Enter value which you want to push into the stack :  ");
      scanf("%d",&my_data);
      my_stack->data = my_data;
      my_stack->next = top_of_stack;
      top_of_stack = my_stack;
    }
  else
    printf("WARNING: STACK FULL\n");
}

 

// to delete elements from stack
void pop()
{
  struct node *my_stack;
  if (top_of_stack == NULL)
    printf("**Stack is empty**\n");
  else
    {
      my_stack = top_of_stack;
      printf("Value popped out is %d \n",my_stack->data);
      top_of_stack = top_of_stack->next;
      free(my_stack);
    }
}

 

// to check if stack is empty
void empty()
{
    if (top_of_stack == NULL)
        printf("STACK IS EMPTY\n");
    else
        printf("elements are present, stack is not empty \n");
}

 

// to check if stack is full
void stack_full()
{
  int count;
 
  count = stack_counter();
  if (count == MAX)
    {
      printf("stack is full\n");
    }
  else
    printf("stack is not full \n");
}

 
// to count the number of elements
void stack_count()
{
    int count = 0;
    
    struct node *my_stack;
    my_stack = top_of_stack;
    while ( my_stack != NULL )
    {
        printf(" %d\n",my_stack->data);
	my_stack = my_stack->next;
        count++;
    }
    printf("size of stack is %d \n",count);
}

 

int stack_counter()
{
    int count = 0;
    struct node *my_stack;
    my_stack = top_of_stack;
    while (my_stack != NULL)
    {
        my_stack = my_stack->next;
        count++;
    }
    return count;
}

 

// to empty and destroy the stack
void destroy()
{

    struct node *my_stack;
    my_stack = top_of_stack;
    while (my_stack->next != NULL)
    {
      my_stack=my_stack->next;
      pop();
    }
    pop();   // pop last item which has the control "emptu stack"
    stack_count();
    printf("stack destroyed\n");
}

 

// to print top element of stack
void print_top()
{
  if (top_of_stack == NULL)
        printf("**Top is not available for an EMPTY stack**\n");
    else
      printf("**Top of the stack is %d \n",top_of_stack->data);
}
                                      thread_mutex.c                                                                                      0000644 0001750 0001750 00000003220 13444425101 012362  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   
/********************************
 * File : thread_mutex.c
 * Threads share variables but lock updates / several outcomes
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>       /* getpid() */
#define NUM_THREADS 3
pthread_mutex_t my_lock = PTHREAD_MUTEX_INITIALIZER;
int my_counter; 


void *addfunction(void *thread_nb) {
  long t_nb = (long) thread_nb;

  //lock mutex and update variable
  pthread_mutex_lock(&my_lock);
  my_counter += 10;
  printf("\n--Thread #%ld; id  %ld ; adding 10 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  my_counter += 20;
  printf("--Thread #%ld; id  %ld ; adding 20 ; my_counter is %d \n",
	 t_nb, pthread_self(), my_counter);
  pthread_mutex_unlock(&my_lock);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t my_threads[NUM_THREADS];
  long l=0;
    
  for(l = 0 ; l < NUM_THREADS; l++){
    if (pthread_create(&my_threads[l], NULL, addfunction, (void *) l) == -1)
      perror("ERROR : pthread create");
  }

  
  //lock parent mutex and update variable
  pthread_mutex_lock(&my_lock);
  my_counter+= 1000;
  printf("\n--Thread #%ld; id  %ld ; adding 1000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  my_counter += 2000;
  printf("--Thread #%ld; id  %ld ; adding 2000 ; my_counter is %d \n",
	 l, pthread_self(), my_counter);
  pthread_mutex_unlock(&my_lock);

  for(l = 0 ; l < NUM_THREADS; l++){
    pthread_join(my_threads[l], NULL);
  }
  //
  printf("\n Children join father. now Thread #%ld; id %ld finishing \n",
 		 l , pthread_self());
  pthread_mutex_destroy(&my_lock);
}
                                                                                                                                                                                                                                                                                                                                                                                trimspacefile.c                                                                                     0000644 0001750 0001750 00000002704 13462530602 012531  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /***************************************
 *File: fd_copyfile.c
 ***************************************/

#include <stdio.h>     
#include <stdlib.h>     // exit
#include <unistd.h>     // fork
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>   // isapce ' ' '\t' '\n' '\v' '\f' '\r'
#define BUF_SIZE  4096

void trim( const char *input_buffer, char *trimmed_buffer){
  int j =0;
  for (int i =0; input_buffer[i] != '\0'; i++) 
  if (!isspace(input_buffer[i]))
    trimmed_buffer[j++]= input_buffer[i];
} 

int main( int argc, char *argv[]){
  int my_fd, write_count, read_count;
  char my_buffer[BUF_SIZE];
  char my_trimmed_buffer[BUF_SIZE];

  if (argc != 2) perror("!! wrong arguments");
    
  my_fd = open(argv[1], O_RDWR); //open source file
  if ( my_fd < 0 ) perror("!! in_fd");

  
  
  read_count = read(my_fd, my_buffer, BUF_SIZE); //read a data block
  printf("my read buffer: %s \n",my_buffer);
  if (read_count == 0) exit(0); // no error on last read
  else printf(" read_count :%d \n", read_count);  // error on last read  


  
  trim(my_buffer,my_trimmed_buffer);
  printf("my trimmed buffer: %s \n",my_trimmed_buffer);
  printf("length of trimed buffer : %d\n", (int)sizeof(my_trimmed_buffer)); 
  
  lseek(my_fd,0,SEEK_SET);
  write_count = write(my_fd, my_trimmed_buffer, sizeof(my_trimmed_buffer));
  printf("write count: %d\n", write_count);
  if (write_count <=0) perror("writing error");
   
  
  close(my_fd);
  
}

                                                            twice.c                                                                                             0000755 0001750 0001750 00000002453 13424052272 011021  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * File twice.c
 ********************************/
#include <stdio.h>		/* stderr, stdout, fprintf */
#include <unistd.h>		/* exec */
#include <stdlib.h>		/* EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/wait.h>		/* wait */

int main(int argc, char *argv[])
{
  pid_t son1_pid = -1;	           /* return code first fork*/
  int son1_exit_status= EXIT_SUCCESS;   /* firston return value*/
  pid_t son2_pid = -1;	           /* return code second fork*/
  int son2_exit_status= EXIT_SUCCESS;   /* firston return value*/


  if (argc < 2) {
    fprintf(stderr, "Use following synthace: ./twice command [options] [arguments]. For example ./twice ps aj");
    exit(EXIT_FAILURE);
	}
  son1_pid = fork();

  if (son1_pid == 0) {	/* Code for first son process */
    execvp(argv[1], &argv[1]);
    // next only exexute if replcament failur
    exit(EXIT_FAILURE);
  }

 son2_pid = fork();
 if (son2_pid == 0) {	/* Code for second son process */
   execvp(argv[1], &argv[1]);
   exit(EXIT_FAILURE);
 }
 /* Father Code */
 wait(&son1_exit_status);
 wait(&son2_exit_status);
 if (WIFEXITED(son1_exit_status) && WIFEXITED(son2_exit_status)
     && ((WEXITSTATUS(son1_exit_status) != EXIT_SUCCESS
	  || WEXITSTATUS(son2_exit_status) != EXIT_SUCCESS)))
		exit(EXIT_FAILURE);
 else
   exit(EXIT_SUCCESS);
}
                                                                                                                                                                                                                     twicefather.c                                                                                       0000644 0001750 0001750 00000001502 13424053450 012201  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * File twicefather.c
 ********************************/
#include <stdio.h>		/* stderr, stdout, fprintf */
#include <unistd.h>		/* exec */
#include <stdlib.h>		/* EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/wait.h>		/* wait */

int main(int argc, char *argv[])
{
  pid_t son1_pid = -1;	           /* return code first fork*/

  if (argc < 2) {
    fprintf(stderr, "Use following synthace: ./twice command [options] [arguments]. For example ./twice ps aj");
    exit(EXIT_FAILURE);
	}
  son1_pid = fork();

  if (son1_pid == 0) {	/* Code for first son process */
    execvp(argv[1], &argv[1]);
    // next only exexute if replcament failur
    exit(EXIT_FAILURE);
  }

 /* Father Code */
 execvp(argv[1], &argv[1]);
 wait(NULL); // father can only wait son if replacement fails
 exit(EXIT_FAILURE);
}
                                                                                                                                                                                              writers_readers.c                                                                                   0000644 0001750 0001750 00000007363 13445210753 013117  0                                                                                                    ustar   ivan                            ivan                                                                                                                                                                                                                   /********************************
 * File : writers_readers.c
 * Shared memory region access by writers and readers
 ********************************/
#include <stdio.h>	   /* stderr, stdout, fprintf, perror */
#include <stdlib.h>
#include <unistd.h>        // fork
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <wait.h>
#define KEY1  12
#define KEY2  13

int i, pid;
int sem_id;
int shm_id;
int *shared_mem,*nb_readers;
struct sembuf operation;

int main(void) {
  // shared memory region allocation
  if ((shm_id = shmget(KEY1, 1000, 0750|IPC_CREAT|IPC_EXCL)) == -1)
    perror("!error shmget");
  if ((shared_mem = shmat(shm_id, NULL, 0)) == NULL)
    perror("!error shmat");
  
  //semaphore creation
  if ((sem_id = semget(KEY2, 2, IPC_CREAT|IPC_EXCL|660)) == -1)
    perror("!error semget");
  
  //semaphore MUTEX initiliazed to 1
  if ((semctl(sem_id, 0, SETVAL, 1)) == -1)
    perror("!error semctl 0");
  //semaphore READ_ACCESS initiliazed to 1
  if ((semctl(sem_id, 1, SETVAL, 1)) == -1)
    perror("!error semctl 1");
  
  nb_readers = shared_mem + 1;
  
  pid = fork();
  if ( pid == 0 ) { //child process 1 reads shared_var
    for (i=1 ; i<10 ;i++){
      // Operation P(MUTEX)
      operation.sem_num = 0;
      operation.sem_op = -1;
      operation.sem_flg = 0;
      semop(sem_id, &operation, 1);
      nb_readers++;
      
      if (*nb_readers == 1) {   // P(ACCESS)
	operation.sem_num = 1;
	operation.sem_op = -1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
      }
      
      // V(MUTEX)
      operation.sem_num = 0;
      operation.sem_op =  1;
      operation.sem_flg = 0;
      semop(sem_id, &operation, 1);
      
      printf("Reader 1: shared_mem equals %d\n", *shared_mem);
      
      // P(MUTEX)
      operation.sem_num = 0;
      operation.sem_op =  -1;
      operation.sem_flg = 0;
      semop(sem_id, &operation, 1);
      nb_readers--;
      
      if(*nb_readers == 0) {  // V(ACCESS)
	operation.sem_num = 1;
	operation.sem_op =  1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
      }
      
      // V(MUTEX)
      operation.sem_num = 0;
      operation.sem_op =  1;
      operation.sem_flg = 0;
      semop(sem_id, &operation, 1);
    }
    exit(0);
  }
  else {
    pid = fork();
    if (pid == 0) { // child 2 reads sahred_var
      for (i=1 ; i<10 ;i++){
	// Operation P(MUTEX)
	operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
	nb_readers++;
	
	if (*nb_readers == 1) {   // P(ACCESS)
	  operation.sem_num = 1;
	  operation.sem_op = -1;
	  operation.sem_flg = 0;
	  semop(sem_id, &operation, 1);
	}
       
	// V(MUTEX)
	operation.sem_num = 0;
	operation.sem_op =  1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
	
	printf("Reader2: shared_mem equals %d\n", *shared_mem);
	// P(MUTEX)
	operation.sem_num = 0;
	operation.sem_op =  -1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
	nb_readers--;
       
	if(*nb_readers == 0) {  // V(ACCESS)
	  operation.sem_num = 1;
	  operation.sem_op =  1;
	  operation.sem_flg = 0;
	  semop(sem_id, &operation, 1);
	}
	
	// V(MUTEX)
	operation.sem_num = 0;
	operation.sem_op =  1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
      }
      exit(0);
    }
    else {// father code is the writer
      for (i=1 ; i<10 ;i++){
	// Operation P(MUTEX)
	operation.sem_num = 0;
	operation.sem_op = -1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
	
	*shared_mem = i*10;
	printf("-->Writer1: shared_var equals %d\n", *shared_mem);
	sleep(1);
	
	// V(MUTEX)
	operation.sem_num = 0;
	operation.sem_op =  1;
	operation.sem_flg = 0;
	semop(sem_id, &operation, 1);
      }
    }
  }
  wait(NULL);wait(NULL);
  shmdt(shared_mem);
  shmctl(shm_id, IPC_RMID,NULL);
  semctl(sem_id, IPC_RMID, 0);
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             