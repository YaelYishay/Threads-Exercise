/*
 main process makes five threads, 
 each thread randomly gets a number, 
 if a thread got a prime number it cancels the 
 other threads and ends
*/

//------includes----------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>


//------functions---------
void *thread_func();
void my_kill();
int is_prime(int);

//------const and enums----
pthread_t thread_data[5];
pthread_once_t threads_init = PTHREAD_ONCE_INIT;

//------Main--------------
//makes five threads and waits for them to end

int main(){
	int status,
	index;

	for(index = 0 ; index < 5 ; index++){
		status = pthread_create(&thread_data[index], NULL,
				thread_func, NULL);
		if(status!=0){
			fputs("pthread create failed", stderr);
			exit(EXIT_FAILURE);
		}
	}
	for(index = 0 ; index < 5 ; index ++)
		pthread_join(thread_data[index],NULL) ;

	return EXIT_SUCCESS;
}


//-----the threads code----------
//get random number if its prime end the threads
void *thread_func(){


	while (thread_data[4] == 0 );

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);

	int number;

	srand(time (NULL));

	while(1){
		number = rand()%1000;
		pthread_testcancel();

		if(is_prime(number) == 1){
			if(pthread_once(&threads_init, my_kill)!=0)
				printf("pthread_once failed\n");
			else{
				printf("prime number is %d\n",number);
				pthread_exit(NULL);
			}
		}
	}
}
//------my kill--------------------
//kill all other treads
void my_kill(){

	int i ;
	for(i=0; i<5 ;i++){
		if( pthread_self() != thread_data[i]){
			if(pthread_cancel(thread_data[i])!= 0)
				printf("pthread cancel did not work\n");
		}
	}
}

//----is prime------------
//check if a number is prime
int is_prime(int number){


	int  i, flag = 0;

	if(number == 0 || number == 1)
		return 0;

	for(i=2; i<=number/2; ++i)
	{

		if(number%i==0)
		{
			flag=1;
			break;
		}
	}
	if (flag==0)
		return 1;
	else
		return 0;

}

