#include "header.h"
int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "RUS");
		if (toggleSwitch(argv[1], argc, argv))
		{
			printf("[WARNING]: ���� ������� ����������� �����\n");
			return 0;
		}
		return 0;
	}