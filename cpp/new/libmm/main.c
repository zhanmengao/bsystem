#include <stdio.h>
#include<mm.h>
int main()
{
    //create 10k memory pool
    MM *pool = mm_create(1024*10,"abc");

    //lock pool : create abc.sem in current dir
    mm_lock(pool,MM_LOCK_RW);

    int* x = mm_malloc(pool,sizeof(int));
    *x = 1234;

    //get block size
    printf("%p = %ld \n",x,mm_sizeof(pool,x));

    //display pool info
    mm_display_info(pool);

    printf("max = %ld ,avail = %ld \n ",mm_maxsize(),mm_available(pool));

    getchar();

    //delete abc.sem -> unlock
    mm_unlock(pool);

    //clear
    mm_destroy(pool);
    return 0;
}
