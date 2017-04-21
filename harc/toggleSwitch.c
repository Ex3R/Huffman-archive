#include "header.h"
void printHelp()
{
	printf("��������� ��������� ����� ��������� ������ ������:\n"
		"harc.exe <��������> <��� ������> <��� �����> ....\n"
		"��������:\n"
		"-a - ��������� ����(�) � �����\n"
		"-x - ������� ����(�) �� �����a\n"
		"-d - ������� ���� �� ������\n"
		"-t - ��������� ����������� ������\n"
		"P.S �� �������������� ��������� ������ �� ������ �����������\n"
	);
}
/*
Parsing parameters:
1 - error
0 - without mistakes
*/
int toggleSwitch(char* operation, int amount, char *param[])
{
	if ((amount<3) & (strcmp(param[1], "-help")))
	{
		printf("[ERROR:] ���������� ���������� �� ����� ���� ������ 2\n");
		printHelp();
		return 0;
	}
	//��������� ����(�) � �����
	if (!strcmp(param[1], "-a"))
	{
		Info *ptrOnStruct = NULL; //��������� �� ���������
		if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)//��������� ������ ��� ���������
			ALLOC_MEMORY_ERR
		/* ��� ������� ����������� �������� ������ � ������������ ������ ��������������� ������*/
		int dubl = 0;
		FILE* tryOpen = NULL;
		char **strarray = NULL;
		int strcount = 0;
		for (int i = 3; i != amount; i++)
		{
			if ((tryOpen = fopen(param[i], "rb")) == NULL)
				//��������� �� ���������
				continue;
			else
			{
				//��������� �� ������
				for (int j = 3; (j < i); j++)
				{
					//���� ������, �� ������� ��������� � ������� ���
					if (!strcmp(param[i], param[j]))
					{
						printf("[WARNING:]��������� ��� ���������� ���� %s\n", param[i]);
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
				strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
				strarray[strcount++] = strdup(param[i]);
			}
		}
		if (strcount > 0) addFiles(param[2], strarray, strcount, &ptrOnStruct);
		else printf("[ERROR:] No files for adding\n");
		//������������ ������ ��� ������ ���
		for (int i = 0; i < strcount; i++)
			free(strarray[i]);
		free(strarray);
		free(ptrOnStruct);
		return 0;
	}

	//������� ����(�) �� ������
	if (!strcmp(param[1], "-x"))
	{
		if ((checkUssd(param[2], SIGNATURE)) != 0)
			return 0;
		if (accessRights(param[2],READING) != 1) {
			printf("[WARNING:]����� %s �� ����� ���� �� ������\n", param[2]);
			return 1;
		}
		FILE *archive = NULL;
		List *head = NULL; makeListOfFiles(amount, param, &head);
		if ((archive = fopen(param[2], "rb")) == NULL)
			OPEN_ERR
		int er = extractFiles(&head,archive);
		if (fclose(archive) == -1)
			CLOSING_FILE_ERR
		return 0;
	}
	//������� ���������� � ������
	if (!strcmp(param[1], "-l"))
	{
		if (amount != 3)
		{
			printf("[WARNING:]�������� ���������� ���������� ��� ����� %s\n", "-l");
			printHelp();
			return 0;
		}
		Info *ptrOnStruct = NULL;
		if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)
			ALLOC_MEMORY_ERR
		if (!checkUssd(param[2], SIGNATURE))
		{
			showInfo(param[2],&ptrOnStruct);
		}
		free(ptrOnStruct);
		return 0;
	}
	//������� ���� �� ������
	if (!strcmp(param[1], "-d"))
	{
		if (amount != 4)
		{
			printf("[WARNING:]�������� ���������� ���������� ��� ����� %s\n", DELETE);
			printHelp();
			return 0;
		}
		if ((checkUssd(param[2],SIGNATURE))!= 0)
			return 0;
		Info *ptrOnStruct = NULL; 
		if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)
			ALLOC_MEMORY_ERR
		delete(param[2],param[3],&ptrOnStruct);
		free(ptrOnStruct);
		return 0;
	}

	//��������� ����������� ������
	if (!strcmp(param[1], "-t"))
	{
		if (amount != 3)
		{
			printf("[WARNING:]�������� ���������� ���������� ��� ����� %s\n", INTEGRITYCHECK);
			printHelp();
			return 0;
		}
		if ((checkUssd(param[2], SIGNATURE)) != 0)
			return 0;
		Info *ptrOnStruct = NULL;
		if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)
			ALLOC_MEMORY_ERR
		char **file =NULL;//��������� �� ����, � ������� ����� ������ ��� ������������ crc
		if (integrity�heck(param[2], &ptrOnStruct, &file) == 1)
			printf("����� %s ��������, � ������ �� ����� %s\n", param[2], file);
		else printf("����� %s ���\n", param[2]);
		free(file);
		file = NULL;
		free(ptrOnStruct);
		return 0;
	}
	if (!strcmp(param[1], "-help"))
	{
		printHelp();
		return 0;
	}
	return 1;
}