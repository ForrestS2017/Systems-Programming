/*
 * simpleCSVsorter.c
 *
 *  Created on: Sep 20, 2018
 *      Author: Justin Chan, Forrest Smith
 */
#include "simpleCSVsorter.h"


int main(int argc, char ** argv) {

	printf("%s", argv[1]);

	int r = 1;

	FILE* fp = NULL;
	fp = fopen(argv[1], "r");

	while(r != EOF)
	{
	char* headers = (char*) malloc(1000*sizeof(char));
	r = fscanf(fp, "%s\n", headers);
	printf("%s___\n", headers);
	}

	return 0;
}
