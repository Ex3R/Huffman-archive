//1)������ ��� �������� �����
#define OPEN_ERR							\
{											\
	printf("������ ��� �������� �����\n");  \
	return 1;								\
}				

//2)������ ��� ��������� ������
#define ALLOC_MEMORY_ERR						\
{											    \
	printf("������ ��� ��������� ������\n");    \
	return 1;								    \
}				

//3)������ ��� ������ ������
#define WRITING_DATA_ERR					    \
{											    \
	printf("������ ��� ������ ������\n");       \
	return 1;								    \
}		

//4)������ ��� ������ ������
#define READING_DATA_ERR					    \
{											    \
	printf("������ ��� ������ ������\n");       \
	return 1;								    \
}	

//5)������ ��� ��������
#define CLOSING_FILE_ERR					    \
{											    \
	printf("������ ��� �������� �����\n");      \
	return 1;								    \
}	