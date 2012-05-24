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
accountnr_t accountNr;
char * accountPIN;

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
	char buffer[MAX_BUFFER_LEN];
	accountnr_t number;
	char pin[MAX_PIN_LEN + 1];
	cls();
	printf("Autenticacao\n"
	"----------------\n");
	// Number:
	do {
		printf("Numero: ");
		gets(buffer);
		number = atoi(buffer);
	} while (strlen(buffer) > 7 || strlen(buffer) == 0 || number < 1
			|| number > 9999999 || !isInteger(buffer));

	// PIN
	do {
		printf("PIN: ");
		gets(buffer);

	} while(strlen(buffer)!=4);
	strncpy(pin,buffer, 4);
	pin[4]='\0';

	// set to the externs
	accountNr=number;
	accountPIN= malloc(sizeof(char)*5);
	strcpy(accountPIN,pin);
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
	char buffer[MAX_BUFFER_LEN];
	double ammount;
	cls();
	printf("Levantamento\n"
		"----------------\n");

	// Ammount
	do{
		printf("Montante: ");
		gets(buffer);
		ammount=atof(buffer);
	}while(ammount<0 || !isFloat(buffer));

}

void client_deposit(){
	char buffer[MAX_BUFFER_LEN];
	double ammount;
	cls();
	printf("Depósito\n"
		"----------------\n");

	// Ammount
	do{
		printf("Montante: ");
		gets(buffer);
		ammount=atof(buffer);
	}while(ammount<0 || !isFloat(buffer));

}

void client_transfer(){
	char buffer[MAX_BUFFER_LEN];
	accountnr_t source,destination;
	double ammount;
	// Source Account
	cls();
	printf("Transferência\n"
			"----------------\n");
	do {
		printf("Conta de Origem: ");
		gets(buffer);
		source = atoi(buffer);
	} while (strlen(buffer) > 7 || strlen(buffer) == 0 || source < 1
			|| source> 9999999 || !isInteger(buffer));

	// Destination Account
	do {
		printf("Conta de Destino: ");
		gets(buffer);
		destination = atoi(buffer);
	} while (strlen(buffer) > 7 || strlen(buffer) == 0 || destination < 1
			|| destination > 9999999 || !isInteger(buffer) || destination==source);

	// Ammount
	do{
		printf("Montante: ");
		gets(buffer);
		ammount=atof(buffer);
	}while(ammount<0 || !isFloat(buffer));



}

void client_checkBalance(){
	cls();
	printf("Ver saldo\n"
			"----------------\n");

}
void client_run() {
	client_accountAuth();
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
