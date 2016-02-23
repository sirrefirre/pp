#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#define MAXBEES 15
#define MAXSIZE 10
#define SHARED 0
#define TRUE 1
/*
Unfair implementation of bear and honeybees, unlucky bees might not be able
to deposit honey.
*/

sem_t fullPot, empty, filling; 	//create semaphores
void *honeyBees(void *);		// consumers
void *bear();			// producer
int bees, potSize, currentPortions = 0;


int main(int argc, char *argv[]){
	long l; /* use long in case of a 64-bit system */
	pthread_attr_t attr;
	pthread_t beeid[MAXBEES];
	pthread_t bearid;

	/* set global thread attributes */
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	
	/* read command line args if any */
	bees = (argc > 1)? atoi(argv[1]) : MAXBEES;
	potSize = (argc > 2)? atoi(argv[2]) : MAXSIZE;
	if(bees > MAXBEES) bees = MAXBEES;
	if(potSize > MAXSIZE) potSize = MAXSIZE;
	printf("%i bees %i potsize\n",bees, potSize);
	
	//set semaphore variables
	sem_init(&fullPot, SHARED, 0);
	sem_init(&empty, SHARED, 1);
	sem_init(&filling, SHARED, 1);
	
	//create threads
	pthread_create(&bearid, &attr, bear, NULL);
	for (l = 0; l < bees; l++)
		pthread_create(&beeid[l], &attr, honeyBees, (void *) l);
	
	// wait for threads
	pthread_join(bearid, NULL);
	for(l; l > 0; l--)
		pthread_join(beeid[l], NULL);	
}

//wait for fullPot and eat it
void *bear(){
	//init
	printf("bear moved to the area!\n");
	while(TRUE){
		sem_wait(&fullPot);
		printf("honey is eaten by bear!\n");
		currentPortions = 0;
		sem_post(&empty);
	}
}

//collect honey and wake bear when pot full
void *honeyBees(void *arg){
	int *me = (int*)arg;
	printf("%i born!\n", me);
	while(TRUE){
		//try to fill		
		sem_wait(&filling);
		printf("%i adding honey!\n", me);
		//fill
		currentPortions++;
		//if last portion, wake bear
		if(currentPortions == potSize){
			printf("%i says pot is full!\n", me);
			sem_post(&fullPot);
			sem_wait(&empty);
		}
		sem_post(&filling);
		//gather honey
		int random = 1 + rand() % 3;
		sleep(random);
	}
}
