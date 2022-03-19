#include <mm.h>
#include <stdio.h>

int main(){
    MM *pool = mm_create(1024*10,"abc");

    char* s1 = mm_malloc(pool,10);
    strcpy(s1,"abcd");

    char* s2 = mm_strdup(pool,s1);
    printf("s1 = %p : %s , s2 = %p : %s \n",s1,s1,s2,s2);
    printf("[Befor Reset] available = %ld \n", mm_available(pool));

    //---------------reset---------------------------------
    mm_reset(pool);
    printf("[After Reset] available = %ld \n ",mm_available(pool));

    int* x = mm_malloc(pool,sizeof(int));
    *x=0x1234;
    printf("x = %p ,0x%x \n ",x,*x);

    char* s= mm_malloc(pool,1024*20);
    if (s == NULL){
        printf("%s \n",mm_error());
    }

    mm_display_info(pool);
    mm_destroy(pool);
    return 0;
}
