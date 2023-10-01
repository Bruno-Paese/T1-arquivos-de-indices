// Trabalho feito por Bruno Paese e Henrique Ambrosi

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct pessoa
{
	char nome[50];
	char genero[50];
} Pessoa;

int bianrySearch(FILE *file, char nome[50])
{
	Pessoa aux;

	int size, comps = 0;
	size = sizeof(Pessoa);

	fseek(file, 0, SEEK_END);
	long totalRecords = ftell(file) / size;

	long left = 0;
	long right = totalRecords - 1;
	long position = -1;

	while (left <= right)
	{
		long mid = left + (right - left) / 2;

		fseek(file, mid * size, SEEK_SET);
		fread(&aux, size, 1, file);

		int comparisonResult = strcmp(nome, aux.nome);

		printf("Nome: %s - Encontrou: %d\n", aux.nome, comparisonResult == 0);

		comps++;

		if (comparisonResult == 0)
		{
			position = mid;
			break;
		}
		else if (comparisonResult > 0)
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}

	return comps;
}

int main()
{
	clock_t start_time = clock();

	FILE *file = fopen("./nomes.bin", "rb");

	if (file == NULL)
	{
		perror("Error opening the file");
		return 1;
	}

	int result = bianrySearch(file, "Gildo");

	printf("Demorou %ld milisegundos e %d comparações\n", clock() - start_time, result);

	return 0;
}
