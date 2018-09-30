/*
 * mergesort.c
 *
 *  Created on: Sep 28, 2018
 *      Author: Justin Chan, Forest Smith
 */

#include "simpleCSVsorter.h"

/**
 * Compares two strings
 * @param str1 First string
 * @param str2 Second string
 * @return number > 0 if str1 comes after str2, 0 if they are equal, number < 0 if str1 comes before str2
 */
int stringComparator(void* str1, void* str2) {
	return strcmp((char*)str1, (char*)str2);
}

/**
 * Compares two integers
 * @param n1 First integer
 * @param n2 Second integer
 * @return number > 0 if n1 > n2, 0 if n1 = n2, number < 0 if n1 < n2
 */
int intComparator(void* n1, void* n2) {
	return *((int*)n1) - *((int*)n2);
}

/**
 * Compares two doubles
 * @param n1 First double
 * @param n2 Second double
 * @return number > 0 if n1 > n2, 0 if n1 = n2, number < 0 if n1 < n2
 */
int doubleComparator(void* n1, void* n2) {
	return *((double*)n1) - *((double*)n2);
}

/**
 * Parses a string and trims white space and quotation marks
 * @param str String to parse
 * @return Altered string
 */
char* parseString(char* str) {
	return 0;
}

/**
 * Reads an int from a string
 * @param str String to parse
 * @return Integer value of string
 */
int parseInt(char* str) {
	return 0;
}

/**
 * Reads a double from a string
 * @param str String to parse
 * @return Double value of string
 */
double parseDouble(char* str) {
	return 0;
}

/**
 * Merges two arrays of rows. Used for merge sort
 * @param arrows1 First array of Row structs to be merged
 * @param length1 Length of first array
 * @param arrow2 Second array of Row structs to be merged
 * @param length2 Length of second array
 * @param index Index of the value being sorted in the row
 * @param f Format enum indicating whether value to be sorted is an integer or a string
 * @return Sorted array of Rows
 */
Row* merge(Row* arrows1, int length1, Row* arrows2, int length2, int index, format f) {

	Row* result = malloc(sizeof(Row) * (length1 + length2)); // Auxiliary array containing result
	int lIndex = 0; // Current index in left array
	int rIndex = 0; // Current index in right array
	int aIndex = 0; // Current index in auxiliary array

	int (*comparator)(void*, void*);
	if (f == INTEGER) {
		comparator = intComparator;
	} else if (f == DOUBLE) {
		comparator = doubleComparator;
	} else if (f == STRING) {
		comparator = stringComparator;
	} else {
		return NULL;
	}

	while (lIndex < length1 && rIndex < length2) {
		if (comparator(arrows1[lIndex].entries[index], arrows2[rIndex].entries[index]) < 0) {
			result[aIndex++] = arrows1[lIndex++];
		} else {
			result[aIndex++] = arrows2[rIndex++];
		}
	}

	while (lIndex < length1) {
		result[aIndex++] = arrows1[lIndex++];
	}

	while (rIndex < length2) {
		result[aIndex++] = arrows2[rIndex++];
	}

	return result;
	// Remember to free after doing mergesort
}

/**
 * Sorts an array of Rows using merge sort
 * @param arrows Array of Row structs to be sorted
 * @param length Length of array
 * @param index Index of the value being sorted in the row
 * @param f Format enum indicating whether value to be sorted is an integer or a string
 * @return Sorted array of Rows
 */
Row* mergeSort(Row* arrows, int length, int index, format f) {
	if (length <= 1) {
		return arrows;
	}
	int l = length / 2; // Length of left subarray
	int r = l + length % 2; // Length of right subarray
	mergeSort(arrows, l, index, f);
	mergeSort(&arrows[l], r, index, f);
	return merge(arrows, l, &arrows[l], r, index, f);
}

// GetLine: Reads each line until the end and returns the string

char* GetLine()
{
	size_t length = 50;
	//size_t position = 0;
	char* line = (char*) malloc(length * sizeof(char));
	//char character;

	/*while (read(0, &character, 1) != 0 && character != '\n')
	{
		line[position] = character;
		position++;
		if (position == length) {
			length *= 2;
			line = realloc(line, length * sizeof(char));
		}
	}

	if (position == 0)
	{
		return NULL;
	}
	*/

	getline(&line, &length, stdin);
	return line;
}
