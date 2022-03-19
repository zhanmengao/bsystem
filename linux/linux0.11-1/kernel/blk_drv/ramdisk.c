/* passed
* linux/kernel/blk_drv/ramdisk.c
*
* Written by Theodore Ts'o, 12/2/91
*/
#include <set_seg.h>

/* ��Theodore Ts'o ���ƣ�12/2/91
*/
// Theodore Ts'o (Ted Ts'o)��linux �����е��������Linux �����緶Χ�ڵ�����Ҳ�����ܴ��
// ���ͣ�����Linux ����ϵͳ������ʱ�����ͻ��ż��������Ϊlinux �ķ�չ�ṩ��maillist����
// �ڱ����޵�������������linux ��ftp վ�㣨tsx-11.mit.edu��������������ȻΪ���linux �û�
// �ṩ��������linux �����������֮һ�������ʵ����ext2 �ļ�ϵͳ�����ļ�ϵͳ�ѳ�Ϊ
// linux ��������ʵ�ϵ��ļ�ϵͳ��׼����������Ƴ���ext3 �ļ�ϵͳ�����������ļ�ϵͳ��
// �ȶ��Ժͷ���Ч�ʡ���Ϊ�������Ƴ磬��97 �ڣ�2002 ��5 �£���linuxjournal �ڿ�������Ϊ
// �˷�����������������˲ɷá�Ŀǰ����ΪIBM linux �������Ĺ��������������й�LSB
// (Linux Standard Base)�ȷ���Ĺ�����(������ҳ��http://thunk.org/tytso/)

#include <string.h>		// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����

#include <linux/config.h>	// �ں�����ͷ�ļ�������������Ժ�Ӳ�����ͣ�HD_TYPE����ѡ�
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/fs.h>		// �ļ�ϵͳͷ�ļ��������ļ���ṹ��file,buffer_head,m_inode �ȣ���
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/system.h>		// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����
#include <asm/memory.h>		// �ڴ濽��ͷ�ļ�������memcpy()Ƕ��ʽ���꺯����

#define MAJOR_NR 1		// �ڴ����豸����1��
#include "blk.h"

char *rd_start = 0;			// ���������ڴ��е���ʼλ�á���52 �г�ʼ������rd_init()��
// ȷ�����μ�(init/main.c,124)����дrd_����ramdisk_����
int rd_length = 0;		// ��������ռ�ڴ��С���ֽڣ���

// ִ��������(ramdisk)��д����������ṹ��do_hd_request()����(kernel/blk_drv/hd.c,294)��
void
do_rd_request (void)
{
	int len;
	char *addr;

	INIT_REQUEST;			// �������ĺϷ���(�μ�kernel/blk_drv/blk.h,127)��
// �������ȡ��ramdisk ����ʼ������Ӧ���ڴ���ʼλ�ú��ڴ泤�ȡ�
// ����sector << 9 ��ʾsector * 512��CURRENT ����Ϊ(blk_dev[MAJOR_NR].current_request)��
	addr = rd_start + (CURRENT->sector << 9);
	len = CURRENT->nr_sectors << 9;
// ������豸�Ų�Ϊ1 ���߶�Ӧ�ڴ���ʼλ��>������ĩβ������������󣬲���ת��repeat ��
// ��������28 �е�INIT_REQUEST �ڿ�ʼ������
	if ((MINOR (CURRENT->dev) != 1) || (addr + len > rd_start + rd_length))
	{
		end_request (0);
		goto repeat;
	}
// �����д����(WRITE)�����������л����������ݸ��Ƶ�addr ��������Ϊlen �ֽڡ�
	if (CURRENT->cmd == WRITE)
	{
		(void) memcpy (addr, CURRENT->buffer, len);
// ����Ƕ�����(READ)����addr ��ʼ�����ݸ��Ƶ��������л������У�����Ϊlen �ֽڡ�
	}
	else if (CURRENT->cmd == READ)
	{
		(void) memcpy (CURRENT->buffer, addr, len);
// ������ʾ������ڣ�������
	}
	else
		panic ("unknown ramdisk-command");
// ������ɹ������ø��±�־�������������豸����һ�����
	end_request (1);
	goto repeat;
}

/* �����ڴ�������ramdisk ������ڴ��� */
// �����̳�ʼ��������ȷ�����������ڴ��е���ʼ��ַ�����ȡ��������������������㡣
long
rd_init (long mem_start, int length)
{
	int i;
	char *cp;

	blk_dev[MAJOR_NR].request_fn = DEVICE_REQUEST;	// do_rd_request()��
	rd_start = (char *) mem_start;
	rd_length = length;
	cp = rd_start;
	for (i = 0; i < length; i++)
		*cp++ = '\0';
	return (length);
}

/*
* ������ļ�ϵͳ�豸(root device)��ramdisk �Ļ������Լ�������root device ԭ����ָ��
* ���̵ģ����ǽ����ĳ�ָ��ramdisk��
*/
//// ���ظ��ļ�ϵͳ��ramdisk��
void
rd_load (void)
{
	struct buffer_head *bh;
	struct super_block s;
	int block = 256;		/* Start at block 256 */
	int i = 1;
	int nblocks;
	char *cp;			/* Move pointer */

	if (!rd_length)		// ���ramdisk �ĳ���Ϊ�㣬���˳���
		return;
	printk ("Ram disk: %d bytes, starting at 0x%x\n", rd_length, (int) rd_start);	// ��ʾramdisk �Ĵ�С�Լ��ڴ���ʼλ�á�
	if (MAJOR (ROOT_DEV) != 2)	// �����ʱ���ļ��豸�������̣����˳���
		return;
// �����̿�256+1,256,256+2��breada()���ڶ�ȡָ�������ݿ飬���������Ҫ���Ŀ飬Ȼ�󷵻�
// �������ݿ�Ļ�����ָ�롣�������NULL�����ʾ���ݿ鲻�ɶ�(fs/buffer.c,322)��
// ����block+1 ��ָ�����ϵĳ����顣
	bh = breada (ROOT_DEV, block + 1, block, block + 2, -1);
	if (!bh)
	{
		printk ("Disk error while looking for ramdisk!\n");
		return;
	}
// ��s ָ�򻺳����еĴ��̳����顣(d_super_block �����г�����ṹ)��
	*((struct d_super_block *) &s) = *((struct d_super_block *) bh->b_data);
	brelse (bh);			// [?? Ϊʲô����û�и��ƾ������ͷ��أ�]
	if (s.s_magic != SUPER_MAGIC)	// �����������ħ�����ԣ���˵������minix �ļ�ϵͳ��
		return;/* ������û��ramdisk ӳ���ļ����˳�ִ��ͨ������������ */
// ���� = �߼�����(������) * 2^(ÿ���ο����Ĵη�)��
// ������ݿ��������ڴ����������������ɵĿ��������ܼ��أ���ʾ������Ϣ�����ء�������ʾ
// �������ݿ���Ϣ��
	nblocks = s.s_nzones << s.s_log_zone_size;
	if (nblocks > (rd_length >> BLOCK_SIZE_BITS))
	{
		printk ("Ram disk image too big! (%d blocks, %d avail)\n",
				nblocks, rd_length >> BLOCK_SIZE_BITS);
		return;
	}
	printk ("Loading %d bytes into ram disk... 0000k",
	nblocks << BLOCK_SIZE_BITS);
// cp ָ����������ʼ����Ȼ�󽫴����ϵĸ��ļ�ϵͳӳ���ļ����Ƶ��������ϡ�
	cp = rd_start;
	while (nblocks)
	{
		if (nblocks > 2)		// ������ȡ�Ŀ�������3 ������ó�ǰԤ����ʽ�����ݿ顣
			bh = breada (ROOT_DEV, block, block + 1, block + 2, -1);
		else			// ����͵����ȡ��
			bh = bread (ROOT_DEV, block);
		if (!bh)
		{
			printk ("I/O error on block %d, aborting load\n", block);
			return;
		}
		(void) memcpy (cp, bh->b_data, BLOCK_SIZE);	// ���������е����ݸ��Ƶ�cp ����
		brelse (bh);		// �ͷŻ�������
		printk ("\010\010\010\010\010%4dk", i);	// ��ӡ���ؿ����ֵ��
		cp += BLOCK_SIZE;		// ������ָ��ǰ�ơ�
		block++;
		nblocks--;
		i++;
	}
	printk ("\010\010\010\010\010done \n");
	ROOT_DEV = 0x0101;		// �޸�ROOT_DEV ʹ��ָ��������ramdisk��
}
