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
//��� ������� �������� �����
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
int isEmptyFile(char* fileName)
{
	FILE* file = NULL;
	if ((file = fopen(fileName, "rb")) == NULL)
	{
		return 0;
	}
	//����� �����
	long endOFFile = 0;
	fseek(file, 0, SEEK_END);
	endOFFile = ftell(file);
	if (fclose(file) == -1)
		CLOSING_FILE_ERR
	if (endOFFile == 0)
	{
		//������ ���� ����, �� �� ����
		return 1;
	}
	else return 2;
}
int addFiles(char *archiveName, char **fileNames,int *amountOfFiles, Info **ptrOnStruct)
{
	const unsigned int ussd = 111;
	struct stat info;
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
			checkUssd(archiveName, 111);
			break;
	}
	//� ����� ������ �� ������� ��������� � ������ �����
	if ((makeTmpArchieve == 0) || (makeTmpArchieve == 1))
	{
		if ((fout = fopen(archiveName, "a+b")) == NULL)
			OPEN_ERR
		for (u; u < amountOfFiles; u++)
		{
			stat(fileNames[u], &info);
			if ((fin = fopen(fileNames[u], "rb")) == NULL)
				OPEN_ERR
			if ((data = (char*)malloc(info.st_size)) == NULL)
				ALLOC_MEMORY_ERR
			//���������� ����� ���������
			(*ptrOnStruct)->checkSum = crc16(data, info.st_size);//����������� �����
			(*ptrOnStruct)->lengthName = strlen(shortNameOnly(fileNames[u])); //����� ����� �����
			strcpy((*ptrOnStruct)->name, shortNameOnly(fileNames[u]));
			(*ptrOnStruct)->flags = 0;
			(*ptrOnStruct)->size = info.st_size;
			//������ ������ � �����
			if ((fwrite(&((*ptrOnStruct)->checkSum),sizeof(unsigned short), 1, fout)) != 1)
				WRITING_DATA_ERR
			if ((fwrite(&((*ptrOnStruct)->lengthName), sizeof(char), 1, fout)) != 1)
				WRITING_DATA_ERR
			if ((fwrite(&((*ptrOnStruct)->name), strlen((*ptrOnStruct)->name), 1, fout)) != 1)
				WRITING_DATA_ERR
			if (fwrite(&((*ptrOnStruct)->flags), sizeof(char), 1, fout) != 1)
				WRITING_DATA_ERR
			if (fwrite(&((*ptrOnStruct)->size), sizeof((*ptrOnStruct)->size), 1, fout) != 1)
				WRITING_DATA_ERR
			//������ � ������ ������ �����
			if ((fread(data, (*ptrOnStruct)->size, 1, fin)) != 1)
				READING_DATA_ERR
			if ((fwrite(data, (*ptrOnStruct)->size, 1, fout)) != 1)
				WRITING_DATA_ERR
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
		if ((fin = fopen(archiveName, "rb")) == NULL)
			OPEN_ERR
		//�������� ���������� �����
		if ((tmp = fopen("output/tmp.txt", "wb")) == NULL)
			OPEN_ERR
		//������ ��������� �� ��������� �����
		if ((fwrite(&(ussd), sizeof(unsigned int), 1, tmp))!= 1)
			WRITING_DATA_ERR
		fflush(tmp);
		//����������� ������� ������
		unsigned int endOFFile = 0;
		fseek(fin, 0, SEEK_END);
		endOFFile = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		//��������� ����������
		unsigned int TMPussd = 0;
		unsigned short TMPcheckSum = 0;
		char TMPlengthName = "";
		char TMPname[256];
		char TMPflags = 0;
		unsigned int TMPsize = 0;
		//������ ���������
		if (fread(&TMPussd, sizeof(unsigned int), 1, fin) != 1)
			READING_DATA_ERR
		while ((ftell(fin)) != endOFFile)
		{
			//���� = 1 , ���� ������� ���� ���� ����
			int flag = 0;
			int y;//���������� ��� ����� for(����� ��������)
				if ((fread(&TMPcheckSum, sizeof(unsigned short), 1, fin))!=1)
					READING_DATA_ERR
				if ((fread(&TMPlengthName, sizeof(char), 1, fin))!=1)
					READING_DATA_ERR
				if ((fread(&TMPname, TMPlengthName, 1, fin))!=1)
					READING_DATA_ERR
				if ((fread(&TMPflags, sizeof(char), 1, fin))!=1)
					READING_DATA_ERR
				if ((fread(&TMPsize, sizeof(unsigned int), 1, fin))!=1)
					READING_DATA_ERR
				//TODO �� ����������� ������ ����, ���� � ����� ���������� �����
				if ((data = (char*)malloc(TMPsize)) == NULL)
					ALLOC_MEMORY_ERR
				if ((fread(data, TMPsize, 1, fin)) != 1)
					READING_DATA_ERR
				//����� ��������
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
					flag = 0;
					free(data);
					continue;
				}
				else
				{
				//���������� �� ��������� �����
				if ((fwrite(&TMPcheckSum,sizeof(unsigned short), 1, tmp)) != 1)
					WRITING_DATA_ERR
				if ((fwrite(&TMPlengthName, sizeof(char), 1, tmp)) != 1)
					WRITING_DATA_ERR
				if ((fwrite(&TMPname, TMPlengthName, 1, tmp)) != 1)
					WRITING_DATA_ERR
				if (fwrite(&TMPflags, sizeof(char), 1, tmp) != 1)
					WRITING_DATA_ERR
				if (fwrite(&TMPsize, sizeof(unsigned int), 1, tmp) != 1)
					WRITING_DATA_ERR
				//������ ������ �����
				if ((fwrite(data, TMPsize, 1, tmp)) != 1)
					WRITING_DATA_ERR
				free(data);
				}
			}
		//��������� ��� �����
			if (fclose(fin) == -1)
				CLOSING_FILE_ERR
		//������ �� ��������� ����� ���������� ��, ��� ���� � ��������
			for (int t = 0; t < amountOfFiles; t++)
			{
				stat(fileNames[t], &info);
				if ((data = (char*)malloc(info.st_size)) == NULL)
				ALLOC_MEMORY_ERR
				if ((fin = fopen(fileNames[t], "rb")) == NULL)
					OPEN_ERR
				//���������� ������
				(*ptrOnStruct)->checkSum = crc16(data, info.st_size);//����������� �����
				(*ptrOnStruct)->lengthName = strlen(shortNameOnly(fileNames[t])); //����� ����� �����
				strcpy((*ptrOnStruct)->name, shortNameOnly(fileNames[t]));
				(*ptrOnStruct)->flags = 0;
				(*ptrOnStruct)->size = info.st_size;
				//������ ������
				if ((fwrite(&((*ptrOnStruct)->checkSum),sizeof(unsigned short), 1, tmp)) != 1)
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
				if ((fread(data, (*ptrOnStruct)->size, 1, fin)) != 1)
					READING_DATA_ERR
				if ((fwrite(data, (*ptrOnStruct)->size, 1, tmp)) != 1)
					WRITING_DATA_ERR
				//�������� ������, � ������� �������� ����
				free(data);
				if (fclose(fin) == -1)
					CLOSING_FILE_ERR
			}
	
			//������� ����� ������������� temp
			if (fclose(tmp) == -1)
				CLOSING_FILE_ERR
			if (remove(archiveName) == -1)
				perror("Could not delete '123'");
			if (rename("output/tmp.txt", archiveName) == -1)
				printf("[ERROR]�� ������� ������������� ��������� �����");
	}
return 0;
}

	

	
	/*
	FILE *fin = NULL, *fout = NULL;
	const unsigned int ussd = 111;
	struct stat info;
	stat(fileName, &info);
	if ((fin = fopen(fileName, "rb")) == NULL)
		OPEN_ERR
	if ((fout = fopen(archiveName, "a+b")) ==NULL)
		OPEN_ERR
	char *data = NULL;
	if ((data = (char*)malloc(info.st_size)) == NULL)
		ALLOC_MEMORY_ERR
	
	(*ptrOnStruct)->checkSum = crc16(data, info.st_size);//����������� �����
	(*ptrOnStruct)->lengthName = strlen(fileName); //����� ����� �����
	strcpy((*ptrOnStruct)->name, fileName);
	(*ptrOnStruct)->flags = 0;
	(*ptrOnStruct)->size = info.st_size;

		//������ ��������� 
		fwrite(&(ussd), 1, sizeof(const unsigned int), fout);

		if ((fwrite(&((*ptrOnStruct)->checkSum),sizeof(unsigned short), 1, fout)) != 1)
			WRITING_DATA_ERR
		if ((fwrite(&((*ptrOnStruct)->lengthName), sizeof(char), 1, fout)) != 1)
			WRITING_DATA_ERR
		if ((fwrite(&((*ptrOnStruct)->name), strlen((*ptrOnStruct)->name), 1, fout)) != 1)
			WRITING_DATA_ERR
		if (fwrite(&((*ptrOnStruct)->flags), sizeof(char), 1, fout) != 1)
			WRITING_DATA_ERR
		if (fwrite(&((*ptrOnStruct)->size), sizeof((*ptrOnStruct)->size), 1, fout) != 1)
			WRITING_DATA_ERR
		//������ ������ �����
	if ((fread(data, (*ptrOnStruct)->size, 1, fin)) != 1)
		READING_DATA_ERR
	if ((fwrite(data, (*ptrOnStruct)->size, 1, fout)) != 1)
		WRITING_DATA_ERR
	//�������� ������, � ������� �������� ����
	free(data);

	if (!fin) CLOSING_FILE_ERR
		else fclose(fin);
	if (!fout) CLOSING_FILE_ERR 
		else fclose(fout);*/
