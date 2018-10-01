/*
 * simpleCSVsorter.c
 *
 *  Created on: Sep 20, 2018
 *      Author: Justin Chan, Forrest Smith
 */

#include "simpleCSVsorter.h"


int SetHeader(Header h) {

	size_t length = 0;
	char* line = NULL;
	getline(&line, &length, stdin);
	if (feof(stdin)) {
		return -1;
	}

	int quotes = 0;
	int arrsize = 1;
	int position = 0;

	size_t i = 0;
	size_t linepos = 0;
	size_t linelength = strlen(line);

	while (linepos < linelength) {

		if (position >= arrsize) {
			arrsize *= 2;
			//h.titles = realloc(h.titles, sizeof(char*) * arrsize);
			char** tmp = malloc(sizeof(char*) * (arrsize + 1));
			memcpy(tmp, h.titles, arrsize / 2);
			//free(h.titles);
			h.titles = tmp;
		}

		i = 0;
		size_t entrylength = 30;
		char* entry = (char*) malloc((entrylength + 1) * sizeof(char));

		int a = 0;
		for (a = 0; a < entrylength; a++) {
			entry[a] = '\0';
		}

		while (linepos < linelength) {
			char c = line[linepos];

			if (c == '"' && quotes == 0)
				quotes++;
			else if (c == '"' && quotes == 1)
				quotes--;

			if ((c == ',' && quotes == 0) || c == '\n') {
				linepos++;
				break;
			}
			entry[i] = line[linepos];

			i++;
			linepos++;
			if (i >= entrylength) {
				entrylength *= 2;
				entry = realloc(entry, sizeof(char) * (entrylength + 1));
				for (a = entrylength / 2 + 2; a < entrylength; a++) {
					entry[a] = '\0';
				}
			}
		}
		h.titles[position] = entry;
		position++;
	}

	return position;
}

/**
 * Reads current row and returns array of entries
 */

char** GetLine() {

	// Use getLine to get current line and store in "line" variable
	size_t length = 0;
	char* line = NULL;
	/*size_t r = */getline(&line, &length, stdin);
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
		char* entry = (char*) malloc((entrylength + 1) * sizeof(char));

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
				entry = realloc(entry, sizeof(char) * (entrylength + 1));
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

format getType(char* str) {
	int i = 0;
	int n = strlen(str);
	format ret = INTEGER;
	int periods = 0;
	for (i = 0; i < n; i++) {
		if (i == 0 && str[i] == '-') {
			continue;
		} else if (!isdigit(str[i])) {
			if (str[i] == '.' && periods == 0) {
				ret = DOUBLE;
				periods++;
			} else {
				ret = STRING;
				break;
			}
		}
	}
	return ret;
}

int FillRows(Row* Rows, Header header, int columns) {
	int rows = 0;
	int capacity = 1;
	int w = 0;

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
		for (w = 0; w < columns; w++) {
			format t = getType(entries[w]);
			if (t > header.types[w]) {
				header.types[w] = t;
			}
		}
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

int main(int argc, char ** argv) {

	if (argc != 3) {
		fprintf(stderr, "Invalid argument count");
		return 0;
	}

	if (strcmp(argv[1], "-c") != 0) {
		printf("u dumb");
		return 0;
	}

	// Get column titles
	Header header = { NULL, NULL };
	header.titles = (char**) malloc(sizeof(char*) * 10);

	int i = 0;
	int c = SetHeader(header); // Number of columns in table
	if (c == -1) {
		// error
		return -1;
	}
	header.types = (format*)malloc(sizeof(format) * c);
	for (i = 0; i < c; i++) {
		header.types[i] = INTEGER;
	}

	//printf("FIND: %s\n", argv[2]);
	Row* rows = (Row*)malloc(sizeof(Row));
	rows[0].entries = NULL;

	int rowcount = FillRows(rows, header, c);

	int index = -1; // index of column to sort on
	char* colname = argv[2];
	printf("%s\n", header.titles[0]);
	/*for (i = 0; i < c; i++) {
		printf("colname: %p %s, header.titles[%d]: %p, %s", colname, colname, i, header.titles[i], header.titles[i]);
		if (strcmp(colname, header.titles[i]) == 0) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		printf("u dun goofd");
		return 0;
	}

	Row* out = mergeSort(rows, rowcount, index, header.types[index]);

	free(rows);

	//printf("\nROWCOUNT == %d", rowcount);
	//printf("\n\nSTRUCTTEST:%s", Rows[3].entries[2]);

	int p = 0;
	for (p = 0; p < c; p++) {
		printf("%s", header.titles[p]); // print out the top row
		if (p != c - 1) {
			printf(",");
		}
	}
	printf("\n");

	for (i = 0; i < rowcount; i++) {
		for (p = 0; p < c; p++){
			printf("%s", out[i].entries[p]);
			if (p != c - 1) {
				printf(",");
			}
		}
		printf("\n");
	}

	free(out);

//	int targetCol = GetIndex(headers, argv[2]);
//	printf("TARGET COLUMN == %d\n", targetCol);

	printf("\n");*/
	return 0;
}
