#include "header.h"
/************************��������������� �������******************************************/
/*����������� ������� �����*/
UINT64 getSize(FILE* file)
{
	UINT64 endOFFile = 0;
	_fseeki64_nolock(file, 0, SEEK_END);
	endOFFile = _ftelli64_nolock(file);
	_fseeki64_nolock(file, 0, SEEK_SET);
	return endOFFile;
}

/*
������� ����������� �����
0 - ���� ������ ��� ������ �� ��������
1- ���� ������
*/
char writeDataToFile(char *buf, FILE *fin, FILE *fout, unsigned short* crc, UINT64 amount)
{
	UINT64 temp = 0;
	UINT64 temp2 = amount;
	int bufferSize = SizeOfBuf;
	while (temp2 != 0)
	{
		if (temp2 <= bufferSize)
		{
			temp = fread(buf, 1, temp2, fin);
			temp2 -= temp;
		}
		else
		{
			temp = fread(buf, 1, bufferSize, fin);
			temp2 -= temp;
		}

		if (crc)
		{
			crc16(buf, temp, crc);
		}

		if (fwrite(buf, 1, temp, fout) != temp)
			WRITING_DATA_ERR
	}
	return 0;
}

/*
������� ��������� ��������������� ��� �����
���������� ��������� �� ������ ������
*/
char* shortNameOnly(char* name)
{
	int i = strlen(name);
	for (i; (((name[i] != '/')) && (i + 1)); i--);
	return &name[++i];
}

/*���������� ������� ������ ����� � ���������*/
double compressionRatio(double firstSize, double lastSize)
{
	double percent;
	percent = (firstSize - lastSize) / firstSize * 100;
	return percent;
}

/* 0 - not compress 1 - compress */
char compressOrNot(UINT64 size)
{
	if (size > LIMIT_FOR_COMPRESSION)
		return 1;
	else return 0;
}

/*�������� ���� �������*/
int fileExists(char * filename)
{
	return (access(filename, 0) == 0);
}
int accessRights(char *fileName, int mode)
{
	return (access(fileName, mode) == 0);
}


/*������� ����� ���������� ������ �����
0 - ���� �� ������� �������
1- ���� ������� �������, �� �� ����
2- ���� �� ����

*/
char isEmptyFile(char* fileName)
{
	FILE* file = NULL;
	if (fileExists(fileName) == 0)
	{
		return 0;
	}
	UINT64 endOFFile = getSize(file);
	if (fclose(file) == -1)
		CLOSING_FILE_ERR
		if (endOFFile == 0)
		{
			return 1;
		}
		else return 2;
}

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

//�������� �� �������
void adding(List **head, char *fileName)
{
	List *cur = NULL;
	cur = *head;
	while (cur)
	{
		if (!strcmp(cur->file, fileName)) return;
		cur = cur->next;
	}
	List *tmp = NULL;
	tmp = (List*)malloc(sizeof(List));
	(tmp->file) = (char*)malloc(strlen(fileName) + 1);
	strncpy(tmp->file, fileName, strlen(fileName));
	tmp->file[strlen(fileName)] = '\0';
	tmp->next = *head;
	*head = tmp;
	return;
}
void printLinkedList(List *head) {
	if (!head) {
		printf("Spisok pust");
	}
	while (head) {
		printf("%s\n", head->file);
		head = head->next;
	}
}
makeListOfFiles(int argc, char* argv[], List **head)
{
	//�������� � ��������, ��� ��� � ���������� ����� � ��������
	for (int i = 3; i < argc; i++)
	{
		adding(head, argv[i]);
	}
}
int  deleteByValue(List **head, char *fileName)
{
	if (*head == NULL)
		return;
	List *pred = NULL;
	List *tmp = NULL;
	tmp = (List*)malloc(sizeof(List));
	pred = (List*)malloc(sizeof(List));
	tmp = *head;
	int count = 0;
	while (tmp)
	{
		tmp = tmp->next;
		count++;
	}
	tmp = *head;
	//����� �������� ���������� ��������� � ������
	while (strcmp(tmp->file, fileName))
	{
		pred = tmp;
		tmp = tmp->next;
	}
	if (!strcmp(tmp->file, fileName))
	{
		if (tmp == (*head))
		{
			*head = tmp->next;
		}
		else pred->next = tmp->next;
		free(tmp);
	}
	return count;
}
/*����� �������� �� �������*/
/************************�������� �������*************************************************/