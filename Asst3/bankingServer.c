#include "bankingServer.h"

int main(int argc, char** argv){
	// Argument Format: ./bankingClient <machine> <port>

	// Verify command line arguments
	if (argc != 3) {
		fprintf(stderr, "ERROR: Invalid argument count\n");
		//fprintf(stdout, "ERROR: Invalid argument count");
		fflush(stderr);
		exit(EXIT_FAILURE);
	}

	if(atoi(argv[2]) < 1) {
		fprintf(stderr, "ERROR: Invalid port\n");
	}

    fflush(stderr);
    
    if (pthread_mutex_init(&_AccountsLock, NULL) != 0) { 
        fprintf(stderr, "Could not initialize mutex for account locking\n"); 
        fprintf(stdout, "Could not initialize mutex for account locking\n"); 
        return 1; 
    }
    
    return 0;
}