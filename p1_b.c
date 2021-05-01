/********************************/
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
  struct service {
    char plan_name[20];
    char num_users[6];
    char turn_over[12];
    char crm[1];
    struct service *next;
  };

  int fd, i, j;
  struct service *my_service, *current_service,*service_list;
  char *mem_address;

  // creates projection
  fd = open ("p1-services.txt", O_RDWR);
  if (fd == -1)
    perror("Error: openfile");
  mem_address = (char*) mmap(NULL, 4*sizeof(struct service),
			     PROT_READ, MAP_SHARED, fd, (off_t)0);
  close(fd);

  //Creating linked list
  current_service = NULL; i = 0;
  while (i < 4 * sizeof(struct service)) {
    my_service =  malloc(sizeof(struct service));

    j=0;
    while (j < sizeof(my_service->plan_name)) {
      my_service->plan_name[j] = mem_address[i];
      i++; j++;
    }

    j=0;
    while (j < sizeof(my_service->num_users)) {
      my_service->num_users[j] = mem_address[i];
      i++; j++;
    }

    j=0;
    while (j < sizeof(my_service->turn_over)) {
      my_service->turn_over[j] = mem_address[i];
      i++; j++;
    }

    j=0;
    while (j < sizeof(my_service->crm)) {
      my_service->crm[j] = mem_address[i];
      i++; j++;
    }

    if (current_service == NULL) { //first link
      current_service = my_service;
      service_list = my_service; // save first link address
    }
    else
      {
	my_service->next = NULL;
	current_service->next = my_service;
	current_service = my_service;
      }
   }

  // printing product database in memory
  for (i=0; i < 4;i++){
    printf("%d. service %s, %s, %s, %s\n", i, service_list->plan_name, service_list->num_users, service_list->turn_over, service_list->crm);
    service_list = service_list->next;
  }
}
