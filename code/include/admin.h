/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/

#ifndef ADMIN_H_
#define ADMIN_H_

#include <stdio.h>

void admin_run();

void admin_showMenu();

int admin_getOption();

int createAccount(char *user, char *pin, double initialBalance);

char* getAccounts();

int deleteAccount(int accNumber);

int shutdownServer();

#endif /* ADMIN_C_ */
