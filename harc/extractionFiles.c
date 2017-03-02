#include "header.h"
#include "errorsAndWarnings.h"
/*
0- ��� ������
1 - ������ ���� ����� �������� �� ��������� ����������
2- ������ ���� ����� �������� �� �������� ���������
*/

int checkUssd(char* archiveName, const unsigned int ussd, char func)
{
	//�������� ����������
	int i = strlen(archiveName);
	for (i; ((archiveName[i] != '.') && (i + 1)); i--);
	char* tmp = NULL;
	tmp = &archiveName[i++];
	//�������� .txt �� .mi
	if (strcmp(tmp, ".txt"))
	{
		printf("������ ���� ����� �������� �� ��������� ���������� .txt\n");
		return 1;
	}
	FILE* archive = NULL;
	if ((archive = fopen(archiveName, "rb")) == NULL)
	OPEN_ERR
	//�������� ���������
	unsigned int curUssd = 0;
	if (fread(&curUssd, sizeof(unsigned int), 1, archive) != 1)
		READING_DATA_ERR
	fflush(archive);
	if (!archive) CLOSING_FILE_ERR
	else fclose(archive);
	if (curUssd != ussd)
	{
		printf("������ ���� ����� �������� �� �������� ���������\n");
		return 2;
	}

	return 0;
}