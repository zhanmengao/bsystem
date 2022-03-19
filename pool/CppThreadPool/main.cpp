#include "PP.hpp"
#include"PthreadPool.h"

void *process(void *arg)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("task %d is end\n", *(int *)arg);
	return NULL;
}
int main(void)
{
	/*threadpool_t *threadpool_create(int min_thr_num, int max_thr_num, int queue_max_size);*/

    PthreadPool tPool(3, 100, 100);/*创建线程池，池里最小3个线程，最大100，队列最大100*/
	printf("pool inited");

	//int *num = (int *)malloc(sizeof(int)*20);
	int num[100], i;
	for (i = 0; i < 100; i++)
	{
		num[i] = i;
		//printf("add task %d\n", i);
		tPool.threadpool_add(process, (void*)&num[i]);     /* 向线程池中添加任务 */
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));                                          /* 等子线程完成任务 */
	tPool.threadpool_destroy();
	return 0;
}

