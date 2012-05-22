/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#include <stdio.h>
#include "account.h"
#include "admin.h"

int show_menu() {
	printf(
			"ADMINISTRADOR DE MULTIBANCO\n--------------------------\n1 - Criar Nova Conta\n2 - Listar Contas\n3 - Eliminar Conta\n4 - Encerrar Servidor\n5 - Sair\n");
	return get_option();
}

int get_option() {
	int opt, opt2;
	char usr[50], pin[4];
	double initialBalance;
	scanf("%d", &opt);
	switch (opt) {
	case 1:
		printf("Nome do utilizador: ");
		scanf("%s", usr);
		printf("PIN: ");
		scanf("%s", pin);
		printf("Saldo inicial: ");
		scanf("%f", &initialBalance);
		createAccount(usr,pin,initialBalance);
		break;
	case 2:
		printf("%s\n", getAccounts());
		break;
	case 3:
		printf("Conta a eliminar: ");
		scanf("%d", &opt2);
		deleteAccount(opt2);
		break;
	case 4:
		shutdownServer();
		break;
	case 5:
		return 0;
	}
	return 1;
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

int main() {
	do {

	}while(show_menu());
	return 0;
}
