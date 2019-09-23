/*
 * catalan.c
 *
 *  Created on: Sep 18, 2019
 *      Author: egonzalez
 */

#include <stdio.h>
#include "catalan.h"
#include <pthread.h>


long double binCoefficient(int n, int k)
{
	long double result = 1;
	int i;

	// binomial coefficient symmetry property
	if(k > n - k)
	{
		k = n - k;
	}

	// special case when n = k
	if(n == k)
	{
		return 1;
	}
	else if( (n - k) < 0 )
	{
		return 0;
	}

	// compute the binomial coefficient n! / (n-k)! * k!
	for(i = 0 ; i < k ; i++)
	{
		result *= (n - i);			// n * (n-1) * (n-2) * ... * (n - k + 1)
		result /= ( (n - k) - i);	// (n-k) * (n-k-1) * ... * 1
	}

	return result;
}


void *computeCatalan(void *param)
{
	unsigned char catFileName[50];
	long double result;
	sTHREAD_WORK* temp;
	FILE *catalanThread;
	int i;

	temp = (sTHREAD_WORK*) param;

	catFileName[0] = '\0';
	sprintf(catFileName, "catalan%d.dat", temp->threadNum);

	/* Open the thread specific file */
	if((catalanThread = fopen(catFileName,"w")) == NULL)
	{
		printf("Failed to open file: %s\n", catFileName);
	}
	else
	{
		/* Calculate the catalan numbers associated with each thread */
		for(i = temp->threadNum ; i <= temp->totalCatNums ; i += temp->totalThreads)
		{
			result = binCoefficient(2*i, i) / (i + 1);
			fprintf(catalanThread,"%.0Lf\n",result);
		}
	}

	fclose(catalanThread);

	pthread_exit(0);
}


void populateThreads(sTHREAD_WORK* work, int catalan, int threads)
{
	int i;

	for(i = 0 ; i < threads ; i++)
	{
		work[i].threadNum = i + 1;
		work[i].totalThreads = threads;
		work[i].totalCatNums = catalan;
	}

	return;
}
