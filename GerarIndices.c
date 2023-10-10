#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct idIndex
{
	char id[256];
	int address;
};

typedef struct idIndex IdIndex;

int executeIndex1()
{
	FILE *bin, *index;
	App tempApp;
	IdIndex tempIndex;
	int i = 0;

	bin = fopen("file.dat", "rb");
	index = fopen("fileIndex.dat", "wb");

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

void binarySearchIndex(char id[256])
{
	IdIndex aux, aux2;

	FILE *file = fopen("fileIndex.dat", "rb");

	int size, comps = 0;
	size = sizeof(IdIndex);

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
		fread(&aux2, size, 1, file);

		// Primeiro id do intervalo
		int comparisonResult = strcmp(id, aux.id);
		// Primeiro id fora do intervalo
		int comparisonResult2 = strcmp(id, aux2.id);

		comps++;

		if (comparisonResult >= 0 && comparisonResult2 < 0)
		{
			position = mid;
			
			FILE *dataFile = fopen("file.dat", 'rb');
			fseek(dataFile, aux.address, SEEK_SET);
			App a;
			int i = 0;
			for(i = 0; i < 10 && !feof(dataFile); i++){
				fread(&a, sizeof(a), 1, dataFile);
				if(strcmp(a.id, id) == 0){
					printf("Nome %s - Endereco: %d\n", a.name, aux.address + i * sizeof(a));
					break;
				}
			}

			if (i == 10) {
				printf("ID não encontrado\n");
			}

			break;
		}
		else if (comparisonResult < 0)
		{
			left = mid + 1;
		}
		else
		{
			right = mid - 1;
		}
	}
}