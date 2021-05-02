/********************************/
// File: msq_server.c
// Client process sends in the MSQ 314

#include <stdio.h>		/* printf */
#include <stdlib.h>		/* exit */
#include <unistd.h>		/* getpid */
#include <sys/msg.h>		/* msq system calls*/ 
#define KEY 314

int main(void)
{

  struct service {
     char plan_name[20];
     char num_users[6];
     char turn_over[12];
     char crm[1];
     struct service *next;
  };

  struct request{
     long type;
     pid_t pid;
  };

  struct answer{
     long type;
     char plan_name[20];
     char num_users[6];
     char turn_over[12];
     char crm[1];
  };

  int msq_id, l;
  struct request my_request;
  struct answer my_answer;
  // gets the MSQ internal id
  if ((msq_id = msgget((key_t)KEY, 0)) == -1) {
    perror("msgget");
    exit(1);
  }

  //prepares the message and send the request
  my_request.type = 1;
  my_request.pid = getpid();

  if (msgsnd(msq_id, &my_request, sizeof(struct request)-4,0) == -1)  {
    perror("msgsnd");
    exit(2);
  }

  int i;
  for(i=0;i<4;i++){
     // receive answer from server
     if ((l = msgrcv(msq_id, &my_answer, sizeof(struct answer)-4, getpid(), 0)) ==-1) {
        perror("msgrcv");
        exit(2);
     }
     printf("service %d\n", i);
     printf("Plan Name: %s\n", my_answer.plan_name);
     printf("Num users: %s\n", my_answer.num_users);
     printf("Turn over: %s\n", my_answer.turn_over);
     printf("CRM: %s\n", my_answer.crm);
  }
  exit(0);
}
