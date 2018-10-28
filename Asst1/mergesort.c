#include "scannerCSVsorter.h"

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
 * Compares two doubles
 * @param n1 First double
 * @param n2 Second double
 * @return number > 0 if n1 > n2, 0 if n1 = n2, number < 0 if n1 < n2
 */
int doubleComparator(void* n1, void* n2) {
    double d = *((double*)n1) - *((double*)n2);
    if (d < 0.0) {
        return -1;
    } else if (d > 0.0) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Merges two arrays of rows. Used for merge sort
 * @param arrows1 First array of Row structs to be merged
 * @param length1 Length of first array
 * @param arrow2 Second array of Row structs to be merged
 * @param length2 Length of second array
 * @param index Index of the value being sorted in the row
 * @param f Format enum indicating whether value to be sorted is a number or a string
 * @return Sorted array of Rows
 */
Row* merge(Row* arrows1, int length1, Row* arrows2, int length2, int index, format f) {

    Row* result = (Row*)malloc(sizeof(Row) * (length1 + length2)); // Auxiliary array containing result
    if (result == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory for merge result, terminating merge.\n");
        printf("ERROR: malloc failed when allocating memory for merge result, terminating merge.\n");
        return NULL;
    }
    int lIndex = 0; // Current index in left array
    int rIndex = 0; // Current index in right array
    int aIndex = 0; // Current index in auxiliary array

    if (f == NUMBER) {
        while (lIndex < length1 && rIndex < length2) {
            char* e1 = arrows1[lIndex].entries[index];
            char* e2 = arrows2[rIndex].entries[index];
            if (strcmp(e1, "") == 0) {
                result[aIndex++] = arrows1[lIndex++];
            } else if (strcmp(e2, "") == 0) {
                result[aIndex++] = arrows2[rIndex++];
            } else {
                double d1 = atof(e1);
                double d2 = atof(e2);
                if (doubleComparator(&d1, &d2) <= 0) {
                    result[aIndex++] = arrows1[lIndex++];
                } else {
                    result[aIndex++] = arrows2[rIndex++];
                }
            }
        }
    } else if (f == STRING) {
        while (lIndex < length1 && rIndex < length2) {
            char* e1 = arrows1[lIndex].entries[index];
            char* e2 = arrows2[rIndex].entries[index];
            if (strcmp(e1, "") == 0) {
                result[aIndex++] = arrows1[lIndex++];
            } else if (strcmp(e2, "") == 0) {
                result[aIndex++] = arrows2[rIndex++];
            } else {
                char* s1 = trim(e1);
                char* s2 = trim(e2);
                if (stringComparator(s1, s2) <= 0) {
                    result[aIndex++] = arrows1[lIndex++];
                } else {
                    result[aIndex++] = arrows2[rIndex++];
                }
                free(s1);
                free(s2);
            }
        }
    } else {
        return NULL;
    }

    while (lIndex < length1) {
        result[aIndex++] = arrows1[lIndex++];
    }

    while (rIndex < length2) {
        result[aIndex++] = arrows2[rIndex++];
    }

    return result;
}

/**
 * Sorts an array of Rows using merge sort
 * @param arrows Array of Row structs to be sorted
 * @param length Length of array
 * @param index Index of the value being sorted in the row
 * @param f Format enum indicating whether value to be sorted is a number or a string
 * @return Sorted array of Rows
 */
Row* mergeSort(Row* arrows, int length, int index, format f) {
    if (length <= 1) {
        return arrows;
    }
    int l = length / 2; // Length of left subarray
    int r = l + length % 2; // Length of right subarray
    Row* m1 = mergeSort(arrows, l, index, f);
    Row* m2 = mergeSort(&arrows[l], r, index, f);
    return merge(m1, l, m2, r, index, f);
}
