#include "header.h"
#include "errorsAndWarnings.h"
int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "RUS");
	FILE *fin  = NULL, *fout = NULL;
	Info *ptrOnStruct = NULL; //��������� �� ���������
	if ((ptrOnStruct = (Info*)malloc(sizeof(Info))) == NULL)//��������� ������ ��� ���������
		ALLOC_MEMORY_ERR

		//addFiles("output.txt", "input2.txt", &ptrOnStruct, fin, fout, 1);
		showInfo("output.txt");

	//�������� ������ ��� ���������, ������� �� ���������� �����������
	free(ptrOnStruct);


	return 0;
	}
/*if ((argc != 3) || (!strcmp(argv[1], "help"))) {
printf("Description");
return 1;
}*/
