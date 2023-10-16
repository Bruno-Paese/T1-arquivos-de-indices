#include "structs.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "GerarIndices.c"
#include "GeracaoIndice2.c"
#include "indice3.c"
#include "indice4.c"

#define FILENAME "file.dat"

void printMenu()
{
	printf("Escolha uma opção:\n");
	printf("0. Sair\n");
	printf("1. Gerar Índices\n");
	printf("2. Buscar app por ID (Índice 1 - parcial)\n");
	printf("3. Buscar app por endereço de email do desenvolvedor (Índice 2 - exaustivo)\n");
	printf("4. Buscar app por avaliação (Índice 3 - índice de listas)\n");
	printf("5. Buscar app por nome (Índice 4 - AVL)\n");
}

int main()
{
	int opcao;
	Indice3 **indice3;
	char input[512];
	Avl *indice4;
	App *temp;
	printf("Gerando índice da avaliação...\n");
	indice3 = executeIndex3();
	printf("Gerando índice dos nomes...\n");
	indice4 = executeIndex4();

	do
	{
		printMenu();
		scanf("%d", &opcao);

		switch (opcao)
		{
		case 1:
			printf("Gerando Indice 1\n");
			executeIndex1();
			printf("Gerando Indice 2\n");
			executeIndex2();
			break;
		case 2:
			printf("Informe o Id do APP a buscar:\n");
			scanf("%s", input);
			binarySearchIndex(input);
			break;
		case 3:
			printf("Informe o e-mail do desenvolvedor a buscar:\n");
			scanf("%s", input);
			binarySearchIndex2(input);
			break;
		case 4:
			printf("Informe a avaliação:\n");
			scanf("%s", input);
			listarComAvaliacao(input, indice3, FILENAME);
			break;
		case 5:
			gets();
			printf("Informe o nome do App:\n");
			gets(input);
			if (strcmp("", input) == 0)
				break;
			temp = buscarPorNomeApp(input, "file.dat", indice4);
			if (temp != NULL)
			{
				printf("AppId: %s\n", temp->id);
			}
			else
			{
				printf("Nome não encontrado!");
			}
			break;
		default:
			break;
		}
	} while (opcao > 0);

	return 0;
}
