/* passed
* linux/fs/truncate.c
*
* (C) 1991 Linus Torvalds
*/
#include <set_seg.h>

#include <linux/sched.h>	// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣

#include <sys/stat.h>		// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

//// �ͷ�һ�μ�ӿ顣
static void free_ind (int dev, int block)
{
  struct buffer_head *bh;
  unsigned short *p;
  int i;

// ����߼����Ϊ0���򷵻ء�
  if (!block)
    return;
// ��ȡһ�μ�ӿ飬���ͷ����ϱ���ʹ�õ������߼��飬Ȼ���ͷŸ�һ�μ�ӿ�Ļ�������
  if (bh = bread (dev, block))
    {
      p = (unsigned short *) bh->b_data;	// ָ�����ݻ�������
      for (i = 0; i < 512; i++, p++)	// ÿ���߼����Ͽ���512 ����š�
	if (*p)
	  free_block (dev, *p);	// �ͷ�ָ�����߼��顣
      brelse (bh);		// �ͷŻ�������
    }
//�����ֶ�
//i_zone[0]
//i_zone[1]
//i_zone[2]
//i_zone[3]
//i_zone[4]
//i_zone[5]
//i_zone[6]
//i �ڵ�
//ֱ�ӿ��
//һ�μ�ӿ�
//���μ�ӿ�
//��һ����
//���μ�ӿ�
//�Ķ�����
//һ�μ�ӿ��
//���μ�ӿ��
//i_zone[7]
//i_zone[8]
// �ͷ��豸�ϵ�һ�μ�ӿ顣
  free_block (dev, block);
}

//// �ͷŶ��μ�ӿ顣
static void
free_dind (int dev, int block)
{
  struct buffer_head *bh;
  unsigned short *p;
  int i;

// ����߼����Ϊ0���򷵻ء�
  if (!block)
    return;
// ��ȡ���μ�ӿ��һ���飬���ͷ����ϱ���ʹ�õ������߼��飬Ȼ���ͷŸ�һ����Ļ�������
  if (bh = bread (dev, block))
    {
      p = (unsigned short *) bh->b_data;	// ָ�����ݻ�������
      for (i = 0; i < 512; i++, p++)	// ÿ���߼����Ͽ�����512 �������顣
	if (*p)
	  free_ind (dev, *p);	// �ͷ�����һ�μ�ӿ顣
      brelse (bh);		// �ͷŻ�������
    }
// ����ͷ��豸�ϵĶ��μ�ӿ顣
  free_block (dev, block);
}

//// ���ڵ��Ӧ���ļ����Ƚ�Ϊ0�����ͷ�ռ�õ��豸�ռ䡣
void
truncate (struct m_inode *inode)
{
  int i;

// ������ǳ����ļ�������Ŀ¼�ļ����򷵻ء�
  if (!(S_ISREG (inode->i_mode) || S_ISDIR (inode->i_mode)))
    return;
// �ͷ�i �ڵ��7 ��ֱ���߼��飬������7 ���߼�����ȫ���㡣
  for (i = 0; i < 7; i++)
    if (inode->i_zone[i])
	{				// �����Ų�Ϊ0�����ͷ�֮��
		free_block (inode->i_dev, inode->i_zone[i]);
		inode->i_zone[i] = 0;
	}
  free_ind (inode->i_dev, inode->i_zone[7]);	// �ͷ�һ�μ�ӿ顣
  free_dind (inode->i_dev, inode->i_zone[8]);	// �ͷŶ��μ�ӿ顣
  inode->i_zone[7] = inode->i_zone[8] = 0;	// �߼�����7��8 ���㡣
  inode->i_size = 0;		// �ļ���С���㡣
  inode->i_dirt = 1;		// �ýڵ����޸ı�־��
  inode->i_mtime = inode->i_ctime = CURRENT_TIME;	// �����ļ��ͽڵ��޸�ʱ��Ϊ��ǰʱ�䡣
}
