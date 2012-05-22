/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/

#ifndef CLIENT_H_
#define CLIENT_H_

#include <stdio.h>

int show_menu();

int auth();

int get_option();

int withdraw(int ammount);

int deposit(int ammount);

int transfer(int destAcc, int ammount);

double checkBalance();

#endif /* CLIENT_C_ */
