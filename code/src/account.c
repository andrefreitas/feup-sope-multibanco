#include "account.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int unsigned lastAccountNumber = 0;

int account_create(struct Account *a, accountnr_t nr, char * usr, char *pin,
		double initialBalance) {
	if (strlen(usr) > MAX_USER_LENGTH || strlen(pin) != PIN_LENGTH
			|| initialBalance < 0 || nr < 1)
		return 0;
	a->number = nr;
	a->user = malloc(sizeof(char) * (MAX_USER_LENGTH + 1));
	a->user = usr;
	a->pin = malloc(sizeof(char) * (PIN_LENGTH + 1));
	a->pin = pin;
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
	if (a->balance < amount)
		return 0;
	return 1;
}
double account_getBalance(struct Account *a) {
	return a->number;
}
char * account_toString(struct Account *a) {
	char * buffer = malloc(sizeof(char) * 100);
	sprintf(buffer, "%08d %-20s %-4s %13.2f", a->number, a->user, a->pin,
			a->balance);
	return buffer;
}
