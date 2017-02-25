#include "header.h";
#include "errorsAndWarnings.h";

void showInfo(char* archiveName)
{
	//�������� �������� ��� �������� ��� ������ �� �����
	int p = 0;
	FILE* archive = NULL;
	FILE* infoAboutFiles = NULL;
	if ((archive = fopen(archiveName, "rb")) == NULL)
		OPEN_ERR
	if ((infoAboutFiles = fopen("infoAboutFiles.txt", "wb")) == NULL)
		OPEN_ERR
	//����������� ������� �����
	unsigned int endOFFile = 0;
	fseek(archive, 0, SEEK_END);
	endOFFile = ftell(archive);
	if (endOFFile == 0)
	{
		printf("����� ����:(\n");
		return;
	}

	fseek(archive, 0, SEEK_SET);

	unsigned int ussd = 0;
	unsigned short checkSum = 0;
	char lengthName = "";
	char name[256];
	char flags = 0;
	unsigned int size = 0;
	while ((ftell(archive))!= endOFFile)
	{
		//������
		if (fread(&ussd, sizeof(unsigned int), 1, archive)!=1)
			READING_DATA_ERR
		if (fread(&checkSum, sizeof(unsigned short), 1, archive)!=1)
			READING_DATA_ERR
		if (fread(&lengthName, sizeof(char), 1, archive)!=1)
			READING_DATA_ERR
		if (fread(&name, lengthName, 1, archive)!=1)
			READING_DATA_ERR
		if (fread(&flags, sizeof(char), 1, archive)!=1)
			READING_DATA_ERR
		if (fread(&size, sizeof(unsigned int), 1, archive)!=1)
			READING_DATA_ERR
		if (fseek(archive, size, SEEK_CUR))
			READING_DATA_ERR

		//������ ����������
	for(int i=0; i<lengthName;i++)
		fprintf(infoAboutFiles, "%c",name[i]);
	fprintf(infoAboutFiles, "  %u  ", size);
	if (flags == 0) fprintf(infoAboutFiles, "%s", "�� ������\n");
		else fprintf(infoAboutFiles, "%s", "C�����\n");
	}
	//�������� ������
	if (!archive) CLOSING_FILE_ERR
	else fclose(archive);
	if (!infoAboutFiles) CLOSING_FILE_ERR
	else fclose(infoAboutFiles);
}