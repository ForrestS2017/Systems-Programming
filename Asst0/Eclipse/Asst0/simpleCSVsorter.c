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

	// Testing mergeSort
	/*char * a[] = { "food", "calories", "fat", "sodium" };
	header.titles = a;
	format b[] = { STRING, INTEGER, INTEGER, DOUBLE };
	header.types = b;
	Rows = malloc(sizeof(Row) * 10);
	char *** c = malloc(sizeof(char**) * 10);
	char* c0[] = { "pizza", "201", "17", "-9.8" };
	c[0] = c0;
	char* c1[] = { "soup", "200", "12", "9.8" };
	c[1] = c1;
	char* c2[] = { "pie", "500", "25", "4.5" };
	c[2] = c2;
	char* c3[] = { "celery", "-10", "0", "-0.9" };
	c[3] = c3;
	char* c4[] = { "chicken", "10", "8", "0.9" };
	c[4] = c4;
	char* c5[] = { "ice cream", "", "4", "10.9" };
	c[5] = c5;
	char* c6[] = { "", "20", "18", "4.5" };
	c[6] = c6;
	char* c7[] = { "banana", "87", "55", "" };
	c[7] = c7;
	char* c8[] = { "kiwi", "91", "46", "108.9" };
	c[8] = c8;
	char* c9[] = { "lemon", "64", "-90", "" };
	c[9] = c9;
	for (int i = 0; i < 10; i++) {
		Rows[i].entries = c[i];
	}

	Row* tmp = mergeSort(Rows, 10, 0, STRING);
	//Row* tmp = mergeSort(Rows, 10, 1, INTEGER);
	//Row* tmp = mergeSort(Rows, 10, 2, INTEGER);
	//Row* tmp = mergeSort(Rows, 10, 3, DOUBLE);
	free(Rows);
	Rows = tmp;
	for (int j = 0; j < 10; j++) {
		printf("%s,%s,%s,%s\n", Rows[j].entries[0], Rows[j].entries[1], Rows[j].entries[2], Rows[j].entries[3]);
	}
	free(c);
	free(Rows);*/

	// Get column titles

	char** headers = GetLine();
	printf("FIND: %s\n", argv[2]);
	int rowcount = FillRows();
	printf("ROWS == %d", rowcount);
//	printf("\n\n%s", Rows[3].entries[0]);
//	int targetCol = GetIndex(headers, argv[2]);
//	printf("TARGET COLUMN == %d\n", targetCol);

	return 0;
}

/**
 * Reads current row and returns array of entries
 */

char** GetLine() {

	// Use getLine to get current line and store in "line" variable
	size_t length = 0;
	char* line = NULL;
	size_t r = getline(&line, &length, stdin);
	//printf("\n[%d]LINE: %s", r,line);
	if (r < 1) {
		free(line);
		return NULL;
	}

	// Create 2D array to hold each entry
	int quotes = 0;
	int arrsize = 1;
	int position = 0;
	char** entries = (char**) malloc(arrsize * sizeof(char*));

	// Parse for commas and quotes
	size_t i = 0;
	size_t linepos = 0;
	size_t linelength = strlen(line);

	//printf("%d>>%s", r, line);
	// While still entering entries
	while (linepos < linelength) {
		//if(position >= 12) return NULL;
		// If we have more entries than array size, double array length
		if (position >= arrsize) {
			arrsize *= 3;
			entries = realloc(entries, arrsize);
		}

		i = 0;
		size_t entrylength = 30;
		char* entry = (char*) malloc(entrylength * sizeof(char));
		// While reading from the entries line
		while (linepos < linelength) {

			char c = line[linepos];

			// Check for quotes
			if (c == '"' && quotes == 0)
				quotes++;
			else if (c == '"' && quotes == 1)
				quotes--;

			// Check for commas sans quotes
			if ((c == ',' && quotes == 0) || c == '\n') {
				linepos++;
				break;
			}
			entry[i] = line[linepos];
			//printf("%s> ", entry);

			i++;
			linepos++;
			if (i >= entrylength) {
				entrylength *= 3;
				entry = realloc(entry, entrylength);
			}
		}

		printf("%s | ", entry);
		entries[position] = entry;
		position++;
	}
	printf("\n");
	return entries;
}

int FillRows() {
	int rows = 0;
	while (1) {

		char** entries = GetLine();
		if (entries == NULL) {
			fprintf(stderr, "End of File");
			return rows;
		}
		printf("1ROW: %d >> ",rows);
		Row* row = (Row*) malloc(sizeof(Row));
		row->entries = entries;
		rows++;
		printf("2ROW: %d\n",rows);
	}
	printf("%d", rows);
	return rows;
}

int GetIndex(char** source, char* target) {

	if (!source || !target) {
		fprintf(stderr, "Cannot find index of null source or null target");
		return -1;
	}

	int i = 0;
	while (source[i]) {
		if (strcmp(source[i], target) == 0)
			return i;
		i++;
	}

	return -1;
}
