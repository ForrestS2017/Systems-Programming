/*
 * simpleCSVsorter.c
 *
 *  Created on: Sep 20, 2018
 *      Author: Justin Chan, Forrest Smith
 */
#include "simpleCSVsorter.h"


int main(int argc, char ** argv) {

	/*printf("%s", argv[1]);

	int r = 1;

	FILE* fp = NULL;
	fp = fopen(argv[1], "r");

	while(r != EOF)
	{
	char* headers = (char*) malloc(1000*sizeof(char));
	r = fscanf(fp, "%s\n", headers);
	printf("%s___\n", headers);
	}*/

	/*char * s = "hi";
	char * t = "apple";

	printf("%d\n", strcmp(s, t)); // 1
	printf("%d\n", stringComparator(s, t)); // 1
	*/

	/*int a = 8;
	int b = 1;
	printf("%d\n", intComparator(&a, &b)); // 7
	*/

	int c = 3;
	int d = 10;
	printf("%d\n", intComparator(&c, &d));

	return 0;
}
