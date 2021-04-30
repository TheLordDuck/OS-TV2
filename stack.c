/* File : stack.c  
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
