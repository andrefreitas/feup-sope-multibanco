/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "admin.h"
#include <string.h>

#define MAX_BUFFER_LEN 100
void cls(void) {
    // Credit goes to http://snipplr.com/view/15319/hacky-screen-clearing-through-printf/
   printf("\033[2J\033[0;0f");
   printf("\033[%d;%df", 0, 0);
}

void admin_showMenu() {
	printf("ADMINISTRADOR DE MULTIBANCO\n"
			"--------------------------\n"
			"1 - Criar Nova Conta\n"
			"2 - Listar Contas\n"
			"3 - Eliminar Conta\n"
			"4 - Encerrar Servidor\n"
			"0 - Sair\n"
			"--------------------------\n");
}

int admin_getOption() {
	char buffer[MAX_BUFFER_LEN];
	do{
		printf("> ");
		gets(buffer);
	} while(strlen(buffer)!=1);
	int option;
	option=atoi(buffer);
	return option;
}

int createAccount(char *user, char *pin, double initialBalance)
{
	//SEND TO FIFO
	return 0;
}

char* getAccounts() {
	//GET ACCOUNTS FROM FIFO
	return "TEST";
}

int deleteAccount(int accNumber) {
	//SEND TO FIFO
	return 0;
}

int shutdownServer() {
	//SEND TO FIFO
	return 0;
}

void admin_run(){
	int option=0;
	do{
		cls();
		admin_showMenu();
		option=admin_getOption();
	}while(option!=0);
}

int main() {
	admin_run();
	return 0;
}
