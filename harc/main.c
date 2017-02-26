#include "header.h"
#include "errorsAndWarnings.h"
int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "RUS");
	//������� infoAboutFiles � ������ ������ ���������
	Info *ptrOnStruct = NULL; //��������� �� ���������
	if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)//��������� ������ ��� ���������
		ALLOC_MEMORY_ERR

		//addFiles("output/output.txt", "data/input2.txt", &ptrOnStruct, 1);
		//showInfo("output/output.txt");
		//checkUssd("output/output.txt",111);

		//toggleSwitch(argv[1], argc, &argv);

	if (argc < 2)
	{
		printf("����� ������� ���������� �� ����� ���� ������ 2\n");
		return 1;
	}

	//��������� ����(�) � �����
	if (!strcmp(argv[1], "-a"))
	{
		printf("1\n");
		return 0;
	}

	//������� ����(�) �� ������
	if (!strcmp(argv[1], "-x"))
	{
		printf("1\n");
		return 0;
	}

	//������� ���������� � ������
	if (!strcmp(argv[1], "-l"))
	{
		if (argc != 3)
		{
			printf("�������� ���������� ���������� ��� ����� -l");
			return 1;
		}

		//�������� ��������� � ����������
		if (!checkUssd(argv[2],111))
		{
			showInfo(argv[2]);
		}
			return 0;
	}
	//������� ���� �� ������
	if (!strcmp(argv[1], "-d"))
	{
		printf("1\n");
		return 0;
	}

	//��������� ����������� ������
	if (!strcmp(argv[1], "-t"))
	{
		printf("1\n");
		return 0;
	}

	//������� help

	if (!strcmp(argv[1], "-help"))
	{
		printf("1\n");
		return 0;
	}
	//���� ������� ����������
	if (1)
	{
		printf("����������� �����");
		return 0;
	}

	//�������� ������ ��� ���������, ������� �� ���������� �����������
	free(ptrOnStruct);
	return 0;
	}
