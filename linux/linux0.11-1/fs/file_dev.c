/* passed
 *  linux/fs/file_dev.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

#include <errno.h>// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)
#include <fcntl.h>// �ļ�����ͷ�ļ��������ļ������������Ĳ������Ƴ������ŵĶ��塣

#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
									// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/segment.h>// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

#define MIN(a,b) (((a)<(b))?(a):(b))	// ȡa,b �е���Сֵ��
#define MAX(a,b) (((a)>(b))?(a):(b))	// ȡa,b �е����ֵ��

//// �ļ�������- ����i �ڵ���ļ��ṹ�����豸���ݡ�
// ��i �ڵ����֪���豸�ţ���filp �ṹ����֪���ļ��е�ǰ��дָ��λ�á�buf ָ���û�̬��
// ��������λ�ã�count Ϊ��Ҫ��ȡ���ֽ���������ֵ��ʵ�ʶ�ȡ���ֽ�����������(С��0)��
int file_read(struct m_inode * inode, struct file * filp, char * buf, int count)
{
	int left,chars,nr;
	struct buffer_head * bh;

// ����Ҫ��ȡ���ֽڼ���ֵС�ڵ����㣬�򷵻ء�
	if ((left=count)<=0)
		return 0;
// ������Ҫ��ȡ���ֽ���������0����ѭ��ִ�����²�����ֱ��ȫ��������
	while (left) {
// ����i �ڵ���ļ���ṹ��Ϣ��ȡ���ݿ��ļ���ǰ��дλ�����豸�϶�Ӧ���߼����nr����nr ��
// Ϊ0�����i �ڵ�ָ�����豸�϶�ȡ���߼��飬���������ʧ�����˳�ѭ������nr Ϊ0����ʾָ��
// �����ݿ鲻���ڣ��û����ָ��ΪNULL��
		if (nr = bmap(inode,(filp->f_pos)/BLOCK_SIZE)) {
			if (!(bh=bread(inode->i_dev,nr)))
				break;
		} else
			bh = NULL;
// �����ļ���дָ�������ݿ��е�ƫ��ֵnr����ÿ��пɶ��ֽ���Ϊ(BLOCK_SIZE-nr)��Ȼ���뻹��
// ��ȡ���ֽ���left ���Ƚϣ�����Сֵ��Ϊ����������ֽ���chars����(BLOCK_SIZE-nr)����˵��
// �ÿ�����Ҫ��ȡ�����һ�����ݣ���֮����Ҫ��ȡһ�����ݡ�
		nr = filp->f_pos % BLOCK_SIZE;
		chars = MIN( BLOCK_SIZE-nr , left );
// ������д�ļ�ָ�롣ָ��ǰ�ƴ˴ν���ȡ���ֽ���chars��ʣ���ֽڼ�����Ӧ��ȥchars��
		filp->f_pos += chars;
		left -= chars;
// �����豸�϶��������ݣ���p ָ��������ݿ黺�����п�ʼ��ȡ��λ�ã����Ҹ���chars �ֽ�
// ���û�������buf �С��������û�������������chars ��0 ֵ�ֽڡ�
		if (bh) {
			char * p = nr + bh->b_data;
			while (chars-->0)
				put_fs_byte(*(p++),buf++);
			brelse(bh);
		} else {
			while (chars-->0)
				put_fs_byte(0,buf++);
		}
	}
// �޸ĸ�i �ڵ�ķ���ʱ��Ϊ��ǰʱ�䡣���ض�ȡ���ֽ���������ȡ�ֽ���Ϊ0���򷵻س���š�
	inode->i_atime = CURRENT_TIME;
	return (count-left)?(count-left):-ERROR;
}

//// �ļ�д����- ����i �ڵ���ļ��ṹ��Ϣ�����û�����д��ָ���豸��
// ��i �ڵ����֪���豸�ţ���filp �ṹ����֪���ļ��е�ǰ��дָ��λ�á�buf ָ���û�̬��
// ��������λ�ã�count Ϊ��Ҫд����ֽ���������ֵ��ʵ��д����ֽ�����������(С��0)��
int file_write(struct m_inode * inode, struct file * filp, char * buf, int count)
{
	off_t pos;
	int block,c;
	struct buffer_head * bh;
	char * p;
	int i=0;

/*
 * ok����������ͬʱдʱ��append �������ܲ��У�����������������������������
 * ���»���һ�š�
 */
// �����Ҫ���ļ���������ݣ����ļ���дָ���Ƶ��ļ�β��������ͽ����ļ���дָ�봦д�롣
	if (filp->f_flags & O_APPEND)
		pos = inode->i_size;
	else
		pos = filp->f_pos;
// ����д���ֽ���i С����Ҫд����ֽ���count����ѭ��ִ�����²�����
	while (i<count) {
// �������ݿ��(pos/BLOCK_SIZE)���豸�϶�Ӧ���߼��飬���������豸�ϵ��߼���š�����߼�
// ���=0�����ʾ����ʧ�ܣ��˳�ѭ����
		if (!(block = create_block(inode,pos/BLOCK_SIZE)))
			break;
// ���ݸ��߼���Ŷ�ȡ�豸�ϵ���Ӧ���ݿ飬���������˳�ѭ����
		if (!(bh=bread(inode->i_dev,block)))
			break;
// ����ļ���дָ�������ݿ��е�ƫ��ֵc����p ָ��������ݿ黺�����п�ʼ��ȡ��λ�á��ø�
// ���������޸ı�־��
		c = pos % BLOCK_SIZE;
		p = c + bh->b_data;
		bh->b_dirt = 1;
// �ӿ�ʼ��дλ�õ���ĩ����д��c=(BLOCK_SIZE-c)���ֽڡ���c ����ʣ�໹��д����ֽ���
// (count-i)����˴�ֻ����д��c=(count-i)���ɡ�
		c = BLOCK_SIZE-c;
		if (c > count-i) c = count-i;
// �ļ���дָ��ǰ�ƴ˴���д����ֽ����������ǰ�ļ���дָ��λ��ֵ�������ļ��Ĵ�С����
// �޸�i �ڵ����ļ���С�ֶΣ�����i �ڵ����޸ı�־��
		pos += c;
		if (pos > inode->i_size) {
			inode->i_size = pos;
			inode->i_dirt = 1;
		}
// ��д���ֽڼ����ۼӴ˴�д����ֽ���c�����û�������buf �и���c ���ֽڵ����ٻ�������p
// ָ��ʼ��λ�ô���Ȼ���ͷŸû�������
		i += c;
		while (c-->0)
			*(p++) = get_fs_byte(buf++);
		brelse(bh);
	}
// �����ļ��޸�ʱ��Ϊ��ǰʱ�䡣
	inode->i_mtime = CURRENT_TIME;
// ����˴β����������ļ�β������ݣ�����ļ���дָ���������ǰ��дλ�ã�������i �ڵ��޸�
// ʱ��Ϊ��ǰʱ�䡣
	if (!(filp->f_flags & O_APPEND)) {
		filp->f_pos = pos;
		inode->i_ctime = CURRENT_TIME;
	}
// ����д����ֽ�������д���ֽ���Ϊ0���򷵻س����-1��
	return (i?i:-1);
}
