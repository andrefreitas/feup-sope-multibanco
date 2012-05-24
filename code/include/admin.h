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

void admin_handleOption(int option);

int admin_createAccount();

int admin_listAccounts();

int admin_deleteAccount();

int admin_shutdownServer();

#endif /* ADMIN_C_ */
