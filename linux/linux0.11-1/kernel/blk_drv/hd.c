/* passed
* linux/kernel/hd.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

/*
* �������ǵײ�Ӳ���жϸ���������Ҫ����ɨ�������б�ʹ���ж��ں���֮����ת��
* �������еĺ����������ж�����õģ�������Щ����������˯�ߡ����ر�ע�⡣
* ��Drew Eckhardt �޸ģ�����CMOS ��Ϣ���Ӳ������
*/

#include <linux/config.h>	// �ں�����ͷ�ļ�������������Ժ�Ӳ�����ͣ�HD_TYPE����ѡ�
#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/fs.h>		// �ļ�ϵͳͷ�ļ��������ļ���ṹ��file,buffer_head,m_inode �ȣ���
#include <linux/kernel.h>	// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/hdreg.h>	// Ӳ�̲���ͷ�ļ����������Ӳ�̼Ĵ����˿ڣ�״̬�룬���������Ϣ��
#include <asm/system.h>		// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ
#include <asm/io.h>		// io ͷ�ļ�������Ӳ���˿�����/���������䡣
#include <asm/segment.h>	// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

#define MAJOR_NR 3		// Ӳ�����豸����3��
#include "blk.h"		// ���豸ͷ�ļ��������������ݽṹ�����豸���ݽṹ�ͺ꺯������Ϣ��

// ��CMOS �����꺯����
_inline unsigned char CMOS_READ(unsigned char addr)
{
	outb_p(addr,0x70);
	return inb_p(0x71);
}

/* Max read/write errors/sector */
#define MAX_ERRORS 7		// ��/дһ������ʱ����������������
#define MAX_HD 2		// ϵͳ֧�ֵ����Ӳ������
static void recal_intr (void);	// Ӳ���жϳ����ڸ�λ����ʱ����õ�����У������(287 ��)��

static int recalibrate = 1;	// ����У����־��
static int reset = 1;	// ��λ��־��

/*
* This struct defines the HD's and their types.
*/
/* ����ṹ������Ӳ�̲��������� */
// ���ֶηֱ��Ǵ�ͷ����ÿ�ŵ�����������������дǰԤ��������š���ͷ��½������š������ֽڡ�
struct hd_i_struct
{
	int head, sect, cyl, wpcom, lzone, ctl;
};

#ifdef HD_TYPE			// ����Ѿ���include/linux/config.h �ж�����HD_TYPE��

struct hd_i_struct hd_info[] ={HD_TYPE};	// ȡ����õĲ�����Ϊhd_info[]�����ݡ�
#define NR_HD ((sizeof (hd_info))/(sizeof (struct hd_i_struct)))	// ����Ӳ������

#else // ���򣬶���Ϊ0 ֵ��

struct hd_i_struct hd_info[] =
{
	{0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0}
};
static int NR_HD = 0;

#endif

// ����Ӳ�̷����ṹ������ÿ��������������ʼ�����š���������������
// ����5 �ı������������hd[0]��hd[5]�ȣ���������Ӳ���еĲ�����
static struct hd_struct
{
	long start_sect;
	long nr_sects;
}

hd[5 * MAX_HD] =
{
	{0, 0},
};

// ���˿�port������nr �֣�������buf �С�
//#define port_read(port,buf,nr) \
//__asm__( "cld;rep;insw":: "d" (port), "D" (buf), "c" (nr): "cx", "di")
_inline void port_read(unsigned short port, void* buf,unsigned long nr)
{_asm{
	pushf
	mov dx,port
	mov edi,buf
	mov ecx,nr
	cld
	rep insw
	popf
}}

// д�˿�port����дnr �֣���buf ��ȡ���ݡ�
//#define port_write(port,buf,nr) \
//__asm__( "cld;rep;outsw":: "d" (port), "S" (buf), "c" (nr): "cx", "si")
_inline void port_write(unsigned short port, void* buf,unsigned long nr)
{_asm{
	pushf
	mov dx,port
	mov esi,buf
	mov ecx,nr
	cld
	rep outsw
	popf
}}

extern void hd_interrupt (void);
extern void rd_load (void);

/* This may be used only once, enforced by 'static int callable' */
/* ����ú���ֻ�ڳ�ʼ��ʱ������һ�Ρ��þ�̬����callable ��Ϊ�ɵ��ñ�־��*/
// �ú����Ĳ����ɳ�ʼ������init/main.c ��init �ӳ�������Ϊָ��0x90080 �����˴������setup.s
// �����BIOS ȡ�õ�2 ��Ӳ�̵Ļ���������(32 �ֽ�)��Ӳ�̲�������Ϣ�μ������б���˵����
// ��������Ҫ�����Ƕ�ȡCMOS ��Ӳ�̲�������Ϣ����������Ӳ�̷����ṹhd��������RAM �����̺�
// ���ļ�ϵͳ��
int sys_setup (unsigned char *BIOS)
{
	static int callable = 1;
	int i, drive;
	unsigned char cmos_disks;
	struct partition *p;
	struct buffer_head *bh;

// ��ʼ��ʱcallable=1�������иú���ʱ��������Ϊ0��ʹ������ֻ��ִ��һ�Ρ�
	if (!callable)
		return -1;
	callable = 0;
// ���û����config.h �ж���Ӳ�̲������ʹ�0x90080 �����롣
#ifndef HD_TYPE
	for (drive = 0; drive < 2; drive++)
	{
		hd_info[drive].cyl = *(unsigned short *) BIOS;	// ��������
		hd_info[drive].head = *(unsigned char *) (2 + BIOS);	// ��ͷ����
		hd_info[drive].wpcom = *(unsigned short *) (5 + BIOS);	// дǰԤ��������š�
		hd_info[drive].ctl = *(unsigned char *) (8 + BIOS);	// �����ֽڡ�
		hd_info[drive].lzone = *(unsigned short *) (12 + BIOS);	// ��ͷ��½������š�
		hd_info[drive].sect = *(unsigned char *) (14 + BIOS);	// ÿ�ŵ���������
		BIOS += 16;		// ÿ��Ӳ�̵Ĳ�����16 �ֽڣ�����BIOS ָ����һ����
	}
// setup.s ������ȡBIOS �е�Ӳ�̲�������Ϣʱ�����ֻ��1 ��Ӳ�̣��ͻὫ��Ӧ��2 ��Ӳ�̵�
// 16 �ֽ�ȫ�����㡣�������ֻҪ�жϵ�2 ��Ӳ���������Ƿ�Ϊ0 �Ϳ���֪����û�е�2 ��Ӳ���ˡ�
	if (hd_info[1].cyl)
		NR_HD = 2;		// Ӳ������Ϊ2��
	else
		NR_HD = 1;
#endif
// ����ÿ��Ӳ�̵���ʼ�����ź��������������б��i*5 ����μ����������й�˵����
	for (i = 0; i < NR_HD; i++)
	{
		hd[i * 5].start_sect = 0;	// Ӳ����ʼ�����š�
		hd[i * 5].nr_sects = hd_info[i].head * hd_info[i].sect * hd_info[i].cyl;	// Ӳ������������
	}

/*
* ���Ƕ�CMOS �й�Ӳ�̵���Ϣ��Щ���ɣ����ܻ���������������������һ��SCSI/ESDI/�ȵ�
* ��������������ST-506 ��ʽ��BIOS ���ݵģ��������������ǵ�BIOS �������У���ȴ�ֲ�
* �ǼĴ������ݵģ������Щ������CMOS ���ֲ����ڡ�
* ���⣬���Ǽ���ST-506 ������������еĻ�����ϵͳ�еĻ�����������Ҳ����������1 ��2
* ���ֵ���������
* ��1 �����������������CMOS �ֽ�0x12 �ĸ߰��ֽ��У���2 ������ڵͰ��ֽ��С���4 λ�ֽ�
* ��Ϣ���������������ͣ�Ҳ���ܽ���0xf��0xf ��ʾʹ��CMOS ��0x19 �ֽ���Ϊ������1 ��8 λ
* �����ֽڣ�ʹ��CMOS ��0x1A �ֽ���Ϊ������2 �������ֽڡ�
* ��֮��һ������ֵ��ζ��������һ��AT ������Ӳ�̼��ݵ���������
*/

// �����������ԭ�������Ӳ�̵����Ƿ���AT ���������ݵġ��й�CMOS ��Ϣ��μ�4.2.3.1 �ڡ�
	if ((cmos_disks = CMOS_READ (0x12)) & 0xf0)
		if (cmos_disks & 0x0f)
			NR_HD = 2;
		else
			NR_HD = 1;
	else
		NR_HD = 0;
// ��NR_HD=0��������Ӳ�̶�����AT ���������ݵģ�Ӳ�����ݽṹ���㡣
// ��NR_HD=1���򽫵�2 ��Ӳ�̵Ĳ������㡣
	for (i = NR_HD; i < 2; i++)
	{
		hd[i * 5].start_sect = 0;
		hd[i * 5].nr_sects = 0;
	}
// ��ȡÿһ��Ӳ���ϵ�1 �����ݣ���1 ���������ã�����ȡ���еķ�������Ϣ��
// �������ú���bread()��Ӳ�̵�1 ������(fs/buffer.c,267)�������е�0x300 ��Ӳ�̵����豸��
// (�μ��б���˵��)��Ȼ�����Ӳ��ͷ1 ������λ��0x1fe ���������ֽ��Ƿ�Ϊ'55AA'���ж�
// ��������λ��0x1BE ��ʼ�ķ������Ƿ���Ч����󽫷�������Ϣ����Ӳ�̷������ݽṹhd �С�
	for (drive = 0; drive < NR_HD; drive++)
	{
		if (!(bh = bread (0x300 + drive * 5, 0)))
		{			// 0x300, 0x305 �߼��豸�š�
			printk ("Unable to read partition table of drive %d\n\r", drive);
			panic ("");
		}
		if (bh->b_data[510] != 0x55 || (unsigned char) bh->b_data[511] != 0xAA)
		{			// �ж�Ӳ����Ϣ��Ч��־'55AA'��
			printk ("Bad partition table on drive %d\n\r", drive);
			panic ("");
		}
		p = (struct partition *)(0x1BE + bh->b_data);	// ������λ��Ӳ�̵�1 ������0x1BE ����
		for (i = 1; i < 5; i++, p++)
		{
			hd[i + 5 * drive].start_sect = p->start_sect;
			hd[i + 5 * drive].nr_sects = p->nr_sects;
		}
		brelse (bh);		// �ͷ�Ϊ���Ӳ�̿��������ڴ滺����ҳ��
	}
	if (NR_HD)			// �����Ӳ�̴��ڲ����Ѷ�����������ӡ������������Ϣ��
		printk ("Partition table%s ok.\n\r", (NR_HD > 1) ? "s" : "");
	rd_load ();			// ���أ�������RAMDISK(kernel/blk_drv/ramdisk.c,71)��
	mount_root ();		// ��װ���ļ�ϵͳ(fs/super.c,242)��
	return (0);
}

//// �жϲ�ѭ���ȴ�������������
// ��Ӳ�̿�����״̬�Ĵ����˿�HD_STATUS(0x1f7)����ѭ�������������������λ�Ϳ�����æλ��
static int controller_ready (void)
{
	int retries = 10000;

	while (--retries && (inb_p (HD_STATUS) & 0xc0) != 0x40);
		return (retries);		// ���صȴ�ѭ���Ĵ�����
}

//// ���Ӳ��ִ��������״̬��(win_��ʾ����˹��Ӳ�̵���д)
// ��ȡ״̬�Ĵ����е�����ִ�н��״̬������0 ��ʾ������1 �������ִ�������
// ���ٶ�����Ĵ���HD_ERROR(0x1f1)��
static int win_result (void)
{
	int i = inb_p (HD_STATUS);	// ȡ״̬��Ϣ��

	if ((i & (BUSY_STAT | READY_STAT | WRERR_STAT | SEEK_STAT | ERR_STAT))
		== (READY_STAT | SEEK_STAT))
		return (0);		/* ok */
	if (i & 1)
		i = inb (HD_ERROR);	// ��ERR_STAT ��λ�����ȡ����Ĵ�����
	return (1);
}

//// ��Ӳ�̿�������������飨�μ��б���˵������
// ���ò�����drive - Ӳ�̺�(0-1)�� nsect - ��д��������
// sect - ��ʼ������ head - ��ͷ�ţ�
// cyl - ����ţ� cmd - �����룻
// *intr_addr() - Ӳ���жϴ�������н����õ�C ��������
static void hd_out (unsigned int drive, unsigned int nsect, unsigned int sect,
		    unsigned int head, unsigned int cyl, unsigned int cmd,
		    void (*intr_addr) (void))
{
	register int port; //asm ("dx");	// port ������Ӧ�Ĵ���dx��

	if (drive > 1 || head > 15)	// �����������(0,1)>1 ���ͷ��>15�������֧�֡�
		panic ("Trying to write bad sector");
	if (!controller_ready ())	// ����ȴ�һ��ʱ�����δ���������������
		panic ("HD controller not ready");
	do_hd = intr_addr;		// do_hd ����ָ�뽫��Ӳ���жϳ����б����á�
	outb_p (hd_info[drive].ctl, HD_CMD);	// ����ƼĴ���(0x3f6)��������ֽڡ�
	port = HD_DATA;		// ��dx Ϊ���ݼĴ����˿�(0x1f0)��
	outb_p (hd_info[drive].wpcom >> 2, ++port);	// ������дԤ���������(���4)��
	outb_p (nsect, ++port);	// ��������/д����������
	outb_p (sect, ++port);	// ��������ʼ������
	outb_p (cyl, ++port);		// ����������ŵ�8 λ��
	outb_p (cyl >> 8, ++port);	// ����������Ÿ�8 λ��
	outb_p (0xA0 | (drive << 4) | head, ++port);	// ��������������+��ͷ�š�
	outb (cmd, ++port);		// ���Ӳ�̿������
}

//// �ȴ�Ӳ�̾�����Ҳ��ѭ���ȴ���״̬������æ��־λ��λ�������о�����Ѱ��������־
// ��λ����ɹ�������0��������һ��ʱ����Ϊæ���򷵻�1��
static int drive_busy (void)
{
	unsigned int i;

	for (i = 0; i < 10000; i++)	// ѭ���ȴ�������־λ��λ��
		if (READY_STAT == (inb_p (HD_STATUS) & (BUSY_STAT | READY_STAT)))
			break;
	i = inb (HD_STATUS);	// ��ȡ��������״̬�ֽڡ�
	i &= BUSY_STAT | READY_STAT | SEEK_STAT;	// ���æλ������λ��Ѱ������λ��
	if (i == READY_STAT | SEEK_STAT)	// �����о�����Ѱ��������־���򷵻�0��
		return (0);
	printk ("HD controller times out\n\r");	// ����ȴ���ʱ����ʾ��Ϣ��������1��
	return (1);
}

//// ��ϸ�λ������У����Ӳ�̿�������
static void reset_controller (void)
{
	int i;

	outb (4, HD_CMD);		// ����ƼĴ����˿ڷ��Ϳ����ֽ�(4-��λ)��
	for (i = 0; i < 100; i++)
		nop ();			// �ȴ�һ��ʱ�䣨ѭ���ղ�������
	outb (hd_info[0].ctl & 0x0f, HD_CMD);	// �ٷ��������Ŀ����ֽ�(����ֹ���ԡ��ض�)��
	if (drive_busy ())		// ���ȴ�Ӳ�̾�����ʱ������ʾ������Ϣ��
		printk ("HD-controller still busy\n\r");
	if ((i = inb (HD_ERROR)) != 1)	// ȡ����Ĵ�������������1���޴��������
		printk ("HD-controller reset failed: %02x\n\r", i);
}

//// ��λӲ��nr�����ȸ�λ������У����Ӳ�̿�������Ȼ����Ӳ�̿��������������������������
// ����recal_intr()����Ӳ���жϴ�������е��õ�����У����������
static void reset_hd (int nr)
{
	reset_controller ();
	hd_out (nr, hd_info[nr].sect, hd_info[nr].sect, hd_info[nr].head - 1,
			hd_info[nr].cyl, WIN_SPECIFY, &recal_intr);
}

//// ����Ӳ���жϵ��ú�����
// ��������Ӳ���ж�ʱ��Ӳ���жϴ�������е��õ�Ĭ��C ���������ڱ����ú���ָ��Ϊ��ʱ
// ���øú������μ�(kernel/system_call.s,241 ��)��
void unexpected_hd_interrupt (void)
{
	printk ("Unexpected HD interrupt\n\r");
}

//// ��дӲ��ʧ�ܴ�����ú�����
static void bad_rw_intr (void)
{
	if (++CURRENT->errors >= MAX_ERRORS)	// ���������ʱ�ĳ���������ڻ����7 ��ʱ��
		end_request (0);		// ��������󲢻��ѵȴ�������Ľ��̣�����
// ��Ӧ���������±�־��λ��û�и��£���
	if (CURRENT->errors > MAX_ERRORS / 2)	// �����һ����ʱ�ĳ�������Ѿ�����3 �Σ�
		reset = 1;			// ��Ҫ��ִ�и�λӲ�̿�����������
}

//// �������жϵ��ú���������ִ��Ӳ���жϴ�������б����á�
static void read_intr (void)
{
	if (win_result ())
	{				// ��������æ����д�������ִ�д�
		bad_rw_intr ();		// ����ж�дӲ��ʧ�ܴ���
		do_hd_request ();		// Ȼ���ٴ�����Ӳ������Ӧ(��λ)����
		return;
	}
	port_read (HD_DATA, CURRENT->buffer, 256);	// �����ݴ����ݼĴ����ڶ�������ṹ��������
	CURRENT->errors = 0;		// ����������
	CURRENT->buffer += 512;	// ����������ָ�룬ָ���µĿ�����
	CURRENT->sector++;		// ��ʼ�����ż�1��
	if (--CURRENT->nr_sectors)
	{				// ��������������������û�ж��꣬��
		do_hd = &read_intr;	// �ٴ���Ӳ�̵���C ����ָ��Ϊread_intr()
		return;			// ��ΪӲ���жϴ������ÿ�ε���do_hd ʱ
	}				// ���Ὣ�ú���ָ���ÿա��μ�system_call.s
	end_request (1);		// ��ȫ�����������Ѿ����꣬��������������ˣ�
	do_hd_request ();		// ִ������Ӳ�����������
}

//// д�����жϵ��ú�������Ӳ���жϴ�������б����á�
// ��д����ִ�к󣬻����Ӳ���ж��źţ�ִ��Ӳ���жϴ�����򣬴�ʱ��Ӳ���жϴ�������е��õ�
// C ����ָ��do_hd()�Ѿ�ָ��write_intr()����˻���д������ɣ��������ִ�иú�����
static void write_intr (void)
{
	if (win_result ())
	{				// ���Ӳ�̿��������ش�����Ϣ��
		bad_rw_intr ();		// �����Ƚ���Ӳ�̶�дʧ�ܴ���
		do_hd_request ();		// Ȼ���ٴ�����Ӳ������Ӧ(��λ)����
		return;			// Ȼ�󷵻أ�Ҳ�˳��˴˴�Ӳ���жϣ���
	}
	if (--CURRENT->nr_sectors)
	{				// ������д��������1������������Ҫд����
		CURRENT->sector++;	// ��ǰ������ʼ������+1��
		CURRENT->buffer += 512;	// �������󻺳���ָ�룬
		do_hd = &write_intr;	// ��Ӳ���жϳ�����ú���ָ��Ϊwrite_intr()��
		port_write (HD_DATA, CURRENT->buffer, 256);	// �������ݼĴ����˿�д256 �ֽڡ�
		return;			// ���صȴ�Ӳ���ٴ����д��������жϴ���
	}
	end_request (1);		// ��ȫ�����������Ѿ�д�꣬��������������ˣ�
	do_hd_request ();		// ִ������Ӳ�����������
}

//// Ӳ������У������λ���жϵ��ú�������Ӳ���жϴ�������б����á�
// ���Ӳ�̿��������ش�����Ϣ�������Ƚ���Ӳ�̶�дʧ�ܴ���Ȼ������Ӳ������Ӧ(��λ)����
static void recal_intr (void)
{
	if (win_result ())
		bad_rw_intr ();
	do_hd_request ();
}

// ִ��Ӳ�̶�д���������
void do_hd_request (void)
{
	int i, r;
	unsigned int block, dev;
	unsigned int sec, head, cyl;
	unsigned int nsect;

	INIT_REQUEST;		// ���������ĺϷ���(�μ�kernel/blk_drv/blk.h,127)��
// ȡ�豸���е����豸��(���б���Ӳ���豸�ŵ�˵��)�����豸�ż���Ӳ���ϵķ����š�
	dev = MINOR (CURRENT->dev);	// CURRENT ����Ϊ(blk_dev[MAJOR_NR].current_request)��
	block = CURRENT->sector;	// �������ʼ������
// ������豸�Ų����ڻ�����ʼ�������ڸ÷���������-2������������󣬲���ת�����repeat ��
// ��������INIT_REQUEST ��ʼ��������Ϊһ��Ҫ���д2 ��������512*2 �ֽڣ������������������
// ���ܴ��ڷ�����������ڶ��������š�
	if (dev >= 5 * NR_HD || block + 2 > hd[dev].nr_sects)
	{
		end_request (0);
		goto repeat;		// �ñ����blk.h ����档
	}
	block += hd[dev].start_sect;	// ��������Ŀ��Ӧ������Ӳ���ϵľ��������š�
	dev /= 5;			// ��ʱdev ����Ӳ�̺ţ�0 ��1����
// ����Ƕ�������������Ӳ����Ϣ�ṹ�и�����ʼ�����ź�ÿ�ŵ������������ڴŵ��е�
// ������(sec)�����������(cyl)�ʹ�ͷ��(head)��
	sec = hd_info[dev].sect;
	_asm {
		mov eax,block
		xor edx,edx
		mov ebx,sec
		div ebx
		mov block,eax
		mov sec,edx
	}
//__asm__ ("divl %4": "=a" (block), "=d" (sec):"" (block), "1" (0),
//	   "r" (hd_info[dev].
//		sect));
	head = hd_info[dev].head;
	_asm {
		mov eax,block
		xor edx,edx
		mov ebx,head
		div ebx
		mov cyl,eax
		mov head,edx
	}
//__asm__ ("divl %4": "=a" (cyl), "=d" (head):"" (block), "1" (0),
//	   "r" (hd_info[dev].
//		head));
	sec++;
	nsect = CURRENT->nr_sectors;	// ����/д����������
// ���reset ��1����ִ�и�λ��������λӲ�̺Ϳ�������������Ҫ����У����־�����ء�
	if (reset)
	{
		reset = 0;
		recalibrate = 1;
		reset_hd (CURRENT_DEV);
		return;
	}
// �������У����־(recalibrate)��λ�������ȸ�λ�ñ�־��Ȼ����Ӳ�̿�������������У�����
	if (recalibrate)
	{
		recalibrate = 0;
		hd_out (dev, hd_info[CURRENT_DEV].sect, 0, 0, 0,
		  WIN_RESTORE, &recal_intr);
		return;
	}
// �����ǰ������д��������������д���ѭ����ȡ״̬�Ĵ�����Ϣ���ж���������־
// DRQ_STAT �Ƿ���λ��DRQ_STAT ��Ӳ��״̬�Ĵ������������λ��include/linux/hdreg.h��27����
	if (CURRENT->cmd == WRITE)
	{
		hd_out (dev, nsect, sec, head, cyl, WIN_WRITE, &write_intr);
		for (i = 0; i < 3000 && !(r = inb_p (HD_STATUS) & DRQ_STAT); i++)
// ����������λ��λ���˳�ѭ�������ȵ�ѭ������Ҳû����λ����˴�дӲ�̲���ʧ�ܣ�ȥ����
// ��һ��Ӳ�����󡣷�����Ӳ�̿��������ݼĴ����˿�HD_DATA д��1 �����������ݡ�
		if (!r)
		{
			bad_rw_intr ();
			goto repeat;		// �ñ����blk.h ����棬Ҳ������301 �С�
		}
		port_write (HD_DATA, CURRENT->buffer, 256);
// �����ǰ�����Ƕ�Ӳ������������Ӳ�̿��������Ͷ��������
	}
	else if (CURRENT->cmd == READ)
	{
		hd_out (dev, nsect, sec, head, cyl, WIN_READ, &read_intr);
	}
	else
		panic ("unknown hd-command");
}

// Ӳ��ϵͳ��ʼ����
void hd_init (void)
{
	blk_dev[MAJOR_NR].request_fn = DEVICE_REQUEST;	// do_hd_request()��
	set_intr_gate (0x2E, &hd_interrupt);	// ����Ӳ���ж������� int 0x2E(46)��
// hd_interrupt ��(kernel/system_call.s,221)��
	outb_p (inb_p (0x21) & 0xfb, 0x21);	// ��λ��������8259A int2 ������λ�������Ƭ
// �����ж������źš�
	outb (inb_p (0xA1) & 0xbf, 0xA1);	// ��λӲ�̵��ж���������λ���ڴ�Ƭ�ϣ�������
// Ӳ�̿����������ж������źš�
}
