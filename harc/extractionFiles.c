#include "header.h"
#include "errorsAndWarnings.h"
//1 - �����������
//0 -���
int checkUssd(char* archiveName, const unsigned int ussd)
{
	int i = strlen(archiveName);
	for (i; ((archiveName[i] != '.') && (i + 1)); i--);
	char* tmp = NULL;
	tmp = &archiveName[i++];
	if (strcmp(tmp, ".mi"))
	{
		printf("������ ���� ����� �������� �� ��������� ���������� .mi\n");
		return 0;
	}
	FILE* archive = NULL;
	if ((archive = fopen(archiveName, "rb")) == NULL)
		OPEN_ERR
	//����������� ������� �����
	unsigned int endOFFile = 0;
	fseek(archive, 0, SEEK_END);
	endOFFile = ftell(archive);
	if (endOFFile == 0)
	{
		printf("����� ����:(\n");
		return 0;
	}
	fseek(archive, 0, SEEK_SET);
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
		return 0;
	}

	return 1;
}