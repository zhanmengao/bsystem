// memory pool test.cpp : Defines the entry point for the console application.
//
#include <tchar.h>
#include "HeapMemoryPool.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <time.h>
#include <math.h>
#include <fstream>
using namespace std;
using namespace HeapMemoryPool;

namespace HeapMemoryPool
{

	int break_time = 0;
	size_t used_size = 0;
	void output_heap(max_heap* heap, ofstream& out)
	{
		for (int k = 0; k < heap->currentSize; k++)
		{
			if (k != 0 && k % 10 == 0)
			{
				out << endl;
			}
			out << heap->heap[k].chunk_size << " ";
		}
		out << endl;
	}
	void check_mem_pool(int& max_chunk_size, int& free_chunk_count, int& min_chunk_size, int& total_free_mem, MEMORYPOOL* mem_pool)
	{
		total_free_mem = 0;
		max_chunk_size = 0;
		min_chunk_size = 1024 * 1024 * 1024;
		free_chunk_count = mem_pool->heap.currentSize;

		size_t size;
		for (int k = 0; k < free_chunk_count; k++)
		{
			size = mem_pool->heap.heap[k].chunk_size * MINUNITSIZE;
			total_free_mem += size;
			if (size > max_chunk_size)
			{
				max_chunk_size = size;
			}
			if (size < min_chunk_size)
			{
				min_chunk_size = size;
			}
		}
	}
	// 申请后紧接着释放
	double test_mem_pool_perf_1(PMEMORYPOOL mem_pool, int iter, int* sizes)
	{
		cout << "*********************test_mem_pool_perf_1*********************" << endl;
		LARGE_INTEGER litmp;
		LONGLONG QPart1, QPart2;
		double t;
		double dfMinus, dfFreq;
		QueryPerformanceFrequency(&litmp);
		dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
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
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		t = dfMinus / dfFreq;// 获得对应的时间值，单位为秒   
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
		dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
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
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		t = dfMinus / dfFreq;// 获得对应的时间值，单位为秒   
		cout << "test_std_perf_1: iter = " << iter << endl;
		cout << "time: " << t << endl;
		cout << "*********************test_std_perf_1*********************" << endl << endl << endl;
		return t;
	}
	double test_mem_pool_perf_rand(PMEMORYPOOL mem_pool, int iter, int* sizes, int* instruction)
	{
		cout << "-----------------------test_mem_pool_perf_rand----------------------- " << endl;
		void** p = new void*[iter];
		if (p == NULL)
		{
			cout << "new failed" << endl;
			return -1;
		}
		cout << "test start" << endl;
		LARGE_INTEGER litmp, gftime;
		LONGLONG QPart1, QPart2, start, finish;
		double t, GetMemory_time, FreeMemory_time;
		double dfMinus, dfFreq;
		QueryPerformanceFrequency(&litmp);
		dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
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
			used_size += size;
			if (instruction[i] == 1)
			{
				FreeMemory(p[--index], mem_pool);
				used_size -= size;
			}
		}

		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		t = dfMinus / dfFreq;// 获得对应的时间值，单位为秒   
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
		dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
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
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		t = dfMinus / dfFreq;// 获得对应的时间值，单位为秒   
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
		cout << "test_std_perf_fix_size" << endl;
		LARGE_INTEGER litmp;
		LONGLONG QPart1, QPart2;
		double t;
		double dfMinus, dfFreq;
		QueryPerformanceFrequency(&litmp);
		dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;// 获得初始值
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
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2 - QPart1);
		t = dfMinus / dfFreq;// 获得对应的时间值，单位为秒   
		cout << "test_std_perf: iter = " << iter << endl;
		cout << "time: " << t << endl << endl;
		return t;
	}
	void rand_test()
	{
		ofstream out("rand_test.txt");
		used_size = 0;
		int iter = 2000;
		size_t sBufSize = 500 * 1024 * 1024;
		void*pBuf = malloc(sBufSize);
		if (pBuf == NULL)
		{
			cout << "malloc failed" << endl;
			return;
		}
		PMEMORYPOOL mem_pool = CreateMemoryPool(pBuf, sBufSize);
		int* instruction = new int[iter];
		int* sizes = new int[iter];
		if (instruction == NULL || sizes == NULL)
		{
			cout << "new failed" << endl;
			return;
		}
		srand(time(NULL));
		ofstream out_rand("rand");
		ofstream out_size("size");
		cout << "generate rand number" << endl;
		int k_count = 0;
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
		free(pBuf);
		delete[]instruction;
		delete[]sizes;
	}
	// 申请紧接着释放
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
		int iter = 1000;
		int test_count = 1024;
		int* sizes = new int[iter];
		double* ratio = new double[test_count];
		double* std_perf = new double[test_count];
		double* mem_pool_perf = new double[test_count];
		if (sizes == NULL || ratio == NULL)
		{
			cout << "new memory failed" << endl;
			return;
		}
		srand(time(NULL));
		cout << "generate rand number" << endl;
		ofstream out("rand_test_2.txt");
		ofstream out_std("std_perf_2.txt");
		ofstream out_mem_pool("mem_pool_perf_2.txt");
		for (int k = 0; k < test_count; k++)
		{
			for (int i = 0; i < iter; i++)
			{
				sizes[i] = (rand() % 16 + 1) * 64 + 1024 * k;
			}
			cout << "******************************************test " << k + 1 << " *************************************************" << endl;
			double mem_pool_t = test_mem_pool_perf_1(mem_pool, iter, sizes);
			double std_t = test_std_perf_1(iter, sizes);
			std_perf[k] = std_t;
			mem_pool_perf[k] = mem_pool_t;
			ratio[k] = std_t / mem_pool_t;
		}
		cout << "*************************** ratio (system time / mem_pool time) ***************************" << endl;
		for (int k = 0; k < test_count; k++)
		{
			out_std << std_perf[k] << ",";
			out_mem_pool << mem_pool_perf[k] << ",";
			out << ratio[k] << ",";
			if (k % 10 == 0 && k != 0)
			{
				cout << endl;
			}
			cout << ratio[k] << " ";
		}
		cout << endl;
		delete[]sizes;
		delete[]ratio;
		delete[]mem_pool_perf;
		delete[]std_perf;
		free(pBuf);
	}
}
int _tmain33(int argc, _TCHAR* argv[])
{
	//	rand_test();
	rand_test_2();

	return 0;
}
