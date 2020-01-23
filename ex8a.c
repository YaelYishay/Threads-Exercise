/*
 * ex8a.c
 *
 *
 ===========================================================
 * Written by: adi ben ezra, id = 206124000, login = adibene
 *            yael yishay , id = 305345811, login = yaelor
 *
 * Algorithm:
 * this program creates 6 threads, the first three get a 
   random number between -10 and 10 if the number is positive
   adds the number to an array otherwise, delete the positive
   the thread returns how many it added and how many it delete
   the last thee threads get a random number between 1 and    	10  and searches for it in the array. returns how many   values where found

 *
 */



//-------------------------Include------------------------
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

//-----------------------Define ---------------------------
#define SIZE_ARR 100
#define NUM_OF_THREAD 6

//--------------------------------------------------------
void createPthreadFirst(pthread_t thread_data[]);
void createPthreadLast(pthread_t thread_data[]);
void waitPthread(pthread_t thread_data[]);
void* first_pthread_job ();
void* last_pthread_job ();
bool addSuccess(int randomNum,int arr[]);
bool removeRandomNum(int randomNum,int arr[]);
bool isInArr(int randomNum,int arr[]);

//---------------------------Main--------------------------
int main()
{
	pthread_t thread_data[NUM_OF_THREAD];

	srand((unsigned)time(NULL));

	createPthreadFirst(thread_data);

	createPthreadLast(thread_data);

	waitPthread(thread_data);

	return EXIT_SUCCESS;

}
//-----------------------------------------------
//this function create pthread first
void createPthreadFirst(pthread_t thread_data[])
{
	int i,
	status;

	for(i = 0; i < NUM_OF_THREAD/2; i++){
		status = pthread_create(&thread_data[i], NULL, first_pthread_job, NULL);

		//if failed
		if(status != 0){
			perror("pthread_create failed\n");
			exit(EXIT_FAILURE);
		}
	}
}
//----------------------------------------------------
//this function create pthread last
void createPthreadLast(pthread_t thread_data[])
{
	int i,
	status;

	for(i = 3; i < NUM_OF_THREAD; i++){
		status = pthread_create(&thread_data[i],NULL,last_pthread_job,NULL);

		//if failed
		if(status != 0){
			perror("pthread_create failed\n");
			exit(EXIT_FAILURE);
		}
	}
}
//--------------------------------------------------
//parent waits for threads and returns output
void waitPthread(pthread_t thread_data[])
{
	int i,j,
	status,
	*result;

	for(i = 0; i < NUM_OF_THREAD;i++){
		status = pthread_join(thread_data[i],(void**)&result);

		if(status != 0){
			perror("pthread_join failed\n");
			exit(EXIT_FAILURE);
		}

		for(j = 0; result[j] != 0; j++)
			printf("%d ",result[j]);
		putchar('\n');
		free(result);
	}

}
//----------------------------------------------------
//code for the first three process
void* first_pthread_job()
{
	int index,
	randomNum,
	arr[SIZE_ARR] = {0},
	*counter;

	counter = (int*)malloc(2*sizeof(int));
	if(counter == NULL){
		perror("cannot allocate memory\n") ;
		exit( EXIT_FAILURE) ;
	}

	for(index = 0; index < 1000; index++){
		randomNum = rand() % 20 + (-10);

		if(randomNum > 0){
			if(addSuccess(randomNum,arr))
				counter[0]++;
		}
		else if(randomNum < 0){
			if(removeRandomNum(randomNum,arr))
				counter[1]++;
		}
	}
	pthread_exit((void*)counter);

	return NULL; // for the compiler
}

//--------------------------------------------------------
void* last_pthread_job()
{
	int index,
	randomNum,
	arr[SIZE_ARR] = {0},
	*counter;

	counter = (int*)malloc(sizeof(int));
	if(counter == NULL){
		perror("cannot allocate memory\n") ;
		exit( EXIT_FAILURE) ;
	}

	for(index = 0; index < 1000; index++){
		randomNum = rand()%11;

		if(isInArr(randomNum,arr))
			(*counter)++;

	}

	pthread_exit((void*)counter);

	return NULL; // for the compiler
}

//------------------------------------------------------
//add random number to array
bool addSuccess(int randomNum,int arr[])
{
	int index;
	for(index = 0; index < SIZE_ARR; index++)
		if(arr[index] == 0){
			arr[index] = randomNum;
			return true;
		}

	return false;
}
//------------------------------------------------------
//delete opposite number from array
bool removeRandomNum(int randomNum,int arr[])
{
	int index;
	for(index = 0; index < SIZE_ARR; index++)
		if(arr[index] == abs(randomNum)){
			arr[index] = 0;
			return true;
		}

	return false;
}
//------------------------------------------------
//checks if random number is in array
bool isInArr(int randomNum,int arr[])
{
	int index;
	for(index = 0; index < SIZE_ARR; index++)
		if(arr[index] == randomNum)
			return true;

	return false;
}
