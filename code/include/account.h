/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 26/05/2012, SOPE, FEUP
 ****************************************************************************/
#ifndef ACCOUNT_H_
#define ACCOUNT_H_

/** Some variables useful to the implementation
 */

#define MAX_USER_LENGTH 20 	/* The max lenght of the user */
#define PIN_LENGTH 4 /* The length of the pin */
typedef int unsigned accountnr_t; /* the account number type */
extern int unsigned lastAccountNumber;

/** A struct for holding the information of a Bank Account.
 */

struct Account {
	accountnr_t number; /* The number that starts from 0 (not the NIB standard) */
	char *user; /* the name of the user of the Account */
	char* pin; /* the 4 digits pin */
	double balance; /* the money balance */
};
/** Create an account
 * @param *a the pointer for the account struct
 * @param nr the number of account
 * @param usr the username
 * @param pin the pin of the account
 * @param initialBalance the initial ammount
 * @param ammount the money to deposit
 */
int account_create(struct Account *a, accountnr_t nr, char *usr, char *pin,
		double initialBalance);

/** Create an account with autoincrement
 * @param *a the pointer for the account struct
 * @param usr the username
 * @param pin the pin of the account
 * @param initialBalance the initial ammount
 * @param ammount the money to deposit
 */
int account_createAutoIncrement(struct Account *a, char *usr, char *pin,
		double initialBalance);

/** Reset the last account number
 */
void account_resetLastAccountNumber();

/** Sets the last account number
 * @param nr the pretended number
 */
void account_setLastAccountNumber(int nr);

/** Does a deposit of money from an account
 * @param *a the pointer for the account struct
 * @param ammount the money to deposit
 */

void account_deposit(struct Account *a, double amount);

/** Withdraw money from an account
 * @param *a the pointer for the account struct
 * @param ammount the money to deposit
 * @return 1 if success
 */
int account_withdraw(struct Account *a, double amount);

/** Get the balance from an account
 * @param *a the pointer for the account struct
 * @return a double
 */
double account_getBalance(struct Account *a);

/** Get the char pointer of the account information.
 * Can be useful to write in the files.
 * @param *a the pointer for the account struct
 * @return a char pointer
 */
char * account_toString(struct Account *a);

#endif /* ACCOUNT_H_ */
