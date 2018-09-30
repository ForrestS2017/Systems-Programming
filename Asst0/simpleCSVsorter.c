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

	// Check for weird space
	//printf("|%s|", trim("\"The Chronicles of Narnia: The Lion, the Witch and the Wardrobeï¿½\""));

	// Get column titles
	char** headers = GetLine();
	//printf("FIND: %s\n", argv[2]);
	int rowcount = FillRows();
	//printf("\nROWCOUNT == %d", rowcount);
	//printf("\n\nSTRUCTTEST:%s", Rows[3].entries[2]);
	int p = 0;
	int o = 0;

	for( o = 0; o < rowcount; o++){
		printf("%d) ",o);
		for(p = 0; p < 12; p++){
			printf("%s | ", Rows[o].entries[p]);
		}
		printf("\n");
	}

//	int targetCol = GetIndex(headers, argv[2]);
//	printf("TARGET COLUMN == %d\n", targetCol);

	printf("\n");
	return 0;
}

/**
 * Reads current row and returns array of entries
 */

char** GetLine() {

	// Use getLine to get current line and store in "line" variable
	//size_t length = 0;
	char* line = NULL;
	//size_t r = getline(&line, &length, stdin);
	if (feof(stdin)) {
		//printf("**EOF**\n");
		//free(line);
		return NULL;
	}
	//printf("\n[%zu]LINE: %s", r, line);

	// Create 2D array to hold each entry
	int quotes = 0;
	int arrsize = 1;
	int position = 0;
	char** entries = (char**) malloc(arrsize * sizeof(char*));

	// Parse for commas and quotes
	size_t i = 0;
	size_t linepos = 0;
	size_t linelength = strlen(line);

	// While still entering entries
	while (linepos < linelength) {

		// If we have more entries than array size, double array length
		if (position >= arrsize) {
			arrsize *= 2;
			entries = realloc(entries, sizeof(char *) * arrsize);
		}

		i = 0;
		size_t entrylength = 30;
		char* entry = (char*) malloc(entrylength * sizeof(char));

		int a = 0;
		for (a = 0; a < entrylength; a++) {
			entry[a] = '\0';
		}

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

			i++;
			linepos++;
			if (i >= entrylength) {
				entrylength *= 2;
				entry = realloc(entry, sizeof(char) * entrylength);
				for (a = entrylength / 2 + 2; a < entrylength; a++) {
					entry[a] = '\0';
				}
			}
		}
		entries[position] = entry;
//		printf("ENTRIES 0: %s\n", entries[0]);
		//*****printf("(%d)%s | ", position, entries[position]);
		//printf("%s\n", entries[0]);
		position++;
	}

	//printf("\n\nPRE-ENTRY: (%s)\n", entries[0]);
	return entries;
}

int FillRows() {
	int rows = 0;
	int capacity = 1;
	int w = 0;
	Rows = (Row*)malloc(capacity * sizeof(Row));
	for (w = 0; w < capacity; w++) {
		Rows[w].entries = NULL;
	}

	while (1) {
		//printf("hi\n");
		char** entries = GetLine();
		if (entries == NULL) {
			//fprintf(stdout, "\nEOF FILLROWS\n");
			//printf("END FILLROWS 1");
			//printf("\nFILLROWS: %d", rows);
			return rows;
		}
		//printf("ENTRY: %s\n", entries[0]);
		Rows[rows].entries = entries;
		//printf("STRUCT: %s\n", entries[2]);
		rows++;
		//printf("ROW: %d\n", rows);

		if (rows >= capacity) {
			capacity *= 2;
			Rows = realloc(Rows, capacity * sizeof(Row));
			for (w = capacity / 2 + 2; w < capacity; w++) {
				Rows[w].entries = NULL;
			}
		}
	}
	//printf("END FILLROWS 2");
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
