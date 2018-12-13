#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <netdb.h> 

/** Global Variables **/


/** Structs **/


/** Functions **/

// Get user input in a loop
void* getUserInput(void*);

void* getServerOutput(void*);

int returnError(int);