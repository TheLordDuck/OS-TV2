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
  struct contact {
    char name[10];
    char phone[10];
    struct contact *next;
  };

  int fd, i, j;
  struct contact *my_contact, *current_contact,*contact_list;
  char *mem_address;

  // creates projection
  fd = open ("directory.txt", O_RDWR);
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
