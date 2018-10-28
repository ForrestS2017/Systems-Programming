#include "scannerCSVsorter.h"

int Sort(int fd, int out, char* column) {
    // Get column titles
    Header header = { NULL, NULL };
    header.titles = (char**) malloc(sizeof(char*) * 10);
    if (header.titles == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return 0;
    }

    int i = 0;
    int c = SetHeader(&header, fd); // Number of columns in table
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

    int rowcount = FillRows(&rows, &header, c, fd);
   
    if (rowcount == -1) {
        fprintf(stderr, "ERROR: Number of columns does not match the number of headings\n");
        return 0;
    } else if (rowcount == 0) {
        fprintf(stderr, "ERROR: No records found in this file.\n");
        return 0;
    }

    int index = -1; // index of column to sort on
    for (i = 0; i < c; i++) {
        if (strcmp(column, trim(header.titles[i])) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        fprintf(stderr, "ERROR: '%s' is not a column found in this file.\n");
        return 0;
    } else if (index == -2) {
        return 0;
    }

    Row* output = mergeSort(rows, rowcount, index, header.types[index]);

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
            printf("%s", output[i].entries[p]);
            if (p != c - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    free(output);
    return 0;
}

int fileHandler(struct dirent* file, char* column, char* inPath, char* outPath) {
    return 0;
}

int directoryHandler(DIR* dir, char* column, char* inPath, char* outPath) {
    int processes = 0;
    
    char ending[strlen(column) + 13];
    strcpy(ending, "-sorted-");
    strcat(ending, column);
    strcat(ending, ".csv");
    int l = strlen(ending);
    
    struct dirent * file;
    while ((file = readdir(dir)) != NULL) {
        int n = strlen(file->d_name);        
        if (n > l) {
            if (strcmp(file->d_name + (n - l), ending) == 0) { // Check if file ends with -sorted-<fieldname>.csv
                continue;
            }
        }
        
        int m = strlen(inPath);
        char path[m + n + 2];
        strcpy(path, inPath);
        strcat(path, file->d_name);        
        
        int status;
        int pid = fork();
        if (pid == 0) {
            fprintf(stdout, "%d,", getpid());
            if (file->d_type = DT_DIR) {
                strcat(path, "/");
                DIR * directory = opendir(path);
                if (directory == NULL) {
                    fprintf(stderr, "ERROR: Could not open directory '%s'\n", path);
                } else {
                    directoryHandler(directory, column, path, outPath);
                }
            } else {
                int fd = open(path, O_RDONLY);
                if (fd == -1) {
                    fprintf(stderr, "ERROR: Could not open file '%s':\n", path);
                } else {
                    fileHandler(file, column, path, outPath);
                }
            }
            exit(1 + processes);
        } else {
            wait(&status);
            if (WIFEXITED(status)) {
                processes += WEXITSTATUS(status);
            }
        }
    }
    
    return processes;
}