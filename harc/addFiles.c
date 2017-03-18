#include "header.h";
//�� ������((((((
char* checkValidFiles(int amount, char * param[], int* strcount)
{
	int noFile = 0;
	int dubl = 0;
	FILE* tryOpen = NULL;
	char **strarray = NULL;
	(*strcount) = 0;
	for (int i = 3; i != amount; i++)
	{
		if ((tryOpen = fopen(param[i], "rb")) == NULL)
			//��������� �� ���������
			continue;
		else
		{
			//��������� �� ������
			for (int j = 3; (j<i); j++)
			{	
				//���� ������, �� ������� ��������� � ������� ���
				if (!strcmp(param[i], param[j]))
				{
					printf("[WARNING]��������� ��� ���������� ���� %s\n", param[i]);
					dubl = 1;
					break;	
				}		
			}
			if (dubl == 1)
			{
				dubl = 0;
				continue;
				//����� ���������� ���
			}
		//���������� ����� � ������
		strarray = (char **)realloc(strarray, ((*strcount) + 1) * sizeof(char *));
		strarray[(*strcount)++] = strdup(param[i]);
		}
	}
	//!�������� free ��� ���� ���������

	/* print the array of strings*/ 
	for (int i = 0; i < strcount; i++)
		printf("strarray[%d] == %s\n", i, strarray[i]);
	
	return strarray;
}
/*���������� ������� �����*/
unsigned __int64 getSize(FILE* file)
{
	unsigned __int64 endOFFile = 0;
	_fseeki64_nolock(file, 0, SEEK_END);
	endOFFile = _ftelli64_nolock(file);
	_fseeki64_nolock(file, 0, SEEK_SET);
	return endOFFile;
}
/*
 ������� ����������� ����� 
0 - ���� ������ ��� ������ �� ��������
1- ���� ������
*/
char writeDataToFile(char *buf, FILE *fin, FILE *fout, unsigned short* crc, unsigned __int64 amount)
{
	unsigned __int64 temp = 0;
	unsigned __int64 temp2 = amount;
	int bufferSize = SizeOfBuf;
	while (temp2!=0)
	{
		if (temp2 <= bufferSize)
		{
			temp = fread(buf, 1, temp2, fin);
			temp2 -= temp;
		}
		else 
		{
			temp = fread(buf, 1, bufferSize, fin);
			temp2 -= temp;
		}

		if (crc)
		{
			crc16(buf, temp, crc);
		}

		if (fwrite(buf, 1, temp, fout) != temp)
			WRITING_DATA_ERR
	}
	return 0;
}
/*
������� ��������� ��������������� ��� �����
���������� ��������� �� ������ ������
*/
char* shortNameOnly(char* name)
{
	int i = strlen(name);
	for (i; ((name[i] != '/') && (i + 1)); i--);
	return &name[++i];
}
/*������� ����� ���������� ������ �����
0 - ���� �� ������� �������
1- ���� ������� �������, �� �� ����
2- ���� �� ����

*/
char isEmptyFile(char* fileName)
{
	FILE* file = NULL;
	if ((file = fopen(fileName, "rb")) == NULL)
	{
		return 0;
	}
	//����� �����
	unsigned __int64 endOFFile = getSize(file);
	if (fclose(file) == -1)
		CLOSING_FILE_ERR
	if (endOFFile == 0)
	{
		return 1;
	}
	else return 2;
}
int addFiles(char *archiveName, char **fileNames,int *amountOfFiles, Info **ptrOnStruct)
{
	unsigned short crc = 0xFFFF;
	const unsigned int ussd = 111;
	struct _stat64 info;
	unsigned __int64 pos = 0;
	char *data = NULL;
	FILE *fin = NULL, *tmp=NULL, *fout = NULL;
	int makeTmpArchieve;//��������� ��������� ����� ��� ���
	int u = 0; 
	switch (makeTmpArchieve = isEmptyFile(archiveName))
	{
		case 0:
		case 1:
			if ((fout = fopen(archiveName, "a+b")) == NULL)
				OPEN_ERR
			if ((fwrite(&(ussd),sizeof(unsigned int), 1 ,fout))!=1)
				WRITING_DATA_ERR
			fflush(fout);
			if (fclose(fout) == -1)
				CLOSING_FILE_ERR
			makeTmpArchieve = 0;
			break;
		case 2:
			if (checkUssd(archiveName, SIGNATURE) != 0) return 1;
			break;
	}
	//� ����� ������ �� ������� ��������� � ������ �����
	if ((makeTmpArchieve == 0) || (makeTmpArchieve == 1))
	{
		if ((fout = fopen(archiveName, "rb+")) == NULL)
			OPEN_ERR
		for (u; u < amountOfFiles; u++)
		{
			_stat64(fileNames[u], &info);
			if ((fin = fopen(fileNames[u], "rb")) == NULL)
				OPEN_ERR
			if ((data = (char*)malloc(info.st_size)) == NULL)
				ALLOC_MEMORY_ERR
			//���������� ����� ���������
			
			(*ptrOnStruct)->lengthName = strlen(shortNameOnly(fileNames[u])); //����� ����� �����
			strcpy((*ptrOnStruct)->name, shortNameOnly(fileNames[u]));
			(*ptrOnStruct)->flags = 0;
			(*ptrOnStruct)->size = info.st_size;

			//������ ������ � �����
			fflush(fout);
			_fseeki64_nolock(fout, 0, SEEK_END);
			 pos = _ftelli64_nolock(fout);
			(*ptrOnStruct)->checkSum = crc;//����������� �����
			if ((fwrite(&((*ptrOnStruct)->checkSum), sizeof(unsigned short), 1, fout)) != 1)
				WRITING_DATA_ERR
			if ((fwrite(&((*ptrOnStruct)->lengthName), sizeof(char), 1, fout)) != 1)
				WRITING_DATA_ERR
			if ((fwrite(&((*ptrOnStruct)->name), (*ptrOnStruct)->lengthName, 1, fout)) != 1)
				WRITING_DATA_ERR
			if (fwrite(&((*ptrOnStruct)->flags), sizeof(char), 1, fout) != 1)
				WRITING_DATA_ERR
			if (fwrite(&((*ptrOnStruct)->size), sizeof(unsigned __int64), 1, fout) != 1)
				WRITING_DATA_ERR
			crc = 0xFFFF;
			writeDataToFile(data,fin,fout,&crc, (*ptrOnStruct)->size);
			fflush(fout);
			//����� ������� ��� ������ ����������� �����
			_fseeki64_nolock(fout, pos, SEEK_SET);
			(*ptrOnStruct)->checkSum = crc;//����������� �����
			if ((fwrite(&((*ptrOnStruct)->checkSum),sizeof(unsigned short), 1, fout)) != 1)
			WRITING_DATA_ERR
			_fseeki64_nolock(fout,0,SEEK_END);
			//�������� ������, � ������� �������� ����
			free(data);
			//�������� ���������� �����, ������� ��������
			if (fclose(fin) == -1)
				CLOSING_FILE_ERR
		}
	//�������� ������
	if (fclose(fout) == -1)
		CLOSING_FILE_ERR
	}
	//���� ����� ��������� ��������� �����
	else
	{
		if ((fin = fopen(archiveName, "rb+")) == NULL)
			OPEN_ERR
		//�������� ���������� �����
		if ((tmp = fopen("output/tmp.txt", "wb+")) == NULL)
			OPEN_ERR
		//������ ��������� �� ��������� �����
		if ((fwrite(&(ussd), sizeof(unsigned int), 1, tmp))!= 1)
			WRITING_DATA_ERR
		fflush(tmp);
		//����������� ������� ������
		unsigned __int64 endOFFile = getSize(fin);
		//��������� ����������
		unsigned int TMPussd = 0;
		unsigned short TMPcheckSum = 0;
		char TMPlengthName = 0;
		char TMPname[256];
		char TMPflags = 0;
		unsigned __int64 TMPsize = 0;
		//������ ���������
		if (fread(&TMPussd, sizeof(unsigned int), 1, fin) != 1)
			READING_DATA_ERR
		while ((_ftelli64_nolock(fin)) != endOFFile)
		{
			//���� = 1 , ���� ������� ���� ���� ����
			int flag = 0;
			int y;//���������� ��� ����� for(����� ��������)
				if ((fread(&TMPcheckSum, sizeof(unsigned short), 1, fin))!=1)
					READING_DATA_ERR
				if ((fread(&TMPlengthName, sizeof(char), 1, fin)) != 1)
					READING_DATA_ERR
				if ((fread(&TMPname, TMPlengthName, 1, fin))!=1)
					READING_DATA_ERR
				if ((fread(&TMPflags, sizeof(char), 1, fin))!=1)
					READING_DATA_ERR
				if ((fread(&TMPsize, sizeof(unsigned __int64), 1, fin))!=1)
					READING_DATA_ERR
				y = 0;
				char *TMP2name = malloc(TMPlengthName+1);
				int b = 0;
				for (b; b < TMPlengthName; b++)
				{
					TMP2name[b] = TMPname[b];
				}
				TMP2name[b] = '\0';
				for (y; y < amountOfFiles; y++)
				{
					if (!strcmp(TMP2name, shortNameOnly(fileNames[y])))
					{
						flag = 1;
						break;
					}
				}
				free(TMP2name);
				//���� ���� ����������, �� �� ������ ���������� ������ ���� � ������� ���������
				if (flag)
				{
					//����� ���������� �� ������ �����
					_fseeki64_nolock(fin, TMPsize, SEEK_CUR);
					flag = 0;
					free(data);
					continue;
				}
				else
				{
				//���������� �� ��������� �����
				fflush(tmp);
				_fseeki64_nolock(tmp, 0, SEEK_END);
				pos = _ftelli64_nolock(tmp);
				TMPcheckSum = crc;//����������� �����
				if ((fwrite(&TMPcheckSum, sizeof(unsigned short), 1, tmp)) != 1)
				WRITING_DATA_ERR
				if ((fwrite(&TMPlengthName, sizeof(char), 1, tmp)) != 1)
					WRITING_DATA_ERR
				if ((fwrite(&TMPname, TMPlengthName, 1, tmp)) != 1)
					WRITING_DATA_ERR
				if (fwrite(&TMPflags, sizeof(char), 1, tmp) != 1)
					WRITING_DATA_ERR
				if (fwrite(&TMPsize, sizeof(unsigned __int64), 1, tmp) != 1)
					WRITING_DATA_ERR
				if ((data = (char*)malloc(TMPsize)) == NULL)
					ALLOC_MEMORY_ERR
				//������ ������ �����
				crc = 0xFFFF;
				writeDataToFile(data,fin,tmp,&crc, TMPsize);
				fflush(tmp);
				//����� ������� ��� ������ ����������� �����
				_fseeki64_nolock(tmp, pos, SEEK_SET);
				TMPcheckSum = crc;//����������� �����
				if ((fwrite(&TMPcheckSum, sizeof(unsigned short), 1, tmp)) != 1)
					WRITING_DATA_ERR
				_fseeki64_nolock(tmp,0,SEEK_END);
				free(data);
				}
			}
		//��������� ��� �����
			if (fclose(fin) == -1)
				CLOSING_FILE_ERR
		//������ �� ��������� ����� ���������� ��, ��� ���� � ��������
			for (int t = 0; t < amountOfFiles; t++)
			{
				_stat64(fileNames[t], &info);
				if ((data = (char*)malloc(info.st_size)) == NULL)
				ALLOC_MEMORY_ERR
				if ((fin = fopen(fileNames[t], "rb")) == NULL)
					OPEN_ERR
				//���������� ������
				(*ptrOnStruct)->checkSum = crc;
				(*ptrOnStruct)->lengthName = strlen(shortNameOnly(fileNames[t])); //����� ����� �����
				strcpy((*ptrOnStruct)->name, shortNameOnly(fileNames[t]));
				(*ptrOnStruct)->flags = 0;
				(*ptrOnStruct)->size = info.st_size;
				//������ ������
				fflush(tmp);
				_fseeki64_nolock(tmp, 0, SEEK_END);
				pos = _ftelli64_nolock(tmp);
				if ((fwrite(&((*ptrOnStruct)->checkSum), sizeof(unsigned short), 1, tmp)) != 1)
					WRITING_DATA_ERR
				if ((fwrite(&((*ptrOnStruct)->lengthName), sizeof(char), 1, tmp)) != 1)
					WRITING_DATA_ERR
				if ((fwrite(&((*ptrOnStruct)->name), strlen((*ptrOnStruct)->name), 1, tmp)) != 1)
					WRITING_DATA_ERR
				if (fwrite(&((*ptrOnStruct)->flags), sizeof(char), 1, tmp) != 1)
					WRITING_DATA_ERR
				if (fwrite(&((*ptrOnStruct)->size), sizeof((*ptrOnStruct)->size), 1, tmp) != 1)
					WRITING_DATA_ERR
				//������ � ������ ������ �����
				crc = 0xFFFF;
				writeDataToFile(data, fin, tmp, &crc, (*ptrOnStruct)->size);
				//����� ������� ��� ������ ����������� �����
				_fseeki64_nolock(tmp, pos, SEEK_SET);
				(*ptrOnStruct)->checkSum = crc;//����������� �����
				if ((fwrite(&((*ptrOnStruct)->checkSum), sizeof(unsigned short), 1, tmp)) != 1)
					WRITING_DATA_ERR
				_fseeki64_nolock(tmp, 0, SEEK_END);
				//�������� ������, � ������� �������� ����
				free(data);
				if (fclose(fin) == -1)
					CLOSING_FILE_ERR
			}
	
			//������� ����� ������������� temp
			if (fclose(tmp) == -1)
				CLOSING_FILE_ERR
			if (remove(archiveName) == -1)
				perror("[ERROR]Could not delete %s\n",archiveName);
			if (rename("output/tmp.txt", archiveName) == -1)
				printf("[ERROR]�� ������� ������������� ��������� �����\n");
	}
return 0;
}

