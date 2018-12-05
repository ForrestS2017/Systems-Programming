#include "bankingClient.h"

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

	/** Begin server functionality **/
	
	int port = atoi(argv[2]);
	int server_fd, new_socket, valread;

	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        fprintf(stderr,"ERROR: Socket connection failure\n"); 
        fflush(stderr);
		exit(EXIT_FAILURE);
    }

	/** Begin User functionality **/

	// Thread for user input and server response
	pthread_t userIn;
	int success = pthread_create(&userIn, NULL, getUserInput, NULL);
	if (success != 0) return returnError(68);

	pthread_t serverOut;
	int success = pthread_create(&serverOut, NULL, getServerOutput(), NULL);
	if (success != 0) return returnError(68);


	return 0; 
	
}
