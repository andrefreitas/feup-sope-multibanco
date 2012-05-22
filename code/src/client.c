#include <stdio.h>
#include "client.h"

int show_menu() {
	printf(
			"CLIENTE DE MULTIBANCO\n-----------------------\n1 - Levantar dinheiro\n2 - Depositar dinheiro\n3 - Transfer�ncia\n4 - Consultar Saldo\n5 - Sair\n");
	return get_option();
}

int auth() {
	int opt, opt2;

	printf("Número de conta: ");
	scanf("%d", &opt);
	printf("Inserir código pin: ");
	scanf("%d", &opt2);
	//AUTH WITH SERVER
	return 0;
}

int get_option() {
	int opt, opt2, opt3;

	scanf("%d", &opt);
	switch (opt) {
	case 1:
		printf("Valor a levantar: ");
		scanf("%d", &opt2);
		withdraw(opt2);
		break;
	case 2:
		printf("Valor a depositar: ");
		scanf("%d", &opt2);
		deposit(opt2);
	case 3:
		printf("Conta de destino: ");
		scanf("%d", &opt2);
		printf("Valor a transferir: ");
		scanf("%d", &opt3);
		transfer(opt2, opt3);
	case 4:
		checkBalance();
	case 5:
		return 0;
	}
	return 1;
}

int withdraw(int ammount) {
	//WRITE TO FIFO
	//WAIT ANSWER
	return 0;
}

int deposit(int ammount) {
	//WRITE TO FIFO
	//WAIT ANSWER
	return 0;
}

int transfer(int destAcc, int ammount) {
	//WRITE TO FIFO
	//WAIT ANSWER
	return 0;
}

double checkBalance() {
	//WRITE TO FIFO
	//WAIT ANSWER
	return 0;
}

int main() {
	do {

	}while(show_menu());
	return 0;
}
