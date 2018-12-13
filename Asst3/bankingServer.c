#include "bankingServer.h"

int main(int argc, char** argv) {
  
    int PORT = 86942;
    
    if (pthread_mutex_init(&_AccountsLock, NULL) != 0) {
        fprintf(stderr, "Could not initialize mutex for account locking\n"); 
        fprintf(stdout, "Could not initialize mutex for account locking\n"); 
        return 1; 
    }
    
    int server_fd, new_socket/*, valread*/; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    char buffer[1024] = {0};
    //char * hello = "Hello from server";
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        fprintf(stderr, "Socket failed\n");
        fprintf(stdout, "Socket failed\n");
        exit(1);
    }
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        fprintf(stderr, "setsockopt failed\n");
        fprintf(stdout, "setsockopt failed\n");
        exit(1);
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
       
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        fprintf(stderr, "bind failed\n");
        fprintf(stdout, "bind failed\n"); 
        exit(1);
    }
    if (listen(server_fd, 3) < 0) {
        fprintf(stderr, "listen failed\n");
        fprintf(stdout, "listen failed\n");
        exit(1);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        fprintf(stderr, "accept failed\n");
        fprintf(stdout, "accept failed\n");
        exit(1);
    }
    while (TRUE) {
    /*valread = */read(new_socket, buffer, 1024);
    }
    printf("%s\n", buffer);
    //send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    
    return 0;
}