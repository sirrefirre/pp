/* matrix summation using OpenMP

   usage with gcc (version 4.2 or higher required):
     gcc -O -fopenmp -o matrixSum-openmp matrixSum-openmp.c 
     ./matrixSum-openmp size numWorkers

*/
//gcc -o p palindromic.c -fopenmp


#include <omp.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define MAXWORKERS 20  /* maximum number of workers */
#define STDFILE "words.small" 

double start_time, end_time;



int current, currentCompare;
int palindromes = 0;
int fileLength, longestWord;
int numWorkers;
char file[20] = STDFILE; 
void reverse(char *, char *);
/* read command line, initialize, and create threads */
int main(int argc, char *argv[]) {
	
	/* read command line args if any */
	numWorkers = (argc > 1)? atoi(argv[1]) : MAXWORKERS;
	if(argc > 2) strncpy(file, argv[2], 20);
	if (numWorkers > MAXWORKERS) numWorkers = MAXWORKERS;
	printf("%i %s\n", numWorkers, file);
	omp_set_num_threads(numWorkers);

	
	FILE *fp = fopen(file, "r"); //open file to read
	//calculate words and longest word
	int ch, wordLength = 0;
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch == '\n'){
			fileLength++;
			if(wordLength > longestWord) longestWord = wordLength;
			wordLength = 0;
		}else wordLength++;
	}
	rewind(fp);	//reset filepointer to read in all words

	printf("%i words, %i longest word\n", fileLength, longestWord);

	//get words from file to array
	char inArray[fileLength][longestWord];
	char copy[longestWord+2];
	int index = 0;
	while(fgets(copy, longestWord+2, fp)){
		if(copy[strlen(copy)-1] == '\n') copy[strlen(copy)-1] = '\0';
		strcpy(inArray[index++], copy);
	}
	close(fp);
	
	//set sizes of variables used for checking palindromes
	char reversed[longestWord], word[longestWord], compare[longestWord];
	char outArray[fileLength][longestWord];
	
	start_time = omp_get_wtime();
	#pragma omp parallel for schedule(dynamic) private(currentCompare, compare, word, reversed)
	for(current = 0; current < fileLength; current++){
		int notRevPal = 1;
		strcpy(word, inArray[current]); //get word to check
		
		reverse(word, reversed); // reverse the word
		// check if reversed is same word
		if(strcmp(word, reversed) == 0){
			#pragma omp critical
			{
				strcpy(outArray[palindromes++], word);
			}
			notRevPal = 0;
		}
		//if reverse palindrome skip going through the list
		if(notRevPal){
			// check rest of list for palindromic word or break when found
			for(currentCompare = current+1; currentCompare < fileLength; j++){
				strcpy(compare, inArray[currentCompare]);
				if(strcmp(compare, reversed) == 0){
					#pragma omp critical
					{
						strcpy(outArray[palindromes++], word);
						strcpy(outArray[palindromes++], reversed);
					}
					break;
				}
			}
		}
	}
	
	
	end_time = omp_get_wtime();
	//write to file
	FILE *fpw = fopen("palindromes", "w");
	int printIndex;
	for(printIndex=0; printIndex < palindromes; printIndex++){ fprintf(fpw, "%s\n", outArray[printIndex]);}
	fclose(fpw);
	
	printf("%i palindromes\n", palindromes);
	printf("it took %g seconds\n", end_time - start_time);
}


//reverse a string to ret value
void reverse(char *string, char *ret){
	int i = strlen(string);
	ret[i] = '\0';
	while(*string != '\0'){
		ret[--i] = *string;
		string++;
	}
}
