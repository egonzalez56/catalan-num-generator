/*
 * catalan.h
 *
 *  Created on: Sep 18, 2019
 *      Author: egonzalez
 */

#ifndef CATALAN_H_
#define CATALAN_H_

typedef struct{
	int threadNum;
//	int catNums;
	int totalThreads;
	int totalCatNums;
//	short int extra;
} sTHREAD_WORK;


long double binCoefficient(int n, int k);
void *computeCatalan(void *param);
void populateThreads(sTHREAD_WORK* work, int catalan, int threads);


#endif /* CATALAN_H_ */
