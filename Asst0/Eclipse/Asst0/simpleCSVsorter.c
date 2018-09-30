/*
 * simpleCSVsorter.c
 *
 *  Created on: Sep 20, 2018
 *      Author: Justin Chan, Forrest Smith
 */

#include "simpleCSVsorter.h"

int main(int argc, char ** argv) {


	if (argc < 3) {
		fprintf(stderr, "Invalid argument count");
	}


	// Get column titles
	char* columns;

	columns = GetLine();
	printf("%s", columns);
}
