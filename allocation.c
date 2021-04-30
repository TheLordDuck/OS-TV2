/********************************/
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
   
  fd = open ("newdirectory.txt", O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
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
