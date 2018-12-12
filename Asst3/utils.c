#include "bankingServer.h"
#include "bankingClient.h"

/**                          **/
/** BANKING SERVER FUNCTIONS **/
/**                          **/

/**
 * CreateAccount - Creates _Account struct with name and starting balance
 * 
 * Creates _Account struct, a _Node with the _Account and adds it to the Accounts linked list 
 * 
 * @param accountName - char[255] accountName to create _Account with
 * @return 1 if successful, 0 if failure (duplicate accountName or invalid input)
 */
int CreateAccount(char * accountName) {
    // Error checking (no name, name length, duplicate account)
    if (accountName == NULL || strlen(accountName) > 255) return returnError(0);
    if (Accounts) {
        Node* temp = Accounts;
        while (temp->next != NULL) {
            if (strcmp(temp->account->name, accountName) == 0) {
                return returnError(1);
            }
            temp = temp->next;
        }
    }

    // Create account object and node
    Account* newAccount = (Account*) malloc(sizeof(Account));
    if (!newAccount) return returnError(69);
    newAccount->name = (char*) malloc(255 * sizeof(char));
    newAccount->name = accountName;
    newAccount->balance = 0.0;
    newAccount->session = NULL;     // TODO - create unique FLAG

    Node* newNode = (Node*) malloc(sizeof(Node));
    if (!newNode) return returnError(69);
    newNode->account = newAccount;
    newNode->next = NULL;

    // Add to end of Account linked list or add first node
    Node* temp = Accounts;
    if (temp == NULL) {
        Accounts = newNode;
    } else {
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Return 1 upon successful creation
    return 1;
}

/**
 * ServeAccount - Puts input account into session to serve (deposit & withdraw). Enables 'session' flag
 * 
 * @param accountName - char[255] accountName to create _Account with
 * @return 1 if successful, 0 if failure
 */
int ServeAccount(char * accountName) {
    // Error checking (no name, name length)
    if (accountName == NULL || strlen(accountName) > 255) return returnError(0);
    if (Accounts == NULL) return returnError(2);

    if (Accounts) {
        Node* temp = Accounts;
        while (temp->next != NULL) {
            if (strcmp(temp->account->name, accountName) == 0) {
                activeAccount = temp;
            }
            temp = temp->next;
        }
    }

    if (activeAccount = NULL) return returnError(3);
    activeAccount->session = 1;

    return 1;
}

/**
 * DepositAccount - Deposit funds into an ccount
 * 
 * @param fund - double value to add to account's balance
 * @return 1 if successful, 0 if failure
 */
int DepositAccount(double fund) {
    // Error checking
    if (fund < 1.0) return Error(10);
    if (Accounts == NULL) return returnError(2);
    if (activeAccount == NULL) return returnError(4);

    // Add fund to active account;
    activeAccount-> balance = activeAccount->balance + fund;

    return 1;
}

/**
 * WithdrawAccount - Withdraw funds into an ccount
 * 
 * @param fund - double value to add to account's balance
 * @return 1 if successful, 0 if failure
 */
int WithdrawAccount(double fund) {
    // Error checking
    if (fund < 1.0) return Error(10);
    if (Accounts == NULL) return returnError(2);
    if (activeAccount == NULL) return returnError(4);
    

    // Prohibit bouncing
    if (fund > activeAccount->balance) return error(11);

    // Withdraw funds
    activeAccount->balance = activeAccount->balance - fund;

    return 1;
}

/**
 * Query - Get balance of account in session
 * 
 * @return double balance of account in session
 */
double Query() {
    // Error checking
    if (Accounts == NULL) return returnError(2);
    if (activeAccount == NULL) return returnError(4);

    // Return active account balance
    fprintf(stdout, "Balance for account %s: %lf\n", activeAccount->name, activeAccount->balance);
    fflush(stdout);
    return activeAccount->balance;
}

/**
 * End - End current session for current account (does not end program)
 * 
 * @return 1 if successful, 0 if failure
 */
int End() {
    fprintf(stdout, "Session terminated for account %s\n", activeAccount->name);
    fflush(stdout);
    return 1;
}

/**
 * Quit - Quit program
 * 
 * @return 1 if successful, 0 if failure
 */
void Quit() {
    // Free all nodes
    if (Accounts) {
        Node* temp = Accounts;
        Node* prev;
        while (temp != NULL) {
            prev = temp;
            temp = temp->next;
            if(prev) free(prev);
        }
    }

    fprintf(stdout, "Session complete - Connection terminated\n", activeAccount->name);
    fflush(stdout);
    exit(0);
}

/**
 * PrintAccounts - Print all account metadata. Should be mutexed
 * 
 * @return 1 if successful, 0 if failure
 */
int PrintAccounts() {
    // Error checking
    if (Accounts == NULL) return returnError(2);

    // Begin printing lists
    Node* temp = Accounts;
    while (temp) {

        fprintf(stderr, "Account: %s\nBalance: %lf\n\n", temp->account->name, temp->account->balance);
    }
    free(temp);

    return 1;
}

/**
 * setTimer - Print account information every 15 seconds
 * 
 * @return 1 if successful, 0 if failure
 */
int setTimer(int seconds){
    // Print first set of accounts (should be none)
    PrintAccounts();
    // Set timer to keep printing
    struct itimerval new;
    new.it_interval.tv_usec = 0;
    new.it_interval.tv_sec = 0;
    new.it_value.tv_usec = 0;
    new.it_value.tv_sec = (long int) seconds; 
    if (setitimer(ITIMER_REAL, &new, 0) < 0) return 0;
}

/**                          **/
/** BANKING CLIENT FUNCTIONS **/
/**                          **/

/**
 * getUserInput - Loop to read user standard input
 * 
 * @return 1 if successful, 0 if failure
 */
void* getUserInput(void* arguments) {
    // Get initial command
    int socketfd = * (int*) arguments;
    char command[11];
    char userInput[255];

    int state = 0;  // 0 = Pre-serve, 1 = Serving, -1 = Quit

    // While we have not quit, loop per active client
    while(state > -1) {
        // Prompt user
        command[0] = '\0';
        userInput[0] = '\0';
        fprintf(stdout,"Enter commands\n");
        sscanf("%s %s", command, userInput);
        // Error check
        if(strlen(command) == 0 || strlen(userInput) == 0) return returnError(70);

        // Call commands
        if ((strcmp(command, "create") == 0) && state == 0) {
            state = 0;
            
            // createAccount()

        } else if ((strcmp(command, "serve") == 0) && state == 0) {
            state = 1;

            // ServeAccount()

        } else if ((strcmp(command, "deposit") == 0) && state == 1) {
            state = 0;
            // Check if user input is float
            // DepositAccount()

        } else if ((strcmp(command, "withdraw") == 0) && state == 1) {
            state = 0;
            // Check if user input is float
            // WithdrawAccount()

        } else if ((strcmp(command, "end") == 0) && state == 1){
            state = 0;

            // End()

        } else if (strcmp(command, "quit") == 0) {
            state = -1;

            // Quit()

        } else {
            // User input command at invalid time
            returnError(71);
        }
    }

    // If out of loop, user quit session
    return (void*) (size_t) 1;
}

/**
 * getServerOutput - Print account information ever 20 seconds
 * 
 * @return 1 if successful, 0 if failure
 */
void* getServerOutput(void* arguments) {
    // TODO
    if (arguments == NULL) return returnError(50);

    int socketfd = * (int*) arguments;
    char response[1000];

    int byte = 1;

    // TODO: read server response somehow

    return (void*) (size_t) 1;
}

/**
 * returnError - Print errors and return 0 because I'm lazy
 * 
 * 0-9 = Account creation | 10 - 19 = Withdraw/Deposit | 50-59 = Server errors | 60 - 69 = Runtime error | 70 - 79 = Formatting 
 * 
 * @param code{Account Name, Deposit Amount}
 * @return 0 only
 */
int returnError(int code) {
    if (code == 0) {
        fprintf(stderr, "ERROR: Invalid account name - 0 to 255 characters only\n");
        fprintf(stdout, "ERROR: Invalid account name - 0 to 255 characters only\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 1) {
        fprintf(stderr, "ERROR: Duplicate account name\n");
        fprintf(stdout, "ERROR: Duplicate account name\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 2) {
        fprintf(stderr, "ERROR: No accounts created\n");
        fprintf(stdout, "ERROR: No accounts created\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 3) {
        fprintf(stderr, "ERROR: Account not found\n");
        fprintf(stdout, "ERROR: Account not found\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 4) {
        fprintf(stderr, "ERROR: Active account not found\n");
        fprintf(stdout, "ERROR: Active account not found\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 10) {
        fprintf(stderr, "ERROR: Invalid amount - please enter a positive decimal value\n");
        fprintf(stdout, "ERROR: Invalid amount - please enter a positive decimal value\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 11) {
        fprintf(stderr, "ERROR: Invalid transaction - reuqeust exceeds account funds\n");
        fprintf(stdout, "ERROR: Invalid transaction - reuqeust exceeds account funds\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 50) {
        fprintf(stderr, "ERROR: Could not pass arguments to server\n");
        fprintf(stdout, "ERROR: Could not pass arguments to server\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 68) {
        fprintf(stderr, "ERROR: Unable to create thread\n");
        fprintf(stdout, "ERROR: Unable to create thread\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 69) {
        fprintf(stderr, "ERROR: Unable to allocate enough memory\n");
        fprintf(stdout, "ERROR: Memory allocation failure\n");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 70) {
        fprintf(stderr, "ERROR: Input format is <command> <name/amount>\n");
        fprintf(stdout, "ERROR: Input format is <command> <name/amount>\n");
        fflush(stderr); fflush(stdout);
        return 0;
    }  else if (code == 71) {
        fprintf(stderr, "ERROR: You may only create and serve an account if you are not already serving an account.\n You may only deposit and withdraw while you are serving an account.\n");
        fprintf(stdout, "ERROR: You may only create and serve an account if you are not already serving an account.\n You may only deposit and withdraw while you are serving an account.\n");
        fflush(stderr); fflush(stdout);
        return 0;

    return 0;
}