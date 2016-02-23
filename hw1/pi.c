#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#define MAXWORKERS 10	/* maximum number of workers */
#define MIN_E 0.000001
#define START_PI 0, 1, 1, 0, 0
/*#define DEBUG*/

pthread_mutex_t workercount;
pthread_attr_t attr;
pthread_t workerid[MAXWORKERS];

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

int numWorkers;
int createdWorkers = 0;
double e;  /*epsilon*/
double start_time, end_time; /* start and end times */

/*keep count of created workers/threads*/
int wc(){
	pthread_mutex_lock(&workercount);
	if(createdWorkers < numWorkers){
		createdWorkers++;
		pthread_mutex_unlock(&workercount);
		return createdWorkers-1;
	}else{
		pthread_mutex_unlock(&workercount);
		return -1;
	}
}


	
double quad(double, double, double, double, double);
void *Worker(void *);

int main(int argc, char *argv[]){
	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	
	/* initialize mutex */
	pthread_mutex_init(&workercount, NULL);
	
	/* read command line args if any */
	e = (argc > 1)? atoi(argv[1]) : MIN_E;
	numWorkers = (argc > 2)? atof(argv[2]) : MAXWORKERS;
	if (e < MIN_E) e = MIN_E;
	if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
	
	
	start_time = read_timer();
	double result = quad(START_PI); /*starting values for pi*/

	end_time = read_timer();
	
	printf("%f is pi approximated with epsilon %f\n", result*4, e);
	printf("The execution time is %g sec\n", end_time - start_time);
}

/*calculate recursive adaptive quadrature*/
double quad(double l, double r, double fl, double fr, double area){
	int currentWorker, leftWorker;
	double m = (l+r)/2; 				/*get middle point*/
	double fm = sqrt(1-m*m); 			/*function for middle point*/
	double larea = (fl + fm)*(m-l)/2;	/*calculate left area*/
	double rarea = (fm+fr)*(r-m)/2;		/*calculate right area*/
	/*if difference between the new areas and the old area is > epsilon*/
	if(fabs((larea+rarea)-area) > e){ 
		double left[5] = {l,m,fl,fm,larea}; /*values to calculate on left area*/
		double right[5] = {m,r,fm,fr,rarea};	/*values to calculate on right area*/
		double *returnStatusLeft, *returnStatusRight;
		currentWorker = wc();				/*get current worker id*/
		/*if numWorkers not achieved*/
		if(currentWorker != -1){
			leftWorker = currentWorker;
			pthread_create(&workerid[currentWorker], &attr, Worker, (void *) &left);
			currentWorker = wc();
			if(currentWorker != -1){
				pthread_create(&workerid[currentWorker], &attr, Worker, (void *) &right);
				/* wait for child threads */
				pthread_join(workerid[leftWorker], (void **)&returnStatusLeft);
				pthread_join(workerid[currentWorker], (void**)&returnStatusRight);
				
				#ifdef DEBUG
				printf("%f %d returnStatusLeft\n", *returnStatusLeft, returnStatusLeft);
				printf("%f %d returnStatusRight\n", *returnStatusRight, returnStatusRight);
				#endif
				/*set areas to returned values of childs*/
				larea = *returnStatusLeft;
				rarea = *returnStatusRight;
				free(returnStatusLeft);
				free(returnStatusRight);
				return larea + rarea;
			}else{
				/*set left area to child return value and set right area*/
				rarea = quad(m,r,fm,fr,rarea);
				pthread_join(workerid[leftWorker], (void**)&returnStatusLeft);
				larea = *returnStatusLeft;
				free (returnStatusLeft);
				return larea + rarea;
			}
		}else{
		/*set areas*/
		larea = quad(l,m,fl,fm,larea);
		rarea = quad(m,r,fm,fr,rarea);
		return larea + rarea;
		}	
	}
	return (larea + rarea);
}
/*child worker*/
void *Worker(void *arg) {
	double *input = (double *) arg; 			/*cast input to double*/
	double *retVal = malloc(sizeof(double)*5); 	/*allocate memory for return value to parent*/
	*retVal = quad(input[0], input[1], input[2], input[3], input[4]); /*run quad*/
	
	#ifdef DEBUG
	printf("%f back\n", *retVal);
	printf("%i back adress\n", retVal);
	#endif
	/*return value to parent and terminate*/
	pthread_exit(retVal);
}
