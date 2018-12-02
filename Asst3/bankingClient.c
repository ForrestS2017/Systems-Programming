#include "bankingClient.h"

int main(int argc, char** argv){
	// Argument Format: ./bankingClient <machine> <port>

	// Verify command line arguments
	if (argc != 3) {
		fprintf(stderr, "ERROR: Invalid argument count");
		//fprintf(stdout, "ERROR: Invalid argument count");
		fflush(stderr);
		exit(EXIT_FAILURE);
	}

	if(atoi(argv[2]) < 1) {
		fprintf(stderr, "ERROR: Invalid port");
	}

	fflush(stderr);

	// Begin server functionality
	int port = atoi(argv[2]);
	int server_fd, new_socket, valread;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        fprintf(stderr,"socket failed"); 
        fflush(stderr);
		exit(EXIT_FAILURE);
    } 

	return 0; 
	
}
