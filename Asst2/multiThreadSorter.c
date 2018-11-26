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
    {
        outPath = NULL;
        outDir = NULL;
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
            outPath = NULL;
            outDir = NULL;
        }
    }
    
    char * path = inPath;
    if (outPath != NULL) {
        strcpy(oPath, outPath);
    }
    char oPath[strlen(outPath) + strlen(colname) + 22];
    strcpy(oPath, outPath);
    strcat(oPath, "AllFiles-sorted-");
    strcat(oPath, colname);
    strcat(oPath, ".csv");
    
    int outFD = open(oPath, O_RDWR | O_CREAT, 0600); // Create a file with read/write permissions for owner
    if (outFD < 0) {
        fprintf(stderr, "ERROR: Could not create file '%s'.\n", outFD);
        return -1;
    }
    
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
    
    ALL_DATA = (Row**)malloc(sizeof(Row*) * 256);
    for (i = 0; i < 256; i++) {
        ALL_DATA[i] = NULL;
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
    fprintf(stderr, "ERROR: Could not join threads.\n");
    if ((int)(intptr_t) joinStatus < 0) return -1;

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

    return 0;
}

