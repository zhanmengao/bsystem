#pragma once
#ifndef _MEMORYPOOL_H
#define _MEMORYPOOL_H
#include <stdlib.h>
#define MINUNITSIZE 64
#define ADDR_ALIGN 8
#define SIZE_ALIGN MINUNITSIZE

namespace LinkMemoryPool
{
	struct memory_chunk;
	typedef struct memory_block
	{
		size_t count;
		size_t start;
		memory_chunk* pmem_chunk;
	}memory_block;
	// ���õ��ڴ��ṹ��
	typedef struct memory_chunk
	{
		memory_block* pfree_mem_addr;
		memory_chunk* pre;
		memory_chunk* next;
	}memory_chunk;
	// �ڴ�ؽṹ��
	typedef struct MEMORYPOOL
	{
		void *memory;
		size_t size;
		memory_block* pmem_map;
		memory_chunk* pfree_mem_chunk;
		memory_chunk* pfree_mem_chunk_pool;
		size_t mem_used_size; // ��¼�ڴ�����Ѿ�������û����ڴ�Ĵ�С
		size_t mem_map_pool_count; // ��¼����Ԫ�������ʣ��ĵ�Ԫ�ĸ���������Ϊ0ʱ���ܷ��䵥Ԫ��pfree_mem_chunk
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