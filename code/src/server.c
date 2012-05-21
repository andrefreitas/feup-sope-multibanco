/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 26/05/2012, SOPE, FEUP
 ****************************************************************************/

#include "account.h"
#include "server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

// Necessary constants
#define MAX_STRING 100
#define MAX_ACCOUNTS 100

void server_create(struct Server *s, char *accountsFileName, char *requestFIFOname) {
	s->accountsFileName = malloc(sizeof(char) * MAX_STRING);
	s->requestsFIFOname = malloc(sizeof(char) * MAX_STRING);
	strcpy(s->accountsFileName, accountsFileName);
	strcpy(s->requestsFIFOname, requestFIFOname);
	mkfifo(s->requestsFIFOname, O_RDONLY);
	s->accounts =malloc(sizeof (struct Account* ) * MAX_ACCOUNTS);
	s->totalAccounts=0;

}

int server_createAccount(struct Server *s, accountnr_t nr, char *usr, char *pin, double initialBalance) {
	struct Account *a = malloc(sizeof(struct Account));
	int status= account_create(a, nr,usr, pin, initialBalance);
	s->accounts[s->totalAccounts]=a;
	s->totalAccounts++;
	return status;
}
int server_createAccountIncrement(struct Server *s, char *usr, char *pin, double initialBalance){

	struct Account *a=malloc(sizeof(struct Account));
	int status= account_createAutoIncrement(a, usr, pin, initialBalance);
	s->accounts[s->totalAccounts]=a;
	s->totalAccounts++;
	return status;
}
int server_deleteAccount(struct Server *s, accountnr_t nr) {
	struct Account *a = server_getAccountbyID(s, nr);
	// Check if account exist
	if (a == NULL)
	return 0;
	// Shift left the accounts
	int unsigned i,j;
	for (i=0; i<s->totalAccounts; i++)
		if(s->accounts[i]->number == nr)
			for (j=i; j< (s->totalAccounts-1);j++)
				s->accounts[j]=s->accounts[j+1];
	s->totalAccounts--;
	return 1;
}

double server_getAccountBalance(struct Server *s, accountnr_t nr) {
	struct Account *a = server_getAccountbyID(s, nr);
	if (a != NULL)
	{
		account_getBalance(a);
		return 0;
	}
	return 1;
}

int server_depositAccount(struct Server *s, accountnr_t nr, double amount) {
	struct Account *a = server_getAccountbyID(s, nr);
	if (a != NULL)
	{
		account_deposit(a, amount);
		return 0;
	}
	return 1;
}


int server_transfer(struct Server *s, accountnr_t source,
		accountnr_t destination, double amount) {
	struct Account *srcacc = server_getAccountbyID(s, source);
	struct Account *destacc = server_getAccountbyID(s, destination);
	if (srcacc != NULL && destacc != NULL
			&& account_getBalance(srcacc) >= amount) {
		account_withdraw(srcacc, amount);
		account_deposit(destacc, amount);
		return 0;
	}
	return 1;

}
int server_witdhdraw(struct Server *s, accountnr_t nr, double amount) {
	struct Account *a = server_getAccountbyID(s, nr);
	if (a != NULL && account_getBalance(a) >= amount) {
		account_withdraw(a, amount);
		return 0;
	}
	return 1;
}

struct Account* server_getAccountbyID(struct Server *s, accountnr_t nr) {
	int i;
	for (i = 0; i <s->totalAccounts; i++)
		if (s->accounts[i] != NULL)
			if (s->accounts[i]->number == nr)
				return s->accounts[i];
	return NULL;
}

int main() {
	struct Server *s=malloc(sizeof(struct Server));
	server_create(s,"accounts.txt","reqiests");
	server_createAccountIncrement(s,"Joana Faria","1234",12.10);
	server_createAccount(s,4,"Ana","1234",12.10);
	struct Account *a;
	a=server_getAccountbyID(s,4);
	if(a!=NULL){
		printf("%s\n",account_toString(a));
	}
	return 0;
}
