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
int Quit() {
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

    fprintf(stdout, "Connection terminated\n", activeAccount->name);
    fflush(stdout);
    return 1;
}

/**
 * PrintAccounts - Print all account metadata
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

    return 1;
}

/**                          **/
/** BANKING CLIENT FUNCTIONS **/
/**                          **/

/**
 * getUserInput - Loop to read user standard input
 * 
 * @return 1 if successful, 0 if failure
 */
int getUserInput() {
    // Get initial command
    printf("Valid commands: create, serve, quit\n");
    char command[10];
    char userInput[255];

    sscanf("%s %s", command, userInput);
    const* cmd0[3] = {"create", "serve", "quit"};
    const* cmd1[5] = {"serve", "deposit", "withdraw", "end", "quit"};

    // Error check
    if(command == NULL || userInput == NULL) return returnError(70);

    int state = 0;  // Check if we are pre-serve (0) or serving (1) or have quit (-1)

    // While we have not quit, loop per active client
    while(state > -1) {
        if ((strcmp(command, "create") == 0) && state == 0) {
            state = 0;

        } else if ((strcmp(command, "serve") == 0) && state == 0) {
            state = 0;

        } else if ((strcmp(command, "deposit") == 0) && state == 1) {
            state = 0;

        } else if ((strcmp(command, "withdraw") == 0) && state == 1) {
            state = 0;

        } else if ((strcmp(command, "end") == 0) && state == 1){
            state = 0;

        } else if (strcmp(command, "quit") == 0) {
            state = -1;

        } else {
            // User input command at invalid time
            return returnError(71);
        }
    }

    // If out of loop, user quit session
    return 1;
}

/**
 * returnError - Print errors and return 0 because I'm lazy
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