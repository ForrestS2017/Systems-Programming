#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h> 
#include <sys/types.h>
#include <netinet/in.h> 
#include <netdb.h> 

/** Global Variables **/


/** Structs **/

typedef struct _Account {
    char name[255];
    double balance;
    int session;

} Account;

typedef struct _Node {
    Account* account;
    struct _Node* next;
} Node;

Node* Accounts;

/** Functions **/

int CreateAccount(char *);

int ServeAccount(char *);

int DepositAccount(double);

int WithdrawACcount(double);

double Query();

int End();

int Quit();
