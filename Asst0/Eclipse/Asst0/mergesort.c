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
 * Sorts an array/linked list whatever using merge sort
 * @param rows
 * @param s Array/linkedlist whatever to be sorted
 * @param f Format enum indicating whether value to be sorted is an integer or a string
 * @return Sorted array/linked list
 */
char* mergeSort(Row * rows, int index, format f) {
	char * b = "a";
	return b;
}

// GetLine: Reads each line until the end and returns the string

char* GetLine()
{
	size_t length = 50;
	//size_t position = 0;
	char* line = (char*) malloc(length * sizeof(char));
	//char character;

/*	while (read(0, &character, 1) != 0 && character != '\n')
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
