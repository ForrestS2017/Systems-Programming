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

}

/**
 * ServeAccount - Puts input account into session to serve (deposit & withdraw). Enables 'session' flag
 * 
 * @param accountName - char[255] accountName to create _Account with
 * @return 1 if successful, 0 if failure
 */
int ServeAccount(char * accountName) {

}

/**
 * DepositAccount - Deposit funds into an ccount
 * 
 * @param fund - double value to add to account's balance
 * @return 1 if successful, 0 if failure
 */
int DepositAccount(double fund) {

}

/**
 * WithdrawAccount - Withdraw funds into an ccount
 * 
 * @param fund - double value to add to account's balance
 * @return 1 if successful, 0 if failure
 */
int WithdrawAccount(double fund) {

}

/**
 * Query - Get balance of account in session
 * 
 * @return double balance of account in session
 */
double Query() {

}

/**
 * End - End current session for current account (does not end program)
 * 
 * @return 1 if successful, 0 if failure
 */
int End() {

}

/**
 * Quit - Quit program
 * 
 * @return 1 if successful, 0 if failure
 */
int Quit() {

}