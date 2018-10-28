#include "scannerCSVsorter.h"

int Sort(int inFD, int outFD, char* column) {
    // Get column titles
    Header header = { NULL, NULL };
    header.titles = (char**)malloc(sizeof(char*) * 10);
    if (header.titles == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return -1;
    }

    int i = 0;
    int c = SetHeader(&header, inFD); // Number of columns in table
    if (c == -1) {
        fprintf(stderr, "ERROR: No heading or columns found");
        return -1;
    }
    header.types = (format*)malloc(sizeof(format) * c);
    if (header.types == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return -1;
    }
    for (i = 0; i < c; i++) {
        header.types[i] = NUMBER;
    }

    Row* rows = (Row*)malloc(sizeof(Row));
    if (rows == NULL) {
        fprintf(stderr, "ERROR: malloc failed\n");
        return -1;
    }
    rows[0].entries = NULL;

    int rowcount = FillRows(&rows, &header, c, inFD);
   
    if (rowcount == -1) {
        fprintf(stderr, "ERROR: Number of columns does not match the number of headings");
        return -1;
    } else if (rowcount == 0) {
        fprintf(stderr, "ERROR: No records found");
        return -1;
    } else if (rowcount == -2) {
        return -1; // Memory allocation failure
    }

    int index = -1; // index of column to sort on
    for (i = 0; i < c; i++) {
        if (strcmp(column, trim(header.titles[i])) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        fprintf(stderr, "ERROR: '%s' is not a column found");
        return -1;
    }

    Row* output = mergeSort(rows, rowcount, index, header.types[index]);

    free(rows);

    int p = 0;
    for (p = 0; p < c; p++) {
        write(outFD, header.titles[p], strlen(header.titles[p])); // print out the top row
        if (p != c - 1) {
            write(outFD, ",", 1);
        }
    }
    write(outFD, "\n", 1);

    for (i = 0; i < rowcount; i++) {
        for (p = 0; p < c; p++){
            write(outFD, output[i].entries[p], strlen(output[i].entries[p]));
            if (p != c - 1) {
                write(outFD, ",", 1);
            }
        }
        write(outFD, "\n", 1);
    }

    free(output);
    close(inFD);
    close(outFD);
    return 0;
}

int fileHandler(struct dirent* file, char* filePath, char* column, char* ending, char* inPath, char* outPath) {
    int n = strlen(file->d_name);   
    if (n < 4 || strcmp(file->d_name + (n - 4), ".csv") != 0) { // Check if file ends with .csv
        fprintf(stderr, "File '%s' is not a .csv file", filePath);
        return 0; // Don't sort if not a .csv file
    }
    int p = 0;
    if (outPath != NULL) {
        p = strlen(outPath);
    } else {
        p = strlen(inPath);
    }
    char oPath[p + n + strlen(ending) - 4]; // [out/in]Path length + file->d_name length + ending length - length of ".csv" (ending already includes .csv so don't duplicate)
    if (outPath != NULL) {
        strcpy(oPath, outPath);
    } else {
        strcpy(oPath, inPath);
    }
    char fileName[n - 3]; // file name without .csv ending
    int i;
    for (i = 0; i < n - 4; i++) {
        fileName[i] = file->d_name[i];
    }
    fileName[i] = '\0';
    strcat(oPath, fileName);
    strcat(oPath, ending);
    
    int inFD = open(filePath, O_RDONLY);
    int outFD = open(oPath, O_RDWR | O_CREAT, 0600);
    if (inFD == -1) {
        fprintf(stderr, "ERROR: Could not open file '%s':\n", filePath);
    } else if (outFD == -1) {
        fprintf(stderr, "ERROR: Could not create file '%s':\n", oPath);
    } else {
        if (Sort(inFD, outFD, column) == -1) {
            fprintf(stderr, " in file '%s'.\n", filePath);
        }
    }
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
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) { // Don't run on . or ..
            continue;
        }
        
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
            if (file->d_type == DT_DIR) {
                strcat(path, "/");
                DIR * directory = opendir(path);
                if (directory == NULL) {
                    fprintf(stderr, "ERROR: Could not open directory '%s'\n", path);
                } else {
                    directoryHandler(directory, column, path, outPath);
                }
            } else if (file->d_type == DT_REG) {
                fileHandler(file, path, column, ending, inPath, outPath);
            } else {
                fprintf(stderr, "ERROR: Directory entry '%s' is not a directory or a regular file\n", path);
            }
            exit(1);
        } else {
            wait(&status);
            if (WIFEXITED(status)) {
                processes += WEXITSTATUS(status);
            }
        }
    }
    
    return processes;
}