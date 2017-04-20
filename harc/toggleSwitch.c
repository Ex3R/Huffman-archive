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
		unsigned short crc = CRC;
		if ((checkUssd(param[2], SIGNATURE)) != 0)
			return 0;
		FILE *archive = NULL;
		if (accessRights(param[2],READING) != 1) {
			printf("[WARNING:]����� %s �� ����� ���� �� ������\n", param[2]);
			return 1;
		}
		if ((archive = fopen(param[2], "rb")) == NULL)
			OPEN_ERR
		Info *ptrOnStruct = NULL;
		if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)
			ALLOC_MEMORY_ERR
		List *head = NULL;
		List *tmp = NULL;
		tmp = (List*)malloc(sizeof(List));
		makeListOfFiles(amount, param, &head);
		if (!head)
		{
			printf("[ERROR:] ������ ����������� ����\n");
			return 0;
		}
		UINT64 size = getSize(archive);
		if (_fseeki64_nolock(archive, SIZE_SIGNATURE, SEEK_SET) != 0)
			FSEEK_ERR
		int count = 0;
		while ((_ftelli64_nolock(archive)) != size)
		{
			if ((fread(&((ptrOnStruct)->checkSum), SIZE_CHECKSUM, 1 , archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((ptrOnStruct)->lengthName), SIZE_LENGTHNAME , 1 , archive))!= 1)
				READING_DATA_ERR
			if ((fread(&((ptrOnStruct)->name), ((ptrOnStruct)->lengthName), 1, archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((ptrOnStruct)->flags), SIZE_FLAGS, 1, archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((ptrOnStruct)->compression), SIZE_FLAGS, 1, archive)) != 1)
				READING_DATA_ERR
			if ((fread(&((ptrOnStruct)->size), SIZE_SIZE, 1, archive)) != 1)
				READING_DATA_ERR
			char * tmpFileName = (char*)malloc(((ptrOnStruct)->lengthName) + 1);
			strncpy((tmpFileName), (ptrOnStruct)->name, (ptrOnStruct)->lengthName);
			tmpFileName[(ptrOnStruct)->lengthName] = '\0';
			char flagForDeleting = 0;
			//��������� �� ������� ������
			tmp = head;
			while (tmp)
			{
				//���� ��� �������
				if (!strcmp(tmp->file, tmpFileName))
				{
					flagForDeleting = 1;
					FILE *newFile = NULL;
					char *data = NULL;
					if (fileExists(tmpFileName))
					{
						if (!accessRights(tmpFileName, WRITING))
						{
								printf("[WARNING:]����� %s �� ����� ���� �� ������ � ������\n", tmpFileName);
								return 1;
						}
						
					}
					if ((newFile = fopen(tmpFileName, "wb")) == NULL)
						CREATE_FILE_ERR
					if ((ptrOnStruct)->flags == 0) {
						if ((data = (char*)malloc(SizeOfBuf)) == NULL)
							ALLOC_MEMORY_ERR
						writeDataToFile(data, archive, newFile, &crc, ptrOnStruct->size);
						free(data);
					}
					else
					{
						decode(archive, newFile,&crc, (ptrOnStruct)->size);
					}
					//�������� crc
					if (crc != (ptrOnStruct)->checkSum)
						printf("[WARNING:] � �������� ���������� ���� ��� ��������:(\n");
					count =deleteByValue(&head, tmpFileName);
					fflush(newFile);
					if (fclose(newFile) == -1)
						CLOSING_FILE_ERR
					break;
				}
				tmp = tmp->next;
			}
			if (flagForDeleting == 0)
			{
				_fseeki64_nolock(archive, ptrOnStruct->size, SEEK_CUR);
			}
			else
			{
				flagForDeleting = 0;
			}
			//���� �� ���� ����������

			free(tmpFileName);
		}
		if (count == 1)  head = NULL;
		if (head)
		{
			printf("[WARNING:] ��������� ����� ����������� � ������\n");
			printLinkedList(head);
		}
		if (fclose(archive) == -1)
			CLOSING_FILE_ERR
		free(ptrOnStruct);
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
		char **file =NULL;
		if (integrity�heck(param[2], &ptrOnStruct, &file) == 1)
			printf("����� %s ��������, � ������ �� ����� %s\n", param[2], file);
		else printf("����� %s ���\n", param[2]);
		//TODO free???
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