#include <memory.h>
#include "MemoryPool.h"
namespace LinkMemoryPool
{
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
	/* ������������ز���
	/************************************************************************/
	memory_chunk* create_list(memory_chunk* pool, size_t count)
	{
		if (!pool)
		{
			return NULL;
		}
		memory_chunk* head = NULL;
		for (size_t i = 0; i < count; i++)
		{
			pool->pre = NULL;
			pool->next = head;
			if (head != NULL)
			{
				head->pre = pool;
			}
			head = pool;
			pool++;
		}
		return head;
	}
	memory_chunk* front_pop(memory_chunk*& pool)
	{
		if (!pool)
		{
			return NULL;
		}
		memory_chunk* tmp = pool;
		pool = tmp->next;
		pool->pre = NULL;
		return  tmp;
	}
	void push_back(memory_chunk*& head, memory_chunk* element)
	{
		if (head == NULL)
		{
			head = element;
			head->pre = element;
			head->next = element;
			return;
		}
		head->pre->next = element;
		element->pre = head->pre;
		head->pre = element;
		element->next = head;
	}
	void push_front(memory_chunk*& head, memory_chunk* element)
	{
		element->pre = NULL;
		element->next = head;
		if (head != NULL)
		{
			head->pre = element;
		}
		head = element;
	}
	void delete_chunk(memory_chunk*& head, memory_chunk* element)
	{
		// ��˫ѭ��������ɾ��Ԫ��
		if (element == NULL)
		{
			return;
		}
		// elementΪ����ͷ
		else if (element == head)
		{
			// ����ֻ��һ��Ԫ��
			if (head->pre == head)
			{
				head = NULL;
			}
			else
			{
				head = element->next;
				head->pre = element->pre;
				head->pre->next = head;
			}
		}
		// elementΪ����β
		else if (element->next == head)
		{
			head->pre = element->pre;
			element->pre->next = head;
		}
		else
		{
			element->pre->next = element->next;
			element->next->pre = element->pre;
		}
		element->pre = NULL;
		element->next = NULL;
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
		mem_pool->mem_map_pool_count = (sBufSize - mem_pool_struct_size + MINUNITSIZE - 1) / MINUNITSIZE;
		mem_pool->mem_map_unit_count = (sBufSize - mem_pool_struct_size + MINUNITSIZE - 1) / MINUNITSIZE;
		mem_pool->pmem_map = (memory_block*)((char*)pBuf + mem_pool_struct_size);
		mem_pool->pfree_mem_chunk_pool = (memory_chunk*)((char*)pBuf + mem_pool_struct_size + sizeof(memory_block) * mem_pool->mem_map_unit_count);

		mem_pool->memory = (char*)pBuf + mem_pool_struct_size + sizeof(memory_block) * mem_pool->mem_map_unit_count + sizeof(memory_chunk) * mem_pool->mem_map_pool_count;
		mem_pool->size = sBufSize - mem_pool_struct_size - sizeof(memory_block) * mem_pool->mem_map_unit_count - sizeof(memory_chunk) * mem_pool->mem_map_pool_count;
		size_t align = check_align_addr(mem_pool->memory);
		mem_pool->size -= align;
		mem_pool->size = check_align_block(mem_pool->size);
		mem_pool->mem_block_count = mem_pool->size / MINUNITSIZE;
		// ����
		mem_pool->pfree_mem_chunk_pool = create_list(mem_pool->pfree_mem_chunk_pool, mem_pool->mem_map_pool_count);
		// ��ʼ�� pfree_mem_chunk��˫��ѭ������
		memory_chunk* tmp = front_pop(mem_pool->pfree_mem_chunk_pool);
		tmp->pre = tmp;
		tmp->next = tmp;
		tmp->pfree_mem_addr = NULL;
		mem_pool->mem_map_pool_count--;

		// ��ʼ�� pmem_map
		mem_pool->pmem_map[0].count = mem_pool->mem_block_count;
		mem_pool->pmem_map[0].pmem_chunk = tmp;
		mem_pool->pmem_map[mem_pool->mem_block_count - 1].start = 0;

		tmp->pfree_mem_addr = mem_pool->pmem_map;
		push_back(mem_pool->pfree_mem_chunk, tmp);
		mem_pool->free_mem_chunk_count = 1;
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
		memory_chunk* tmp = pMem->pfree_mem_chunk;
		for (index = 0; index < pMem->free_mem_chunk_count; index++)
		{
			if (tmp->pfree_mem_addr->count * MINUNITSIZE >= sMemorySize)
			{
				break;
			}

			tmp = tmp->next;
		}

		if (index == pMem->free_mem_chunk_count)
		{
			return NULL;
		}
		pMem->mem_used_size += sMemorySize;
		if (tmp->pfree_mem_addr->count * MINUNITSIZE == sMemorySize)
		{
			// ��Ҫ������ڴ��С�뵱ǰchunk�е��ڴ��С��ͬʱ����pfree_mem_chunk������ɾ����chunk
			size_t current_index = (tmp->pfree_mem_addr - pMem->pmem_map);
			delete_chunk(pMem->pfree_mem_chunk, tmp);
			tmp->pfree_mem_addr->pmem_chunk = NULL;

			push_front(pMem->pfree_mem_chunk_pool, tmp);
			pMem->free_mem_chunk_count--;
			pMem->mem_map_pool_count++;

			return index2addr(pMem, current_index);
		}
		else
		{
			// ��Ҫ������ڴ�С�ڵ�ǰchunk�е��ڴ�ʱ������pfree_mem_chunk����Ӧchunk��pfree_mem_addr

			// ���Ƶ�ǰmem_map_unit
			memory_block copy;
			copy.count = tmp->pfree_mem_addr->count;
			copy.pmem_chunk = tmp;
			// ��¼��block����ʼ�ͽ�������
			memory_block* current_block = tmp->pfree_mem_addr;
			current_block->count = sMemorySize / MINUNITSIZE;
			size_t current_index = (current_block - pMem->pmem_map);
			pMem->pmem_map[current_index + current_block->count - 1].start = current_index;
			current_block->pmem_chunk = NULL; // NULL��ʾ��ǰ�ڴ���ѱ�����
											  // ��ǰblock��һ��Ϊ�������µڶ���block�е�����
			pMem->pmem_map[current_index + current_block->count].count = copy.count - current_block->count;
			pMem->pmem_map[current_index + current_block->count].pmem_chunk = copy.pmem_chunk;
			// ����ԭ����pfree_mem_addr
			tmp->pfree_mem_addr = &(pMem->pmem_map[current_index + current_block->count]);

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
					next_block->pmem_chunk->pfree_mem_addr = current_block;
					pMem->pmem_map[current_index + current_block->count + next_block->count - 1].start = current_index;
					current_block->count += next_block->count;
					current_block->pmem_chunk = next_block->pmem_chunk;
					next_block->pmem_chunk = NULL;
				}
				// �����һ���ڴ治�ǿ��еģ���pfree_mem_chunk������һ��chunk
				else
				{
					memory_chunk* new_chunk = front_pop(pMem->pfree_mem_chunk_pool);
					new_chunk->pfree_mem_addr = current_block;
					current_block->pmem_chunk = new_chunk;
					push_back(pMem->pfree_mem_chunk, new_chunk);
					pMem->mem_map_pool_count--;
					pMem->free_mem_chunk_count++;
				}
			}
			else
			{
				memory_chunk* new_chunk = front_pop(pMem->pfree_mem_chunk_pool);
				new_chunk->pfree_mem_addr = current_block;
				current_block->pmem_chunk = new_chunk;
				push_back(pMem->pfree_mem_chunk, new_chunk);
				pMem->mem_map_pool_count--;
				pMem->free_mem_chunk_count++;
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
					pMem->pmem_map[current_index + current_block->count - 1].start = current_index - pre_block->count;
					pre_block->count += current_block->count;
					current_block->pmem_chunk = NULL;
				}
				// ���ǰһ���ڴ治�ǿ��еģ���pfree_mem_chunk������һ��chunk
				else
				{
					memory_chunk* new_chunk = front_pop(pMem->pfree_mem_chunk_pool);
					new_chunk->pfree_mem_addr = current_block;
					current_block->pmem_chunk = new_chunk;
					push_back(pMem->pfree_mem_chunk, new_chunk);
					pMem->mem_map_pool_count--;
					pMem->free_mem_chunk_count++;
				}
			}
			else
			{
				memory_chunk* new_chunk = front_pop(pMem->pfree_mem_chunk_pool);
				new_chunk->pfree_mem_addr = current_block;
				current_block->pmem_chunk = new_chunk;
				push_back(pMem->pfree_mem_chunk, new_chunk);
				pMem->mem_map_pool_count--;
				pMem->free_mem_chunk_count++;
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
				memory_chunk* new_chunk = front_pop(pMem->pfree_mem_chunk_pool);
				new_chunk->pfree_mem_addr = current_block;
				current_block->pmem_chunk = new_chunk;
				push_back(pMem->pfree_mem_chunk, new_chunk);
				pMem->mem_map_pool_count--;
				pMem->free_mem_chunk_count++;
			}
			// ��һ���ڴ��
			if (next_block->pmem_chunk != NULL)
			{
				next_block->pmem_chunk->pfree_mem_addr = current_block;
				pMem->pmem_map[current_index + current_block->count + next_block->count - 1].start = current_index;
				current_block->count += next_block->count;
				current_block->pmem_chunk = next_block->pmem_chunk;
				next_block->pmem_chunk = NULL;
				is_back_merge = true;
			}
			// ǰһ���ڴ��
			if (pre_block->pmem_chunk != NULL)
			{
				pMem->pmem_map[current_index + current_block->count - 1].start = current_index - pre_block->count;
				pre_block->count += current_block->count;
				if (is_back_merge)
				{
					delete_chunk(pMem->pfree_mem_chunk, current_block->pmem_chunk);
					push_front(pMem->pfree_mem_chunk_pool, current_block->pmem_chunk);
					pMem->free_mem_chunk_count--;
					pMem->mem_map_pool_count++;
				}
				current_block->pmem_chunk = NULL;
			}
		}
		pMem->mem_used_size -= size;
	}
}