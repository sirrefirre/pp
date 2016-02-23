#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#define MAXBIRDS 15
#define MAXSIZE 10
#define SHARED 0
#define TRUE 1

/*
Unfair implementation of hungry birds, unlucky birds might starve when they
wait for the dish. 
 */

sem_t empty, food, eating; 	//create semaphores
void *babybird(void *);		// consumers
void *mamabird();			// producer
int birds, dishsize, currentworms;

int main(int argc, char *argv[]){
	long l; /* use long in case of a 64-bit system */
	pthread_attr_t attr;
	pthread_t birdid[MAXBIRDS];
	pthread_t mamaid;

	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	
	/* read command line args if any */
	birds = (argc > 1)? atoi(argv[1]) : MAXBIRDS;
	dishsize = (argc > 2)? atoi(argv[2]) : MAXSIZE;
	if(birds > MAXBIRDS) birds = MAXBIRDS;
	if(dishsize > MAXSIZE) dishsize = MAXSIZE;
	printf("%i birds %i worms\n",birds, dishsize);
	
	//set semaphore variables
	sem_init(&empty, SHARED, 0);
	sem_init(&food, SHARED, 0);
	sem_init(&eating, SHARED, 1);
	
	//fill dish
	currentworms = dishsize;

	//create threads
	pthread_create(&mamaid, &attr, mamabird, NULL);
	for (l = 0; l < birds; l++)
		pthread_create(&birdid[l], &attr, babybird, (void *) l);
	
	// wait for threads
	pthread_join(mamaid, NULL);
	for(l; l > 0; l--)
		pthread_join(birdid[l], NULL);	
}

//wait for empty dish and refill
void *mamabird(){
	//init
	printf("mama built a nest\n");
	printf("worms collected to the new birds!\n");
	while(TRUE){
		sem_wait(&empty);
		printf("worms refilled\n");
		currentworms = dishsize;
		sem_post(&food);
	}
}

// eat from dish and tell mama when empty
void *babybird(void *arg){
	int sval;
	int *me = (int*)arg;
	printf("%i born!\n", me);
	sleep(rand()%5);
	while(TRUE){
		//try to eat	
		printf("%i at barrier", me);
		sem_wait(&eating);
		//if no food, tell mama
		if(currentworms < 1){
			printf("%i I'M HUNGRY MAMA!!!!!\n", me);
			sem_post(&empty);
			sem_wait(&food);
		}

		//eat
		printf("%i eating!\n", me);
		currentworms--;
		sem_post(&eating);

		//sleep
		int random = 1 + rand() % 3;
		sleep(random);
	}
}
