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
#include <sys/time.h>
#include <netinet/in.h> 
#include <netdb.h> 

/** Global Variables **/


/** Structs **/

// Basic data stucture for user accounts
typedef struct _Account {
    char* name;
    double balance;
    char* session;

} Account;

// Node to link all accounts
typedef struct _Node {
    Account* account;
    struct _Node* next;
} Node;

// References (linked list, active account)
Node* Accounts;
Account* activeAccount;

// Threads and locks


pthread_mutex_t _AccountsLock;

/** Functions **/

// Creates new unique accounts to add to Node* Accounts
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
void Quit();

// Print all account information
int PrintAccounts();

// Throw errors easily
int returnError(int);

// Set timer to print account data
int setTimer(int);