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
}