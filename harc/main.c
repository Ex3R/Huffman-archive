#include "header.h"
#include "errorsAndWarnings.h"
int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "RUS");
	Info *ptrOnStruct = NULL; //��������� �� ���������
	if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)//��������� ������ ��� ���������
		ALLOC_MEMORY_ERR

		//addFiles("output/output.txt", "data/input2.txt", &ptrOnStruct, 1);
		//showInfo("output/output.txt");
	checkUssd("output/output.txt",111);
	//�������� ������ ��� ���������, ������� �� ���������� �����������
	free(ptrOnStruct);


	return 0;
	}
