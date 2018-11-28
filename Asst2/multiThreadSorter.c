#include "multiThreadSorter.h"

int main(int argc, char **argv)
{

    // Argument global variables
    char *cvalue = NULL;
    char *dvalue = NULL;
    char *ovalue = NULL;
    int a;
    opterr = 0; // Suppress errors from getopt

    // Thread global variables
    int tid;
    int i;

    // Parse command line arguments with getopt
    while ((a = getopt(argc, argv, "c:d:o:")) != -1)
    {
        switch (a)
        {
        case 'c':
            if (cvalue != NULL)
            {
                fprintf(stderr, "Error: Multiple '-%c' flags found. Using last specified value.\n", a);
            }
            cvalue = optarg; // Set cvalue to argument after -c
            break;
        case 'd':
            if (dvalue != NULL)
            {
                fprintf(stderr, "Error: Multiple '-%c' flags found. Using last specified value.\n", a);
            }
            dvalue = optarg;
            break;
        case 'o':
            if (ovalue != NULL)
            {
                fprintf(stderr, "Error: Multiple '-%c' flags found. Using last specified value.\n", a);
            }
            ovalue = optarg;
            break;
        case '?':
            if (optopt == 'c' || optopt == 'd' || optopt == 'o')
            {
                fprintf(stderr, "Error: The flag '-%c' should have an argument after it. Using default value instead.\n", optopt);
                if (optopt == 'c')
                {
                    printf("ERROR: Cannot execute because the flag '-c' has no argument specified for it.\n");
                    return 0;
                }
            }
            else
            {
                fprintf(stderr, "Error: Extra flag '-%c' found. Will continue as if it doesn't exist.\n", optopt);
            }
            break;
        }
    }

    if (cvalue == NULL)
    { // Don't run if value for '-c' is not found
        fprintf(stderr, "ERROR: Missing -c argument, cannot run because the column to sort on is unknown.\n");
        printf("ERROR: Missing -c argument, cannot run because the column to sort on is unknown.\n");
        return 0;
    }
    else
    {
        colname = cvalue;
    }
    if (dvalue == NULL)
    { // Default inPath is current working directory ('.')
        inPath = "./";
    }
    else
    {
        int length = strlen(dvalue);
        char *name = (char *)malloc(sizeof(char) * (length + 4));
        if (name == NULL)
        {
            fprintf(stderr, "ERROR: malloc failed when allocating memory for inPath, terminating program.\n");
            printf("ERROR: malloc failed when allocating memory for inPath, terminating program.\n");
            return 1;
        }
        int absolute = 0;
        if (length > 0)
        {
            if (strcmp(dvalue, ".") == 0)
            {
                strcpy(name, "./"); // Add / to the end of the directory if it's not there
            }
            else
            {
                for (i = 0; i < length; i++)
                {
                    if (dvalue[i] == '/')
                    { // If a / is found, assume that an absolute file path is given
                        absolute = 1;
                        break;
                    }
                }
                if (absolute)
                { // Copy the exact value of the -d argument if the path is absolute
                    strcpy(name, dvalue);
                }
                else
                { // Else assume that the file path is relative, start with "./" then the rest of the file name
                    strcpy(name, "./");
                    strcat(name, dvalue);
                }
                if (dvalue[length - 1] != '/')
                { // Add / to the end of the directory if it's not there
                    strcat(name, "/");
                }
            }
            inPath = name;
        }
        else
        {
            inPath = "./"; // Default
        }
    }
    inDir = opendir(inPath);
    if (inDir == NULL)
    {
        fprintf(stderr, "ERROR: Could not open directory '%s'\n", dvalue);
        printf("ERROR: Could not open directory '%s'\n", dvalue);
        return 0;
    }
    if (ovalue == NULL)
    { // Default outPath is current working directory ('.')
        outPath = "./";
    }
    else
    {
        int length = strlen(ovalue);
        char *name = (char *)malloc(sizeof(char) * (length + 4));
        if (name == NULL)
        {
            fprintf(stderr, "ERROR: malloc failed when allocating memory for outPath, terminating program.\n");
            printf("ERROR: malloc failed when allocating memory for outPath, terminating program.\n");
            return 1;
        }
        int absolute = 0;
        if (length > 0)
        {
            if (strcmp(ovalue, ".") == 0)
            {
                strcpy(name, "./");
            }
            else
            {
                for (i = 0; i < length; i++)
                {
                    if (ovalue[i] == '/')
                    {
                        absolute = 1;
                        break;
                    }
                }
                if (absolute)
                {
                    strcpy(name, ovalue);
                }
                else
                {
                    strcpy(name, "./");
                    strcat(name, ovalue);
                }
                if (ovalue[length - 1] != '/')
                {
                    strcat(name, "/");
                }
            }
            outPath = name;
            outDir = opendir(outPath);
            if (outDir == NULL)
            {
                fprintf(stderr, "ERROR: Could not open directory '%s'\n", ovalue);
                printf("ERROR: Could not open directory '%s'\n", ovalue);
                return 0;
            }
        }
        else
        {
            outPath = "./";
        }
    }
    
    
    int pathL = 0;
    if (outPath != NULL) {
        pathL = strlen(outPath);
    } else if (inPath != NULL){
        pathL = strlen(inPath);
    } else {
        fprintf(stderr, "ERROR: Could not validate input/output paths");
    }
    char oPath[pathL + strlen(colname) + 22];
    if (outPath != NULL) {
        strcpy(oPath, outPath);
    } else {
        strcpy(oPath, inPath);
    }
    strcat(oPath, "AllFiles-sorted-");
    strcat(oPath, colname);
    strcat(oPath, ".csv");
    
    int outFD = open(oPath, O_RDWR | O_CREAT, 0600); // Create a file with read/write permissions for owner
    if (outFD < 0) {
        fprintf(stderr, "ERROR: Could not create file '%d'.\n", outFD);
        return -1;
    }
    
    totalCols = 28;
    totalRows = 0;
    sortIndex = -1;
    
    types[0] = STRING;
    types[1] = STRING;
    types[2] = NUMBER;
    types[3] = NUMBER;
    types[4] = NUMBER;
    types[5] = NUMBER;
    types[6] = STRING;
    types[7] = NUMBER;
    types[8] = NUMBER;
    types[9] = STRING;
    types[10] = STRING;
    types[11] = STRING;
    types[12] = NUMBER;
    types[13] = NUMBER;
    types[14] = STRING;
    types[15] = NUMBER;
    types[16] = STRING;
    types[17] = STRING;
    types[18] = NUMBER;
    types[19] = STRING;
    types[20] = STRING;
    types[21] = STRING;
    types[22] = NUMBER;
    types[23] = NUMBER;
    types[24] = NUMBER;
    types[25] = NUMBER;
    types[26] = NUMBER;
    types[27] = NUMBER;

    sortHeaders[0] = "color";
    sortHeaders[1] = "director_name";
    sortHeaders[2] = "num_critic_for_reviews";
    sortHeaders[3] = "duration";
    sortHeaders[4] = "director_facebook_likes";
    sortHeaders[5] = "actor_3_facebook_likes";
    sortHeaders[6] = "actor_2_name";
    sortHeaders[7] = "actor_1_facebook_likes";
    sortHeaders[8] = "gross";
    sortHeaders[9] = "genres";
    sortHeaders[10] = "actor_1_name";
    sortHeaders[11] = "movie_title";
    sortHeaders[12] = "num_voted_users";
    sortHeaders[13] = "cast_total_facebook_likes";
    sortHeaders[14] = "actor_3_name";
    sortHeaders[15] = "facenumber_in_poster";
    sortHeaders[16] = "plot_keywords";
    sortHeaders[17] = "movie_imdb_link";
    sortHeaders[18] = "num_user_for_reviews";
    sortHeaders[19] = "language";
    sortHeaders[20] = "country";
    sortHeaders[21] = "content_rating";
    sortHeaders[22] = "budget";
    sortHeaders[23] = "title_year";
    sortHeaders[24] = "actor_2_facebook_likes";
    sortHeaders[25] = "imdb_score";
    sortHeaders[26] = "aspect_ratio";
    sortHeaders[27] = "movie_facebook_likes";
    
    EMPTY = (char*)malloc(sizeof(char));
    EMPTY[0] = '\0';
    
    int HEADER_INDEX = -1;
    for (i = 0; i < 28; i++) {
        if (strcmp(colname, sortHeaders[i]) == 0) {
            HEADER_INDEX = i;
            break;
        }
    }
    
    if (HEADER_INDEX == -1) {
        fprintf(stderr, "ERROR: '%s' is not a valid column.\n", colname);
        fprintf(stdout, "ERROR: '%s' is not a valid column.\n", colname);
        return 0;
    }
    
    ALL_DATA = (Data*)malloc(sizeof(Data) * 256);
    Data empty = { NULL, 0};
    for (i = 0; i < 256; i++) {
        ALL_DATA[i] = empty;
    }
    ALL_DATA_COUNT = (int*)(malloc(sizeof(int)));
    *ALL_DATA_COUNT = 0;
    ALL_DATA_MAX = (int*)(malloc(sizeof(int)));
    *ALL_DATA_MAX = 256;
    ALL_DATA_ROW_COUNT = (int*)(malloc(sizeof(int)));
    *ALL_DATA_ROW_COUNT = 0;

    /***   Begin Sorting   ***/
    
    // Print required metadata
    tid = 1; // Initial directory TID
    int pid = getpid();
    
    if (pthread_mutex_init(&_fileLock, NULL) != 0) { 
        fprintf(stderr, "Could not initialize mutex for file locking\n"); 
        fprintf(stdout, "Could not initialize mutex for file locking\n"); 
        return -1; 
    }
    
    if (pthread_mutex_init(&_printLock, NULL) != 0) { 
        fprintf(stderr, "Could not initialize mutex for print locking\n"); 
        fprintf(stdout, "Could not initialize mutex for print locking\n"); 
        return -1; 
    }

    TArguments *args = (TArguments *)malloc(sizeof(TArguments));
    args->dir = inDir;
    args->inPath = inPath;
    args->outPath = outPath;
    args->column = colname;
    args->file = NULL;
    args->filePath = NULL;

    pthread_t thread;
    totalTIDs = 1;
    int success = pthread_create(&thread, NULL, directoryHandler, (void*)args);
    //int processes = directoryHandler(inDir, colname, inPath, outPath); // Call directoryHandler on inDir (from sorter.c)

    // Abort if thread failed to create
    if (success != 0)
    {
        fprintf(stderr, "ERROR: Could not create main thread\n");
        fprintf(stdout, "ERROR: Could not create main thread\n");
        return -1;
    }

    fprintf(stdout, "Initial PID: %d\n", getpid());
    fprintf(stdout, "TIDS of all spawned threads: "); // No newline, PIDs will be outputed to stdout by other processes
    for (tid = tid; tid <= totalTIDs; tid++) {
        fprintf(stdout, ", %d", tid);
    }
    fflush(stdout);                                   // Make sure to fflush stdout so printing errors don't occur

    fprintf(stdout, "\n");
    fprintf(stdout, "Total number of threads: %d\n", totalTIDs);

    // Check if threads failed to join
    void* joinStatus;
    pthread_join(thread, &joinStatus);
    //fprintf(stderr, "ERROR: Could join threads.\n");
    if ((int)(intptr_t) joinStatus < 0) return -1;

    int number_files = (*ALL_DATA_COUNT);
    
    if (number_files == 0 || ALL_DATA[0].rows == NULL) {
        int p = 0;
        for (p = 0; p < totalCols; p++) {
            write(outFD, sortHeaders[p], strlen(sortHeaders[p])); // write header to output file
            if (p != totalCols - 1) {
                write(outFD, ",", 1);
            }
        }
        write(outFD, "\n", 1);
        closedir(inDir);
        close(outFD);
        if (outDir != NULL)
        {
            closedir(outDir);
        }
        if (dvalue != NULL)
        {
            free(inPath);
        }
        if (ovalue != NULL)
        {
            free(outPath);
        }
        return 0;
    }
    
    totalRows = *ALL_DATA_ROW_COUNT;
    output = ALL_DATA[0].rows;
    int count = ALL_DATA[0].count;
    for (i = 1; i < number_files; i++) {
        if (ALL_DATA[i].rows != NULL) {
            Row* combine = merge(output, count, ALL_DATA[i].rows, ALL_DATA[i].count, HEADER_INDEX, types[HEADER_INDEX]);
            count += ALL_DATA[i].count;
            if (combine != output) {
                free(output);
            }
            output = combine;
        }
    }

    int p = 0;
    for (p = 0; p < totalCols; p++) {
        write(outFD, sortHeaders[p], strlen(sortHeaders[p])); // write header to output file
        if (p != totalCols - 1) {
            write(outFD, ",", 1);
        }
    }
    write(outFD, "\n", 1);

    for (i = 0; i < totalRows; i++) {
        for (p = 0; p < totalCols; p++){
            write(outFD, output[i].entries[p], strlen(output[i].entries[p])); // write row to output file
            if (p != totalCols - 1) {
                write(outFD, ",", 1);
            }
        }
        write(outFD, "\n", 1);
    }

    // Close dirs and free stuff
    closedir(inDir);
    close(outFD);
    if (outDir != NULL)
    {
        closedir(outDir);
    }
    if (dvalue != NULL)
    {
        free(inPath);
    }
    if (ovalue != NULL)
    {
        free(outPath);
    }
    free(EMPTY);

    return 0;
}

