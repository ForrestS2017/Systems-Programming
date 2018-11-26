#include "multiThreadSorter.h"

/**
 * Sorts a given file and outputs result in another file
 * @param filePath Path for file to be sorted
 * @param oPath Path for the file to output to
 * @param column Name of column to be sorted on
 * @return 0 if successful, -1 if error occurred
 */
int Sort(char* filePath, char* oPath, char* column) {
    int inFD = open(filePath, O_RDONLY);
    if (inFD == -1) {
        fprintf(stderr, "ERROR: Could not open file '%s'.\n", filePath);
        return -1;
    }
    
    // Get column titles
    Header header = { NULL, NULL };
    header.titles = (char**)malloc(sizeof(char*) * 10);
    if (header.titles == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory for header.titles in Sort, terminating Sort.\n");
        printf("ERROR: malloc failed when allocating memory for header.titles in Sort, terminating Sort.\n");
        return -1;
    }

    int i = 0;
    int c = SetHeader(&header, inFD); // Number of columns in table
    if (c == -1) {
        fprintf(stderr, "ERROR: No heading or columns found in file '%s'.\n", filePath);
        return -1;
    }
    header.types = (format*)malloc(sizeof(format) * c);
    if (header.types == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory for header.types in Sort, terminating Sort.\n");
        printf("ERROR: malloc failed when allocating memory for header.types in Sort, terminating Sort.\n");
        return -1;
    }
    for (i = 0; i < c; i++) {
        header.types[i] = NUMBER; // Initiate types for all headers to NUMBER format
    }

    // Check if column to sort on is present
    int index = -1; // index of column to sort on
    for (i = 0; i < c; i++) {
        if (strcmp(column, trim(header.titles[i])) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        fprintf(stderr, "ERROR: '%s' is not a column found in file '%s'.\n", column, filePath);
        return -1;
    } else {
        sortIndex = index;
    }

    if(sortRows == NULL) {
        sortRows = (Row*)malloc(sizeof(Row));
        if (sortRows == NULL) {
            fprintf(stderr, "ERROR: malloc failed when allocating memory for rows in Sort, terminating Sort.\n");
            printf("ERROR: malloc failed when allocating memory for rows in Sort, terminating Sort.\n");
            return -1;
        }
        sortRows[0].entries = NULL;
    }
    

    int rowcount = FillRows(&sortRows, &header, c, inFD); // Fill in row data
   
    if (rowcount == -1) {
        fprintf(stderr, "ERROR: Number of columns does not match the number of headings in file '%s'.\n", filePath);
        return -1;
    } else if (rowcount == 0) {
        fprintf(stderr, "ERROR: No records found in file '%s'.\n", filePath);
        return -1;
    } else if (rowcount == -2) {
        return -1; // Memory allocation failure
    }


    
    

    // int outFD = open(oPath, O_RDWR | O_CREAT, 0600); // Create a file with read/write permissions for owner
    // if (outFD == -1) {
    //     fprintf(stderr, "ERROR: Could not create file '%s'.\n", oPath);
    //     return -1;
    // }
    
    // Row* output = mergeSort(rows, rowcount, index, header.types[index]); // Call mergeSort (from mergesort.c)

    // if (rows != output) {
    //     free(rows);
    // }

    // int p = 0;
    // for (p = 0; p < c; p++) {
    //     write(outFD, header.titles[p], strlen(header.titles[p])); // write header to output file
    //     if (p != c - 1) {
    //         write(outFD, ",", 1);
    //     }
    // }
    // write(outFD, "\n", 1);

    // for (i = 0; i < rowcount; i++) {
    //     for (p = 0; p < c; p++){
    //         write(outFD, output[i].entries[p], strlen(output[i].entries[p])); // write row to output file
    //         if (p != c - 1) {
    //             write(outFD, ",", 1);
    //         }
    //     }
    //     write(outFD, "\n", 1);
    // }

    // Free and close stuff
    //free(output);
    close(inFD);
    return 0;
}

/**
 * Performs operations on files
 * @param file struct dirent pointer to the file being processed
 * @param filePath Path to the file being processed
 * @param column Name of column to be sorted
 * @param inPath Path to directory that the file being processed is in
 * @param outPath Path to output directory
 * @return number of child processes
 */
void* fileHandler(void* args) {
    TArguments * tArgs = args;
    struct dirent * file = tArgs->file;
    char * filePath = tArgs->filePath;
    char * inPath = tArgs->inPath;
    char * outPath = tArgs->outPath;
    char * column = tArgs->column;

    /*char ending[50]; // Ending of output file
    strcpy(ending, "AllFiles-sorted-");
    strcat(ending, column);
    strcat(ending, ".csv");
    int l = strlen(ending);*/
    
    int n = strlen(file->d_name);
    // if (n > l) {
    //     if (strcmp(file->d_name + (n - l), ending) == 0) { // Check if file ends with -sorted-<fieldname>.csv
    //         return 0; // Don't sort if file is already sorted
    //     }
    // }    
    if (n < 4 || strcmp(file->d_name + (n - 4), ".csv") != 0) { // Check if file ends with .csv
        fprintf(stderr, "File '%s' is not a .csv file\n", filePath);
        return (void*)(size_t)  0; // Don't sort if not a .csv file
    }
    /*int p = 0; // Length of path to output directory
    if (outPath != NULL) {
        p = strlen(outPath); // Use outPath if not null
    } else {
        p = strlen(inPath); // Else use the current directory
    }*/
    // oPath is the full path to the file that will be written to
    // char oPath[p + n + l - 2]; // [out/in]Path length + file->d_name length + ending length - length of ".csv" (ending already includes .csv so don't duplicate) + 2 (for \0 and whatever else just in case)
    // if (outPath != NULL) {
    //     strcpy(oPath, outPath); // Use outPath if not null
    // } else {
    //     strcpy(oPath, inPath); // Else use the current directory
    // }
    /*char fileName[n - 3]; // file name without .csv ending
    int i;
    for (i = 0; i < n - 4; i++) {
        fileName[i] = file->d_name[i];
    }
    fileName[i] = '\0';
    char oPath[1];

    // Set output file to write to
    strcpy(oPath,outPath);
    strcat(oPath, ending);*/
    
    //Sort(filePath, oPath, column);

    int inFD = open(filePath, O_RDONLY);
    if (inFD == -1) {
        fprintf(stderr, "ERROR: Could not open file '%s'.\n", filePath);
        return -1;
    }
    
    // Get column titles
    Header header = { NULL, NULL };
    header.titles = (char**)malloc(sizeof(char*) * 10);
    if (header.titles == NULL) {
        fprintf(stderr, "ERROR: malloc failed when allocating memory for header.titles in Sort, terminating Sort.\n");
        printf("ERROR: malloc failed when allocating memory for header.titles in Sort, terminating Sort.\n");
        return -1;
    }

    int i = 0;
    int c = SetHeader(&header, inFD); // Number of columns in table
    if (c == -1) {
        fprintf(stderr, "ERROR: No heading or columns found in file '%s'.\n", filePath);
        return -1;
    }

    int rowcount = FillRows(&rows, header, totalCols, inFD); // Fill in row data
   
    if (rowcount == -1) {
        fprintf(stderr, "ERROR: Number of columns does not match the number of headings in file '%s'.\n", filePath);
        return -1;
    } else if (rowcount == 0) {
        fprintf(stderr, "ERROR: No records found in file '%s'.\n", filePath);
        return -1;
    } else if (rowcount == -2) {
        return -1; // Memory allocation failure
    }

    int index = -1; // index of column to sort on
    for (i = 0; i < c; i++) {
        if (strcmp(column, sortHeaders[i]) == 0) {
            index = i;
            break;
        }
    }
    
    Row* Output = mergeSort(rows, rowcount, index, types[index]);
    
    pthread_mutex_lock(&_fileLock); 
    if ((*ALL_DATA_COUNT) >= (*ALL_DATA_MAX)) {
        (*ALL_DATA_MAX) *= 2;
        ALL_DATA = (Row**)realloc(ALL_DATA, sizeof(Row*) * (*ALL_DATA_MAX));
        for (i = 0; i < (*ALL_DATA_MAX) / 2 + 1; i++) {
            ALL_DATA[i] = NULL;
        }        
    }
    ALL_DATA[(*ALL_DATA_COUNT)] = Output;
    *ALL_DATA_COUNT++;
    *ALL_DATA_ROW_COUNT += rowcount;
    pthread_mutex_unlock(&_fileLock);

    close(inFD);
    
    return (void*) (size_t) 0;
}

/**
 * Performs operations on directories
 * @param dir DIR pointer for the directory being processed
 * @param column Name of column to be sorted
 * @param inPath Path to directory being processed
 * @param outPath Path to output directory
 * @return number of child processes
 */
void* directoryHandler(void* args) {
    TArguments * tArgs = (TArguments*)args;
    int threads = 0;
    
    DIR * dir = tArgs->dir;
    char * inPath = tArgs->inPath;
    char * outPath = tArgs->outPath;
    char * column = tArgs->column;
    
    
    int * tids = (int*)malloc(sizeof(int) * 256);
    tids[0] = tid;
    int j;
    for (j = 1; j < 256; j++) {
        tids[j] = 0;
    }
    
    struct dirent * file;
    while ((file = readdir(dir)) != NULL) { // Loop through directory
        if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) { // Don't run on . or ..
            continue;
        }
        
        int n = strlen(file->d_name);        
        int m = strlen(inPath);
        char path[m + n + 2]; // Get full path to current file
        strcpy(path, inPath);
        strcat(path, file->d_name);
        
        int status;
        //pthread_create(&thread, NULL, directoryHandler, (void*)args);
        threads = 0; // Set threads to 0 since at this point, the child has 0 spawned threads as far as we know
        //fprintf(stdout, "%d,", getpid()); // Print PID
        //fflush(stdout); // Force write to stdout
        if (file->d_type == DT_DIR) { // If dir entry is a directory
            strcat(path, "/"); // Add / to the nd of the path
            DIR * directory = opendir(path);
            if (directory == NULL) {
                fprintf(stderr, "ERROR: Could not open directory '%s'\n", path);
                exit(1);
            } else {
                TArguments * nextArgs = (TArguments *)malloc(sizeof(TArguments));
                nextArgs->dir = directory;
                nextArgs->inPath = inPath;
                nextArgs->outPath = outPath;
                nextArgs->column = column;
                nextArgs->file = NULL;
                nextArgs->filePath = NULL;
                pthread_t thread;
                
                pthread_create(&thread, NULL, directoryHandler, nextArgs); // Create thread for next subdirectory
                void* joinStatus;
                pthread_join(thread, &joinStatus);
                fprintf(stderr, "ERROR: Could join threads.\n");
                if ((int)(intptr_t) joinStatus < 0) return (void*)(size_t)  -1;
                totalTIDs += 1;
                threads++;
                //return (void*) (size_t) threads; // Exit with the number of children
            }
        } else if (file->d_type == DT_REG) { // If dir entry is a regular file
            TArguments * nextArgs = (TArguments *)malloc(sizeof(TArguments));
            nextArgs->file = file;
            nextArgs->filePath = path;
            nextArgs->inPath = inPath;
            nextArgs->outPath = outPath;
            nextArgs->column = column;
            nextArgs->dir = NULL;

            pthread_t thread;
            pthread_create(&thread, NULL, fileHandler, nextArgs); // Create thread for next file via file handler on the file
            void* joinStatus;
            pthread_join(thread, &joinStatus);
            fprintf(stderr, "ERROR: Could join threads.\n");
            if ((int)(intptr_t) joinStatus < 0) return (void*) (size_t) -1;
            totalTIDs += 1;
            threads++;
            //..exit(1); // Exit with 1 to indicate 1 process
        } else {
            fprintf(stderr, "ERROR: Directory entry '%s' is not a directory or a regular file\n", path);
            //return (void*)(size_t) threads;
        }
    }
    
    //output = mergeSort(sortRows, totalRows, sortIndex, types[sortIndex]);

    return (void*) (size_t) threads;
}
