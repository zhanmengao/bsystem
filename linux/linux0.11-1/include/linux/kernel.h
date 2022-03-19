/*
* 'kernel.h'������һЩ���ú�����ԭ�͵ȡ�
*/
// ��֤������ַ��ʼ���ڴ���Ƿ��ޡ���������׷���ڴ档( kernel/fork.c, 24 )��
void verify_area (void *addr, int count);
// ��ʾ�ں˳�����Ϣ��Ȼ�������ѭ����( kernel/panic.c, 16 )��
void panic (const char *str);
// ��׼��ӡ����ʾ��������( init/main.c, 151)��
int printf (const char *fmt, ...);
// �ں�ר�õĴ�ӡ��Ϣ������������printf()��ͬ��( kernel/printk.c, 21 )��
int printk (const char *fmt, ...);
// ��tty ��дָ�����ȵ��ַ�����( kernel/chr_drv/tty_io.c, 290 )��
int tty_write (unsigned ch, char *buf, int count);
// ͨ���ں��ڴ���亯����( lib/malloc.c, 117)��
void *malloc (unsigned int size);
// �ͷ�ָ������ռ�õ��ڴ档( lib/malloc.c, 182)��
void free_s (void *obj, int size);

#define free(x) free_s((x), 0)

/*
* ���溯�����Ժ����ʽ����ģ�������ĳ�������������Գ�Ϊһ���������ӳ���
* ���������true ʱ�������ñ�־�����ʹ��root �û�Ȩ�޵Ľ��������˱�־������
* ��ִ��BSD ��ʽ�ļ��ʴ���������ζ����Ӧ������ִ�г���Ȩ�޼�飬�����
* ���suser()��
*/
#define suser() (current->euid == 0)	// ����Ƿ��ǳ����û���
