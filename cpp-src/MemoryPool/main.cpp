// memory pool test.cpp : Defines the entry point for the console application.
#include <tchar.h>
#include "MemoryPool.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <fstream>
using namespace std;
int break_time = 0;
using namespace LinkMemoryPool;
// ����ڴ����ز���
void check_mem_pool(int& max_chunk_size, int& free_chunk_count, int& min_chunk_size, int& total_free_mem, MEMORYPOOL* mem_pool)
{
	memory_chunk* head = mem_pool->pfree_mem_chunk;
	memory_chunk* tmp = head;
	free_chunk_count = 0;
	total_free_mem = 0;
	max_chunk_size = 0;
	min_chunk_size = 500 * 1024 * 1024;
	if (head == NULL)
	{
		min_chunk_size = 0;
		return;
	}
	while (tmp->next != head)
	{
		free_chunk_count++;
		total_free_mem += tmp->pfree_mem_addr->count * MINUNITSIZE;
		if (tmp->pfree_mem_addr->count * MINUNITSIZE > max_chunk_size)
		{
			max_chunk_size = tmp->pfree_mem_addr->count * MINUNITSIZE;
		}
		if (tmp->pfree_mem_addr->count * MINUNITSIZE < min_chunk_size)
		{
			min_chunk_size = tmp->pfree_mem_addr->count * MINUNITSIZE;
		}
		tmp = tmp->next;
	}
	free_chunk_count++;
	total_free_mem += tmp->pfree_mem_addr->count * MINUNITSIZE;
	if (tmp->pfree_mem_addr->count * MINUNITSIZE > max_chunk_size)
	{
		max_chunk_size = tmp->pfree_mem_addr->count * MINUNITSIZE;
	}
	if (tmp->pfree_mem_addr->count * MINUNITSIZE < min_chunk_size)
	{
		min_chunk_size = tmp->pfree_mem_addr->count * MINUNITSIZE;
	}
}
// �����������ͷ�
double test_mem_pool_perf_1(PMEMORYPOOL mem_pool, int iter, int* sizes)
{
	cout << "*********************test_mem_pool_perf_1*********************" << endl;
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double t;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	for (int i = 0; i < iter; i++)
	{
		void *p = GetMemory(sizes[i], mem_pool);
		if (p == NULL)
		{
			cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << sizes[i] << " Byte" << endl;
			cout << "total memory is: " << mem_pool->size << " Byte" << endl;
			cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl;
			cout << "memory left is: " << mem_pool->size - mem_pool->mem_used_size << endl << endl;
			int max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem;
			check_mem_pool(max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem, mem_pool);
			cout << "check memory pool result:" << endl;
			cout << "free_chunk_count:	" << free_chunk_count << endl
				<< "total_free_mem:	" << total_free_mem << endl
				<< "max_chunk_size:	" << max_chunk_size << endl
				<< "min_chunk_size:	" << min_chunk_size << endl;
			break;
		}
		FreeMemory(p, mem_pool);
	}
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	t = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��   
	cout << "test_mem_pool_perf_1: iter = " << iter << endl;
	cout << "time: " << t << endl;
	cout << "*********************test_mem_pool_perf_1*********************" << endl << endl << endl;
	return t;
}
double test_std_perf_1(int iter, int* sizes)
{
	cout << "*********************test_std_perf_1*********************" << endl;
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double t;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	for (int i = 0; i < iter; i++)
	{
		void *p = malloc(sizes[i]);
		if (p == NULL)
		{
			cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << sizes[i] << " Byte" << endl;
			break;
		}
		free(p);
	}
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	t = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��   
	cout << "test_std_perf_1: iter = " << iter << endl;
	cout << "time: " << t << endl;
	cout << "*********************test_std_perf_1*********************" << endl << endl << endl;
	return t;
}
// ��������iter/2�Σ�Ȼ���ͷ����������ڴ棻���ظ�һ��
double test_mem_pool_perf_2(PMEMORYPOOL mem_pool, int iter, int size)
{
	cout << "*********************test_mem_pool_perf_2*********************" << endl;
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double t;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	void **p = new void*[iter];
	if (p == NULL)
	{
		cout << "new faild" << endl;
		return -1;
	}
	int count = 0;
	for (int i = 0; i < iter / 2; i++)
	{
		p[i] = GetMemory(size, mem_pool);
		if (p[i] == NULL)
		{
			cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << size << " Byte" << endl;
			cout << "total memory is: " << mem_pool->size << " Byte" << endl;
			cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl;
			cout << "memory left is: " << mem_pool->size - mem_pool->mem_used_size << endl << endl;
			int max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem;
			check_mem_pool(max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem, mem_pool);
			cout << "check memory pool result:" << endl;
			cout << "free_chunk_count:	" << free_chunk_count << endl
				<< "total_free_mem:	" << total_free_mem << endl
				<< "max_chunk_size:	" << max_chunk_size << endl
				<< "min_chunk_size:	" << min_chunk_size << endl;
			break;
		}
		count++;
	}
	for (int i = 0; i < count; i++)
	{
		FreeMemory(p[i], mem_pool);
	}
	count = 0;
	for (int i = 0; i < iter / 2; i++)
	{
		p[i] = GetMemory(size, mem_pool);
		if (p[i] == NULL)
		{
			cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << size << " Byte" << endl;
			cout << "total memory is: " << mem_pool->size << " Byte" << endl;
			cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl;
			cout << "memory left is: " << mem_pool->size - mem_pool->mem_used_size << endl << endl;

			int max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem;
			check_mem_pool(max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem, mem_pool);
			cout << "check memory pool result:" << endl;
			cout << "free_chunk_count:	" << free_chunk_count << endl
				<< "total_free_mem:	" << total_free_mem << endl
				<< "max_chunk_size:	" << max_chunk_size << endl
				<< "min_chunk_size:	" << min_chunk_size << endl;
			break;
		}
		count++;
	}
	for (int i = 0; i < count; i++)
	{
		if (p[i] == NULL)
		{
			cout << i << endl;
			break;
		}
		FreeMemory(p[i], mem_pool);
	}
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	t = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��   
	cout << "test_mem_pool_perf_2: iter = " << iter << endl;
	cout << "time: " << t << endl;
	delete[]p;
	cout << "*********************test_mem_pool_perf_2*********************" << endl << endl << endl;
	return t;
}
// ��������inner_iter�Σ��ͷţ��ظ�iter/inner_iter��
double test_mem_pool_perf_3(PMEMORYPOOL mem_pool, int iter, int size)
{
	cout << "*********************test_mem_pool_perf_3*********************" << endl;
	int inner_iter = 10;
	void **p = new void*[inner_iter];
	if (p == NULL)
	{
		cout << "new faild" << endl;
		return -1;
	}
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2, start, finish;
	double t;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	for (int k = 0; k < iter / inner_iter; k++)
	{
		int j = 0;
		for (j = 0; j < inner_iter; j++)
		{
			p[j] = GetMemory(size, mem_pool);
			if (p[j] == NULL)
			{
				cout << "break @ iterator = " << j << " / " << iter << ",    need memory " << size << " Byte" << endl;
				cout << "total memory is: " << mem_pool->size << " Byte" << endl;
				cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl;
				cout << "memory left is: " << mem_pool->size - mem_pool->mem_used_size << endl << endl;
				int max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem;
				check_mem_pool(max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem, mem_pool);
				cout << "check memory pool result:" << endl;
				cout << "free_chunk_count:	" << free_chunk_count << endl
					<< "total_free_mem:	" << total_free_mem << endl
					<< "max_chunk_size:	" << max_chunk_size << endl
					<< "min_chunk_size:	" << min_chunk_size << endl;
				break;
			}
		}
		for (int i = 0; i < j; i++)
		{
			FreeMemory(p[i], mem_pool);
		}
	}
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	t = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��   
	cout << "test_mem_pool_perf_3: iter = " << iter << endl;
	cout << "time: " << t << endl;
	cout << "*********************test_mem_pool_perf_3*********************" << endl << endl << endl;
	return t;
}
// ����ڴ��С������ͷŲ���
double test_mem_pool_perf_rand(PMEMORYPOOL mem_pool, int iter, int* sizes, int* instruction)
{
	cout << "-----------------------test_mem_pool_perf_rand----------------------- " << endl;
	void** p = new void*[iter];
	if (p == NULL)
	{
		cout << "new failed" << endl;
		return -1;
	}
	LARGE_INTEGER litmp, gftime;
	LONGLONG QPart1, QPart2, start, finish;
	double t, GetMemory_time, FreeMemory_time;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	int index = 0;
	int size;
	int free_tmp = 0;
	double seach_time;
	for (int i = 0; i < iter; i++)
	{
		size = sizes[i];
		p[index++] = GetMemory(size, mem_pool);
		if (p[index - 1] == NULL)
		{
			break_time++;
			cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << size << " Byte" << endl;
			cout << "total memory is: " << mem_pool->size << " Byte" << endl;
			cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl;
			cout << "memory left is: " << mem_pool->size - mem_pool->mem_used_size << endl << endl;
			int max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem;
			check_mem_pool(max_chunk_size, free_chunk_count, min_chunk_size, total_free_mem, mem_pool);
			cout << "check memory pool result:" << endl;
			cout << "free_chunk_count:	" << free_chunk_count << endl
				<< "total_free_mem:	" << total_free_mem << endl
				<< "max_chunk_size:	" << max_chunk_size << endl
				<< "min_chunk_size:	" << min_chunk_size << endl;
			break;
		}

		if (instruction[i] == 1)
		{
			FreeMemory(p[--index], mem_pool);
		}
	}
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	t = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��   
	cout << "test_mem_pool_perf_rand: iter = " << iter << endl;
	cout << "time: " << t << endl << endl;
	delete[]p;
	return t;
}
double test_std_perf(int iter, int* sizes, int* instruction)
{
	cout << "test_std_perf" << endl;
	void** p = new void*[iter];
	if (p == NULL)
	{
		cout << "new failed" << endl;
		return -1;
	}

	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double t;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
							//	cout << "test start" << endl;
	int index = 0;
	int size;
	for (int i = 0; i < iter; i++)
	{
		size = sizes[i];
		p[index++] = malloc(size);
		if (p[index - 1] == NULL)
		{
			cout << i << endl;
			break;
		}
		if (instruction[i] == 1)
		{
			free(p[--index]);
		}
	}
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	t = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��   
	cout << "test_std_perf: iter = " << iter << endl;
	cout << "time: " << t << endl << endl;
	for (int k = 0; k < index; k++)
	{
		free(p[k]);
	}
	return t;
}
double test_std_perf_fix_size(int iter, int size)
{
	cout << "******************* test_std_perf_fix_size *******************" << endl;
	LARGE_INTEGER litmp;
	LONGLONG QPart1, QPart2;
	double t;
	double dfMinus, dfFreq;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// ��ü�������ʱ��Ƶ��
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// ��ó�ʼֵ
	int index = 0;

	for (int i = 0; i < iter; i++)
	{
		void *p = malloc(size);
		if (p == NULL)
		{
			cout << i << endl;
			break;
		}
		free(p);
	}
	QueryPerformanceCounter(&litmp);
	QPart2 = litmp.QuadPart;//�����ֵֹ
	dfMinus = (double)(QPart2 - QPart1);
	t = dfMinus / dfFreq;// ��ö�Ӧ��ʱ��ֵ����λΪ��   
	cout << "test_std_perf: iter = " << iter << endl;
	cout << "time: " << t << endl;
	cout << "******************* test_std_perf_fix_size *******************" << endl << endl << endl;
	return t;
}
void test_correct_1(PMEMORYPOOL mem_pool, int iter, int size)
{
	vector<void*>vec;
	vector<void*>::iterator vec_iter;
	int i = 0;
	cout << "**************************** Get Memory Test Start ****************************" << endl << endl;
	for (i = 0; i < iter; i++)
	{
		void *p = GetMemory(size, mem_pool);
		if (p == NULL)
		{
			cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << size << " Byte" << endl;
			cout << "memory left is: " << mem_pool->size << " Byte" << endl;
			cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl << endl;
			break;
		}
		vec.push_back(p);
	}
	cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << size << " Byte" << endl;
	cout << "memory left is: " << mem_pool->size << " Byte" << endl;
	cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl << endl;
	cout << "verify memory size" << endl;
	memory_chunk* tmp = mem_pool->pfree_mem_chunk;
	int free_size = 0;
	for (int k = 0; k < mem_pool->free_mem_chunk_count; k++)
	{
		free_size += tmp->pfree_mem_addr->count * MINUNITSIZE;
		tmp = tmp->next;
	}
	cout << "memory free size is " << free_size << " Byte" << endl;
	cout << "memory used size is " << mem_pool->mem_used_size << " Byte" << endl;
	cout << "*************************** Get Memory Test Finish ***************************" << endl << endl;
	cout << "*************************** Free Memory Test Start ***************************" << endl << endl;
	int index = 0;
	for (vec_iter = vec.begin(); vec_iter != vec.end(); vec_iter++)
	{
		index++;
		FreeMemory(*vec_iter, mem_pool);
	}
	cout << "memory left is: " << mem_pool->size << " Byte" << endl;
	cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl << endl;
	cout << "*************************** Free Memory Test Finish ***************************" << endl << endl;
	cout << "********************* Get Memory Test (after Free) Start *********************" << endl << endl;
	for (i = 0; i < iter; i++)
	{
		void *p = GetMemory(size, mem_pool);
		if (p == NULL)
		{
			cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << size << " Byte" << endl;
			cout << "memory left is: " << mem_pool->size << " Byte" << endl;
			cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl << endl;
			int max_size = 0;
			memory_chunk* tmp = mem_pool->pfree_mem_chunk;
			for (int k = 0; k < mem_pool->free_mem_chunk_count; k++)
			{
				if (tmp->pfree_mem_addr->count * MINUNITSIZE > max_size)
				{
					max_size = tmp->pfree_mem_addr->count * MINUNITSIZE > max_size;
				}
			}
			cout << "max chunk size is: " << max_size << " Byte" << endl;
			break;
		}
		vec.push_back(p);
	}
	cout << "break @ iterator = " << i << " / " << iter << ",    need memory " << size << " Byte" << endl;
	cout << "memory left is: " << mem_pool->size << " Byte" << endl;
	cout << "memory used is: " << mem_pool->mem_used_size << " Byte" << endl << endl;
	cout << "verify memory size" << endl;
	tmp = mem_pool->pfree_mem_chunk;
	free_size = 0;
	for (int k = 0; k < mem_pool->free_mem_chunk_count; k++)
	{
		free_size += tmp->pfree_mem_addr->count * MINUNITSIZE;
		tmp = tmp->next;
	}
	cout << "memory free size is " << free_size << " Byte" << endl;
	cout << "memory used size is " << mem_pool->mem_used_size << " Byte" << endl;
	cout << "********************* Get Memory Test (after Free) Finish *********************" << endl << endl;
}
/************************************************************************/
/* �ڴ�����ܲ��Դ���
* �̶���С
/************************************************************************/
/*
void test_mem_pool_fix_size(PMEMORYPOOL mem_pool)
{
int iter = 200000;
int size = 512;
double t1 = test_std_perf_fix_size(iter, size);
double t2 = test_mem_pool_perf_1(mem_pool, iter, size);
double t3 = test_mem_pool_perf_2(mem_pool, iter, size);
double t4 = test_mem_pool_perf_3(mem_pool, iter, size);
cout  << endl << endl
<< "test count: " << iter << ", test size: " << size << endl
<< "test result (system time / mem_pool time) : " << endl;
cout << "test_mem_pool_perf_1:	" << t1 / t2 << endl
<< "test_mem_pool_perf_2:	" << t1 / t3 << endl
<< "test_mem_pool_perf_3:	" << t1 / t4 << endl;
}
*/
/************************************************************************/
/* �ڴ�����ܲ��Դ���

* �����С������ͷŲ���
/************************************************************************/
void rand_test()
{
	size_t sBufSize = 500 * 1024 * 1024;
	void*pBuf = malloc(sBufSize);
	if (pBuf == NULL)
	{
		cout << "malloc failed" << endl;
		return;
	}
	PMEMORYPOOL mem_pool = CreateMemoryPool(pBuf, sBufSize);
	ofstream out("rand_test.txt");
	int iter = 2000;
	int* instruction = new int[iter];
	int* sizes = new int[iter];
	if (instruction == NULL || sizes == NULL)
	{
		cout << "new memory failed" << endl;
		return;
	}
	srand(time(NULL));
	cout << "generate rand number" << endl;
	// instruction ��Ԫ��Ϊ1ʱ��ʾ��GetMemory��ִ��FreeMemory��0��ʾ��ִ��FreeMemory
	// sizes����ÿ�η����ڴ�Ĵ�С����Χ��64B~1024B
	for (int i = 0; i < iter; i++)
	{
		instruction[i] = rand() % 2;
		sizes[i] = (rand() % 16 + 1) * 64;
	}
	int test_count = 200;
	double t1, t2;
	double* ratio = new double[test_count];
	int count = 0;
	for (int k = 0; k < test_count; k++)
	{
		if (break_time != 0)
		{
			cout << "break @ " << k << " / " << test_count << endl;
			break;
		}
		count++;
		cout << "******************************************test " << k + 1 << " *************************************************" << endl;
		t1 = test_std_perf(iter, sizes, instruction);
		t2 = test_mem_pool_perf_rand(mem_pool, iter, sizes, instruction);
		cout << "total memory: " << mem_pool->size << ", memory used: " << mem_pool->mem_used_size
			<< ", memory left: " << mem_pool->size - mem_pool->mem_used_size << endl;
		ratio[k] = t1 / t2;

	}
	if (break_time == 0)
		break_time = test_count;
	break_time = count - 1;
	cout << "*************************** ratio (system time / mem_pool time) ***************************" << endl;
	for (int k = 0; k < break_time; k++)
	{
		out << ratio[k] << ",";
		if (k % 10 == 0 && k != 0)
		{
			cout << endl;
		}
		cout << ratio[k] << " ";
	}
	cout << endl;
	delete[]ratio;
	delete[]instruction;
	delete[]sizes;
	free(pBuf);
}
// ����������ͷ�
void rand_test_2()
{
	size_t sBufSize = 500 * 1024 * 1024;
	void*pBuf = malloc(sBufSize);
	if (pBuf == NULL)
	{
		cout << "malloc failed" << endl;
		return;
	}
	PMEMORYPOOL mem_pool = CreateMemoryPool(pBuf, sBufSize);
	int iter = 2000;
	int test_count = 511;
	int* sizes = new int[iter];
	double* ratio = new double[test_count];
	if (sizes == NULL || ratio == NULL)
	{
		cout << "new memory failed" << endl;
		return;
	}
	srand(time(NULL));
	cout << "generate rand number" << endl;
	ofstream out("rand_test_2.txt");
	for (int k = 0; k < test_count; k++)
	{
		for (int i = 0; i < iter; i++)
		{
			sizes[i] = (rand() % 16 + 1) * 64 + 1024 * k;
		}
		double mem_pool_t = test_mem_pool_perf_1(mem_pool, iter, sizes);
		double std_t = test_std_perf_1(iter, sizes);

		ratio[k] = std_t / mem_pool_t;
	}
	cout << "*************************** ratio (system time / mem_pool time) ***************************" << endl;
	for (int k = 0; k < test_count; k++)
	{
		out << ratio[k] << ",";
		if (k % 10 == 0 && k != 0)
		{
			cout << endl;
		}
		cout << ratio[k] << " ";
	}
	cout << endl;

	delete[]sizes;
	delete ratio;
	free(pBuf);
}
int _tmain(int argc, _TCHAR* argv[])
{
	rand_test();
	//	rand_test_2();

	return 0;
}
