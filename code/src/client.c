/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "client.h"
#include <string.h>
#include <unistd.h>
#define MAX_BUFFER_LEN 100
#define MAX_PIN_LEN 4
void cls(void) {
	// Credit goes to http://snipplr.com/view/15319/hacky-screen-clearing-through-printf/
	printf("\033[2J\033[0;0f");
	printf("\033[%d;%df", 0, 0);
}

int isInteger(char *buffer){
	int unsigned i;
	char numbers[]="1234567890";
	char *find;
	for(i=0; i<strlen(buffer); i++){
		find=strchr(numbers,buffer[i]);
		if(find==NULL) return 0;
	}
	return 1;
}

int isFloat(char *buffer){
	int unsigned i;
	char numbers[]="1234567890.";
	char *find;
	for(i=0; i<strlen(buffer); i++){
		find=strchr(numbers,buffer[i]);
		if(find==NULL) return 0;
	}
	return 1;
}

void client_showMenu() {
	printf("CLIENTE DE MULTIBANCO\n-----------------------\n"
			"1 - Levantar dinheiro\n"
			"2 - Depositar dinheiro\n"
			"3 - Transferencia\n"
			"4 - Consultar Saldo\n"
			"0 - Sair\n");
}


int client_accountAuth() {
	int opt, opt2;

	printf("Número de conta: ");
	scanf("%d", &opt);
	printf("Inserir código pin: ");
	scanf("%d", &opt2);
	//AUTH WITH SERVER
	return 0;
}

int client_getOption(){
	char buffer[MAX_BUFFER_LEN];
	do {
		printf("> ");
		gets(buffer);
	} while (strlen(buffer) != 1);
	int option;
	option = atoi(buffer);
	return option;
}

void client_handleOption(int option){
	switch (option) {
		case 1:
			client_withdraw();
			break;
		case 2:
			client_deposit();
			break;
		case 3:
			client_transfer();
			break;
		case 4:
			client_checkBalance();
			break;
		}
}

void client_withdraw(){

}

void client_deposit(){

}

void client_transfer(){

}

void client_checkBalance(){

}
void client_run() {
	int option = 0;
	do {
		cls();
		client_showMenu();
		option = client_getOption();
		client_handleOption(option);
	} while (option != 0);
}

int main() {
	client_run();
	return 0;
}
