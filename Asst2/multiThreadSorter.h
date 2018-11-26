#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef MULTITHREADSORTER_H_
#define MULTITHREADSORTER_H_

/***** Variables, Structs *****/

typedef enum _format
{
    NUMBER = 1,
    STRING = 2
} format;

typedef struct _Header
{                  // Only for first row data
    char **titles; // Hold the titles of each column
    format *types; // Declare the types of each column, corresponds with row entry
} Header;

typedef struct _Row
{
    char **entries; // Hold each entry of the row. The type of each corresponds to format enum
} Row;

typedef struct _TArguments
{
    struct dirent* file;
    DIR * dir;
    char* filePath;
    char * column;
    char * outPath;
    char * inPath; // DIR* dir, char* column, char* inPath, char* outPath
} TArguments;

typedef struct _Data
{
    Row* rows;
    int count;
} Data;

// Directory global variables
char *inPath;
char *outPath;
char *colname;
DIR *inDir;
DIR *outDir;
Row* sortRows;
Row* output;
Data* ALL_DATA;
int* ALL_DATA_COUNT;
int* ALL_DATA_MAX;
int* ALL_DATA_ROW_COUNT;

char * sortHeaders[28]; // probably want to be const to make this work best

format types[28];

// Countable variables
int totalTIDs;
int totalCols;
int totalRows;
int sortIndex;

// Mutex locks 
pthread_mutex_t _fileLock;
pthread_mutex_t _printLock;
/***** Functions *****/

// Compares two strings
int stringComparator(void *, void *);

// Compares two doubles
int doubleComparator(void *, void *);

// Trims white space and removes quotation marks from a string
char *trim(char *);

// Merges two arrays of rows. Used for merge sort
Row *merge(Row *, int, Row *, int, int, format);

// Sorts an array of Rows using merge sort
Row *mergeSort(Row *, int, int, format);

// Checks for valid arguments
int CheckInput(int, char **);

// Reads current row and returns number of entries
int GetLine(char***, int, Header, int, int);

// Get Index of a row entry
int GetIndex(char **source, char *target);

// Fills Rows with Row structs, using GetLine()
int FillRows(Row**, Header, int, int, int);

// Sets the header for a given file
int SetHeader(Header *, int);

// Gets type (_format) for a cell
format getType(char *str);

// Sorts a file
int Sort(char *, char *, char *);

// Handles operations on files
void * fileHandler(void *);

// Handles operations on directories
void * directoryHandler(void *);

#endif /* MULTITHREADSORTER_H_ */
