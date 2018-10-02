/*
 * simpleCSVsorter.c
 *
 *  Created on: Sep 20, 2018
 *      Author: Justin Chan, Forrest Smith
 */

#include "simpleCSVsorter.h"


int SetHeader(Header* h) {

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
			h->titles = (char**)realloc(h->titles, sizeof(char*) * (arrsize + 1));
			if (h->titles == NULL) {
				printf("ERROR: realloc failed\n");
				return -1;
			}
		}

		i = 0;
		size_t entrylength = 30;
		char* entry = (char*) malloc((entrylength + 1) * sizeof(char));
		if (entry == NULL) {
			printf("ERROR: malloc failed\n");
			return -1;
		}

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

			if (c == '\r') {
				linepos++;
				continue;
			}

			entry[i] = line[linepos];

			i++;
			linepos++;
			if (i >= entrylength) {
				entrylength *= 2;
				entry = (char*)realloc(entry, sizeof(char) * (entrylength + 1));
				if (entry == NULL) {
					printf("ERROR: realloc failed\n");
					return -1;
				}
				for (a = entrylength / 2 + 2; a < entrylength; a++) {
					entry[a] = '\0';
				}
			}
		}
		h->titles[position] = entry;
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
	getline(&line, &length, stdin);
	if (feof(stdin)) {
		return NULL;
	}

	// Create 2D array to hold each entry
	int quotes = 0;
	int arrsize = 1;
	int position = 0;
	char** entries = (char**) malloc(arrsize * sizeof(char*));
	if (entries == NULL) {
		printf("ERROR: malloc failed\n");
		return NULL;
	}

	// Parse for commas and quotes
	size_t i = 0;
	size_t linepos = 0;
	size_t linelength = strlen(line);

	// While still entering entries
	while (linepos < linelength) {

		// If we have more entries than array size, double array length
		if (position >= arrsize) {
			arrsize *= 2;
			entries = (char**)realloc(entries, sizeof(char *) * arrsize);
			if (entries == NULL) {
				printf("ERROR: realloc failed\n");
				return NULL;
			}
		}

		i = 0;
		size_t entrylength = 30;
		char* entry = (char*) malloc((entrylength + 1) * sizeof(char));
		if (entry == NULL) {
			printf("ERROR: malloc failed\n");
			return NULL;
		}

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
				entry = (char*)realloc(entry, sizeof(char) * (entrylength + 1));
				if (entry == NULL) {
					printf("ERROR: realloc failed\n");
					return NULL;
				}
				for (a = entrylength / 2 + 2; a < entrylength; a++) {
					entry[a] = '\0';
				}
			}
		}
		entries[position] = entry;
		position++;
	}

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

int FillRows(Row** Rows, Header* header, int columns) {
	int rows = 0;
	int capacity = 1;
	int w = 0;

	while (1) {
		char** entries = GetLine();
		if (entries == NULL) {
			return rows;
		}

		(*Rows)[rows].entries = entries;
		for (w = 0; w < columns; w++) {
			format t = getType(entries[w]);
			if (t > header->types[w]) {
				header->types[w] = t;
			}
		}
		rows++;
		if (rows >= capacity) {
			capacity *= 2;
			*Rows = (Row*)realloc(*Rows, capacity * sizeof(Row));
			if (*Rows == NULL) {
				printf("ERROR: realloc failed\n");
				return -1;
			}
			for (w = capacity / 2 + 2; w < capacity; w++) {
				(*Rows)[w].entries = NULL;
			}
		}
	}
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
		printf("ERROR: Incorrect number of arguments. Correct usage is ./simpleCSVsorter <sortBy> <columnName>\nExample:./simpleCSVsorter -c food\n");
		return 0;
	}

	if (strcmp(argv[1], "-c") != 0) {
		printf("ERROR: Invalid first argument found. Can only use -c to indicate sorting by column.");
		return 0;
	}

	// Get column titles
	Header header = { NULL, NULL };
	header.titles = (char**) malloc(sizeof(char*) * 10);
	if (header.titles == NULL) {
		printf("ERROR: malloc failed\n");
		return 0;
	}

	int i = 0;
	int c = SetHeader(&header); // Number of columns in table
	if (c == -1) {
		printf("ERROR: No columns found.\n");
		return 0;
	}
	header.types = (format*)malloc(sizeof(format) * c);
	if (header.types == NULL) {
		printf("ERROR: malloc failed\n");
		return 0;
	}
	for (i = 0; i < c; i++) {
		header.types[i] = INTEGER;
	}

	Row* rows = (Row*)malloc(sizeof(Row));
	if (rows == NULL) {
		printf("ERROR: malloc failed\n");
		return 0;
	}
	rows[0].entries = NULL;

	int rowcount = FillRows(&rows, &header, c);

	int index = -1; // index of column to sort on
	char* colname = argv[2];
	for (i = 0; i < c; i++) {
		if (strcmp(colname, header.titles[i]) == 0) {
			index = i;
			break;
		}
	}

	if (index == -1) {
		printf("ERROR: '%s' is not a column found in this file.\n", colname);
		return 0;
	}

	Row* out = mergeSort(rows, rowcount, index, header.types[index]);

	free(rows);

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

	printf("\n");

	return 0;
}
