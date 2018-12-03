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

// Basic data stucture for user accounts
typedef struct _Account {
    char name[255];
    double balance;
    int session;

} Account;

// Node to link all accounts
typedef struct _Node {
    Account* account;
    struct _Node* next;
} Node;

// Linked list of accounts
Node* Accounts;

/** Functions **/

// Creates new unique accounts
int CreateAccount(char *);

// Enables session to deposit to and withdraw from accounts
int ServeAccount(char *);

// Deposit funds into an account
int DepositAccount(double);

// Withdraw funds from an account
int WithdrawAccount(double);

// Return balance of account in session
double Query();

// End current session
int End();

// Quit program
int Quit();
