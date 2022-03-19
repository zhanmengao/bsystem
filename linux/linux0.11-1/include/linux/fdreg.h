/*
 * ���ļ��к���һЩ���̿�������һЩ���塣��Щ��Ϣ�жദ��Դ�������ȡ��Sanches ��Canton
 * ������"IBM ΢�ͼ����������Ա�ֲ�"һ�顣
 */
#ifndef _FDREG_H	// �ö��������ų��������ظ�������ͷ�ļ���
#define _FDREG_H

// һЩ�������ͺ�����ԭ��˵����
extern int ticks_to_floppy_on(unsigned int nr);
extern void floppy_on(unsigned int nr);
extern void floppy_off(unsigned int nr);
extern void floppy_select(unsigned int nr);
extern void floppy_deselect(unsigned int nr);

// �������й����̿�����һЩ�˿ںͷ��ŵĶ��塣
/* ���̿�����(FDC)�������˿ڡ�ժ��S&C ����Լ340 ҳ */
#define FD_STATUS	0x3f4		// ��״̬�Ĵ����˿ڡ�
#define FD_DATA		0x3f5		// ���ݶ˿ڡ�
#define FD_DOR		0x3f2		/* Digital Output Register */
								// ��������Ĵ�����Ҳ��Ϊ���ֿ��ƼĴ�������
#define FD_DIR		0x3f7		/* Digital Input Register (read) */
								// ��������Ĵ�����
#define FD_DCR		0x3f7		/* Diskette Control Register (write)*/
								// ���ݴ����ʿ��ƼĴ�����

/* ��״̬�Ĵ���������λ�ĺ��� */
#define STATUS_BUSYMASK	0x0F		/* drive busy mask */
									// ������æλ��ÿλ��Ӧһ������������
#define STATUS_BUSY	0x10		/* FDC busy */ 
								// ���̿�����æ��
#define STATUS_DMA	0x20		/* 0- DMA mode */ 
								// 0 - ΪDMA ���ݴ���ģʽ��1 - Ϊ��DMA ģʽ��
#define STATUS_DIR	0x40		/* 0- cpu->fdc */
								// ���䷽��0 - CPU -> fdc��1 - �෴��
#define STATUS_READY 0x80		/* Data reg ready */
								// ���ݼĴ�������λ��

/* ״̬�ֽ�0��ST0��������λ�ĺ��� */
#define ST0_DS		0x03		/* drive select mask */
								// ������ѡ��ţ������ж�ʱ�������ţ���
#define ST0_HA		0x04		/* Head (Address) */
								// ��ͷ�š�
#define ST0_NR		0x08		/* Not Ready */
								// ����������δ׼���á�
#define ST0_ECE		0x10		/* Equipment chech error */
								// �豸��������ŵ�У׼������
#define ST0_SE		0x20		/* Seek end */
								// Ѱ��������У������ִ�н�����
#define ST0_INTR	0xC0		/* Interrupt code mask */
								// �жϴ���λ���ж�ԭ�򣩣�00 - ��������������
							// 01 - �����쳣������10 - ������Ч��11 - FDD ����״̬�ı䡣

/* ״̬�ֽ�1��ST1��������λ�ĺ��� */
#define ST1_MAM		0x01		/* Missing Address Mark */
								// δ�ҵ���ַ��־(ID AM)��
#define ST1_WP		0x02		/* Write Protect */
								// д������
#define ST1_ND		0x04		/* No Data - unreadable */
								// δ�ҵ�ָ����������
#define ST1_OR		0x10		/* OverRun */
								// ���ݴ��䳬ʱ��DMA ���������ϣ���
#define ST1_CRC		0x20		/* CRC error in data or addr */
								// CRC �������
#define ST1_EOC		0x80		/* End Of Cylinder */
								// ���ʳ���һ���ŵ��ϵ���������š�

/* ״̬�ֽ�2��ST2��������λ�ĺ��� */
#define ST2_MAM		0x01		/* Missing Addess Mark (again) */
								// δ�ҵ����ݵ�ַ��־��
#define ST2_BC		0x02		/* Bad Cylinder */
								// �ŵ�����
#define ST2_SNS		0x04		/* Scan Not Satisfied */
								// ������ɨ�裩���������㡣
#define ST2_SEH		0x08		/* Scan Equal Hit */
								// �����������㡣
#define ST2_WC		0x10		/* Wrong Cylinder */
								// �ŵ������棩�Ų�����
#define ST2_CRC		0x20		/* CRC error in data field */
								// ���ݳ�CRC У���
#define ST2_CM		0x40		/* Control Mark = deleted */
								// ����������ɾ����־��

/* ״̬�ֽ�3��ST3��������λ�ĺ��� */
#define ST3_HA		0x04		/* Head (Address) */
								// ��ͷ�š�
#define ST3_TZ		0x10		/* Track Zero signal (1=track 0) */
								// ��ŵ��źš�
#define ST3_WP		0x40		/* Write Protect */
								// д������

/* ���������� */
#define FD_RECALIBRATE	0x07		/* move to track 0 */
									// ����У��(��ͷ�˵���ŵ�)��
#define FD_SEEK		0x0F		/* seek track */
								// ��ͷѰ����
#define FD_READ		0xE6		/* read with MT, MFM, SKip deleted */
								// �����ݣ�MT ��ŵ�������MFM ��ʽ������ɾ�����ݣ���
#define FD_WRITE	0xC5		/* write with MT, MFM */
								// д���ݣ�MT��MFM����
#define FD_SENSEI	0x08		/* Sense Interrupt Status */
								// ����ж�״̬��
#define FD_SPECIFY	0x03		/* specify HUT etc */
								// �趨�������������������ʡ���ͷж��ʱ��ȣ���

/* DMA ���� */
#define DMA_READ	0x46		// DMA ���̣�DMA ��ʽ�֣���DMA �˿�12��11����
#define DMA_WRITE	0x4A		// DMA д�̣�DMA ��ʽ�֡�

#endif
