#include "header.h";
char integrity�heck(char *archiveName, Info **ptrOnStruct,char **file)
{
	FILE *archive = NULL;
	char *data = NULL;
	unsigned short currentCheckSum;
	if (!fileExists(archiveName))
	{
		printf("����� %s �� ����������\n", archiveName);
	}
	if ((archive = fopen(archiveName, "rb")) == NULL)
		OPEN_ERR;
	UINT64 size =getSize(archive);
	if (_fseeki64_nolock(archive, SIZE_SIGNATURE, SEEK_SET) != 0)
		FSEEK_ERR
	char flagErorr = 0;
		while (((_ftelli64_nolock(archive)) != size) && !flagErorr)
		{
			if ((fread(&((*ptrOnStruct)->checkSum), SIZE_CHECKSUM, 1, archive)) != 1)
			{
				flagErorr =1;
				READING_DATA_ERR
			}
			if ((fread(&((*ptrOnStruct)->lengthName), SIZE_LENGTHNAME , 1 , archive))!= 1)
			{
				flagErorr = 1;
				READING_DATA_ERR
			}
			if ((fread(&((*ptrOnStruct)->name), ((*ptrOnStruct)->lengthName), 1, archive)) != 1)
			{
				flagErorr = 1;
				READING_DATA_ERR
			}
			//���������� ����� ��� ������ ������ �������:
			//��������� ������ � ����� ....
			(*file) = (char*)malloc(((*ptrOnStruct)->lengthName) + 1);
			strncpy((*file), (*ptrOnStruct)->name, (*ptrOnStruct)->lengthName);
			(*file)[(*ptrOnStruct)->lengthName] = '\0';

			if ((fread(&((*ptrOnStruct)->flags), SIZE_FLAGS, 1, archive)) != 1)
			{
				flagErorr = 1;
				READING_DATA_ERR
			}
			if ((fread(&((*ptrOnStruct)->compression), SIZE_FLAGS, 1, archive)) != 1)
			{
				flagErorr = 1;
				READING_DATA_ERR
			}
			if ((fread(&((*ptrOnStruct)->size), SIZE_SIZE, 1, archive)) != 1)
			{
				flagErorr = 1;
				READING_DATA_ERR
			}
			if ((*ptrOnStruct)->flags == COMPRESSED) {
				(*ptrOnStruct)->size -= sizeof(UINT64);
				//����� �� ��������� ���������� � �������������� �����
				_fseeki64_nolock(archive, sizeof(UINT64), SEEK_CUR);
			}
			if ((data = (char*)malloc(SizeOfBuf)) == NULL)
				ALLOC_MEMORY_ERR
			//��������� ����������� ����
			currentCheckSum = CRC;
			computeCRC(data, archive, &currentCheckSum, (*ptrOnStruct)->size, NOSHIFT);
			free(data);
			if (currentCheckSum != (*ptrOnStruct)->checkSum)
				flagErorr = 1;
	 }
		if (fclose(archive) == -1)
			CLOSING_FILE_ERR
	return flagErorr;
}