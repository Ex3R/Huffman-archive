#include "header.h";
void showInfo(char* archiveName, Info **ptrOnStruct)
{
	FILE* archive = NULL;
	if (!fileExists(archiveName))
	{
		printf("����� %s �� ����������\n", archiveName);
	}
	if ((archive = fopen(archiveName, "rb")) == NULL)
		OPEN_ERR
	UINT64 endOFFile = getSize(archive);
	if (endOFFile == 0)
	{
		printf("[WARNING:]����� %s ����:(\n",archive);
		return;
	}
	//����������� ������ ���������
	if (_fseeki64_nolock(archive, SIZE_SIGNATURE, SEEK_SET) != 0)
		FSEEK_ERR
	printf("���������� � ������ � ������ %s:\n",shortNameOnly(archiveName));
	printf("��� �����    ������ �����  ������/��������   %%������\n");
	while ((ftell(archive))!= endOFFile)
	{
		if ((fread(&((*ptrOnStruct)->checkSum), SIZE_CHECKSUM, 1 , archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((*ptrOnStruct)->lengthName), SIZE_LENGTHNAME , 1 , archive))!= 1)
				READING_DATA_ERR
			if ((fread(&((*ptrOnStruct)->name), ((*ptrOnStruct)->lengthName), 1, archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((*ptrOnStruct)->flags), SIZE_FLAGS, 1, archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((*ptrOnStruct)->compression), SIZE_FLAGS, 1, archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((*ptrOnStruct)->size), SIZE_SIZE, 1, archive)) != 1)
				READING_DATA_ERR
			if (_fseeki64_nolock(archive, (*ptrOnStruct)->size, SEEK_CUR)!=0)
			FSEEK_ERR
	for(int i=0; (i<(*ptrOnStruct)->lengthName);i++)
		printf("%c", (*ptrOnStruct)->name[i]);
	/*�������� �����*/
		if ((*ptrOnStruct)->size < 1024)
		{
			printf("%9llu%-10s", (*ptrOnStruct)->size, BYTE);
		}
		else
		{
			(*ptrOnStruct)->size /= 1024;
			if ((*ptrOnStruct)->size < 1024)
			{
				printf("%9llu%-10s", (*ptrOnStruct)->size, KB);
			}
			else
			{
				(*ptrOnStruct)->size /= 1024;
				if ((*ptrOnStruct)->size < 1024)
				{
					printf("%9llu%-10s", (*ptrOnStruct)->size, MB);
				}
				else
				{
					(*ptrOnStruct)->size /= 1024;
					printf("%9llu%-10s", (*ptrOnStruct)->size, GB);
				}
			}
		}
	if ((*ptrOnStruct)->flags == ZERO) printf("%8s", "�� ������");
		else printf("%8s", "C�����");
	printf("%15d%s", (*ptrOnStruct)->compression, "%\n");
	}
	if (fclose(archive) == -1)
		CLOSING_FILE_ERR
}