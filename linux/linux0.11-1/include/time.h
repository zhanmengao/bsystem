#ifndef _TIME_H
#define _TIME_H

#ifndef _TIME_T
#define _TIME_T
typedef long time_t;		// ��GMT 1970 ��1 ��1 �տ�ʼ�����������ʱ�䣨����ʱ�䣩��
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned int size_t;
#endif

#define CLOCKS_PER_SEC 100	// ϵͳʱ�ӵδ�Ƶ�ʣ�100HZ��

typedef long clock_t;		// �ӽ��̿�ʼϵͳ������ʱ�ӵδ�����

struct tm
{
  int tm_sec;			// ���� [0��59]��
  int tm_min;			// ������ [ 0��59]��
  int tm_hour;			// Сʱ�� [0��59]��
  int tm_mday;			// 1 ���µ����� [0��31]��
  int tm_mon;			// 1 �����·� [0��11]��
  int tm_year;			// ��1900 �꿪ʼ��������
  int tm_wday;			// 1 �����е�ĳ�� [0��6]�������� =0����
  int tm_yday;			// 1 ���е�ĳ�� [0��365]��
  int tm_isdst;			// ����ʱ��־��
};

// �������й�ʱ������ĺ���ԭ�͡�
// ȷ��������ʹ��ʱ�䡣���س������ô�����ʱ�䣨�δ������Ľ���ֵ��
clock_t clock (void);
// ȡʱ�䣨�����������ش�1970.1.1:0:0:0 ��ʼ����������Ϊ����ʱ�䣩��
time_t time (time_t * tp);
// ����ʱ������ʱ��time2 ��time1 ֮�侭����������
double difftime (time_t time2, time_t time1);
// ��tm �ṹ��ʾ��ʱ��ת��������ʱ�䡣
time_t mktime (struct tm *tp);

// ��tm �ṹ��ʾ��ʱ��ת����һ���ַ���������ָ��ô���ָ�롣
char *asctime (const struct tm *tp);
// ������ʱ��ת����һ���ַ�����ʽ���硰Wed Jun 30 21:49:08:1993\n����
char *ctime (const time_t * tp);
// ������ʱ��ת����tm �ṹ��ʾ��UTC ʱ�䣨UTC - ����ʱ�����Universal Time Code����
struct tm *gmtime (const time_t * tp);
// ������ʱ��ת����tm �ṹ��ʾ��ָ��ʱ����(timezone)��ʱ�䡣
struct tm *localtime (const time_t * tp);
// ��tm �ṹ��ʾ��ʱ�����ø�ʽ�ַ���fmt ת������󳤶�Ϊsmax ���ַ�����������洢��s �С�
size_t strftime (char *s, size_t smax, const char *fmt, const struct tm *tp);
// ��ʼ��ʱ��ת����Ϣ��ʹ�û�������TZ����zname �������г�ʼ����
// ����ʱ������ص�ʱ��ת�������н��Զ����øú�����
void tzset (void);

#endif
