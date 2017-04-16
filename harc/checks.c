#include "header.h"
/*�������� ������(��������� � ����������*/
/*
0- ��� ������
1 - ������ ���� ����� �������� �� ��������� ����������
2- ������ ���� ����� �������� �� �������� ���������
*/
char checkUssd(char* archiveName)
{
	//�������� ����������
	int i = strlen(archiveName);
	for (i; ((archiveName[i] != '.') && (i + 1)); i--);
	char* tmp = NULL;
	tmp = &archiveName[i++];
	if (strcmp(tmp, EXTENTION))
	{
		printf("[ERROR:]���� %s ����� �������� �� ��������� ���������� %s\n", archiveName, EXTENTION);
		return 1;
	}
	FILE* archive = NULL;
	if ((archive = fopen(archiveName, "rb")) == NULL)
		OPEN_ERR
		//�������� ���������
		unsigned int curUssd = 0;
	if (fread(&curUssd, SIZE_SIGNATURE, 1, archive) != 1)
		READING_DATA_ERR
		if (curUssd != SIGNATURE)
		{
			printf("[ERROR:]������ ���� ����� �������� �� �������� ���������\n");
			return 2;
		}
	if (fclose(archive) == -1)
		CLOSING_FILE_ERR
		return 0;
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
	UINT64 endOFFile = getSize(file);
	if (fclose(file) == -1)
		CLOSING_FILE_ERR
		if (endOFFile == 0)
		{
			return 1;
		}
		else return 2;
}
/*
0 - not compress
1 - compress
*/
char compressOrNot(UINT64 size)
{
	if (size > LIMIT_FOR_COMPRESSION)
		return 1;
	else return 0;
}