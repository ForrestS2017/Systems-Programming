/*
* simpleCSVsorter.h
*
*  Created on: Sep 28, 2018
*      Author: Justin Chan, Forrest Smith
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>

#ifndef SCANNERCSVSORTER_H_
#define SCANNERCSVSORTER_H_
char* inPath;
char* outPath;
char* colname;
DIR* inDir;
DIR* outDir;

/***** Variables, Structs, Prototypes *****/

typedef enum _format{ INTEGER=1, DOUBLE=2, STRING=3 } format;

typedef struct _Header {		// Only for first row data
	char** titles;			// Hold the titles of each column
	format* types;		// Declare the types of each column, corresponds with row entry
} Header;

typedef struct _Row {
	char** entries;			// Hold each entry of the row. The type of each corresponds to format enum
} Row;

/***** Functions *****/

// Compares two strings
int stringComparator(void*, void*);

// Compares two integers
int intComparator(void*, void*);

// Compares two doubles
int doubleComparator(void*, void*);

// Trims white space and removes quotation marks from a string
char* trim(char*);

// Merges two arrays of rows. Used for merge sort
Row* merge(Row*, int, Row*, int, int, format);

// Sorts an array of Rows using merge sort
Row* mergeSort(Row*, int, int, format);

// Checks for valid arguments
int CheckInput(int argc, char** argv);

// Reads current row and returns array of entries
char** GetLine();

// Get Index of a row entry
int GetIndex(char** source, char* target);

// Fills Rows with Row structs, using GetLine()
int FillRows();

// Gets type (_format) for a cell
format getType(char* str);



/**
* Calls getline() and returns the line parsed into a char** array where delimiter = ','
* @param argc Number of arguments
* @param argv String array of arguments
* @return -1 if any fatal errors, otherwise 1 upon success
*/
int CheckInput(int argc, char** argv) {
	// Check for correct argument count
	if (argc != 3 || argc != 5 || argc != 7) {
		fprintf(stderr, "ERROR: Incorrect number of arguments. Correct usage is ./scannerCSVsorter <sortBy> <columnName> [-d <input-directory>] [-o <output-directory>]\nExample:./simpleCSVsorter -c food\n");
		return -1;
	}

	// Check if user is using proper arguments by iterating over two args at a time
	int argpos = 1, errors = 0;
	int flags[3] = {0,0,0};

	for(argpos = 1; argpos < argc; argpos++){
		if (strcmp(argv[argpos], "-c") == 0) {
			strcpy(argv[argpos+1],colname);
			flags[0]++;
		} else if (strcmp(argv[argpos], "-d") == 0) {
			strcpy(argv[argpos+1],inPath);
			inDir = opendir(inPath);
			flags[1]++;
		} else if (strcmp(argv[argpos], "-o") == 0) {
			strcpy(argv[argpos+1],outPath);
			outDir = opendir(outPath);
			flags[2]++;
		} else {
			fprintf(stderr, "ERROR: Invalid flag. Only use -c, -d, or -o\n");
			return -1;
		}
	}

	// Handle missing -c, -d, and -o flags

	if(flags[0] != 1) {
		fprintf(stderr, "ERROR: Missing/ambiguous column to sort by. Please enter the column name after -c\n");
		return -1;
	}
	if(flags[1] < 1) {
		inPath = ".";
		inDir = opendir(inPath);
	}
	if(flags[2] < 1) {
		outPath = ".";
		outDir = opendir(outPath);
	}

	// Handle too many -d or -o flags

	if(flags[1] > 1 || flags[2] > 1) {
		fprintf(stderr, "ERROR: Too many targeted directories. Limit to one input and one output directory.\n");
		return -1;
	}

	// Handle non-existent directory

	if(!(inDir || outDir)) {
		fprintf(stderr, "ERROR: Directory not found. Missing directories will not be created.\n");
		return -1;
	}
	return 1;
}

/**
* Gets the index of an entry in a row (used for target column index)
* @param source String array of cells (from row or header)
* @param target Target String
* @return target index, or -1 upon failure
*/
int GetIndex(char** source, char* target) {

	if (!source || !target) {
		fprintf(stderr, "ERROR: Missing source or target row");
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

/**
* Calls getline() and returns the line parsed into a char** array where delimiter = ','
* @return array of string cells
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
		fprintf(stderr, "ERROR: malloc failed\n");
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
				fprintf(stderr, "ERROR: realloc failed\n");
				return NULL;
			}
		}

		i = 0;
		size_t entrylength = 30;
		char* entry = (char*) malloc((entrylength + 1) * sizeof(char));
		if (entry == NULL) {
			fprintf(stderr, "ERROR: malloc failed\n");
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
					fprintf(stderr, "ERROR: realloc failed\n");
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

/**
* Copy the contents of input csv and parse into Row structs and validates each cell has correct type per its column
* @param Rows Row struct to fill
* @param header Header struct to fill
* @param columns Number of columns
* @return number of rows
*/
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
				fprintf(stderr, "ERROR: realloc failed\n");
				return -1;
			}
			for (w = capacity / 2 + 2; w < capacity; w++) {
				(*Rows)[w].entries = NULL;
			}
		}
	}
	return rows;
}

/**
* Using GetLine(), fills a Header input, specifically
* @param h Header to be filled
* @return Number of columns, or -1 upon failure or no columns
*/

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
				fprintf(stderr, "ERROR: realloc failed\n");
				return -1;
			}
		}

		i = 0;
		size_t entrylength = 30;
		char* entry = (char*) malloc((entrylength + 1) * sizeof(char));
		if (entry == NULL) {
			fprintf(stderr, "ERROR: malloc failed\n");
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
					fprintf(stderr, "ERROR: realloc failed\n");
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
* Returns format enum of the input string/cell
* @param str String to be determined the format type
* @return format of input str
*/
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

#endif /* SIMPLECSVSORTER_H_ */
