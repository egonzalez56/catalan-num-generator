/*
 *  main.c
 *
 *  Created on: Sep 7, 2019
 *      Author: egonzalez
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "catalan.h"

#define BUFFER_SIZE 10000

unsigned char fileName[50];
unsigned char buffer[BUFFER_SIZE-1];


int main(int argc, char **argv)
{
	int catalanNums, threadNums, i;
	sTHREAD_WORK *catalanThreadWork;	// structure for thread work
	FILE **files, *catalanFile;			// file pointers for each thread, and final file pointer
	pthread_t *threads;
	pthread_attr_t attrs;
	int bytesWritten, numsWritten = 0;

	if(argc != 3)
	{
		printf("Usage: catalan catalanNums threadNums\n");
		return -1;
	}

	catalanNums = atol(argv[1]);
	threadNums = atol(argv[2]);

	/* Allocate memory for the threads, thread work, and file pointers */
	threads = malloc(sizeof(pthread_t) * threadNums);
	catalanThreadWork = malloc(sizeof(sTHREAD_WORK) * threadNums);
	files = malloc(sizeof(FILE*) * threadNums);

	/* Populate the work threads */
	populateThreads(catalanThreadWork, catalanNums, threadNums);

	printf("There are %d catalan numbers, %d threads\n", catalanNums, threadNums);

	pthread_attr_init(&attrs);

	/* Create the threads */
	for(i = 0 ; i < threadNums ; i++)
	{
		pthread_create(&threads[i], &attrs, computeCatalan, (void *)&catalanThreadWork[i]);
	}

	/* Wait for the threads, open each of their respective files */
	for(i = 0 ; i < threadNums ; i++)
	{
		pthread_join(threads[i], NULL);
		printf("Thread %d finished.\n", i+1);

		fileName[0] = '\0';
		sprintf(fileName, "catalan%d.dat", i+1);

		if((files[i] = fopen(fileName,"r")) == NULL)
		{
			printf("Failed to open file: %s\n", fileName);
		}
	}

	/* Open the final catalan file */
	if((catalanFile = fopen("catalan.dat","a")) == NULL)
	{
		printf("Failed to open file: catalan.dat\n");
	}
	else
	{
		/* combine all the files' outputs into the final file */
		while(numsWritten < catalanNums)
		{
			for(i = 0 ; i < threadNums ; i++)
			{
				if(fgets(buffer, sizeof(buffer),files[i]) != NULL)
				{
					bytesWritten = fwrite(buffer,1,strlen(buffer),catalanFile);
					numsWritten++;
				}
			}
		}
	}

	/* Close all the files */
	for(i = 0 ; i < threadNums ; i++)
	{
		fclose(files[i]);
	}
	fclose(catalanFile);

	/* Free memory used */
	free(files);
	free(catalanThreadWork);
	free(threads);

	return 0;
}

