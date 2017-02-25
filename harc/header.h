#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <sys\types.h>
#include <sys/stat.h>//��� �����


/*���������(�����)*/
typedef struct {
	unsigned short checkSum;
	char lengthName;
	char name[256];
	char flags;// 0 -���� �� ������
	unsigned int size;
} Info;
char* makeNameShorter(char* name);
unsigned short crc16(unsigned char * pcBlock, unsigned short len);
void showInfo(char* archiveName);
