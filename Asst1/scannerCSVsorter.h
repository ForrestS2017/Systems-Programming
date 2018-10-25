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

typedef enum _format{ NUMBER=1, STRING=2 } format;

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

// Compares two doubles
int doubleComparator(void*, void*);

// Trims white space and removes quotation marks from a string
char* trim(char*);

// Merges two arrays of rows. Used for merge sort
Row* merge(Row*, int, Row*, int, int, format);

// Sorts an array of Rows using merge sort
Row* mergeSort(Row*, int, int, format);

// Checks for valid arguments
int CheckInput(int, char**);

// Reads current row and returns number of entries
int GetLine(char***);

// Get Index of a row entry
int GetIndex(char** source, char* target);

// Fills Rows with Row structs, using GetLine()
int FillRows();

// Gets type (_format) for a cell
format getType(char* str);

#endif /* SIMPLECSVSORTER_H_ */
