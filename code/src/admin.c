/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "account.h"
#include "admin.h"
#include "request.h"
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_BUFFER_LEN 100
#define MAX_USER_LEN 20
#define MAX_PIN_LEN 4

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
	do {
		printf("> ");
		gets(buffer);
	} while (strlen(buffer) != 1);
	int option;
	option = atoi(buffer);
	return option;
}

int admin_createAccount() {
	// Variables
	char buffer[MAX_BUFFER_LEN];
	char user[MAX_USER_LEN + 1];
	char pin[MAX_PIN_LEN + 1];
	double balance;
	accountnr_t number;

	// Ask input
	cls();
	printf("Criar conta\n"
			"----------------\n");

	// Number:
	do {
		printf("Numero: ");
		gets(buffer);
		number = atoi(buffer);
	} while (strlen(buffer) > 7 || strlen(buffer) == 0 || number < 1
			|| number > 9999999 || !isInteger(buffer));

	// User
	do {
		printf("Utilizador: ");
		gets(buffer);
	} while (strlen(buffer) > 20 || strlen(buffer) < 3);
	strncpy(user, buffer, 20);
	user[20] = '\0';

	// PIN
	do {
		printf("PIN(size=4): ");
		gets(buffer);

	} while (strlen(buffer) != 4);
	strncpy(pin, buffer, 4);
	pin[4] = '\0';

	// Initial balance
	do {
		printf("Balance: ");
		gets(buffer);
		balance = atof(buffer);
	} while (balance < 0 || !isFloat(buffer));

	struct Account a;
	if (!account_create(&a, number, user, pin, balance)) {
		printf("Dados da conta invalidos!\n");
		return 1;
	} else {
		printf("echo: %s\n", account_toString(&a));

		struct Request r;
		char *msg = malloc(sizeof(char) * 10);
		char* wrStr = malloc(sizeof(char) * 128);
		sprintf(wrStr, "CREATE ACCOUNT %i %s %s %f\n", number, user, pin,
				balance);
		request_create(&r, getpid(), "ADMIN", wrStr);
		request_writeFIFO("/tmp/requests", &r, NULL);
		request_waitFIFO(fifoname, NULL, msg);
		printf("%s\n", msg);
		getchar();

		if (strcmp(msg, "OK") == 0)
			return 1;
		else
			return 0;

	}
}

void admin_listAccounts() {
	cls();
	printf("Listar contas\n"
			"----------------\n");

	struct Request r;
	//TODO OPTIMIZE THIS MALLOC
	char msg[500];
	char* wrStr = malloc(sizeof(char) * 50);
	sprintf(wrStr, "LIST");
	request_create(&r, getpid(), "ADMIN", wrStr);
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);

	if (strcmp(msg, "FAIL") == 0)
		printf("Não existem contas!\n");
	else {
		int i;
		for (i = 0; i < sizeof(msg); i++) {
			if (msg[i] == '\0')
				break;
			if (msg[i] == '>')
				msg[i] = '\n';
		}
		printf("%s", msg);
	}
	getchar();
}

int admin_deleteAccount() {
	char buffer[MAX_BUFFER_LEN];
	accountnr_t number;
	cls();
	printf("Eliminar conta\n"
			"----------------\n");
	// Number:
	do {
		printf("Numero de conta: ");
		gets(buffer);
		number = atoi(buffer);
	} while (strlen(buffer) > 7 || strlen(buffer) == 0 || number < 1
			|| number > 9999999 || !isInteger(buffer));

	struct Request r;
	char *msg = malloc(sizeof(char) * 10);
	char* wrStr = malloc(sizeof(char) * 128);
	sprintf(wrStr, "DELETE ACCOUNT %i\n", number);
	request_create(&r, getpid(), "ADMIN", wrStr);
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);
	printf("%s\n", msg);
	getchar();

	if (strcmp(msg, "OK") == 0)
		return 1;
	else
		return 0;

}

int admin_shutdownServer() {
	cls();
	printf("Encerrar Servidor\n"
			"----------------\n");
	struct Request r;
	char* msg = malloc(sizeof(char) * 10);
	request_create(&r, getpid(), "ADMIN", "SHUTDOWN");
	request_writeFIFO("/tmp/requests", &r, NULL);
	request_waitFIFO(fifoname, NULL, msg);
	printf("%s\n", msg);
	if (strcmp(msg, "OK") == 0)
		return 1;
	else
		return 0;
}

void admin_run() {
	int option = 0;
	fifoname = malloc(sizeof(char) * 50);
	sprintf(fifoname, "/tmp/ans%d", getpid());
	mkfifo(fifoname, 0777);
	do {
		cls();
		admin_showMenu();
		option = admin_getOption();
		admin_handleOption(option);
	} while (option != 0);
}

void admin_handleOption(int option) {
	switch (option) {
	case 1:
		admin_createAccount();
		break;
	case 2:
		admin_listAccounts();
		break;
	case 3:
		admin_deleteAccount();
		break;
	case 4:
		if (admin_shutdownServer())
			exit(0);
		break;
	}

}
int main() {
	admin_run();
	return 0;
}
