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
#include "request.h"
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_BUFFER_LEN 100
#define MAX_PIN_LEN 4
accountnr_t accountNr;
char * accountPIN;
char* fifoname;

void cls(void) {
	// Credit goes to http://snipplr.com/view/15319/hacky-screen-clearing-through-printf/
	printf("\033[2J\033[0;0f");
	printf("\033[%d;%df", 0, 0);
}

int isInteger(char *buffer) {
	int unsigned i;
	char numbers[] = "1234567890";
	char *find;
	for (i = 0; i < strlen(buffer); i++) {
		find = strchr(numbers, buffer[i]);
		if (find == NULL)
			return 0;
	}
	return 1;
}

int isFloat(char *buffer) {
	int unsigned i;
	char numbers[] = "1234567890.";
	char *find;
	for (i = 0; i < strlen(buffer); i++) {
		find = strchr(numbers, buffer[i]);
		if (find == NULL)
			return 0;
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

	} while (strlen(buffer) != 4);
	strncpy(pin, buffer, 4);
	pin[4] = '\0';

	// set to the externs
	accountNr = number;
	accountPIN = malloc(sizeof(char) * 5);
	strcpy(accountPIN, pin);
	return 0;
}

int client_getOption() {
	char buffer[MAX_BUFFER_LEN];
	do {
		printf("> ");
		gets(buffer);
	} while (strlen(buffer) != 1);
	int option;
	option = atoi(buffer);
	return option;
}

void client_handleOption(int option) {
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

void client_withdraw() {
	char buffer[MAX_BUFFER_LEN];
	double ammount;
	cls();
	printf("Levantamento\n"
			"----------------\n");

	// Ammount
	do {
		printf("Montante: ");
		gets(buffer);
		ammount = atof(buffer);
	} while (ammount < 0 || !isFloat(buffer));

	struct Request r;
	char* msg = malloc(sizeof(char) * 10);
	char* wrStr = malloc(sizeof(char) * 50);
	sprintf(wrStr, "WITHDRAW %i %f", accountNr, ammount);
	request_create(&r, getpid(), "CLIENT", wrStr);
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);

	printf("%s\n", msg);
	getchar();

}

void client_deposit() {
	char buffer[MAX_BUFFER_LEN];
	double ammount;
	cls();
	printf("Depósito\n"
			"----------------\n");

	// Ammount
	do {
		printf("Montante: ");
		gets(buffer);
		ammount = atof(buffer);
	} while (ammount < 0 || !isFloat(buffer));

	struct Request r;
	char* msg = malloc(sizeof(char) * 10);
	char* wrStr = malloc(sizeof(char) * 50);
	sprintf(wrStr, "DEPOSIT %i %f", accountNr, ammount);
	request_create(&r, getpid(), "CLIENT", wrStr);
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);

	printf("%s\n", msg);
	getchar();

}

void client_transfer() {
	char buffer[MAX_BUFFER_LEN];
	//accountnr_t source;
	accountnr_t destination;
	double ammount;
	// Source Account  (THIS ONE)
	/*cls();
	 printf("Transferência\n"
	 "----------------\n");
	 do {
	 printf("Conta de Origem: ");
	 gets(buffer);
	 source = atoi(buffer);
	 } while (strlen(buffer) > 7 || strlen(buffer) == 0 || source < 1
	 || source > 9999999 || !isInteger(buffer));*/

	// Destination Account
	do {
		printf("Conta de Destino: ");
		gets(buffer);
		destination = atoi(buffer);
	} while (strlen(buffer) > 7 || strlen(buffer) == 0 || destination < 1
			|| destination > 9999999 || !isInteger(buffer)
			|| destination == accountNr);

	// Ammount
	do {
		printf("Montante: ");
		gets(buffer);
		ammount = atof(buffer);
	} while (ammount < 0 || !isFloat(buffer));

	struct Request r;
	char* msg = malloc(sizeof(char) * 10);
	char* wrStr = malloc(sizeof(char) * 50);
	sprintf(wrStr, "TRANSFER %i %i %f", accountNr, destination, ammount);
	request_create(&r, getpid(), "CLIENT", wrStr);
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);

	printf("%s\n", msg);
	getchar();

}

void client_checkBalance() {
	cls();
	printf("Ver saldo\n"
			"----------------\n");
	struct Request r;
	char* msg = malloc(sizeof(char) * 10);
	char* wrStr = malloc(sizeof(char) * 50);
	sprintf(wrStr, "BALANCE %i", accountNr);
	request_create(&r, getpid(), "CLIENT", wrStr);
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);
	printf("Saldo: %s\n", msg);
	getchar();

}
void client_run() {
	fifoname = malloc(sizeof(char) * 50);
	sprintf(fifoname, "/tmp/ans%d", getpid());
	mkfifo(fifoname, 0777);
	client_accountAuth();
	int option = 0;
	do {
		cls();
		client_showMenu();
		option = client_getOption();
		if (!client_authWithServer()) {
			printf("Autenticação falhada\n");
			getchar();
			exit(0);
		}
		client_handleOption(option);
	} while (option != 0);
}

int client_authWithServer() {
	struct Request r;
	char* msg = malloc(sizeof(char) * 10);
	char* wrStr = malloc(sizeof(char) * 50);
	sprintf(wrStr, "AUTH %i %s", accountNr, accountPIN);
	request_create(&r, getpid(), "CLIENT", wrStr);
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);
	if (strcmp(msg, "OK") == 0)
		return 1;
	return 0;
}

int main() {
	client_run();
	return 0;
}
