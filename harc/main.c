#include "header.h"
#include "errorsAndWarnings.h"
int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "RUS");
	//������� infoAboutFiles � ������ ������ ���������
	//Info *ptrOnStruct = NULL; //��������� �� ���������
	//if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)//��������� ������ ��� ���������
	//	ALLOC_MEMORY_ERR

		//addFiles("output/output.txt", "data/input2.txt", &ptrOnStruct, 1);
		//showInfo("output/output.txt");
		//checkUssd("output/output.txt",111);
		//int* strcount = (int *)malloc(sizeof(int));
		
		//char** s= checkValidFiles(argc, argv, strcount);
		//int k = *strcount;

		if (toggleSwitch(argv[1], argc, argv))
		{
			printf("[ERROR] ���� ������� ����������� �����");
			return 0;
		}
	
	return 0;
	}
