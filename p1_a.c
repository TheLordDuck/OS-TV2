/********************************/
// File:  problem_set_1.c
// Program creates a file and store service information
// type data in the file
// File contains 4 registries for the differnt plans or services

#include <stdio.h>		/* printf */
#include <unistd.h>             /* close */
#include <stdlib.h>             /* malloc */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

int main(void)
{
  struct service {
    char plan_name[20];
    int num_users;
    double turn_over;
    _Bool crm;
  };

  int fd;
  struct service *my_service;

  fd = open ("p1-services.txt", O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
  if (fd == -1)
    perror("Error: open file");

  my_service = ( struct service* ) malloc (sizeof(struct service));
  strcpy(my_service->plan_name, "Basic Plan");
  my_service->num_users = 1;
  my_service->turn_over = 19.99;
  my_service->crm = 0;
  write(fd, my_service, sizeof(struct service));

  my_service = ( struct service* ) malloc (sizeof(struct service));
  strcpy(my_service->plan_name, "Advanced Plan");
  my_service->num_users = 5;
  my_service->turn_over = 49.99;
  my_service->crm = 0;
  write(fd, my_service, sizeof(struct service));

  my_service = ( struct service* ) malloc (sizeof(struct service));
  strcpy(my_service->plan_name, "Professional Plan");
  my_service->num_users = 10;
  my_service->turn_over = 79.99;
  my_service->crm = 0;
  write(fd, my_service, sizeof(struct service));

  my_service = ( struct service* ) malloc (sizeof(struct service));
  strcpy(my_service->plan_name, "Dual Plan");
  my_service->num_users = 0;
  my_service->turn_over = 79.99;
  my_service->crm = 1;
  write(fd, my_service, sizeof(struct service));

  close(fd);
}
