#include "header.h"
//��� operation - argv[1];
//0 -��� ������
//1- ���� ������
int toggleSwitch(char* operation, int amount, char *param[])
{
	if (amount<3)
	{
		printf("[ERROR]���������� ���������� �� ����� ���� ������ 3\n");
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
				strarray = (char **)realloc(strarray, (strcount + 1) * sizeof(char *));
				strarray[strcount++] = strdup(param[i]);
			}
		}
		/* print the array of strings*/
		//for (int i = 0; i < strcount; i++)
		//	printf("strarray[%d] == %s\n", i, strarray[i]);

		if (strcount > 0) addFiles(param[2], strarray, strcount, &ptrOnStruct);
		else printf("[ERROR] No files for adding");
		//free all
		for (int i = 0; i < strcount; i++)
			free(strarray[i]);
		free(strarray);
		free(ptrOnStruct);
		return 0;
	}

	//������� ����(�) �� ������
	if (!strcmp(param[1], "-x"))
	{
		printf("1\n");
		return 0;
	}

	//������� ���������� � ������
	if (!strcmp(param[1], "-l"))
	{
		if (amount != 3)
		{
			printf("�������� ���������� ���������� ��� ����� -l");
			return 1;
		}

		//�������� ��������� � ����������
		if (!checkUssd(param[2], 111))
		{
			showInfo(param[2]);
		}
		return 0;
	}
	//������� ���� �� ������
	if (!strcmp(param[1], "-d"))
	{
		printf("1\n");
		return 0;
	}

	//��������� ����������� ������
	if (!strcmp(param[1], "-t"))
	{
		printf("1\n");
		return 0;
	}

	//������� help

	if (!strcmp(param[1], "-help"))
	{
		printf("1\n");
		return 0;
	}

	return 1;
}