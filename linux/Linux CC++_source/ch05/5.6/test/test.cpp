 
#include <unistd.h>  
      
int main(void)  
{  
    //ִ��/binĿ¼�µ�ls  
    //��һ������Ϊ������ls���ڶ�������Ϊ-al,����������Ϊ/etc/passwd  
	execl("/bin/ls", "ls", "-al", "/etc/passwd", NULL);  
	return 0;  
}  