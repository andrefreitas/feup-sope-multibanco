/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/

#ifndef SERVER_H_
#define SERVER_H_
#include "account.h"
#include "request.h"
#include <pthread.h>
/** A struct for defining a server that handles multibanco accounts requests.
 */
struct Server {
	char *accountsFileName; /* The name where the accounts data is saved */
	char *requestsFIFOname; /* The name of the requests fifo */
	struct Account *accounts; /* The list of accounts */
	int totalAccounts;
	int shutDown;
};

void server_create(struct Server *s, char *accountsFileName,
		char *requestFIFOname);

void server_loadAccounts(struct Server *s);

void server_run(struct Server *s);

void server_saveAccounts(struct Server *s);

int server_createAccount(struct Server *s, accountnr_t nr, char *usr, char *pin,
		double initialBalance);

int server_createAccountIncrement(struct Server *s, char *usr, char *pin,
		double initialBalance);

int server_deleteAccount(struct Server *s, accountnr_t nr);

double server_getAccountBalance(struct Server *s, accountnr_t nr);

int server_depositAccount(struct Server *s, accountnr_t nr, double amount);

int server_authAccount(struct Server *s, accountnr_t nr, char *pinGiven);

int server_transfer(struct Server *s, accountnr_t source,
		accountnr_t destination, double amount);

int server_withdraw(struct Server *s, accountnr_t nr, double amount);

void server_printAccounts(struct Server *s);

char* server_getAccounts(struct Server *s);

void server_sortAccounts(struct Server *s);

void server_addAccountRealloc(struct Server *s, struct Account *a);

int server_accountAlreadyExists(struct Server *s, accountnr_t nr);

struct Account* server_getAccountbyID(struct Server *s, accountnr_t nr);

void server_run(struct Server *s);

void server_handleRequest(struct Server *s, struct Request *r);

void * server_handleRequestThread(void * arg);
#endif /* SERVER_C_ */
