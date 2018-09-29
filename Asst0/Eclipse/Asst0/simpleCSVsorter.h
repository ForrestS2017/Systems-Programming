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

typedef enum _format{NUM, STR} format;

typedef struct Column {		// Only for first row data
	char* entry;			// Hold the titles of each column
	format type;		// Declare the types of each column, corresponds with row entry
} Column;

typedef struct Row {
	char** entry;			// Hold each entry of the row. The type of each corresponds to format enum
} Row;

Column* Columns;
Row* Rows;

#endif /* SIMPLECSVSORTER_H_ */
