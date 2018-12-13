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
    
    int PORT = atoi(argv[2]);
    
	if (PORT < 1) {
		fprintf(stderr, "ERROR: Invalid port\n");
        fprintf(stdout, "ERROR: Invalid port\n");
        return 1;
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
    
    //struct sockaddr_in address; 
    int sock = 0/*, valread*/; 
    struct sockaddr_in serv_addr; 
    char *hello = "Hello from client"; 
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        fprintf(stderr, "Socket could not be created\n");
        fprintf(stdout, "Socket could not be created\n");
        return -1; 
    } 
   
    memset(&serv_addr, '0', sizeof(serv_addr)); 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) { 
        fprintf(stderr, "Invalid address\n");
        fprintf(stdout, "Invalid address\n");
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
        fprintf(stderr, "Connect failed\n");
        fprintf(stdout, "Connect failed\n");
        return -1; 
    } 
    send(sock, hello, strlen(hello), 0 ); 
    printf("Hello message sent\n"); 
    /*valread = */read(sock, buffer, 1024); 
    printf("%s\n", buffer); 
	
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
