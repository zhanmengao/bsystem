/* passed
 *  linux/fs/file_table.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

// �ļ�ϵͳͷ�ļ��������ļ���ṹ��file,buffer_head,m_inode �ȣ���
#include <linux/fs.h>

struct file file_table[NR_FILE] = {0};// �ļ�������(64 ��)��
