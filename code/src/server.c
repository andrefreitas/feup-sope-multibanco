/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/

#include "account.h"
#include "server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include "logs.h"
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// Necessary constants
#define MAX_STRING 100
#define MAX_ACCOUNTS 100

struct Server *currentServer;

void server_saveAccounts(struct Server *s) {
	if (s->totalAccounts > 0) {
		int fileDes = open(s->accountsFileName, O_CREAT | O_RDWR, 0777);
		if (fileDes == 1)
			perror("Account file\n");

		// Write total nr of accounts
		char totalAccounts[MAX_STRING];
		sprintf(totalAccounts, "%07d\n", s->totalAccounts);
		write(fileDes, totalAccounts, strlen(totalAccounts) * sizeof(char));

		// write all the accounts
		unsigned int i;
		char accountBuffer[MAX_STRING];
		for (i = 0; i < s->totalAccounts; i++) {
			sprintf(accountBuffer, "%s\n", account_toString(&s->accounts[i]));
			write(fileDes, accountBuffer, strlen(accountBuffer) * sizeof(char));
		}

		close(fileDes);
	}
}

void server_loadAccounts(struct Server *s) {
	FILE *file = fopen(s->accountsFileName, "r");
	if (file != NULL) {
		char line[128];
		int total = 0;
		int unsigned i;
		int maxAccountnr = 1;
		// Get the total accounts
		fgets(line, sizeof line, file);
		total = atoi(line);
		for (i = 0; i < total; i++) {
			if (fgets(line, sizeof line, file) != NULL) {
				struct Account *a = malloc(sizeof(struct Account));
				account_createFromString(a, line);
				if (a->number > maxAccountnr)
					maxAccountnr = a->number;
				server_addAccountRealloc(s, a);

			}
		}

		server_sortAccounts(s);
		fclose(file);
		account_setLastAccountNumber(maxAccountnr);
	}

}

void server_create(struct Server *s, char *accountsFileName,
		char *requestFIFOname) {
	s->accountsFileName = malloc(sizeof(char) * MAX_STRING);
	s->requestsFIFOname = malloc(sizeof(char) * MAX_STRING);
	strcpy(s->accountsFileName, accountsFileName);
	strcpy(s->requestsFIFOname, requestFIFOname);
	mkfifo(s->requestsFIFOname, 0777);
	s->accounts = malloc(sizeof(struct Account));
	s->totalAccounts = 0;
	s->shutDown = 0;
	server_loadAccounts(s);

}

void server_addAccountRealloc(struct Server *s, struct Account *a) {
	// Dynamic memory allocation
	struct Account *accountsTemp;
	s->totalAccounts++;
	accountsTemp = realloc(s->accounts,
			s->totalAccounts * sizeof(struct Account));
	s->accounts = accountsTemp;
	s->accounts[s->totalAccounts - 1] = *a;
	server_sortAccounts(s);
}

int server_createAccount(struct Server *s, accountnr_t nr, char *usr, char *pin,
		double initialBalance) {
	if (server_accountAlreadyExists(s, nr))
		return 0;
	struct Account *a = malloc(sizeof(struct Account));
	int status = account_create(a, nr, usr, pin, initialBalance);
	// Dynamic memory allocation
	server_addAccountRealloc(s, a);
	return status;
}
int server_createAccountIncrement(struct Server *s, char *usr, char *pin,
		double initialBalance) {

	struct Account *a = malloc(sizeof(struct Account));
	int status = account_createAutoIncrement(a, usr, pin, initialBalance);

	// Dynamic memory allocation
	server_addAccountRealloc(s, a);
	return status;
}
int server_deleteAccount(struct Server *s, accountnr_t nr) {
	struct Account *a = server_getAccountbyID(s, nr);
// Check if account exist
	if (a == NULL)
		return 0;
// Shift left the accounts
	int unsigned i, j;
	for (i = 0; i < s->totalAccounts; i++)
		if (s->accounts[i].number == nr)
			for (j = i; j < (s->totalAccounts - 1); j++)
				s->accounts[j] = s->accounts[j + 1];
	s->totalAccounts--;
	return 1;
}

double server_getAccountBalance(struct Server *s, accountnr_t nr) {
	struct Account *a = server_getAccountbyID(s, nr);
	return account_getBalance(a);
}

int server_depositAccount(struct Server *s, accountnr_t nr, double amount) {
	struct Account *a = server_getAccountbyID(s, nr);
	if (a != NULL) {
		account_deposit(a, amount);
		return 0;
	}
	return 1;
}

int server_transfer(struct Server *s, accountnr_t source,
		accountnr_t destination, double amount) {
	struct Account *srcacc = server_getAccountbyID(s, source);
	struct Account *destacc = server_getAccountbyID(s, destination);
	if (srcacc != NULL && destacc != NULL) {
		if (account_withdraw(srcacc, amount)) {
			account_deposit(destacc, amount);
			return 1;
		}
	}
	return 0;

}
int server_withdraw(struct Server *s, accountnr_t nr, double amount) {
	struct Account *a = server_getAccountbyID(s, nr);
	return account_withdraw(a, amount);
}

struct Account* server_getAccountbyID(struct Server *s, accountnr_t nr) {
	struct Account *a;
	struct Account key;
	account_create(&key, nr, "KEY", "keyy", 0);
	a = (struct Account*) bsearch(&key, s->accounts, s->totalAccounts,
			sizeof(struct Account), account_compare);
	return a;
}

void server_printAccounts(struct Server *s) {
	int unsigned i = 0;
	for (i = 0; i < s->totalAccounts; i++) {
		printf("%s\n", account_toString(&s->accounts[i]));
	}
}

char* server_getAccounts(struct Server *s) {
	char* retStr = malloc(sizeof(char) * s->totalAccounts * 100);
	int unsigned i = 0;
	strcpy(retStr, account_toStringFIFO(&s->accounts[i]));
	for (i = 1; i < s->totalAccounts; i++) {
		strcat(retStr, ">");
		strcat(retStr, account_toStringFIFO(&s->accounts[i]));
	}

	return retStr;
}

void server_sortAccounts(struct Server *s) {
	qsort(s->accounts, s->totalAccounts, sizeof(struct Account),
			account_compare);

}

int server_accountAlreadyExists(struct Server *s, accountnr_t nr) {
	int unsigned i;
	for (i = 0; i < s->totalAccounts; i++) {
		if (s->accounts[i].number == nr)
			return 1;
	}
	return 0;
}

void server_run(struct Server *s) {
	int total = 0;
	do {
		char buffer[100];
		struct Request *req = malloc(sizeof(struct Request));
		req->pid = 0;
		if (request_readFIFO(s->requestsFIFOname, req, buffer)) {
			if (req->pid != 0) {
				pthread_t thread;
				currentServer=s;
				pthread_create(&thread, NULL, server_handleRequestThread, req);
				//server_handleRequest(s, req);
			}
			total++;
			printf("Total: %d\n", total);
		}
	} while (!s->shutDown);
	server_saveAccounts(s);
}

void * server_handleRequestThread(void * arg) {
	int i;
	server_handleRequest(currentServer, (struct Request *) arg);
	return NULL;
}

void server_handleRequest(struct Server *s, struct Request *r) {
	char *tmp = malloc(sizeof(char) * 200);
	char* ansfifo = malloc(sizeof(char) * 50);
	sprintf(ansfifo, "/tmp/ans%d", r->pid);

	if (strcmp(r->request, "SHUTDOWN") == 0) {
		s->shutDown = 1;

		request_writeFIFO(ansfifo, NULL, "OK");
		return;
	}

	if (strcmp(r->request, "LIST") == 0) {
		if (s->totalAccounts == 0)
			request_writeFIFO(ansfifo, NULL, "FAIL");
		else
			request_writeFIFO(ansfifo, NULL, server_getAccounts(s));
		return;
	}

	strcpy(tmp, r->request);
	tmp[4] = '\0';
	if (strcmp(tmp, "AUTH") == 0) {
		tmp = r->request + 5;
		char* tmp2 = malloc(sizeof(char) * 10);
		tmp2 = strtok(tmp, " \n\0");
		accountnr_t accnr = atoi(tmp2);
		if (server_accountAlreadyExists(s, accnr)) {
			tmp2 = strtok(NULL, " \n\0");
			if (strcmp(server_getAccountbyID(s, accnr)->pin, tmp2) == 0) {
				request_writeFIFO(ansfifo, NULL, "OK");
			} else {
				request_writeFIFO(ansfifo, NULL, "FAIL");
			}
		} else
			request_writeFIFO(ansfifo, NULL, "FAIL");
		return;
	}

	strcpy(tmp, r->request);
	tmp[7] = '\0';
	if (strcmp(tmp, "BALANCE") == 0) {
		tmp = r->request + 8;
		char* tmp2 = malloc(sizeof(char) * 15);
		tmp2 = strtok(tmp, " \n\0");
		accountnr_t accnr = atoi(tmp2);
		if (server_accountAlreadyExists(s, accnr)) {
			sprintf(tmp2, "%f", server_getAccountBalance(s, accnr));
			request_writeFIFO(ansfifo, NULL, tmp2);
		} else
			request_writeFIFO(ansfifo, NULL, "FAIL");
		return;
	}

	strcpy(tmp, r->request);
	tmp[7] = '\0';
	if (strcmp(tmp, "DEPOSIT") == 0) {
		tmp = r->request + 8;
		char* tmp2 = malloc(sizeof(char) * 15);
		tmp2 = strtok(tmp, " \n\0");
		accountnr_t accnr = atoi(tmp2);
		if (server_accountAlreadyExists(s, accnr)) {
			tmp2 = strtok(NULL, " \n\0");
			double ammount = atof(tmp2);
			server_depositAccount(s, accnr, ammount);
			request_writeFIFO(ansfifo, NULL, "OK");
		} else
			request_writeFIFO(ansfifo, NULL, "FAIL");
		return;
	}

	strcpy(tmp, r->request);
	tmp[8] = '\0';
	if (strcmp(tmp, "WITHDRAW") == 0) {
		tmp = r->request + 9;
		char* tmp2 = malloc(sizeof(char) * 15);
		tmp2 = strtok(tmp, " \n\0");
		accountnr_t accnr = atoi(tmp2);
		if (server_accountAlreadyExists(s, accnr)) {
			tmp2 = strtok(NULL, " \n\0");
			double ammount = atof(tmp2);
			if (server_withdraw(s, accnr, ammount))
				request_writeFIFO(ansfifo, NULL, "OK");
			else
				request_writeFIFO(ansfifo, NULL, "FAIL");
		} else
			request_writeFIFO(ansfifo, NULL, "FAIL");
		return;
	}

	strcpy(tmp, r->request);
	tmp[8] = '\0';
	if (strcmp(tmp, "TRANSFER") == 0) {
		tmp = r->request + 9;
		char* tmp2 = malloc(sizeof(char) * 15);
		tmp2 = strtok(tmp, " \n\0");
		accountnr_t accnr = atoi(tmp2);
		if (server_accountAlreadyExists(s, accnr)) {
			tmp2 = strtok(NULL, " \n\0");
			accountnr_t destaccnr = atoi(tmp2);
			if (server_accountAlreadyExists(s, destaccnr)) {
				tmp2 = strtok(NULL, " \n\0");
				double ammount = atof(tmp2);
				if (server_transfer(s, accnr, destaccnr, ammount))
					request_writeFIFO(ansfifo, NULL, "OK");
				else
					request_writeFIFO(ansfifo, NULL, "FAIL");
			} else
				request_writeFIFO(ansfifo, NULL, "FAIL");
		} else
			request_writeFIFO(ansfifo, NULL, "FAIL");
		return;
	}

	strcpy(tmp, r->request);
	tmp[14] = '\0';
	if (strcmp(tmp, "DELETE ACCOUNT") == 0) {
		tmp = r->request + 15;
		char* tmp2 = malloc(sizeof(char) * 15);
		tmp2 = strtok(tmp, " \n\0");
		accountnr_t accnr = atoi(tmp2);
		if (server_accountAlreadyExists(s, accnr)) {
			if (server_deleteAccount(s, accnr))
				request_writeFIFO(ansfifo, NULL, "OK");
			else
				request_writeFIFO(ansfifo, NULL, "FAIL");
		} else
			request_writeFIFO(ansfifo, NULL, "FAIL");
		return;
	}

	strcpy(tmp, r->request);
	tmp[14] = '\0';
	if (strcmp(tmp, "CREATE ACCOUNT") == 0) {
		tmp = r->request + 15;
		char* tmp2 = malloc(sizeof(char) * 15);
		tmp2 = strtok(tmp, " \n\0");
		accountnr_t accnr = atoi(tmp2);
		tmp2 = strtok(NULL, "|\n\0");
		char *user = malloc(sizeof(char) * 50);
		strcpy(user, tmp2);
		tmp2 = strtok(NULL, " \n\0");
		char *pin = malloc(sizeof(char) * 4);
		strcpy(pin, tmp2);
		tmp2 = strtok(NULL, " \n\0");
		double balance = atof(tmp2);
		if (server_createAccount(s, accnr, user, pin, balance))
			request_writeFIFO(ansfifo, NULL, "OK");
		else
			request_writeFIFO(ansfifo, NULL, "FAIL");
		return;
	}

}

int main() {
	struct Server *s = malloc(sizeof(struct Server));
	server_create(s, "/tmp/accounts.txt", "/tmp/requests");
	//server_createAccount(s, 1231, "vascoFG", "bino", 0);
	/*server_createAccount(s, 1234, "vascoFG", "bino", 0);
	 server_createAccount(s, 4321, "vascoFG", "bino", 0);*/
	server_run(s);
	return 0;
}
