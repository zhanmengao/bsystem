#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Teacher
{
	char name[64];
	char *alisname;
	int age ;
	int id;
}Teacher3;

void printTeacher31(Teacher3 *array, int num)
{
	int i = 0;

	for (i=0; i<num; i++)
	{
		printf("age:%d \n", array[i].age);
	}
}

void sortTeacer31(Teacher3 *array, int num)
{
	int		i,j;
	Teacher3 tmp;

	for (i=0; i<num; i++)
	{
		for (j=i+1; j<num; j++)
		{
			if (array[i].age > array[j].age)
			{
				tmp = array[i];  //=�Ų���  ��ֵ����
				array[i] = array[j];
				array[j] = tmp;
			}
		}
	}
}

Teacher3 * createTeacher31(int num)
{
	Teacher3 * tmp = NULL;
	tmp = (Teacher3 *)malloc(sizeof(Teacher3) * num); //	Teacher		Array[3]
	if (tmp == NULL)
	{
		return NULL;
	}
	return tmp; //

}

int createTeacher32(Teacher3 **pT, int num)
{
	int i = 0;
	Teacher3 * tmp = NULL;
	tmp = (Teacher3 *)malloc(sizeof(Teacher3) * num); //	Teacher		Array[3]
	if (tmp == NULL)
	{
		return -1;
	}
	memset(tmp, 0, sizeof(Teacher3) * num);

	for (i=0; i<num; i++)
	{
		tmp[i].alisname = (char *)malloc(60);
	}
	
	*pT = tmp;  //����ָ�� �β� ȥ��ӵ��޸� ʵ�� ��ֵ 
	return 0; //
}

void FreeTeacher31(Teacher3 *p, int num)
{
	int	i = 0;
	if (p == NULL)
	{
		return;
	}
	for (i=0; i<num; i++)
	{
		if (p[i].alisname != NULL)
		{
			free(p[i].alisname);
		}
	}
	free(p);
}

void main403()
{
	int			ret = 0;
	int			i = 0;
	//Teacher		Array[3];  //��stack �����ڴ�
	int			num = 3;
	Teacher3 *pArray = NULL;
	ret = createTeacher32(&pArray, num);
	if (ret != 0)
	{
		printf("func createTeacher02() er:%d \n ", ret);
		return ret;
	}

	for (i=0; i<num; i++)
	{
		printf("\nplease enter age:");
		scanf("%d", & (pArray[i].age) );

		printf("\nplease enter name:");
		scanf("%s",  pArray[i].name ); //��ָ����ָ���ڴ�ռ�copy����

		printf("\nplease enter alias:");
		scanf("%s",  pArray[i].alisname );  //��ָ����ָ���ڴ�ռ�copy����
	}

	//��ӡ��ʦ����
	// 	for (i=0; i<num; i++)
	// 	{
	// 		printf("age:%d \n", Array[i].age);
	// 	}

	printTeacher31(pArray, num);

	sortTeacer31(pArray, num);

	printf("����֮��\n");

	printTeacher31(pArray, num);

	FreeTeacher31(pArray, num);

	printf("hello...\n");
	system("pause");
	return ;
}