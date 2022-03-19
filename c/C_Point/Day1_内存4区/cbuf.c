#include<stdio.h>
#include<stdlib.h>

int play(){
    char buf[20] = "12345";
    char buf2[] = "12345";
    char *str1 = "12345";
    char *str2 = "12345";

    printf("buf1 = %s addr = %d &buf1 = %d \n",buf,buf,&buf);
    printf("buf2 = %s addr = %d &buf2 = %d \n",buf,buf2,&buf2);
    printf("str1 = %s addr = %d &buf1 = %d \n",str1,str1,&str1);
    printf("str2 = %s addr = %d &buf2 = %d \n",str2,str2,&str2);
    buf[7] = 'a';
    printf("buf1 = %s addr = %d &buf1 = %d \n",buf,buf,&buf);
}
int main(){
    play();
    printf("\n ---------------------------main---------------------\n");
    char buf[20] = "12345";
    char buf2[] = "12345";

    printf("buf1 = %s addr = %d &buf1 = %d \n",buf,buf,&buf);
    printf("buf2 = %s addr = %d &buf2 = %d \n",buf,buf2,&buf2);

    buf[7] = 'a';
    printf("buf1 = %s addr = %d &buf1 = %d \n",buf,buf,&buf);
    return 0;
}
