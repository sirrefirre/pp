/* matrix summation using pthreads

features: uses a sum; the Worker[0] computes
the total sum from partial sums computed by Workers
and prints the total sum to the standard output

usage under Linux:
gcc matrixSum.c -lpthread
a.out size numWorkers

*/
#ifndef _REENTRANT 
#define _REENTRANT 
#endif 
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#define MAXSIZE 10000  		/* maximum matrix size */
#define MAXWORKERS 10	/* maximum number of workers */
//#define DEBUG

pthread_mutex_t sumLock; 	/* mutex lock for the sum */
pthread_mutex_t bot;	 	/* mutex lock for bag of tasks */
int numWorkers;			 	/* number of workers */ 
int numArrived = 0;		  	/* number who have arrived */
int tasks = 0;			  	/* performed tasks */
int sums = 0;			  	/* total sum of matrix */
int maxInd[3] = {0, 0, 0}; 	/* maximum value and indexes in matrix */
int minInd[3] = {32767, 0, 0};
int size;  					/* assume size is multiple of numWorkers */


/* bag of tasks giving out tasks to threads */
int bagOfTasks(){
	pthread_mutex_lock(&bot);
	if(tasks < size){
		tasks++;
		pthread_mutex_unlock(&bot);
		return tasks-1;
	} else{
		pthread_mutex_unlock(&bot);
		return -1;
	}
}
//add partial sums to total and set min, max if smaller/greater
void sum(int partialsum, int max, int maxI, int maxJ, int min, int minI, int minJ){
	pthread_mutex_lock(&sumLock);
	sums += partialsum;
	if(max > maxInd[0]){
		maxInd[0] = max;
		maxInd[1] = maxI;
		maxInd[2] = maxJ;
	}
	if(min < minInd[0]){
		minInd[0] = min;
		minInd[1] = minI;
		minInd[2] = minJ;
	}
	pthread_mutex_unlock(&sumLock);
}
/* timer */
double read_timer() {
	static bool initialized = false;
	static struct timeval start;
	struct timeval end;
	if( !initialized ){
		gettimeofday( &start, NULL );
		initialized = true;
	}
	gettimeofday( &end, NULL );
	return (end.tv_sec - start.tv_sec) + 1.0e-6 * (end.tv_usec - start.tv_usec);
}

double start_time, end_time; /* start and end times */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */

void *Worker(void *);

/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
	int i, j;
	long l; /* use long in case of a 64-bit system */
	pthread_attr_t attr;
	pthread_t workerid[MAXWORKERS];

	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	/* initialize mutex and condition variable */
	pthread_mutex_init(&sumLock, NULL);
	pthread_mutex_init(&bot, NULL);
	//pthread_cond_init(&go, NULL);

	/* read command line args if any */
	size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE) size = MAXSIZE;
	if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;

	/* initialize the matrix */
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			matrix[i][j] = rand()%99; //random value from 0 to 99
		}
	}

	/* print the matrix */
	#ifdef DEBUG
	for (i = 0; i < size; i++) {
		printf("[ ");
		for (j = 0; j < size; j++) {
			printf(" %d", matrix[i][j]);
		}
		printf(" ]\n");
	}
	#endif

	/* do the parallel work: create the workers */
	start_time = read_timer();
	for (l = 0; l < numWorkers; l++)
		pthread_create(&workerid[l], &attr, Worker, (void *) l);
	//wait for all childs
	for(l; l > 0; l--)
		pthread_join(workerid[l], NULL);
			end_time = read_timer();
		/* print results */
	printf("The maxImum element is %i with index %i %i\n", maxInd[0], maxInd[1], maxInd[2]);
	printf("The minImum element is %i with index %i %i\n", minInd[0], minInd[1], minInd[2]);
	printf("The total is %d\n", sums);
	printf("The execution time is %g sec\n", end_time - start_time);
}

/* Each worker sums the values in one strip of the matrix.
After a sum, worker(0) computes and prints the total */
void *Worker(void *arg) {
	long myid = (long) arg;
	int total, j;
	int maxIndex, minIndex, max, min;

	#ifdef DEBUG
	printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
	#endif

	/* sum values in my strip */
	int currentRow;
	while(true){
		currentRow = bagOfTasks();
		#ifdef DEBUG
		printf("%i currentRow value \n", currentRow);
		#endif
		//if no more tasks end
		if(currentRow == -1)
			break;
		total = 0;
		max = 0;
		min = 32767;
		for (j = 0; j < size; j++){
			total += matrix[currentRow][j];
			//currentRow for local max
			if(matrix[currentRow][j] > max){
				max = matrix[currentRow][j];
				maxIndex = j;
			}
			//currentRow for local min
			if(matrix[currentRow][j] < min){
				min = matrix[currentRow][j];
				minIndex = j;
			}
		}
		#ifdef DEBUG
		printf("%i %i %i\n", max, currentRow, maxIndex);
		printf("%i %i %i\n", min, currentRow, minIndex);
		#endif
		
		//save local min, max with indexes associated to myid
		sum(total, max, currentRow, maxIndex, min, currentRow, minIndex);
	}
	#ifdef DEBUG
	printf("worker %d stopped", myid);
	#endif
	pthread_exit(NULL);
}