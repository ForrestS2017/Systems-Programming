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

#ifndef SIMPLECSVSORTER_H_
#define SIMPLECSVSORTER_H_

/***** Variables, Structs, Prototypes *****/

typedef enum _format{ STRING, INTEGER, DOUBLE } format;

typedef struct _Header {		// Only for first row data
	char* entry;			// Hold the titles of each column
	format* types;		// Declare the types of each column, corresponds with row entry
} Header;

typedef struct _Row {
	char** entries;			// Hold each entry of the row. The type of each corresponds to format enum
} Row;

Header header;
Row* Rows;

/***** Functions *****/

// Compares two strings
int stringComparator(void*, void*);

// Compares two integers
int intComparator(void*, void*);

// Compares two doubles
int doubleComparator(void*, void*);

// Parses a string and removes white space and quotation marks
char* parseString(char*);

// Reads an int from a string
int parseInt(char*);

// Reads a double from a string
double parseDouble(char*);

// Merges two arrays of rows. Used for merge sort
Row* merge(Row*, int, Row*, int, int, format);

// Sorts an array of Rows using merge sort
Row* mergeSort(Row*, int, int, format);

char* GetLine();

#endif /* SIMPLECSVSORTER_H_ */
