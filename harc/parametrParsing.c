#include "header.h"
void basicCheckParam(int count, char *param[])
{
	if (count < 3 || (!strcmp(param[1], "help")))
	{
		printf("��������� ���������");
		//�������� ���, ���������� �� ���������� ����� ������� �������� ���������
		return 0;
	}
}


//��� operation - argv[1];
//0 -��� ������
//1- ���� ������
int toggleSwitch(char* operation, int amountOfParam, char *param[])
{
	if (amountOfParam < 2)
	{
		printf("����� ������� ���������� �� ����� ���� ������ 2\n");
		return 1;
	}

	//��������� ����(�) � �����
	if (!strcmp(operation, "-a"))
	{
		printf("1\n");
		return 0;
	}

	//������� ����(�) �� ������
	if (!strcmp(operation, "-x"))
	{
		printf("1\n");
		return 0;
	}

	//������� ���������� � ������
	if (!strcmp(operation, "-l"))
	{
		if (amountOfParam != 3)
		{
			printf("�������� ���������� ���������� ��� ����� -l");
			return 1;
		}
		
		//�������� ��������� � ����������
		if (!checkUssd)
		return 0;
	}
	//������� ���� �� ������
	if (!strcmp(operation, "-d"))
	{
		printf("1\n");
		return 0;
	}

	//��������� ����������� ������
	if (!strcmp(operation, "-t"))
	{
		printf("1\n");
		return 0;
	}

	//������� help

	if (!strcmp(operation, "-help"))
	{
		printf("1\n");
		return 0;
	}
	return 1;
}