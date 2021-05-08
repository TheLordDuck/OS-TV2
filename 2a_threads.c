#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <pthread.h>

struct args{
  int op1;
  int op2;
  int op3;
};

//function that returns the price of each position
void* getPrice(void *arguments){
  float matriu[3][3]={{3.50,2.50,3.00},{4.00,4.50,5.00},{1.00,1.50,2.50}};
  float* result = malloc(sizeof(float));
  struct args *args = arguments;
  float res = matriu[0][(args->op1)-1] + matriu[1][(args->op2)-1] + matriu[2][(args->op3)-1];
  *result = res;
  return (void*) result;
}

//function that returns the time of each position
void* getTime(void *arguments){
  int matriu[3][3]={{3,1,2},{3,4,5},{1,1,1}};
  int* result = malloc(sizeof(int));
  struct args *args = arguments;
  int res = matriu[0][(args->op1)-1] + matriu[1][(args->op2)-1] + matriu[2][(args->op3)-1];
  *result = res;
  return (void*) result;
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
	  int* res;
	  // create thread to treat service 2
	  pthread_t th;
	  struct args *args = (struct args *)malloc(sizeof(struct args));
	  args->op1 = op1;
	  args->op2 = op2;
	  args->op3 = op3;
	  if(pthread_create(&th, NULL, getTime, (void *)args) != 0){
	    return 1;
	  }
	  if(pthread_join(th, (void**) &res) != 0){
	    return 2;
	  }
	  printf("Time for the meal: %d mins.\n", *res);
	  free(res);
	}
	break;
      case 3 :
	//execute service 3 code
	if(op1 == -1 && op2 == -1 && op3 == -1){
	  printf("No order to be delivered, can't calculate total meal cost, please select service 1 and make your order.\n");
	} else {
	  printf("Creating child to make the calculations.\n");
	  float* res;
	  // create thread to treat service 3
	  pthread_t th;
	  struct args *args = (struct args *)malloc(sizeof(struct args));
	  args->op1 = op1;
	  args->op2 = op2;
	  args->op3 = op3;
	  if(pthread_create(&th, NULL, getPrice, (void *)args) != 0){
	    return 1;
	  }
	  if(pthread_join(th, (void**) &res) != 0){
	    return 2;
	  }
	  printf("Price of the meal: %.2f EUR.\n", *res);
	  free(res);
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
