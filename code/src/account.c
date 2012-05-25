/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#include "account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BUFFER_LEN 100
int unsigned lastAccountNumber = 0;

int account_create(struct Account *a, accountnr_t nr, char * usr, char *pin,
		double initialBalance) {
	if (strlen(usr) > MAX_USER_LENGTH || strlen(pin) != PIN_LENGTH
			|| initialBalance < 0 || nr < 1)
		return 0;
	a->number = nr;
	a->user = malloc(sizeof(char) * (MAX_USER_LENGTH + 1));
	strcpy(a->user, usr);
	a->pin = malloc(sizeof(char) * (PIN_LENGTH + 1));
	strcpy(a->pin, pin);
	a->balance = initialBalance;

	return 1;

}

int account_createAutoIncrement(struct Account *a, char *usr, char *pin,
		double initialBalance) {
	return account_create(a, ++lastAccountNumber, usr, pin, initialBalance);
}

void account_resetLastAccountNumber() {
	lastAccountNumber = 0;
}
void account_deposit(struct Account *a, double amount) {
	a->balance += amount;
}
int account_withdraw(struct Account *a, double amount) {
	if (a->balance >= amount) {
		a->balance -= amount;
		return 1;
	}
	return 0;
}
double account_getBalance(struct Account *a) {
	return a->balance;
}
char * account_toString(struct Account *a) {
	char * buffer = malloc(sizeof(char) * MAX_BUFFER_LEN);
	sprintf(buffer, "%07d %-20s %-4s %13.2f", a->number, a->user, a->pin,
			a->balance);
	return buffer;
}
char * account_toStringFIFO(struct Account *a) {
	char * buffer = malloc(sizeof(char) * MAX_BUFFER_LEN);
	sprintf(buffer, "%07d %-20s|%-4s %13.2f", a->number, a->user, a->pin,
			a->balance);
	return buffer;
}
void account_setLastAccountNumber(int nr) {
	lastAccountNumber = nr;
}
int account_compare(const void * a1, const void * a2) {
	return (((struct Account*) a1)->number - ((struct Account*) a2)->number);
}

int account_createFromString(struct Account *a, char *buffer) {
	// Necessary buffers
	char nrBuffer[MAX_BUFFER_LEN];
	char userBuffer[17];
	char pinBuffer[MAX_BUFFER_LEN];
	char balanceBuffer[MAX_BUFFER_LEN];
	double balance;
	unsigned int number;

	// Get the account number
	strncpy(nrBuffer, buffer, 7);
	nrBuffer[7] = '\0';
	buffer = buffer + 8; // Shift right
	number = atoi(nrBuffer);

	// Get the user name
	strncpy(userBuffer, buffer, 20);
	userBuffer[20] = '\0';
	buffer = buffer + 21; // Shift right

	// Get the pin
	strncpy(pinBuffer, buffer, 4);
	pinBuffer[4] = '\0';
	buffer = buffer + 5; // Shift right

	// Get the balance
	strcpy(balanceBuffer, buffer);
	balance = atof(balanceBuffer);

	account_create(a, number, userBuffer, pinBuffer, balance);
	return 1;

}
