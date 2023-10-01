#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARTITION_SIZE 10
#define PARTITION_NAME_TEMPLATETE "./index/temp/index-file%d.dat"

/*
	Esse script cria um indice de todos os e-mails do desenvolvedor
*/

typedef struct app
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
} APP;

typedef struct textIndex
{
	char text[256];
	FILE *dataset;
	int address;
} TEXT_INDEX;

int cmpFunctionByEmail(const void *a, const void *b)
{
	if (a == NULL) {
		return 1;
	}
	return strcmp(((TEXT_INDEX *)a)->text, ((TEXT_INDEX *)b)->text);
}

void writePartitionFile(FILE *file, TEXT_INDEX *partition)
{
	fwrite(partition, sizeof(TEXT_INDEX), PARTITION_SIZE, file);
}

void joinOrderedPartitions(FILE *finalFile, char partitionTemplate[], int partitionCount, int partitionNumberStart)
{
	char filename[50];
	FILE *partitions[512];
	int i;

	if (partitionCount > 512)
	{
		for (i = 0; i < partitionCount % 512 + 1; i++)
		{
			joinOrderedPartitions(finalFile, partitionTemplate, 512, i * 512);
		}
		partitionNumberStart = 0;
		partitionCount = i;
	}

	// Abre todos os arquivos
	for (i = partitionNumberStart; i < partitionCount + partitionNumberStart; i++)
	{
		int realPartitionIndex = i % 512;
		sprintf(filename, partitionTemplate, realPartitionIndex + 1);
		partitions[realPartitionIndex] = fopen(filename, "rb");
		if (partitions[realPartitionIndex] == NULL)
		{
			break;
		}
	}
	int emptyPartitions = 0;

	int size = i;
	TEXT_INDEX *values = malloc(sizeof(TEXT_INDEX) * i);
	for (int j = 1, k = 0; j <= i; j++, k++)
	{
		if (partitions[k] != NULL && !fread(&(values[k]), sizeof(TEXT_INDEX), 1, partitions[k])) {
			partitions[k] = NULL;
			emptyPartitions++;
		}
	}

	//Insertion sorting
	while (emptyPartitions < partitionCount) {
		int smallestIndex;
		TEXT_INDEX smallest;

		for (int i = 0; i < partitionCount; i++) {
			if (partitions[i] != NULL) {
				smallest = values[i];
				smallestIndex = i;
				break;
			}
		}

		for (int i = 1; i < size; i++) {
			if (strcmp(values[i].text, smallest.text) < 0 && partitions[i] != NULL) {
				smallest = values[i];
				smallestIndex = i;
			}
		}

		fwrite(&smallest, sizeof(TEXT_INDEX), 1, finalFile);

		if (partitions[smallestIndex] != NULL && fread(&(values[smallestIndex]), sizeof(TEXT_INDEX), 1, partitions[smallestIndex]) != 1) {
			values[smallestIndex].text[0] = '\0';
			partitions[smallestIndex] = NULL;
			emptyPartitions++;
		}
	}
}

void criarIndice(FILE *file, FILE *index)
{
	APP aux;
	TEXT_INDEX *partition;

	// Particionamento para a ordenação
	partition = (TEXT_INDEX *)malloc(sizeof(TEXT_INDEX) * PARTITION_SIZE);
	int partitionCounter = 1, i = 0;
	char filename[50];

	while (fread(&aux, sizeof(APP), 1, file) != 0)
	{

		strcpy(partition[i].text, aux.devEmail);
		partition[i].dataset = file;
		partition[i].address = i + (partitionCounter-1) * PARTITION_SIZE;
		i++;
		if (i == PARTITION_SIZE)
		{
			// Ordena partição
			qsort(partition, PARTITION_SIZE, sizeof(TEXT_INDEX), cmpFunctionByEmail);

			sprintf(filename, PARTITION_NAME_TEMPLATETE, partitionCounter);
			FILE *partitionFile = fopen(filename, "wb");

			// Salva partição
			writePartitionFile(partitionFile, partition);

			fclose(partitionFile);
			free(partition);
			i = 0;
			partitionCounter++;
			partition = (TEXT_INDEX *) malloc(sizeof(TEXT_INDEX) * PARTITION_SIZE);
		}
	}

	// Junta os arquivos
	FILE * finalIndex = fopen("./finalindex2.dat", "wb");
	joinOrderedPartitions(finalIndex, PARTITION_NAME_TEMPLATETE, partitionCounter -1, 0);
	fclose(finalIndex);
}

int main()
{
	FILE *source, *index;

	source = fopen("file.dat", "rb");
	index = fopen("DevEmail.index", "rwb");

	criarIndice(source, index);

	return 0;
}