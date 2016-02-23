/* matrix summation using pthreads

features: uses a sumLock; the Worker[0] computes
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
#define MAXSIZE 10000  /* maximum matrix size */
#define MAXWORKERS 10	/* maximum number of workers */
#define DEBUG

pthread_mutex_t sumLock;  /* mutex lock for the sumLock */
int numWorkers;			  /* number of workers */ 
int numArrived = 0;		  /* number who have arrived */
int sums = 0;
int maxInd[3] = {0, 0, 0};
int minInd[3] = {32767, 0, 0};


void sum(int partialsum, int max, int maxI, int maxJ, int min, int minI, int minJ){
	pthread_mutex_lock(&sumLock);
	sums += partialsum;
	if(max >maxInd[0]){
		maxInd[0] = max;
		maxInd[1] = maxI;
		maxInd[2] = maxJ;
	}
	if(min <minInd[0]){
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
int size, stripSize;  /* assume size is multiple of numWorkers */
int matrix[MAXSIZE][MAXSIZE]; /* matrix */
int maxi[MAXWORKERS][3];
int mini[MAXWORKERS][3];

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
	//pthread_cond_init(&go, NULL);

	/* read command line args if any */
	size = (argc > 1)? atoi(argv[1]) : MAXSIZE;
	numWorkers = (argc > 2)? atoi(argv[2]) : MAXWORKERS;
	if (size > MAXSIZE) size = MAXSIZE;
	if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
	stripSize = size/numWorkers;

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
	//printf("%i l \n", l);
	for(l; l>0;l--)
		pthread_join(workerid[l], NULL);
	end_time = read_timer();
		/* print results */
	printf("The maximum element is %i with index %i %i\n", maxInd[0], maxInd[1], maxInd[2]);
	printf("The minimum element is %i with index %i %i\n", minInd[0], minInd[1], minInd[2]);
	printf("The total is %d\n", sums);
	printf("The execution time is %g sec\n", end_time - start_time);
}

/* Each worker sums the values in one strip of the matrix.
After a sumLock, worker(0) computes and prints the total */
void *Worker(void *arg) {
	long myid = (long) arg;
	int total, i, j, first, last;
	int maxIndex[2], minIndex[2], max, min;

	#ifdef DEBUG
	printf("worker %d (pthread id %d) has started\n", myid, pthread_self());
	#endif

	/* determine first and last rows of my strip */
	first = myid*stripSize;
	last = (myid == numWorkers - 1) ? (size - 1) : (first + stripSize - 1);

	/* sum values in my strip */
	total = 0;
	max = 0;
	min = 32767;
	for (i = first; i <= last; i++)
		for (j = 0; j < size; j++){
			total += matrix[i][j];
			//check for local max
			if(matrix[i][j]>max){
				max = matrix[i][j];
				maxIndex[0] = i;
				maxIndex[1] = j;
			}
			//check for local min
			if(matrix[i][j]<min){
				min = matrix[i][j];
				minIndex[0] = i;
				minIndex[1] = j;
			}
		}
	#ifdef DEBUG
	printf("%i %i %i\n", max, maxIndex[0], maxIndex[1]);
	printf("%i %i %i\n", min, minIndex[0], minIndex[1]);
	#endif
	
	//save local min, max with indexes associated to myid
	sum(total, max, maxIndex[0], maxIndex[1], min, minIndex[0], minIndex[1]);
	pthread_exit(NULL);
}