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
	// 可用的内存块结构体
	typedef struct memory_chunk
	{
		memory_block* pfree_mem_addr;
		memory_chunk* pre;
		memory_chunk* next;
	}memory_chunk;
	// 内存池结构体
	typedef struct MEMORYPOOL
	{
		void *memory;
		size_t size;
		memory_block* pmem_map;
		memory_chunk* pfree_mem_chunk;
		memory_chunk* pfree_mem_chunk_pool;
		size_t mem_used_size; // 记录内存池中已经分配给用户的内存的大小
		size_t mem_map_pool_count; // 记录链表单元缓冲池中剩余的单元的个数，个数为0时不能分配单元给pfree_mem_chunk
		size_t free_mem_chunk_count; // 记录 pfree_mem_chunk链表中的单元个数
		size_t mem_map_unit_count; // 
		size_t mem_block_count; // 一个 mem_unit 大小为 MINUNITSIZE
	}MEMORYPOOL, *PMEMORYPOOL;
	/************************************************************************/
	/* 生成内存池
	* pBuf: 给定的内存buffer起始地址
	* sBufSize: 给定的内存buffer大小
	* 返回生成的内存池指针
	/************************************************************************/
	PMEMORYPOOL CreateMemoryPool(void* pBuf, size_t sBufSize);
	/************************************************************************/
	/* 暂时没用
	/************************************************************************/
	void ReleaseMemoryPool(PMEMORYPOOL* ppMem);
	/************************************************************************/
	/* 从内存池中分配指定大小的内存
	* pMem: 内存池 指针
	* sMemorySize: 要分配的内存大小
	* 成功时返回分配的内存起始地址，失败返回NULL
	/************************************************************************/
	void* GetMemory(size_t sMemorySize, PMEMORYPOOL pMem);

	/************************************************************************/
	/* 从内存池中释放申请到的内存
	* pMem：内存池指针
	* ptrMemoryBlock：申请到的内存起始地址
	/************************************************************************/
	void FreeMemory(void *ptrMemoryBlock, PMEMORYPOOL pMem);

#endif //_MEMORYPOOL_H
}