#ifndef _BLK_H
#define _BLK_H

#define NR_BLK_DEV 7		// ���豸��������
/*
* ���涨���NR_REQUEST �������������������������
* ע�⣬��������ʹ����Щ��Ͷ˵�2/3�����������ȴ���
*
* 32 �������һ����������֣��Ѿ��㹻�ӵ����㷨�л�úô���
* �����������ڶ����ж���סʱ�ֲ��Ե��Ǻܴ������64 �Ϳ���
* ȥ̫���ˣ���������д/ͬ����������ʱ����������ʱ�����ͣ����
*/
#define NR_REQUEST 32

/*
* OK��������request �ṹ��һ����չ��ʽ�������ʵ���Ժ����ǾͿ����ڷ�ҳ������
* ʹ��ͬ����request �ṹ���ڷ�ҳ�����У�'bh'��NULL����'waiting'�����ڵȴ���/д����ɡ�
*/
// �����������������Ľṹ���������dev=-1�����ʾ����û�б�ʹ�á�
struct request
{
  int dev;			/* -1 if no request */// ʹ�õ��豸�š�
  int cmd;			/* READ or WRITE */// ����(READ ��WRITE)��
  int errors;			//����ʱ�����Ĵ��������
  unsigned long sector;		// ��ʼ������(1 ��=2 ����)
  unsigned long nr_sectors;	// ��/д��������
  char *buffer;			// ���ݻ�������
  struct task_struct *waiting;	// ����ȴ�����ִ����ɵĵط���
  struct buffer_head *bh;	// ������ͷָ��(include/linux/fs.h,68)��
  struct request *next;		// ָ����һ�����
};

/*
* ����Ķ������ڵ����㷨��ע�������������д����֮ǰ���С�
* ���Ǻ���Ȼ�ģ���������ʱ���Ҫ��Ҫ��д�ϸ�öࡣ
*/
#define IN_ORDER(s1,s2) \
((s1)->cmd<(s2)->cmd || (s1)->cmd==(s2)->cmd && \
((s1)->dev < (s2)->dev || ((s1)->dev == (s2)->dev && \
(s1)->sector < (s2)->sector)))

// ���豸�ṹ��
struct blk_dev_struct
{
  void (*request_fn) (void);	// ��������ĺ���ָ�롣
  struct request *current_request;	// ������Ϣ�ṹ��
};

extern struct blk_dev_struct blk_dev[NR_BLK_DEV];	// ���豸���飬ÿ�ֿ��豸ռ��һ�
extern struct request request[NR_REQUEST];	// ����������顣
extern struct task_struct *wait_for_request;	// �ȴ����������ṹ��

#ifdef MAJOR_NR			// ���豸�š�

/*
* ��Ҫʱ������Ŀ��Ŀǰ���豸��֧��Ӳ�̺����̣����������̣���
*/

#if (MAJOR_NR == 1)		// RAM �̵����豸����1����������Ķ�����������ڴ�����豸��ҲΪ1��
/* ram disk *//* RAM �̣��ڴ������̣� */
#define DEVICE_NAME "ramdisk"	// �豸����ramdisk��
#define DEVICE_REQUEST do_rd_request	// �豸������do_rd_request()��
#define DEVICE_NR(device) ((device) & 7)	// �豸��(0--7)��
#define DEVICE_ON(device)	// �����豸�����������뿪���͹رա�
#define DEVICE_OFF(device)	// �ر��豸��

#elif (MAJOR_NR == 2)		// ���������豸����2��
/* floppy */
#define DEVICE_NAME "floppy"	// �豸����floppy��
#define DEVICE_INTR do_floppy	// �豸�жϴ������do_floppy()��
#define DEVICE_REQUEST do_fd_request	// �豸������do_fd_request()��
#define DEVICE_NR(device) ((device) & 3)	// �豸�ţ�0--3����
#define DEVICE_ON(device) floppy_on(DEVICE_NR(device))	// �����豸����floppyon()��
#define DEVICE_OFF(device) floppy_off(DEVICE_NR(device))	// �ر��豸����floppyoff()��

#elif (MAJOR_NR == 3)		// Ӳ�����豸����3��
/* harddisk */
#define DEVICE_NAME "harddisk"	// Ӳ������harddisk��
#define DEVICE_INTR do_hd	// �豸�жϴ������do_hd()��
#define DEVICE_REQUEST do_hd_request	// �豸������do_hd_request()��
#define DEVICE_NR(device) (MINOR(device)/5)	// �豸�ţ�0--1����ÿ��Ӳ�̿�����4 ��������
#define DEVICE_ON(device)	// Ӳ��һֱ�ڹ��������뿪���͹رա�
#define DEVICE_OFF(device)

#elif
/* unknown blk device *//* δ֪���豸 */
#error "unknown blk device"

#endif

#define CURRENT (blk_dev[MAJOR_NR].current_request)	// CURRENT Ϊָ�����豸�ŵĵ�ǰ����ṹ��
#define CURRENT_DEV DEVICE_NR(CURRENT->dev)	// CURRENT_DEV ΪCURRENT ���豸�š�

#ifdef DEVICE_INTR
void (*DEVICE_INTR) (void) = NULL;
#endif
static void (DEVICE_REQUEST) (void);

// �ͷ������Ļ�������
extern _inline void
unlock_buffer (struct buffer_head *bh)
{
  if (!bh->b_lock)		// ���ָ���Ļ�����bh ��û�б�����������ʾ������Ϣ��
    printk (DEVICE_NAME ": free buffer being unlocked\n");
  bh->b_lock = 0;		// ���򽫸û�����������
  wake_up (&bh->b_wait);	// ���ѵȴ��û������Ľ��̡�
}

// ��������
extern _inline void
end_request (int uptodate)
{
  DEVICE_OFF (CURRENT->dev);	// �ر��豸��
  if (CURRENT->bh)
    {				// CURRENT Ϊָ�����豸�ŵĵ�ǰ����ṹ��
      CURRENT->bh->b_uptodate = uptodate;	// �ø��±�־��
      unlock_buffer (CURRENT->bh);	// ������������
    }
  if (!uptodate)
    {				// ������±�־Ϊ0 ����ʾ�豸������Ϣ��
      printk (DEVICE_NAME " I/O error\n\r");
      printk ("dev %04x, block %d\n\r", CURRENT->dev, CURRENT->bh->b_blocknr);
    }
  wake_up (&CURRENT->waiting);	// ���ѵȴ���������Ľ��̡�
  wake_up (&wait_for_request);	// ���ѵȴ�����Ľ��̡�
  CURRENT->dev = -1;		// �ͷŸ������
  CURRENT = CURRENT->next;	// ������������ɾ���������
}

// �����ʼ������ꡣ
#define INIT_REQUEST \
repeat: \
if (!CURRENT) 			/* �����ǰ����ṹָ��Ϊnull �򷵻ء�*/\
return; \
if (MAJOR (CURRENT->dev) != MAJOR_NR) 	/* �����ǰ�豸�����豸�Ų�����������*/ \
    panic (DEVICE_NAME ": request list destroyed"); \
if (CURRENT->bh) \
{ \
    if (!CURRENT->bh->b_lock) 	/* ����ڽ����������ʱ������û������������*/\
	panic (DEVICE_NAME ": block not locked"); \
}

#endif

#endif
