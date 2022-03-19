/* passed
* linux/kernel/blk_dev/ll_rw.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

/*
* This handles all read/write requests to block devices
*/
/*
* �ó�������豸�����ж�/д������
*/
#include <errno.h>		// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/system.h>		// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ

#include "blk.h"		// ���豸ͷ�ļ��������������ݽṹ�����豸���ݽṹ�ͺ꺯������Ϣ��

/*
* The request-struct contains all necessary data
* to load a nr of sectors into memory
*/
/*
* ����ṹ�к��м���nr �������ݵ��ڴ�����б������Ϣ��
*/
struct request request[NR_REQUEST] = {0};

/*
* used to wait on when there are no free requests
*/
/* ��������������û�п�����ʱ����ʱ�ȴ��� */
struct task_struct *wait_for_request = NULL;

/* blk_dev_struct is:
* do_request-address
* next-request
*/
/* blk_dev_struct ���豸�ṹ�ǣ�(kernel/blk_drv/blk.h,23)
* do_request-address //��Ӧ���豸�ŵ����������ָ�롣
* current-request // ���豸����һ������
*/
// ������ʹ�����豸����Ϊ�������±꣩��
struct blk_dev_struct blk_dev[NR_BLK_DEV] = {
	{NULL, NULL},		/* no_dev */// 0 - ���豸��
	{NULL, NULL},		/* dev mem */// 1 - �ڴ档
	{NULL, NULL},		/* dev fd */// 2 - �����豸��
	{NULL, NULL},		/* dev hd */// 3 - Ӳ���豸��
	{NULL, NULL},		/* dev ttyx */// 4 - ttyx �豸��
	{NULL, NULL},		/* dev tty */// 5 - tty �豸��
	{NULL, NULL}		/* dev lp */// 6 - lp ��ӡ���豸��
};

// ����ָ���Ļ�����bh�����ָ���Ļ������Ѿ�������������������ʹ�Լ�˯�ߣ������жϵصȴ�����
// ֱ����ִ�н�����������������ȷ�ػ��ѡ�
static _inline void
lock_buffer (struct buffer_head *bh)
{
	cli ();			// ���ж���ɡ�
	while (bh->b_lock)		// ����������ѱ���������˯�ߣ�ֱ��������������
		sleep_on (&bh->b_wait);
	bh->b_lock = 1;		// ���������û�������
	sti ();			// ���жϡ�
}

// �ͷţ������������Ļ�������
static _inline void
unlock_buffer (struct buffer_head *bh)
{
	if (!bh->b_lock)		// ����û�������û�б����������ӡ������Ϣ��
		printk ("ll_rw_block.c: buffer not locked\n\r");
	bh->b_lock = 0;		// ��������־��
	wake_up (&bh->b_wait);	// ���ѵȴ��û�����������
}

/*
* add-request adds a request to the linked list.
* It disables interrupts so that it can muck with the
* request-lists in peace.
*/
/*
* add-request()�������м���һ���������ر��жϣ�
* �������ܰ�ȫ�ش������������� 
*/
//// �������м������������dev ָ�����豸��req ������Ľṹ��Ϣ��
  static void
add_request (struct blk_dev_struct *dev, struct request *req)
{
	struct request *tmp;

	req->next = NULL;
	cli ();			// ���жϡ�
	if (req->bh)
		req->bh->b_dirt = 0;	// �建�������ࡱ��־��
// ���dev �ĵ�ǰ����(current_request)�Ӷ�Ϊ�գ����ʾĿǰ���豸û������������ǵ�1 ��
// �������˿ɽ����豸��ǰ����ָ��ֱ��ָ�������������ִ����Ӧ�豸����������
	if (!(tmp = dev->current_request))
	{
		dev->current_request = req;
		sti ();			// ���жϡ�
		(dev->request_fn) ();	// ִ���豸������������Ӳ��(3)��do_hd_request()��
		return;
	}
// ���Ŀǰ���豸�Ѿ����������ڵȴ������������õ����㷨�������λ�ã�Ȼ�󽫵�ǰ�������
// ���������С�
	for (; tmp->next; tmp = tmp->next)
	if ((IN_ORDER (tmp, req) ||
		!IN_ORDER (tmp, tmp->next)) && IN_ORDER (req, tmp->next))
		break;
	req->next = tmp->next;
	tmp->next = req;
	sti ();
}

//// �������������������С������ǣ����豸��major������rw��������ݵĻ�����ͷָ��bh��
static void
make_request (int major, int rw, struct buffer_head *bh)
{
	struct request *req;
	int rw_ahead;

/* WRITEA/READA ���������� - ���ǲ����Ǳ�Ҫ�ģ���������������Ѿ�������*/
/* ���ǾͲ��������˳�������Ļ���ִ��һ��Ķ�/д������ */
// ����'READ'��'WRITE'�����'A'�ַ�����Ӣ�ĵ���Ahead����ʾ��ǰԤ��/д���ݿ����˼��
// ��ָ���Ļ���������ʹ�ã��ѱ�����ʱ���ͷ���Ԥ��/д����
	if (rw_ahead = (rw == READA || rw == WRITEA))
	{
		if (bh->b_lock)
			return;
		if (rw == READA)
			rw = READ;
		else
			rw = WRITE;
	}
// ��������READ ��WRITE ���ʾ�ں˳����д���ʾ������Ϣ��������
	if (rw != READ && rw != WRITE)
		panic ("Bad block dev command, must be R/W/RA/WA");
// ����������������������Ѿ���������ǰ���񣨽��̣��ͻ�˯�ߣ�ֱ������ȷ�ػ��ѡ�
	lock_buffer (bh);
// ���������д���һ��������ݲ��࣬���������Ƕ����һ����������Ǹ��¹��ģ��������
// ������󡣽��������������˳���
	if ((rw == WRITE && !bh->b_dirt) || (rw == READ && bh->b_uptodate))
	{
		unlock_buffer (bh);
		return;
	}
repeat:
/* ���ǲ����ö�����ȫ����д�����������ҪΪ��������һЩ�ռ䣺������
* �����ȵġ�������еĺ�����֮һ�ռ���Ϊ��׼���ġ�
*/
// �������Ǵ���������ĩβ��ʼ������������ġ���������Ҫ�󣬶��ڶ��������󣬿���ֱ��
// �Ӷ���ĩβ��ʼ��������д������ֻ�ܴӶ��е�2/3 ����ͷ�������������롣
	if (rw == READ)
		req = request + NR_REQUEST;	// ���ڶ����󣬽�����ָ��ָ�����β����
	else
		req = request + ((NR_REQUEST * 2) / 3);	// ����д���󣬶���ָ��ָ�����2/3 ����
/* ����һ���������� */
// �Ӻ���ǰ������������ṹrequest ��dev �ֶ�ֵ=-1 ʱ����ʾ����δ��ռ�á�
	while (--req >= request)
		if (req->dev < 0)
			break;
/* ���û���ҵ���������øô�������˯�ߣ������Ƿ���ǰ��/д */
// ���û��һ���ǿ��еģ���ʱrequest ����ָ���Ѿ�����Խ��ͷ��������鿴�˴������Ƿ���
// ��ǰ��/д��READA ��WRITEA���������������˴����󡣷����ñ�������˯�ߣ��ȴ��������
// �ڳ��������һ����������������С�
	if (req < request)
	{				// ������������û�п����
		if (rw_ahead)
		{			// �������ǰ��/д������������������˳���
			unlock_buffer (bh);
			return;
		}
		sleep_on (&wait_for_request);	// �����ñ�������˯�ߣ������ٲ鿴������С�
		goto repeat;
	}
/* ���������������д������Ϣ���������������� */
// ����ṹ�μ���kernel/blk_drv/blk.h,23����
	req->dev = bh->b_dev;		// �豸�š�
	req->cmd = rw;		// ����(READ/WRITE)��
	req->errors = 0;		// ����ʱ�����Ĵ��������
	req->sector = bh->b_blocknr << 1;	// ��ʼ������(1 ��=2 ����)
	req->nr_sectors = 2;		// ��д��������
	req->buffer = bh->b_data;	// ���ݻ�������
	req->waiting = NULL;		// ����ȴ�����ִ����ɵĵط���
	req->bh = bh;			// ������ͷָ�롣
	req->next = NULL;		// ָ����һ�����
	add_request (major + blk_dev, req);	// ����������������(blk_dev[major],req)��
}

//// �Ͳ��д���ݿ麯����
// �ú�����Ҫ����fs/buffer.c �б����á�ʵ�ʵĶ�д���������豸��request_fn()������ɡ�
// ����Ӳ�̲������ú�����do_hd_request()����kernel/blk_drv/hd.c,294��
void ll_rw_block (int rw, struct buffer_head *bh)
{
	unsigned int major;		// ���豸�ţ�����Ӳ����3����

// ����豸�����豸�Ų����ڻ��߸��豸�Ķ�д�������������ڣ�����ʾ������Ϣ�������ء�
	if ((major = MAJOR (bh->b_dev)) >= NR_BLK_DEV ||
		!(blk_dev[major].request_fn))
	{
		printk ("Trying to read nonexistent block-device\n\r");
		return;
	}
	make_request (major, rw, bh);	// �������������������С�
}

//// ���豸��ʼ���������ɳ�ʼ������main.c ���ã�init/main.c,128����
// ��ʼ���������飬��������������Ϊ������(dev = -1)����32 ��(NR_REQUEST = 32)��
void blk_dev_init (void)
{
	int i;

	for (i = 0; i < NR_REQUEST; i++)
	{
		request[i].dev = -1;
		request[i].next = NULL;
	}
}
