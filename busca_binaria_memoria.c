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

	int size, comps = 0, i = 0;
	size = sizeof(Pessoa);

	fseek(file, 0, SEEK_END);
	long totalRecords = ftell(file) / size;
	rewind(file);

	Pessoa *pessoas = (Pessoa *)malloc(totalRecords * size);
	while (1)
	{
		fread(&aux, size, 1, file);
		pessoas[i] = aux;
		i++;
		if (feof(file))
		{
			break;
		}
	}

	long left = 0;
	long right = totalRecords - 1;
	long position = -1;

	while (left <= right)
	{
		long mid = left + (right - left) / 2;

		int comparisonResult = strcmp(nome, pessoas[mid].nome);

		printf("Nome: %s - Encontrou: %d\n", pessoas[mid], comparisonResult == 0);

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
