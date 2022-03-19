#ifndef _CONFIG_H
#define _CONFIG_H

/*
* The root-device is no longer hard-coded. You can change the default
* root-device by changing the line ROOT_DEV = XXX in boot/bootsect.s
*/
/*
* ���ļ�ϵͳ�豸�Ѳ�����Ӳ������ˡ�ͨ���޸�boot/bootsect.s �ļ�����
* ROOT_DEV = XXX������Ըı���豸��Ĭ������ֵ��
*/

/*
* define your keyboard here -
* KBD_FINNISH for Finnish keyboards
* KBD_US for US-type
* KBD_GR for German keyboards
* KBD_FR for Frech keyboard
*/
/*
* �����ﶨ����ļ������� -
* KBD_FINNISH �Ƿ������̡�
* KBD_US ����ʽ���̡�
* KBD_GR �ǵ�ʽ���̡�
* KBD_FR �Ƿ�ʽ���̡�
*/
#define KBD_US 
/*#define KBD_GR */
/*#define KBD_FR */
/*#define KBD_FINNISH*/

/*
* Normally, Linux can get the drive parameters from the BIOS at
* startup, but if this for some unfathomable reason fails, you'd
* be left stranded. For this case, you can define HD_TYPE, which
* contains all necessary info on your harddisk.
*
* The HD_TYPE macro should look like this:
*
* #define HD_TYPE { head, sect, cyl, wpcom, lzone, ctl}
*
* In case of two harddisks, the info should be sepatated by
* commas:
*
* #define HD_TYPE { h,s,c,wpcom,lz,ctl },{ h,s,c,wpcom,lz,ctl }
*/
/*
* ͨ����Linux �ܹ�������ʱ��BIOS �л�ȡ�������²���������������δ֪ԭ��
* ��û�еõ���Щ����ʱ����ʹ���������޲ߡ������������������Զ���HD_TYPE��
* ���а���Ӳ�̵�������Ϣ��
*
* HD_TYPE ��Ӧ����������������ʽ��
*
* #define HD_TYPE { head, sect, cyl, wpcom, lzone, ctl}
*
* ����������Ӳ�̵������������Ϣ���ö��ŷֿ���
*
* #define HD_TYPE { h,s,c,wpcom,lz,ctl }, {h,s,c,wpcom,lz,ctl }
*/
/*
This is an example, two drives, first is type 2, second is type 3:

#define HD_TYPE { 4,17,615,300,615,8 }, { 6,17,615,300,615,0 }

NOTE: ctl is 0 for all drives with heads<=8, and ctl=8 for drives
with more than 8 heads.

If you want the BIOS to tell what kind of drive you have, just
leave HD_TYPE undefined. This is the normal thing to do.
*/
/*
* ������һ�����ӣ�����Ӳ�̣���1 ��������2����2 ��������3��
*
11.23 fdreg.h ͷ�ļ�
* #define HD_TYPE { 4,17,615,300,615,8 }, {6,17,615,300,615,0 }
*
* ע�⣺��Ӧ����Ӳ�̣������ͷ��<=8����ctl ����0������ͷ������8 ����
* ��ctl=8��
*
* ���������BIOS ����Ӳ�̵����ͣ���ôֻ�費����HD_TYPE������Ĭ�ϲ�����
*/

#endif
