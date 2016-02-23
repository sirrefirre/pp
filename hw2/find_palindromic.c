/*
	Author Kardå Chalak
	12 Feb 2016
	kardac@kth.se
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h> 
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <omp.h>

double start_time, end_time;

#define MAXWORDS 500000

char* reverse_string(char* str);
//void* str_tolower(char* str);

int main(int argc, char *argv[]) {
	struct stat fs;
	int fd;
	char *filepath;
	char *addr;
	char *word[MAXWORDS];
	char *pivot;
	char *buffer;
	int i;
	int j;
	int wCount = 0;
	int pCount = 0;
	
	if(argc < 2){
		fprintf(stderr,"Argument 1 need to be path to file");
	}	
	
	/*opens word file for reading and writing*/
	filepath = argv[1];
	fd = open(filepath, O_RDWR);
	if(fd == -1){
		perror("open");
		return 1;
	}
	
	/*open file for writing found palindromes*/
	FILE *fp;
	fp = fopen("palindromes", "w");
	if(fp == NULL){
		perror("fopen");
		return 1;
	} 
	
	/* sets threads*/
	int maxthreads = omp_get_max_threads();
	int numWorkers;
	numWorkers = (argc > 2)? atoi(argv[2]) : maxthreads;
	if (numWorkers > maxthreads) numWorkers = maxthreads;
	
	omp_set_dynamic( 0 );				/*Disable dynamic adjustment of the number of threads*/
	omp_set_num_threads(numWorkers);	/*Request numWorkers of threads*/
	fstat(fd, &fs);						/*get file stats: length*/
	
	/*map file into memory for effective reading*/
	addr =	(char*) mmap(	0,				
							fs.st_size,		
							PROT_READ | PROT_WRITE,		
							MAP_PRIVATE,	
							fd ,		
							0);	
	if(addr == (void *) -1){
		perror("mmap");
		return 1;
	}
	close(fd);						/*can close the fd as the fd counter was incremented by mmap*/
	buffer = strtok(addr, "\n");	/*get word*/
	
	/*traverse true all words and store in str array*/
	while(buffer != NULL){
		word[wCount] = buffer;
		wCount++;
		buffer = strtok(NULL, "\n");
	}
	printf("Word Count: %d\n", wCount);
	
	/**************************** Search for palindromic words *********************/
	/*outer loop coses the word to reverse, inner loop compares*/
	start_time = omp_get_wtime();
	/*chunksize 2 showed best performance in tests (tested 1,2,4, and 8)*/
	#pragma omp parallel for schedule(dynamic,2) private(j,pivot)
	for(i=0; i < wCount; i++ ){
		pivot = (char *)malloc(strlen(word[i]) + 1);			/*pivot will be reversed and compared*/
		memcpy((void *) pivot, word[i], strlen(word[i]) + 1);	/*copy word to pivot*/
		pivot=reverse_string(pivot);
		for(j=i; j<wCount; j++){
			if( 0 == strcmp(pivot, word[j])){
				
				if(i == j){
				#pragma omp critical
					fputs(word[i], fp);				
					fputs("\n", fp);
					pCount = pCount +1;
				}else{
				#pragma omp critical
					fputs(word[i], fp);				
					fputs("\n", fp);
					fputs(word[j], fp);				
					fputs("\n", fp);
					pCount = pCount +2;
				}
				
				break;
			}  
		}
		free(pivot);
	}
	printf("Palindromes: %d\n", pCount);
	/*close the file*/
	end_time = omp_get_wtime();
	printf("it took %f seconds\n", end_time - start_time);
	fclose(fp);
}

char* reverse_string(char *str){

	int length = strlen(str);
	char temp;
	int start;
	int end = length -1;
	for(start = 0; start < end; start++){
		temp = str[start];
		str[start] = str[end];
		str[end--] = temp;
	}
	//printf("wordReverse: %s\n", str);
	return str;
}	
/*void* str_tolower(char* str){
	int i = 0;
	while( str[i] ){
		str[i] = tolower(str[i]);
		i++;
	}
	printf("wordLowercase: %s\n", str);
}*/

