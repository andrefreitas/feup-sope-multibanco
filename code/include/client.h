/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/

#ifndef CLIENT_H_
#define CLIENT_H_
#include "account.h"
#include <stdio.h>

void client_run();

void client_showMenu();

int client_getOption();

void client_handleOption(int option);

int client_accountAuth();

void client_withdraw();

void client_deposit();

void client_transfer();

void client_checkBalance();

int client_authWithServer();

extern accountnr_t accountNr;
extern char * accountPIN;

#endif /* CLIENT_C_ */
