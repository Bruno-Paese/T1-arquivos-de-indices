#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LIMIT 1000000000000000

/*
	Código para gerar arquivo binário com
	tamanho fixo e já definido em uma struct
*/

struct app
{
	char name[256];
	char id[256];
	char rating[5];
	char devEmail[60];
};

typedef struct app App;

char *sepToken(char linha[], int *i)
{
	int j = 0, k = 0;

	(*i)++; // Pula primeira aspas
	while (linha[*i] && (linha[*i] != '"' || (linha[*i + 1] != ',' && linha[*i + 1] != '\0')))
	{		 // Busca final da palavra
		j++; // Determina tamanho da palavra
		(*i)++;
	}

	char *str = (char *)malloc(sizeof(char) * (j + 1));

	k = j;
	for (k = j; k > 0; k--)
		str[j - k] = linha[*i - k]; // Faz o str copy mas respeitando o tamanho da primeira string

	str[j] = '\0'; // Substitui ultima aspas por caracter de final de linha
	*i += 2;	   // Pula para inicio da proxima string
	return str;
}

int main()
{
	FILE *csv, *bin;
	App tempApp;
	char linha[1000];
	char *campo = NULL;
	int i, c = 0;

	// csv = fopen("file.csv", "r");
	csv = fopen("Google-Playstore-Ordered.csv", "r");
	bin = fopen("file.dat", "wb");

	if (!csv || !bin)
	{
		printf("Arquivos não encontrados\n");
		return 1;
	}

	// Pula cabeçalho
	fgets(linha, 1000, csv);

	while (fgets(linha, 1000, csv) != NULL && c < LIMIT)
	{
		linha[strlen(linha) - 1] = '\0';

		i = 0;

		// Desconsidera linhas finais que são apenas quebras de linha
		// Desconsidera linhas que tenham interrogação (caracteres chineses) e que tenham o campo nome vazio
		if (strlen(linha) < 2 || linha[1] == '?' || linha[1] == '"')
			continue;

		campo = sepToken(linha, &i);
		strcpy(tempApp.name, (campo ? campo : ""));
		free(campo);
		campo = sepToken(linha, &i);
		strcpy(tempApp.id, (campo ? campo : ""));
		free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.category, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		strcpy(tempApp.rating, (campo ? campo : ""));
		free(campo);
		campo = sepToken(linha, &i);
		// tempApp.ratingCount = (campo ? atoi(campo) : 0);
		// free(campo);
		campo = sepToken(linha, &i);
		// tempApp.installs = (campo ? atoi(campo) : 0);
		// free(campo);
		campo = sepToken(linha, &i);
		// tempApp.minimumInstalls = (campo ? atoi(campo) : 0);
		// free(campo);
		campo = sepToken(linha, &i);
		// tempApp.maximumInstalls = (campo ? atoi(campo) : 0);
		// free(campo);
		campo = sepToken(linha, &i);
		// tempApp.free = strcmp(campo, "True") == 0;
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.price, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.currency, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.size, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.minimumAndroid, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.devId, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.devWebsite, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		strcpy(tempApp.devEmail, (campo ? campo : ""));
		free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.released, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.lastUpdated, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.contentRating, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.privacyPolicy, (campo ? campo : ""));
		// free(campo);
		campo = sepToken(linha, &i);
		// tempApp.adSupported = strcmp(campo, "True") == 0;
		// free(campo);
		campo = sepToken(linha, &i);
		// tempApp.inAppPurchases = strcmp(campo, "True") == 0;
		// free(campo);
		campo = sepToken(linha, &i);
		// tempApp.editorsChoice = strcmp(campo, "True") == 0;
		// free(campo);
		campo = sepToken(linha, &i);
		// strcpy(tempApp.scrapedTime, (campo ? campo : ""));
		free(campo);

		fwrite(&tempApp, sizeof(App), 1, bin);
		c++;
	}

	fclose(csv);
	fclose(bin);

	return 0;
}
