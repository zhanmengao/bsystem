#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct Teacher2
{
	char name[64];
	int age ;
	int id;
}Teacher2;

void printTeacher21(Teacher2 *array, int num)
{
	int i = 0;

	for (i=0; i<num; i++)
	{
		printf("age:%d \n", array[i].age);
	}
}

void sortTeacer21(Teacher2 *array, int num)
{
	int		i,j;
	Teacher2 tmp;

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


// �ṹ������ 3  ������ʦ�����䣬����
void main502()
{
	int			i = 0;
	Teacher2		Array[3];  //��stack �����ڴ�
	int			num = 3;

	for (i=0; i<num; i++)
	{
		printf("\nplease enter age:");
		scanf("%d", &(Array[i].age) );
	}

	//��ӡ��ʦ����
// 	for (i=0; i<num; i++)
// 	{
// 		printf("age:%d \n", Array[i].age);
// 	}

	printTeacher21(Array, num);

	sortTeacer21(Array, num);

	printf("����֮��\n");

	printTeacher21(Array, num);
}

Teacher2 * createTeacher21(int num)
{
	Teacher2 * tmp = NULL;
	tmp = (Teacher2 *)malloc(sizeof(Teacher2) * num); //	Teacher		Array[3]
	if (tmp == NULL)
	{
		return NULL;
	}
	return tmp; //

}

void FreeTeacher21(Teacher2 *p)
{
	if (p != NULL)
	{
		free(p);
	}
}

void main233()
{
	int			i = 0;
	//Teacher		Array[3];  //��stack �����ڴ�
	int			num = 3;
	Teacher2 *pArray = NULL;
	pArray = createTeacher21(num);

	for (i=0; i<num; i++)
	{
		printf("\nplease enter age:");
		scanf("%d", & (pArray[i].age) );
	}

	//��ӡ��ʦ����
	// 	for (i=0; i<num; i++)
	// 	{
	// 		printf("age:%d \n", Array[i].age);
	// 	}

	printTeacher21(pArray, num);

	sortTeacer21(pArray, num);

	printf("����֮��\n");

	printTeacher21(pArray, num);

	FreeTeacher21(pArray);
}