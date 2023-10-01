#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct app
{
    char name[256];
    char id[256];
    char category[100];
    char rating[5];
    int ratingCount;
    int installs;
    int minimumInstalls;
    int maximumInstalls;
    short free;
    char price[8];
    char currency[5];
    char size[10];
    char minimumAndroid[20];
    char devId[256];
    char devWebsite[256];
    char devEmail[60];
    char released[13];
    char lastUpdated[13];
    char contentRating[30];
    char privacyPolicy[10000];
    short adSupported;
    short inAppPurchases;
    short editorsChoice;
    char scrapedTime[20];
};

typedef struct app App;

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
    Indice3 *tempIndice;
    App temp;
    FILE *bin;
    bin = fopen(path, "rb");
    int i;

    if (!bin)
    {
        printf("Arquivo não encontrados\n");
        return 1;
    }

    for (i = 0; i < 51; i++)
        indice[i] = NULL;

    i = 0;

    while (!feof(bin))
    {
        fread(&temp, sizeof(App), 1, bin);
        Indice3 *index = (Indice3 *)malloc(sizeof(Indice3));
        index->address = i * sizeof(App);
        index->next = NULL;

        tempIndice = indice[obterPosicaoIndice3(temp.rating)];
        if (tempIndice != NULL)
        {
            while (tempIndice->next != NULL)
                tempIndice = tempIndice->next;
            tempIndice->next = index;
        }
        else
        {
            indice[obterPosicaoIndice3(temp.rating)] = index;
        }

        i++;
    }

    fclose(bin);

    return 0;
}

int listarComAvaliacao(char *rating, Indice3 *indice[], char *path)
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

void main()
{
    Indice3 *indice3[51]; // Contempla todas as possibilidades de avaliação
    gerarIndice3(indice3, "file2.dat");
    listarComAvaliacao("5.0", indice3, "file2.dat");
}
