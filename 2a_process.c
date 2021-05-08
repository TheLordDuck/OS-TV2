#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>

//function that returns the price of each position
float getPrice(int pos, int servei){
  float matriu[3][3]={{3.50,2.50,3.00},{4.00,4.50,5.00},{1.00,1.50,2.50}};
  return matriu[pos][servei];
}

//function that returns the time of each position
int getTime(int pos, int servei){
  int matriu[3][3]={{3,1,2},{3,4,5},{1,1,1}};
  return matriu[pos][servei];
}

//main function
int main()
{
  //I inicialize op1,op2 and op3 to -1 because first i cant calculate time if these variables are in -1
  //because makes no sense to calculate time on a meal u have not ordered, so first u have to make an order, o get a message to do it
  //once u made the order u can use service 2 and 3 to meke these calculations from the dishes u have choosen, and get back the price or time of your meal.
  int opcio, op1=-1, op2=-1, op3=-1;
  
  do{
    printf("-----------------------------------------\n");
    printf("Select and option: type 0 to exit program\n");
    printf("1. Service one: Meal order.\n");
    printf("2. Service two: Calculate deliver time of actual chosen order\n");
    printf("3. Service three: Calculate total meal cost of actual chosen order\n");
    scanf("%d", &opcio);
    
    switch(opcio){
    case 1 :{
	//execute service 1 code
	int opcio1, opcio2, opcio3;
	opcio1=0;opcio2=0;opcio3=0;
	//first part of the meal order
	while(opcio1 < 1 || opcio1 > 3){
	  printf("ENTRANT CHOISES.\n1)Cripsy baked falafel - 3.50 EUR - 3 min.\n2)Peanut Slaw in soba Noodles - 2.50 EUR - 1 min.\n3)Bio Hummus - 3.00 EUR - 2 min.\n");
	  scanf("%d", &opcio1);
	}
	op1 = opcio1;
	//second part of the meal order
	while(opcio2 < 1 || opcio2 > 3){
	  printf("MAIN DISH CHOISES.\n1)Minimal Eggplant Lasagna - 4.00 EUR - 3 min.\n2)Spicy Veggie Tacos- 4.50 EUR - 4 min.\n3)Plant Based XXL Burger - 5.00 EUR - 5 min.\n");
	  scanf("%d", &opcio2);
	}
        op2 = opcio2;
	//third part of the meal order
	while(opcio3 < 1 || opcio3 > 3){
	  printf("DRINKS.\n1)Sparling Water - 1.00 EUR - 1 min.\n2)Macha Ice Tea - 1.50 EUR - 1 min.\n3)Bio Orange Juice - 2.50 EUR - 1 min.\n");
	  scanf("%d", &opcio3);
	}
	op3 = opcio3;
    }
        break;
      case 2 :
	//execute service 2 code
	if(op1 == -1 && op2 == -1 && op3 == -1){
	  printf("No order to be delivered, can't calculate deliver time, please select service 1 and make your order.\n");
	} else {
	  printf("Creating child to make the calculations.\n");
	  //create pipe to give op1, op2 and op3 values to child process
	  int fd[2];
	  // fd[0] - read
	  // fd[1] - write
	  if(pipe(fd) == -1){
	    printf("An error ocurred with opening the pipe.\n");
	    return 1;
	  }
	  // create fork to make the child work
	  int pid;
	  pid = fork();
	  if(pid == -1){
	    printf("Error with fork.\n");
	    exit(-1);
	  } else if(pid == 0){
	    close(fd[0]);
	    //calculations
	    int x = getTime(0,op1-1);
	    int y = getTime(1,op2-1);
	    int z = getTime(2,op3-1);
	    int result = x+y+z;
	    // after calculations
	    if(write(fd[1], &result, sizeof(int)) == -1){
	      exit(-1);
	    } else {
	      close(fd[1]);
	      exit(0);
	    }
	  } else {
	    close(fd[1]);
	    int wstatus;
	    wait(&wstatus);
	    if(WIFEXITED(wstatus)){
	      int statusCode = WEXITSTATUS(wstatus);
	      if(statusCode == 0){
		int valueFromChild;
		read(fd[0], &valueFromChild, sizeof(int));
		close(fd[0]);
		printf("Child returned code (%d).\nTotal delivery time:  %d mins.\n", statusCode, valueFromChild);
	      } else {
		printf("Child got error code (%d).", statusCode);
	      }
	    }
	  }
	}
	break;
      case 3 :
	//execute service 3 code
	if(op1 == -1 && op2 == -1 && op3 == -1){
	  printf("No order to be delivered, can't calculate total meal cost, please select service 1 and make your order.\n");
	} else {
	  printf("Creating child to make the calculations.\n");
	  //create pipe to give op1, op2 and op3 values to child process
	  int fd[2];
	  // fd[0] - read
	  // fd[1] - write
	  if(pipe(fd) == -1){
	    printf("An error ocurred with opening the pipe.\n");
	    return 1;
	  }
	  // create fork to make the child work
	  int pid;
	  pid = fork();
	  if(pid == -1){
	    printf("Error with fork.\n");
	    exit(-1);
	  } else if(pid == 0){
	    close(fd[0]);
	    //calculations
	    float x = getPrice(0,op1-1);
	    float y = getPrice(1,op2-1);
	    float z = getPrice(2,op3-1);
	    float result = x+y+z;
	    // after calculations
	    if(write(fd[1], &result, sizeof(float)) == -1){
	      exit(-1);
	    } else {
	      close(fd[1]);
	      exit(0);
	    }
	  } else {
	    close(fd[1]);
	    int wstatus;
	    wait(&wstatus);
	    if(WIFEXITED(wstatus)){
	      int statusCode = WEXITSTATUS(wstatus);
	      if(statusCode == 0){
		float valueFromChild;
		read(fd[0], &valueFromChild, sizeof(float));
		close(fd[0]);
		printf("Child returned code (%d).\nTotal meal cost:  %.2f EUR.\n", statusCode, valueFromChild);
	      } else {
		printf("Child got error code (%d).", statusCode);
	      }
	    }
	  }
	}
	break;
      case 0:
	//exit program
	printf("Exiting program.\n");
	return 0;
      default :
	//unwanted values in menu
	printf("Opcio incorrecta, escull un servei del menu.\n");
	break;
    }
  } while (opcio != 0);
  return 0;
}
