#include <stdio.h>
#include <stdlib.h>
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

struct idIndex
{
	char id[256];
	int address;
};

typedef struct idIndex IdIndex;

int main()
{
	FILE *bin, *index;
	App tempApp;
	IdIndex tempIndex;
	int i = 0;

	bin = fopen("file.dat", "rb");
	index = fopen("fileindex.dat", "wb");

	if (!bin || !index)
	{
		printf("Arquivo não encontrado\n");
		return 1;
	}

	while (fread(&tempApp, sizeof(App), 1, bin) != 0)
	{
		if (i % 10)
		{
			i++;
			continue;
		}

		strcpy(tempIndex.id, tempApp.id);
		tempIndex.address = i;

		fwrite(&tempIndex, sizeof(IdIndex), 1, index);
		i++;
	}

	fclose(bin);
	fclose(index);

	return 0;
}