#include "scannerCSVsorter.h"

int main(int argc, char ** argv) {

    // THIS DOESN'T WORK
    // Check if flags and arguments are correct
    /*if (!CheckInput(argc, argv)) {
        return -1;
    }

    // Print required metadata
    int pid = getpid();
    fprintf(stdout, "Initial PID: %d\n", pid);*/
    
    colname = argv[2]; // here so the rest of the program can run until it can be properly set

    // Get column titles
    Header header = { NULL, NULL };
    header.titles = (char**) malloc(sizeof(char*) * 10);
    if (header.titles == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return 0;
    }

    int i = 0;
    int c = SetHeader(&header); // Number of columns in table
    if (c == -1) {
        fprintf(stderr, "ERROR: No heading or columns found.\n");
        return 0;
    }
    header.types = (format*)malloc(sizeof(format) * c);
    if (header.types == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return 0;
    }
    for (i = 0; i < c; i++) {
        header.types[i] = NUMBER;
    }

    Row* rows = (Row*)malloc(sizeof(Row));
    if (rows == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return 0;
    }
    rows[0].entries = NULL;

    int rowcount = FillRows(&rows, &header, c);
   
    if (rowcount == -1) {
        fprintf(stderr, "ERROR: Number of columns does not match the number of headings\n");
        return 0;
    } else if (rowcount == 0) {
        fprintf(stderr, "ERROR: No records found in this file.\n", colname);
        return 0;
    }

    int index = -1; // index of column to sort on
    for (i = 0; i < c; i++) {
        if (strcmp(colname, trim(header.titles[i])) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        fprintf(stderr, "ERROR: '%s' is not a column found in this file.\n", colname);
        return 0;
    } else if (index == -2) {
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

    return 0;
}
