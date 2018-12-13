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
	int socket_fd/*, new_socket, valread*/;

	// Socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        fprintf(stderr,"ERROR: Socket connection failure\n"); 
        fflush(stderr);
		exit(EXIT_FAILURE);
    }

	// Host
	struct hostent *hostentry;

	if((hostentry = gethostbyname(argv[1])) == NULL) {
        fprintf(stderr,"ERROR: Could not retrieve specified host\n"); 
        fflush(stderr);
		exit(EXIT_FAILURE);
	}

	// Server
	struct sockaddr_in server;
	server.sin_family = AF_INET;
	memcpy(&server.sin_addr, hostentry->h_addr_list[0], hostentry->h_length);
	server.sin_port = htons(port);

	// Connection
	
	/**
	 * TODO: CONNECT TO SERVER
	 * 
	 */

	/** Begin Client functionality **/

	// Thread for user input and server response
	pthread_t userIn;
	int success = pthread_create(&userIn, NULL, getUserInput, &socket_fd);
	if (success != 0) return returnError(68);

	pthread_t serverOut;
	int success2 = pthread_create(&serverOut, NULL, getServerOutput, &socket_fd);
	if (success2 != 0) return returnError(68);

	pthread_join(userIn, NULL);
	pthread_join(serverOut, NULL);

	fprintf(stdout, "Session completed - Connection terminated\n");
	fflush(stdout);

	return 0; 
	
}
