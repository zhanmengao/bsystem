#pragma once
#ifndef _MEMORYPOOL_H
#define _MEMORYPOOL_H
#include <stdlib.h>
#define MINUNITSIZE 64
#define ADDR_ALIGN 8
#define SIZE_ALIGN MINUNITSIZE
#define MAXCHUNKSIZE 1024*1024*64
namespace HeapMemoryPool
{
	struct memory_chunk;
	typedef struct memory_block
	{
		size_t count;
		size_t start;
		memory_chunk* pmem_chunk;
	}memory_block;
	typedef struct memory_chunk
	{
		size_t chunk_size;
		memory_block* pfree_mem_addr;
	}memory_chunk;
	typedef struct max_heap
	{
		memory_chunk *heap;
		size_t maxSize;
		size_t currentSize;
	}max_heap;
	typedef struct MEMORYPOOL
	{
		void *memory;
		size_t size;
		memory_block* pmem_map;
		max_heap heap;
		size_t mem_used_size; // ��¼�ڴ�����Ѿ�������û����ڴ�Ĵ�С
		size_t free_mem_chunk_count; // ��¼ pfree_mem_chunk�����еĵ�Ԫ����
		size_t mem_map_unit_count; // 
		size_t mem_block_count; // һ�� mem_unit ��СΪ MINUNITSIZE
	}MEMORYPOOL, *PMEMORYPOOL;
	/************************************************************************/
	/* �����ڴ��
	* pBuf: �������ڴ�buffer��ʼ��ַ
	* sBufSize: �������ڴ�buffer��С
	* �������ɵ��ڴ��ָ��
	/************************************************************************/
	PMEMORYPOOL CreateMemoryPool(void* pBuf, size_t sBufSize);
	/************************************************************************/
	/* ��ʱû��
	/************************************************************************/
	void ReleaseMemoryPool(PMEMORYPOOL* ppMem);
	/************************************************************************/
	/* ���ڴ���з���ָ����С���ڴ�
	* pMem: �ڴ�� ָ��
	* sMemorySize: Ҫ������ڴ��С
	* �ɹ�ʱ���ط�����ڴ���ʼ��ַ��ʧ�ܷ���NULL
	/************************************************************************/
	void* GetMemory(size_t sMemorySize, PMEMORYPOOL pMem);
	/************************************************************************/
	/* ���ڴ�����ͷ����뵽���ڴ�
	* pMem���ڴ��ָ��
	* ptrMemoryBlock�����뵽���ڴ���ʼ��ַ
	/************************************************************************/
	void FreeMemory(void *ptrMemoryBlock, PMEMORYPOOL pMem);
#endif //_MEMORYPOOL_H
}

