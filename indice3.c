#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct i3
{
	int address;
	struct i3 *next;
};

typedef struct i3 Indice3;

int obterPosicaoIndice3(char *rating)
{
	float floatRating = atof(rating);
	return (int)(floatRating * 10);
}

int gerarIndice3(Indice3 *indice[], char path[])
{
	Indice3 *header[51];
	Indice3 *tempIndice;
	App temp;
	FILE *bin;
	bin = fopen(path, "rb");
	int i, j;

	if (!bin)
	{
		printf("Arquivo não encontrados\n");
		return 1;
	}

	for (i = 0; i < 51; i++)
	{
		indice[i] = NULL;
		header[i] = NULL;
	}

	i = 0;

	while (!feof(bin))
	{
		fread(&temp, sizeof(App), 1, bin);
		Indice3 *index = (Indice3 *)malloc(sizeof(Indice3));
		index->address = i * sizeof(App);
		index->next = NULL;
		j = obterPosicaoIndice3(temp.rating);
		if (header[j] != NULL)
		{
			header[j]->next = index;
			header[j] = header[j]->next;
		}
		else
		{
			header[j] = index;
			indice[j] = index;
		}

		i++;
	}

	fclose(bin);

	return 0;
}

int listarComAvaliacao(char *rating, Indice3 *indice[], char path[])
{
	FILE *bin = fopen(path, "rb");
	Indice3 *index = indice[obterPosicaoIndice3(rating)];
	App temp;

	if (!bin)
	{
		printf("Arquivo não encontrados\n");
		return 1;
	}

	printf("Lista de Apps com avaliação %s estrelas:\n", rating);
	if (index != NULL)
	{
		do
		{
			fseek(bin, index->address, SEEK_SET);
			fread(&temp, sizeof(App), 1, bin);
			printf("%s\n", temp.name);
			index = index->next;
		} while (index != NULL);
	}

	return 0;
}

Indice3 **executeIndex3()
{

	Indice3 *indice3 = (Indice3 *)malloc(sizeof(Indice3) * 51); // Contempla todas as possibilidades de avaliação
	gerarIndice3(indice3, "file.dat");

	return indice3;
}
