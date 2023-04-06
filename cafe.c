//libraries that are used
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h> // time library
#include<pthread.h> // thread library
#include<string.h>
#include<semaphore.h> // semaphore library

//defines variables that user can change from here


#define CUSTOMER_NUMBER 25 //customer number is constant and its value is 25
#define CUSTOMER_ARRIVAL_TIME_MIN 1 //this is the minimum elapsed time that customer/thread creation can take
#define CUSTOMER_ARRIVAL_TIME_MAX 3 //this is the maximum elapsed time that customer/thread creation can take
#define REGISTER_NUMBER 5  //semaphore's value is 5 (maximum thread number that can run simultaneously)
#define COFFEE_TIME_MIN 2 //this is the minimum elapsed time that a customer/thread can stay in that register (critical section)
#define COFFEE_TIME_MAX 5 //this is the maximum elapsed time that a customer/thread can stay in that register (critical section)

//defines registers array,threads array and the semaphore that will be used

int registers[REGISTER_NUMBER]; //register array
sem_t semaphore; //creates semaphore using semaphore struct from its library (semaphore.h)
pthread_t tid[CUSTOMER_NUMBER]; //creates thread array using thread struct from its library (pthread.h)


//this is the main function for customers to buy coffee

void* buyCoffee(void *arg){

	int register_id; //variable for register id

	
	int id=(int)arg; //id of the thread
	
	sem_wait(&semaphore); //when the new customer arrives, sem_wait function decreases the semaphore value by 1
	
	
	while(1){ //register id detection loop
		srand(time(NULL)); //makes use of the computer's internal clock to control the choice of the seed.
		int r=(rand()%(REGISTER_NUMBER)); //(rand()%(upper-lower+1))+lower; generates random numbers according to the formula
		if(registers[r]==1){ //checks whether the randomly determined index specifies an empty register
			register_id=r; //store index in register_id variable
			registers[r]=0; //register lock so that no other customers can come.
			break; //used to bring the program control out of the loop.
		}
	}

	
	//creates a randomized a number between min and max to find the creation time of that thread/customer	
	srand(time(NULL)); //makes use of the computer's internal clock to control the choice of the seed. 
	int r=(rand()%(COFFEE_TIME_MAX-COFFEE_TIME_MIN+1))+COFFEE_TIME_MIN; //(rand()%(upper-lower+1))+lower; generates random numbers according to the formula		
	sleep(r); //allows the users to wait for a current thread for a specific time.
	
	//displayes the register of that customer
	printf("CUSTOMER <%d> GOES TO REGISTER <%d>.\n", id,register_id);
	
	
	sem_post(&semaphore); //sem_post function increases the semaphore value after the critical section has ended
	
	registers[register_id]=1; //because the critical section ended, made the register_id 1 to unlock it
	
 
    //calculates elapsed time by finding the time spent while buying from the register   	
	printf("CUSTOMER <%d> FINISHED BUYING FROM REGISTER <%d> AFTER <%d> SECONDS.\n", id,register_id,r); 
	
	pthread_exit(NULL); //exits thread
	
}

int main(){

	//changes every register value to 1 because all of them are unlocked at the start
	for(int i=0;i<REGISTER_NUMBER;i++){
		registers[i]=1;
	}

	int i=0,j=0; //creates variables i and j initialize to 0
	int err; //creates variables err
	int value; //creates variables value
	
	//initializes the semaphore	
	sem_init(&semaphore,0,REGISTER_NUMBER);
	
	
	//this is the main loop that we use our buyCoffee function
	
	while(i<CUSTOMER_NUMBER){
		srand(time(NULL)); //makes use of the computer's internal clock to control the choice of the seed. 
		
		//(rand()%(upper-lower+1))+lower; generates random numbers according to the formula
		int r=(rand()%(CUSTOMER_ARRIVAL_TIME_MAX-CUSTOMER_ARRIVAL_TIME_MIN+1))+CUSTOMER_ARRIVAL_TIME_MIN; 	
		sleep(r); //allows the users to wait for a current thread for a specific time.
		err=pthread_create(&(tid[i]),NULL,&buyCoffee,(void *)(i)); // 0 = there is not an error while creating the thread , 1 = there is an error (after this line 98 checks the result)
		if(err!=0) //checks if there is an error while creating the thread
			printf("Thread creation error: [%s]",strerror(err));
		printf("CUSTOMER <%d> IS CREATED AFTER <%d> SECONDS.\n",i,r);
		i++; //increments the variable i by 1
		
	}
	
	while(j<CUSTOMER_NUMBER){
		pthread_join(tid[j],NULL); //the pthread_join function waits for a thread to terminate then returns the threads exit status
		j++; //increments the variable j by 1
	}

}
