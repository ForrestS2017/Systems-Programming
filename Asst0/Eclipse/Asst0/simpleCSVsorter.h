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

typedef enum _format{ NUM, STR } format;

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

int stringComparator(void*, void*);

int intComparator(void*, void*);

char* mergeSort(Row * row, int i, format f);

char** GetLine();

#endif /* SIMPLECSVSORTER_H_ */
