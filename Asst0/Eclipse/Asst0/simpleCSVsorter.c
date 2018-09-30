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

	char** headers = GetLine();
	printf("FIND: %s\n", argv[2]);
	int targetCol = GetIndex(headers, argv[2]);
	printf("%d", targetCol);

	// Testing mergeSort
	//char ** ee = { "" };

	return 0;
}

// GetLine: Reads each line until the end and returns the string

char** GetLine() {
	// Use getLine to get current line and store in "line" variable
	size_t length = 0;
	char* line = (char*) malloc(length * sizeof(char));
	getline(&line, &length, stdin);
	//printf("%s\n", line);

	// Tokenize based on quotes counts
	// Create 2D array to hold each entry
	int quotes = 0;
	int arrsize = 1;
	int position = 0;
	char** entries = (char**) malloc(arrsize * sizeof(char*));

	// Parse for commas and quotes
	int i = 0;
	int linepos = 0;
	int linelength = strlen(line);

	// While still entering entries
	while (linepos < linelength) {

		// If we have more entries than array size, double array length
		if (position >= arrsize) {
			arrsize *= 2;
			entries = realloc(entries, arrsize);
		}

		i = 0;
		int entrylength = 30;
		char* entry = (char*) malloc(entrylength * sizeof(char));

		// While reading from the entries line
		while (linepos < linelength) {

			char c = line[linepos];

			// Check for quotes
			if(c == '"' && quotes == 0)quotes++;
			else if(c == '"' && quotes == 1) quotes--;

			// Check for commas sans quotes
			if((c == ',' && quotes == 0) || c == '\n')
			{
				linepos++;
				break;
			}
			entry[i] = line[linepos];

			i++;
			linepos++;
			if (i >= entrylength) {
				entrylength *= 2;
				entry = realloc(entry, entrylength);
			}
		}
		printf("%s | ", entry);
		entries[position] = entry;
		position++;
	}
	printf("\n---------\n");
	//printf("\n%s",entries[2]);
	return entries;
}

int GetIndex(char** source, char* target)
{
	if(!source || !target)
	{
		fprintf(stderr, "Cannot find index of null source or null target");
		return -1;
	}

	int i = 0;

	while(source[i])
	{
		if (strcmp(source[i], target) == 0) return i;
		i++;
	}

	return -1;
}
