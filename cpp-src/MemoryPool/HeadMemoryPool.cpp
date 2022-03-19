#include <memory.h>
#include "HeapMemoryPool.h"
/************************************************************************/
/* ����Ϊ�󶥶Ѳ���*/
namespace HeapMemoryPool
{
	void init_max_heap(size_t max_heap_size, memory_chunk* heap_arr, max_heap* heap)
	{
		heap->maxSize = max_heap_size;
		heap->currentSize = 0;
		heap->heap = heap_arr;
	}
	bool is_heap_empty(max_heap* heap)
	{
		return heap->currentSize == 0;
	}
	bool is_heap_full(max_heap* heap)
	{
		return heap->currentSize == heap->maxSize;
	}
	memory_chunk* filter_up(max_heap* heap, size_t start)
	{
		size_t i = start;
		size_t j = (i - 1) / 2;
		memory_chunk temp = heap->heap[i];
		while (i > 0)
		{
			if (temp.chunk_size <= heap->heap[j].chunk_size)
				break;
			else
			{
				heap->heap[i] = heap->heap[j];
				heap->heap[j].pfree_mem_addr->pmem_chunk = &(heap->heap[i]);
				i = j;
				j = (i - 1) / 2;
			}
		}
		heap->heap[i] = temp;
		return &(heap->heap[i]);
	}
	memory_chunk* filter_down(max_heap* heap, size_t start, size_t endOfHeap)
	{
		size_t i = start;
		size_t j = i * 2 + 1;
		memory_chunk temp = heap->heap[i];
		while (j <= endOfHeap)
		{
			if (j < endOfHeap && heap->heap[j].chunk_size < heap->heap[j + 1].chunk_size)
				j++;
			if (temp.chunk_size > heap->heap[j].chunk_size)
				break;
			else
			{
				heap->heap[i] = heap->heap[j];
				heap->heap[j].pfree_mem_addr->pmem_chunk = &(heap->heap[i]);
				i = j;
				j = 2 * i + 1;
			}
		}
		heap->heap[i] = temp;
		return &(heap->heap[i]);
	}
	memory_chunk* insert_heap(memory_chunk& chunk, max_heap* heap)
	{
		if (is_heap_full(heap))
		{
			return NULL;
		}
		heap->heap[heap->currentSize] = chunk;
		memory_chunk* ret = filter_up(heap, heap->currentSize);
		heap->currentSize++;
		return ret;
	}
	bool get_max(memory_chunk*& chunk, max_heap* heap)
	{
		if (is_heap_empty(heap))
		{
			return false;
		}
		chunk = heap->heap;
		return true;
	}
	bool remove_max(max_heap* heap)
	{
		if (is_heap_empty(heap))
		{
			return false;
		}
		heap->heap[0] = heap->heap[heap->currentSize - 1];
		heap->currentSize--;
		if (heap->currentSize > 0)
		{
			filter_down(heap, 0, heap->currentSize - 1);
		}
		return true;
	}
	void remove_element(memory_chunk* chunk, max_heap* heap)
	{
		// ɾ��ĳ����maxԪ����������ɣ�
		// 1. ����Ԫ��size������󣨴���max element����Ȼ�����������Ѷ���
		// 2. ɾ���Ѷ�Ԫ��
		size_t index = chunk - heap->heap;
		chunk->chunk_size = MAXCHUNKSIZE;
		filter_up(heap, index);
		remove_max(heap);
	}
	memory_chunk* increase_element_value(memory_chunk* chunk, max_heap* heap, size_t increase_value)
	{
		size_t index = chunk - heap->heap;
		chunk->chunk_size += increase_value;
		return filter_up(heap, index);
	}
	memory_chunk* decrease_element_value(memory_chunk* chunk, max_heap* heap, size_t decrease_value)
	{
		size_t index = chunk - heap->heap;
		chunk->chunk_size -= decrease_value;
		return filter_down(heap, index, heap->currentSize - 1);
	}
	/************************************************************************/
	/* �ڴ����ʼ��ַ���뵽ADDR_ALIGN�ֽ�
	/************************************************************************/
	size_t check_align_addr(void*& pBuf)
	{
		size_t align = 0;
		size_t addr = (int)pBuf;
		align = (ADDR_ALIGN - addr % ADDR_ALIGN) % ADDR_ALIGN;
		pBuf = (char*)pBuf + align;
		return align;
	}
	/************************************************************************/
	/* �ڴ�block��С���뵽MINUNITSIZE�ֽ�
	/************************************************************************/
	size_t check_align_block(size_t size)
	{
		size_t align = size % MINUNITSIZE;

		return size - align;
	}
	/************************************************************************/
	/* �����ڴ��С���뵽SIZE_ALIGN�ֽ�
	/************************************************************************/
	size_t check_align_size(size_t size)
	{
		size = (size + SIZE_ALIGN - 1) / SIZE_ALIGN * SIZE_ALIGN;
		return size;
	}
	/************************************************************************/
	/* �ڴ�ӳ����е�����ת��Ϊ�ڴ���ʼ��ַ
	/************************************************************************/
	void* index2addr(PMEMORYPOOL mem_pool, size_t index)
	{
		char* p = (char*)(mem_pool->memory);
		void* ret = (void*)(p + index *MINUNITSIZE);

		return ret;
	}
	/************************************************************************/
	/* �ڴ���ʼ��ַת��Ϊ�ڴ�ӳ����е�����
	/************************************************************************/
	size_t addr2index(PMEMORYPOOL mem_pool, void* addr)
	{
		char* start = (char*)(mem_pool->memory);
		char* p = (char*)addr;
		size_t index = (p - start) / MINUNITSIZE;
		return index;
	}
	/************************************************************************/
	/* �����ڴ��
	* pBuf: �������ڴ�buffer��ʼ��ַ
	* sBufSize: �������ڴ�buffer��С
	* �������ɵ��ڴ��ָ��
	/************************************************************************/
	PMEMORYPOOL CreateMemoryPool(void* pBuf, size_t sBufSize)
	{
		memset(pBuf, 0, sBufSize);
		PMEMORYPOOL mem_pool = (PMEMORYPOOL)pBuf;
		// ������Ҫ����memory map��Ԫ��
		size_t mem_pool_struct_size = sizeof(MEMORYPOOL);
		mem_pool->mem_map_unit_count = (sBufSize - mem_pool_struct_size + MINUNITSIZE - 1) / MINUNITSIZE;
		mem_pool->pmem_map = (memory_block*)((char*)pBuf + mem_pool_struct_size);
		size_t max_heap_size = (sBufSize - mem_pool_struct_size + MINUNITSIZE - 1) / MINUNITSIZE;
		memory_chunk* heap_arr = (memory_chunk*)((char*)pBuf + mem_pool_struct_size + sizeof(memory_block) * mem_pool->mem_map_unit_count);

		mem_pool->memory = (char*)pBuf + mem_pool_struct_size + sizeof(memory_block) * mem_pool->mem_map_unit_count + sizeof(memory_chunk) * max_heap_size;
		mem_pool->size = sBufSize - mem_pool_struct_size - sizeof(memory_block) * mem_pool->mem_map_unit_count - sizeof(memory_chunk) * max_heap_size;
		size_t align = check_align_addr(mem_pool->memory);
		mem_pool->size -= align;
		mem_pool->size = check_align_block(mem_pool->size);
		mem_pool->mem_block_count = mem_pool->size / MINUNITSIZE;
		init_max_heap(mem_pool->mem_block_count, heap_arr, &(mem_pool->heap));
		memory_chunk chunk;
		chunk.chunk_size = mem_pool->mem_block_count;
		memory_chunk* pos = insert_heap(chunk, &(mem_pool->heap));

		// ��ʼ�� pmem_map
		mem_pool->pmem_map[0].count = mem_pool->mem_block_count;
		mem_pool->pmem_map[0].pmem_chunk = pos;
		mem_pool->pmem_map[mem_pool->mem_block_count - 1].start = 0;

		pos->pfree_mem_addr = mem_pool->pmem_map;
		mem_pool->mem_used_size = 0;
		return mem_pool;
	}
	/************************************************************************/
	/* ��ʱû��
	/************************************************************************/
	void ReleaseMemoryPool(PMEMORYPOOL* ppMem)
	{}
	/************************************************************************/
	/* ���ڴ���з���ָ����С���ڴ�
	* pMem: �ڴ�� ָ��
	* sMemorySize: Ҫ������ڴ��С
	* �ɹ�ʱ���ط�����ڴ���ʼ��ַ��ʧ�ܷ���NULL
	/************************************************************************/
	void* GetMemory(size_t sMemorySize, PMEMORYPOOL pMem)
	{
		sMemorySize = check_align_size(sMemorySize);
		size_t index = 0;
		memory_chunk* max_chunk;
		bool ret = get_max(max_chunk, &(pMem->heap));
		if (ret == false || max_chunk->chunk_size * MINUNITSIZE < sMemorySize)
		{
			return NULL;
		}
		pMem->mem_used_size += sMemorySize;
		if (max_chunk->chunk_size * MINUNITSIZE == sMemorySize)
		{
			// ��Ҫ������ڴ��С�뵱ǰchunk�е��ڴ��С��ͬʱ����pfree_mem_chunk������ɾ����chunk
			size_t current_index = (max_chunk->pfree_mem_addr - pMem->pmem_map);
			remove_max(&(pMem->heap));

			return index2addr(pMem, current_index);
		}
		else
		{
			// ��Ҫ������ڴ�С�ڵ�ǰchunk�е��ڴ�ʱ������pfree_mem_chunk����Ӧchunk��pfree_mem_addr

			// ���Ƶ�ǰmem_map_unit
			memory_block copy;
			copy.count = max_chunk->pfree_mem_addr->count;
			copy.pmem_chunk = max_chunk;
			// ��¼��block����ʼ�ͽ�������
			memory_block* current_block = max_chunk->pfree_mem_addr;
			current_block->count = sMemorySize / MINUNITSIZE;
			size_t current_index = (current_block - pMem->pmem_map);
			pMem->pmem_map[current_index + current_block->count - 1].start = current_index;
			current_block->pmem_chunk = NULL; // NULL��ʾ��ǰ�ڴ���ѱ�����
											  // ��ǰblock��һ��Ϊ�������µڶ���block�е�����
			memory_chunk* pos = decrease_element_value(max_chunk, &(pMem->heap), current_block->count);
			pMem->pmem_map[current_index + current_block->count].count = copy.count - current_block->count;
			pMem->pmem_map[current_index + current_block->count].pmem_chunk = pos;
			// ����ԭ����pfree_mem_addr
			pos->pfree_mem_addr = &(pMem->pmem_map[current_index + current_block->count]);

			size_t end_index = current_index + copy.count - 1;
			pMem->pmem_map[end_index].start = current_index + current_block->count;
			return index2addr(pMem, current_index);
		}
	}
	/************************************************************************/
	/* ���ڴ�����ͷ����뵽���ڴ�
	* pMem���ڴ��ָ��
	* ptrMemoryBlock�����뵽���ڴ���ʼ��ַ
	/************************************************************************/
	void FreeMemory(void *ptrMemoryBlock, PMEMORYPOOL pMem)
	{
		size_t current_index = addr2index(pMem, ptrMemoryBlock);
		size_t size = pMem->pmem_map[current_index].count * MINUNITSIZE;
		// �ж��뵱ǰ�ͷŵ��ڴ�����ڵ��ڴ���Ƿ�����뵱ǰ�ͷŵ��ڴ��ϲ�
		memory_block* pre_block = NULL;
		memory_block* next_block = NULL;
		memory_block* current_block = &(pMem->pmem_map[current_index]);
		// ��һ��
		if (current_index == 0)
		{
			if (current_block->count < pMem->mem_block_count)
			{
				next_block = &(pMem->pmem_map[current_index + current_block->count]);
				// �����һ���ڴ���ǿ��еģ��ϲ�
				if (next_block->pmem_chunk != NULL)
				{
					memory_chunk* pos = increase_element_value(next_block->pmem_chunk, &(pMem->heap), current_block->count);
					pos->pfree_mem_addr = current_block;
					pMem->pmem_map[current_index + current_block->count + next_block->count - 1].start = current_index;
					current_block->count += next_block->count;
					current_block->pmem_chunk = pos;
					next_block->pmem_chunk = NULL;
				}
				// �����һ���ڴ治�ǿ��еģ���pfree_mem_chunk������һ��chunk
				else
				{
					memory_chunk new_chunk;
					new_chunk.chunk_size = current_block->count;
					new_chunk.pfree_mem_addr = current_block;
					memory_chunk* pos = insert_heap(new_chunk, &(pMem->heap));
					current_block->pmem_chunk = pos;
				}
			}
			else
			{
				memory_chunk new_chunk;
				new_chunk.chunk_size = current_block->count;
				new_chunk.pfree_mem_addr = current_block;
				memory_chunk* pos = insert_heap(new_chunk, &(pMem->heap));
				current_block->pmem_chunk = pos;
			}
		}

		// ���һ��
		else if (current_index == pMem->mem_block_count - 1)
		{
			if (current_block->count < pMem->mem_block_count)
			{
				pre_block = &(pMem->pmem_map[current_index - 1]);
				size_t index = pre_block->count;
				pre_block = &(pMem->pmem_map[index]);

				// ���ǰһ���ڴ���ǿ��еģ��ϲ�
				if (pre_block->pmem_chunk != NULL)
				{
					memory_chunk* pos = increase_element_value(pre_block->pmem_chunk, &(pMem->heap), current_block->count);
					pre_block->pmem_chunk = pos;
					pMem->pmem_map[current_index + current_block->count - 1].start = current_index - pre_block->count;
					pre_block->count += current_block->count;
					current_block->pmem_chunk = NULL;
				}
				// ���ǰһ���ڴ治�ǿ��еģ���pfree_mem_chunk������һ��chunk
				else
				{
					memory_chunk new_chunk;
					new_chunk.chunk_size = current_block->count;
					new_chunk.pfree_mem_addr = current_block;
					memory_chunk* pos = insert_heap(new_chunk, &(pMem->heap));
					current_block->pmem_chunk = pos;
				}
			}
			else
			{
				memory_chunk new_chunk;
				new_chunk.chunk_size = current_block->count;
				new_chunk.pfree_mem_addr = current_block;
				memory_chunk* pos = insert_heap(new_chunk, &(pMem->heap));
				current_block->pmem_chunk = pos;
			}
		}
		else
		{
			next_block = &(pMem->pmem_map[current_index + current_block->count]);
			pre_block = &(pMem->pmem_map[current_index - 1]);
			size_t index = pre_block->start;
			pre_block = &(pMem->pmem_map[index]);
			bool is_back_merge = false;
			if (next_block->pmem_chunk == NULL && pre_block->pmem_chunk == NULL)
			{
				memory_chunk new_chunk;
				new_chunk.chunk_size = current_block->count;
				new_chunk.pfree_mem_addr = current_block;
				memory_chunk* pos = insert_heap(new_chunk, &(pMem->heap));
				current_block->pmem_chunk = pos;
			}
			// ��һ���ڴ��
			if (next_block->pmem_chunk != NULL)
			{
				memory_chunk* pos = increase_element_value(next_block->pmem_chunk, &(pMem->heap), current_block->count);
				pos->pfree_mem_addr = current_block;
				pMem->pmem_map[current_index + current_block->count + next_block->count - 1].start = current_index;
				current_block->count += next_block->count;
				current_block->pmem_chunk = pos;
				next_block->pmem_chunk = NULL;
				is_back_merge = true;
			}
			// ǰһ���ڴ��
			if (pre_block->pmem_chunk != NULL)
			{
				pMem->pmem_map[current_index + current_block->count - 1].start = current_index - pre_block->count;
				pre_block->count += current_block->count;
				memory_chunk* pos = increase_element_value(pre_block->pmem_chunk, &(pMem->heap), current_block->count);
				pre_block->pmem_chunk = pos;
				pos->pfree_mem_addr = pre_block;
				if (is_back_merge)
				{
					remove_element(current_block->pmem_chunk, &(pMem->heap));
				}
				current_block->pmem_chunk = NULL;
			}
		}
		pMem->mem_used_size -= size;
	}
}