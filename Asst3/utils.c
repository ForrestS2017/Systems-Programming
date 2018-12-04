#include "bankingClient.h"

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
    if (accountName == NULL) return returnError(0);
    if (strlen(accountName) > 255) return returnError(0);
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
    newAccount->balance = 0;
    newAccount->session = 0;

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
    if (accountName == NULL) return returnError(0);
    if (strlen(accountName) > 255) return returnError(0);
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
    fprintf(stdout, "Balance for account %s: %lf", activeAccount->name, activeAccount->balance);
    fflush(stdout);
    return activeAccount->balance;
}

/**
 * End - End current session for current account (does not end program)
 * 
 * @return 1 if successful, 0 if failure
 */
int End() {
    fprintf(stdout, "Session terminated for account %s", activeAccount->name);
    fflush(stdout);
    return 1;
}

/**
 * Quit - Quit program
 * 
 * @return 1 if successful, 0 if failure
 */
int Quit() {
    fprintf(stdout, "Connection terminated", activeAccount->name);
    fflush(stdout);
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
        fprintf(stderr, "ERROR: Invalid account name - 0 to 255 characters only");
        fprintf(stdout, "ERROR: Invalid account name - 0 to 255 characters only");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 1) {
        fprintf(stderr, "ERROR: Duplicate account name");
        fprintf(stdout, "ERROR: Duplicate account name");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 2) {
        fprintf(stderr, "ERROR: No accounts created");
        fprintf(stdout, "ERROR: No accounts created");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 3) {
        fprintf(stderr, "ERROR: Account not found");
        fprintf(stdout, "ERROR: Account not found");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 4) {
        fprintf(stderr, "ERROR: Active account not found");
        fprintf(stdout, "ERROR: Active account not found");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 10) {
        fprintf(stderr, "ERROR: Invalid amount - please enter a positive decimal value");
        fprintf(stdout, "ERROR: Invalid amount - please enter a positive decimal value");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 11) {
        fprintf(stderr, "ERROR: Invalid transaction - reuqeust exceeds account funds");
        fprintf(stdout, "ERROR: Invalid transaction - reuqeust exceeds account funds");
        fflush(stderr); fflush(stdout);
        return 0;
    } else if (code == 69) {
        fprintf(stderr, "ERROR: Unable to allocate enough memory");
        fflush(stderr);
        return 0;
    }

    return 0;
}

