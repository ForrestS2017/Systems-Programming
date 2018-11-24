#include "multiThreadSorter.h"

/**
 * TO-DO:
 * free and close stuff
 */

int main(int argc, char **argv)
{
    // Directory global variables
    char *inPath;
    char *outPath;
    char *colname;
    DIR *inDir;
    DIR *outDir;

    // Argument global variables
    char *cvalue = NULL;
    char *dvalue = NULL;
    char *ovalue = NULL;
    int a;
    opterr = 0; // Suppress errors from getopt

    // Thread global variables
    int tid;

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
                int i;
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
                int i;
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

    /***   Begin Sorting   ***/

    // Print required metadata
    tid = 1; // Initial directory TID
    fprintf(stdout, "Initial TID: %d\n", tid);

    fprintf(stdout, "TIDS of all spawned threads: "); // No newline, PIDs will be outputed to stdout by other processes
    fflush(stdout);                                   // Make sure to fflush stdout so printing errors don't occur

    TArguments *args = (TArguments *)mallo(sizeof(TArguments));
    args->dir = inDir;
    args->inPath = inPath;
    args->outPath = outPath;
    args->colname = colname;

    pthread_t thread;
    int threads = pthread_create(&thread, NULL, directoryHandler, args);
    //int processes = directoryHandler(inDir, colname, inPath, outPath); // Call directoryHandler on inDir (from sorter.c)

    // Abort if thread failed to create
    if (!threads)
    {
        fprintf(stderr, "ERROR: Could not create main thread\n");
        return -1;
    }

    fprintf(stdout, "\n");
    fprintf(stdout, "Total number of threads: %d\n", threads);

    // Close dirs and free stuff
    closedir(inDir);
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
