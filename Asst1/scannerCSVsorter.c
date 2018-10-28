#include "scannerCSVsorter.h"

/**
 * TO-DO:
 * Create a sorter.c file or something for reading file/directory
 * Use WEXITSTATUS for counting processes? Or keep a pointer to somewhere in memory?
*/

int main(int argc, char ** argv) {
    /*char* inPath;
    char* outPath;
    char* colname;
    DIR* inDir;
    DIR* outDir;
    
    char * cvalue = NULL;
    char * dvalue = NULL;
    char * ovalue = NULL;
    int a;
    opterr = 0;
    
    while ((a = getopt(argc, argv, "c:d:o:")) != -1) {
        switch (a) {
            case 'c':
                if (cvalue != NULL) {
                    fprintf(stderr, "Error: Multiple '-%c' flags found. Using last specified value.\n", a);
                }
                cvalue = optarg;
                break;
            case 'd':
                if (dvalue != NULL) {
                    fprintf(stderr, "Error: Multiple '-%c' flags found. Using last specified value.\n", a);
                }
                dvalue = optarg;
                break;
            case 'o':
                if (ovalue != NULL) {
                    fprintf(stderr, "Error: Multiple '-%c' flags found. Using last specified value.\n", a);
                }
                ovalue = optarg;
                break;
            case '?':
                if (optopt == 'c' || optopt == 'd' || optopt == 'o') {
                    fprintf(stderr, "Error: The flag '-%c' should have an argument after it. Using default value instead.\n", optopt);
                    if (optopt == 'c') {
                        printf("ERROR: Cannot execute because the flag '-c' has no argument specified for it.\n");
                        return 0;
                    }
                } else {
                    fprintf(stderr, "Error: Extra flag '-%c' found. Will continue as if it doesn't exist.\n", optopt);
                }
                break;
        }
    }
    
    if (cvalue == NULL) {
        fprintf(stderr, "ERROR: Missing -c argument, cannot run because the column to sort on is unknown.\n");
        printf("ERROR: Missing -c argument, cannot run because the column to sort on is unknown.\n");
        return 0;
    } else {
        colname = cvalue;
    }
    if (dvalue == NULL) {
        inPath = "./";
    } else {
        int length = strlen(dvalue);
        char * name = (char*)malloc(sizeof(char) * (length + 4));
        int absolute = 0;
        if (name == NULL) {
            printf("malloc failed\n");
            return 1;
        }
        name[length + 1] = 'F'; // Testing to make sure strcpy/strcat add null terminators
        if (length > 0) {
            if (strcmp(dvalue, ".") == 0) {
                strcpy(name, "./");
            } else {
                int i;
                for (i = 0; i < length; i++) {
                    if (dvalue[i] == '/') {
                        absolute = 1;
                        break;
                    }
                }
                if (absolute) {
                    strcpy(name, dvalue);
                } else {
                    strcpy(name, "./");
                    strcat(name, dvalue);
                }
                if (dvalue[length - 1] != '/') {
                    strcat(name, "/");
                }
            }
            inPath = name;
        } else {
            inPath = "./";
        }
    }
    inDir = opendir(inPath);
    if (inDir == NULL) {
        fprintf(stderr, "ERROR: Could not open directory '%s'\n", dvalue);
        printf("ERROR: Could not open directory '%s'\n", dvalue);
        return 0;
    }
    if (ovalue == NULL) {
        outPath = NULL;
        outDir = NULL;
    } else {
        int length = strlen(ovalue);
        char * name = (char*)malloc(sizeof(char) * (length + 4));
        int absolute = 0;
        if (name == NULL) {
            printf("malloc failed\n");
            return 1;
        }
        name[length + 1] = 'F';
        if (length > 0) {
            if (strcmp(ovalue, ".") == 0) {
                strcpy(name, "./");
            } else {
                int i;
                for (i = 0; i < length; i++) {
                    if (ovalue[i] == '/') {
                        absolute = 1;
                        break;
                    }
                }
                if (absolute) {
                    strcpy(name, ovalue);
                } else {
                    strcpy(name, "./");
                    strcat(name, ovalue);
                }
                if (ovalue[length - 1] != '/') {
                    strcat(name, "/");
                }
            }
            outPath = name;
            outDir = opendir(outPath);
            if (outDir == NULL) {
                fprintf(stderr, "ERROR: Could not open directory '%s'\n", ovalue);
                printf("ERROR: Could not open directory '%s'\n", ovalue);
                return 0;
            }
        } else {
            outPath = NULL;
            outDir = NULL;
        }
    }*/

    // Print required metadata
    /*int pid = getpid();
    fprintf(stdout, "Initial PID: %d\n", pid);*/
    //fprintf(stdout, "PIDS of all child processes:");
    //fprintf(stdout, "Total number of processes:");

    char * colname = argv[2];
    
    // Get column titles
    Header header = { NULL, NULL };
    header.titles = (char**) malloc(sizeof(char*) * 10);
    if (header.titles == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return 0;
    }

    int i = 0;
    int c = SetHeader(&header, 0); // Number of columns in table
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

    int rowcount = FillRows(&rows, &header, c, 0);
   
    if (rowcount == -1) {
        fprintf(stderr, "ERROR: Number of columns does not match the number of headings\n");
        return 0;
    } else if (rowcount == 0) {
        fprintf(stderr, "ERROR: No records found in this file.\n");
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
