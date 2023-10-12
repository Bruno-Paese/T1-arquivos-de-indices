#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PARTITION_SIZE 10000
#define PARTITION_NAME_TEMPLATETE "./index-file%d.dat"

/*
	Esse script cria um indice de todos os e-mails do desenvolvedor
*/

typedef struct textIndex
{
	char text[50];
	FILE *dataset;
	int address;
} TEXT_INDEX;

int cmpFunctionByEmail(const void *a, const void *b)
{
	if (a == NULL)
	{
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
		if (partitions[k] != NULL && !fread(&(values[k]), sizeof(TEXT_INDEX), 1, partitions[k]))
		{
			partitions[k] = NULL;
			emptyPartitions++;
		}
	}

	// Insertion sorting
	while (emptyPartitions < partitionCount)
	{
		int smallestIndex;
		TEXT_INDEX smallest;

		for (int i = 0; i < partitionCount; i++)
		{
			if (partitions[i] != NULL)
			{
				smallest = values[i];
				smallestIndex = i;
				break;
			}
		}

		for (int i = 1; i < size; i++)
		{
			if (strcmp(values[i].text, smallest.text) < 0 && partitions[i] != NULL)
			{
				smallest = values[i];
				smallestIndex = i;
			}
		}

		fwrite(&smallest, sizeof(TEXT_INDEX), 1, finalFile);

		if (partitions[smallestIndex] != NULL && fread(&(values[smallestIndex]), sizeof(TEXT_INDEX), 1, partitions[smallestIndex]) != 1)
		{
			values[smallestIndex].text[0] = '\0';
			partitions[smallestIndex] = NULL;
			emptyPartitions++;
		}
	}
}

void removeTempFiles(int fileCount)
{
	int i;
	char filename[100]; // Adjust the buffer size as needed

	for (i = 0; i < fileCount; i++)
	{
		snprintf(filename, sizeof(filename), PARTITION_NAME_TEMPLATETE, i);

		if (remove(filename) == 0)
		{
			printf("Removed file: %s\n", filename);
		}
		else
		{
			printf("Failed to remove file: %s\n", filename);
		}
	}
}

void criarIndice(FILE *file)
{
	App aux;
	TEXT_INDEX *partition;

	// Particionamento para a ordenação
	partition = (TEXT_INDEX *)malloc(sizeof(TEXT_INDEX) * PARTITION_SIZE);
	int partitionCounter = 1, i = 0;
	char filename[50];

	while (fread(&aux, sizeof(App), 1, file) != 0)
	{

		strcpy(partition[i].text, aux.devEmail);
		partition[i].dataset = file;
		partition[i].address = i + (partitionCounter - 1) * PARTITION_SIZE;
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
			partition = (TEXT_INDEX *)malloc(sizeof(TEXT_INDEX) * PARTITION_SIZE);
		}
	}

	// Junta os arquivos
	FILE *finalIndex = fopen("./fileIndex2.dat", "wb");
	joinOrderedPartitions(finalIndex, PARTITION_NAME_TEMPLATETE, partitionCounter - 1, 0);
	printf("Removendo arquivos temporários...\n");
	removeTempFiles(partitionCounter);
	fclose(finalIndex);
}

int executeIndex2()
{
	FILE *source, *index;

	source = fopen("file.dat", "rb");
	criarIndice(source);
	fclose(source);

	return 0;
}

int binarySearchIndex2(char nome[50])
{
	TEXT_INDEX aux;

	FILE *file = fopen("fileIndex2.dat", "rb");

	int size, comps = 0;
	size = sizeof(TEXT_INDEX);

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

		int comparisonResult = strcmp(nome, aux.text);

		// printf(" %s - Encontrou: %d\n", aux.text, comparisonResult == 0);

		comps++;

		if (comparisonResult == 0)
		{
			position = mid;

			FILE *dataFile = fopen("./file.dat", "rb");
			fseek(dataFile, aux.address * sizeof(App), SEEK_SET);
			App a;
			fread(&a, sizeof(a), 1, dataFile);

			printf("Nome %s - Endereco: %d\n", a.name, aux.address * sizeof(App));


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