/****************************************************************************
 * Author:      - André Freitas, p.andrefreitas@gmail.com / ei10036@fe.up.pt
 * Author:      - Vasco Gonçalves, vascofg@gmail.com / ei10054@fe.up.pt
 * Copyright:   - 25/05/2012, SOPE, FEUP
 ****************************************************************************/
#include <stdio.h>

int show_menu()
{
	int opt;
	printf("ADMINISTRADOR DE MULTIBANCO\n--------------------------\n1 - Criar Nova Conta\n2 - Listar Contas\n3 - Eliminar Conta\n4 - Encerrar Servidor\n5 - Sair\n");
	scanf("%d", &opt);
	switch(opt)
	{
	case 5:
		return 0;
	}
	return 1;
}
