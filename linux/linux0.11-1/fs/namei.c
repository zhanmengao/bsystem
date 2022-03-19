/* passed
 *  linux/fs/namei.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

/*
 * tytso ����һЩ������
 */

#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
							// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <asm/segment.h>// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

#include <string.h>// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����
#include <fcntl.h>// �ļ�����ͷ�ļ��������ļ������������Ĳ������Ƴ������ŵĶ��塣
#include <errno.h>// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <const.h>// ��������ͷ�ļ���Ŀǰ��������i �ڵ���i_mode �ֶεĸ���־λ��
#include <sys/stat.h>// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����

// ����ģʽ�ꡣx ��include/fcntl.h ��7 �п�ʼ������ļ����ʱ�־��
// ����x ֵ������Ӧ��ֵ����ֵ��ʾrwx Ȩ��: r, w, rw, wxrwxrwx��(��ֵ��8 ����)��
#define ACC_MODE(x) ("\004\002\006\377"[(x)&O_ACCMODE])

/*
 * ��������ļ�������>NAME_LEN ���ַ����ص����ͽ����涨��ע�͵���
 */
/* #define NO_TRUNCATE */

#define MAY_EXEC 1		// ��ִ��(�ɽ���)��
#define MAY_WRITE 2		// ��д��
#define MAY_READ 4		// �ɶ���

/*
 *	permission()
 *
 * �ú������ڼ��һ���ļ��Ķ�/д/ִ��Ȩ�ޡ��Ҳ�֪���Ƿ�ֻ����euid������
 * ��Ҫ���euid ��uid ���ߣ�������������޸ġ�
 */
//// ����ļ��������Ȩ�ޡ�
// ������inode - �ļ���Ӧ��i �ڵ㣻mask - �������������롣
// ���أ�������ɷ���1�����򷵻�0��
static int permission(struct m_inode * inode,int mask)
{
	int mode = inode->i_mode;// �ļ���������

/* �����������ʹ�ǳ����û�(root)Ҳ���ܶ�/дһ���ѱ�ɾ�����ļ� */
// ���i �ڵ��ж�Ӧ���豸������i �ڵ������������0���򷵻ء�
	if (inode->i_dev && !inode->i_nlinks)
		return 0;
// ����������̵���Ч�û�id(euid)��i �ڵ���û�id ��ͬ����ȡ�ļ��������û�����Ȩ�ޡ�
	else if (current->euid==inode->i_uid)
		mode >>= 6;
// ����������̵���Ч��id(egid)��i �ڵ����id ��ͬ����ȡ���û��ķ���Ȩ�ޡ�
	else if (current->egid==inode->i_gid)
		mode >>= 3;
// ���������ȡ�ĵķ���Ȩ������������ͬ�������ǳ����û����򷵻�1�����򷵻�0��
	if (((mode & mask & 0007) == mask) || suser()) // suser()��linux/kernel.h��
		return 1;
	return 0;
}

/*
 * ok�����ǲ���ʹ��strncmp �ַ����ȽϺ�������Ϊ���Ʋ������ǵ����ݿռ�(�����ں˿ռ�)��
 * �������ֻ��ʹ��match()�����ⲻ��match()ͬ��Ҳ����һЩ�����Ĳ��ԡ�
 *
 * ע�⣡��strncmp ��ͬ����match()�ɹ�ʱ����1��ʧ��ʱ����0��
 */
//// ָ�������ַ����ȽϺ�����
// ������len - �Ƚϵ��ַ������ȣ�name - �ļ���ָ�룻de - Ŀ¼��ṹ��
// ���أ���ͬ����1����ͬ����0��
static int match(int len,const char * name,struct dir_entry * de)
{
	register int same; //__asm__("ax")
	char *de_name;

// ���Ŀ¼��ָ��գ�����Ŀ¼��i �ڵ����0������Ҫ�Ƚϵ��ַ������ȳ����ļ������ȣ��򷵻�0��
	if (!de || !de->inode || len > NAME_LEN)
		return 0;
// ���Ҫ�Ƚϵĳ���len С��NAME_LEN������Ŀ¼�����ļ������ȳ���len���򷵻�0��
	if (len < NAME_LEN && de->name[len])
		return 0;
// ����Ƕ������䣬���û����ݿռ�(fs)ִ���ַ����ıȽϲ�����
// %0 - eax(�ȽϽ��same)��%1 - eax(eax ��ֵ0)��%2 - esi(����ָ��)��%3 - edi(Ŀ¼����ָ��)��
// %4 - ecx(�Ƚϵ��ֽڳ���ֵlen)��
/*	__asm__("cld\n\t"				// �巽��λ��
		"fs ; repe ; cmpsb\n\t"		// �û��ռ�ִ��ѭ���Ƚ�[esi++]��[edi++]������
		"setz %%al"					// ���ȽϽ��һ��(z=0)������al=1(same=eax)��
		:"=a" (same)
		:"0" (0),"S" ((long) name),"D" ((long) de->name),"c" (len)
		:"cx","di","si");*/
	de_name = de->name;
	_asm{
		pushf
		xor eax,eax
		mov esi,name
		mov edi,de_name
		mov ecx,len
		cld		// �巽��λ��
		// �û��ռ�ִ��ѭ���Ƚ�[esi++]��[edi++]������
		repe cmps byte ptr fs:[edi],[esi]
		//�����Ӧ���Ǵ���ģ����Ҳ�֪����ô�ġ�����ϵͳ��������:)
		setz al			// ���ȽϽ��һ��(z=0)������al=1(same=eax)��
		mov same,eax
		popf
	}
	return same;			// ���رȽϽ����
}

/*
 *	find_entry()
 *
 * ��ָ����Ŀ¼��Ѱ��һ��������ƥ���Ŀ¼�����һ�������ҵ�Ŀ¼��ĸ���
 * �������Լ�Ŀ¼���(��Ϊһ������- res_dir)��������Ŀ¼���i �ڵ�- ��
 * ����Ҫ�Ļ����Լ�������
 *
 * '..'Ŀ¼������ڼ�Ҳ��Լ�����������ֱ���- �����Խһ��α��Ŀ¼��
 * ����װ�㡣
 */
//// ����ָ��Ŀ¼���ļ�����Ŀ¼�
// ������dir - ָ��Ŀ¼i �ڵ��ָ�룻name - �ļ�����namelen - �ļ������ȣ�
// ���أ����ٻ�����ָ�룻res_dir - ���ص�Ŀ¼��ṹָ�룻
static struct buffer_head * find_entry(struct m_inode ** dir,
	const char * name, int namelen, struct dir_entry ** res_dir)
{
	int entries;
	int block,i;
	struct buffer_head * bh;
	struct dir_entry * de;
	struct super_block * sb;

// ���������NO_TRUNCATE�������ļ������ȳ�����󳤶�NAME_LEN���򷵻ء�
#ifdef NO_TRUNCATE
	if (namelen > NAME_LEN)
		return NULL;
//���û�ж���NO_TRUNCATE�������ļ������ȳ�����󳤶�NAME_LEN����ض�֮��
#else
	if (namelen > NAME_LEN)
		namelen = NAME_LEN;
#endif
// ���㱾Ŀ¼��Ŀ¼������entries���ÿշ���Ŀ¼��ṹָ�롣
	entries = (*dir)->i_size / (sizeof (struct dir_entry));
	*res_dir = NULL;
// ����ļ������ȵ���0���򷵻�NULL���˳���
	if (!namelen)
		return NULL;
/* ���Ŀ¼��'..'����Ϊ������Ҫ�����ر��� */
	if (namelen==2 && get_fs_byte(name)=='.' && get_fs_byte(name+1)=='.') {
/* α���е�'..'��ͬһ����'.'(ֻ��ı����ֳ���) */
// �����ǰ���̵ĸ��ڵ�ָ�뼴��ָ����Ŀ¼�����ļ����޸�Ϊ'.'��
		if ((*dir) == current->root)
			namelen=1;
// ���������Ŀ¼��i �ڵ�ŵ���ROOT_INO(1)�Ļ���˵�����ļ�ϵͳ���ڵ㡣��ȡ�ļ�ϵͳ�ĳ����顣
		else if ((*dir)->i_num == ROOT_INO) {
/* ��һ����װ���ϵ�'..'������Ŀ¼��������װ���ļ�ϵͳ��Ŀ¼i �ڵ㡣
   ע�⣡����������mounted ��־����������ܹ�ȡ������Ŀ¼ */
			sb=get_super((*dir)->i_dev);
// �������װ����i �ڵ���ڣ������ͷ�ԭi �ڵ㣬Ȼ��Ա���װ����i �ڵ���д���
// ��*dir ָ��ñ���װ����i �ڵ㣻��i �ڵ����������1��
			if (sb->s_imount) {
				iput(*dir);
				(*dir)=sb->s_imount;
				(*dir)->i_count++;
			}
		}
	}
// �����i �ڵ���ָ��ĵ�һ��ֱ�Ӵ��̿��Ϊ0���򷵻�NULL���˳���
	if (!(block = (*dir)->i_zone[0]))
		return NULL;
// �ӽڵ������豸��ȡָ����Ŀ¼�����ݿ飬������ɹ����򷵻�NULL���˳���
	if (!(bh = bread((*dir)->i_dev,block)))
		return NULL;
// ��Ŀ¼�����ݿ�������ƥ��ָ���ļ�����Ŀ¼�������de ָ�����ݿ飬���ڲ�����Ŀ¼��Ŀ¼����
// �������£�ѭ��ִ��������
	i = 0;
	de = (struct dir_entry *) bh->b_data;
	while (i < entries) {
// �����ǰĿ¼�����ݿ��Ѿ������꣬��û���ҵ�ƥ���Ŀ¼����ͷŵ�ǰĿ¼�����ݿ顣
		if ((char *)de >= BLOCK_SIZE+bh->b_data) {
			brelse(bh);
			bh = NULL;
// �ڶ�����һĿ¼�����ݿ顣�����Ϊ�գ���ֻҪ��û��������Ŀ¼�е�����Ŀ¼��������ÿ飬
// ��������һĿ¼�����ݿ顣���ÿ鲻�գ�����de ָ���Ŀ¼�����ݿ飬����������
			if (!(block = bmap(*dir,i/DIR_ENTRIES_PER_BLOCK)) ||
			    !(bh = bread((*dir)->i_dev,block))) {
				i += DIR_ENTRIES_PER_BLOCK;
				continue;
			}
			de = (struct dir_entry *) bh->b_data;
		}
// ����ҵ�ƥ���Ŀ¼��Ļ����򷵻ظ�Ŀ¼��ṹָ��͸�Ŀ¼�����ݿ�ָ�룬�˳���
		if (match(namelen,name,de)) {
			*res_dir = de;
			return bh;
		}
// ���������Ŀ¼�����ݿ��бȽ���һ��Ŀ¼�
		de++;
		i++;
	}
// ��ָ��Ŀ¼�е�����Ŀ¼������껹û���ҵ���Ӧ��Ŀ¼����ͷ�Ŀ¼�����ݿ飬����NULL��
	brelse(bh);
	return NULL;
}

/*
 *	add_entry()
 *
 * ʹ����find_entry()ͬ���ķ�������ָ��Ŀ¼�����һ�ļ�Ŀ¼�
 * ���ʧ���򷵻�NULL��
 *
 * ע�⣡��'de'(ָ��Ŀ¼��ṹָ��)��i �ڵ㲿�ֱ�����Ϊ0 - ���ʾ
 * �ڵ��øú�������Ŀ¼���������Ϣ֮�䲻��˯�ߣ���Ϊ��˯����ô����
 * ��(����)���ܻ��Ѿ�ʹ���˸�Ŀ¼�
 */
//// ����ָ����Ŀ¼���ļ������Ŀ¼�
// ������dir - ָ��Ŀ¼��i �ڵ㣻name - �ļ�����namelen - �ļ������ȣ�
// ���أ����ٻ�����ָ�룻res_dir - ���ص�Ŀ¼��ṹָ�룻
static struct buffer_head * add_entry(struct m_inode * dir,
	const char * name, int namelen, struct dir_entry ** res_dir)
{
	int block,i;
	struct buffer_head * bh;
	struct dir_entry * de;

	*res_dir = NULL;
// ���������NO_TRUNCATE�������ļ������ȳ�����󳤶�NAME_LEN���򷵻ء�
#ifdef NO_TRUNCATE
	if (namelen > NAME_LEN)
		return NULL;
//���û�ж���NO_TRUNCATE�������ļ������ȳ�����󳤶�NAME_LEN����ض�֮��
#else
	if (namelen > NAME_LEN)
		namelen = NAME_LEN;
#endif
// ����ļ������ȵ���0���򷵻�NULL���˳���
	if (!namelen)
		return NULL;
// �����Ŀ¼i �ڵ���ָ��ĵ�һ��ֱ�Ӵ��̿��Ϊ0���򷵻�NULL �˳���
	if (!(block = dir->i_zone[0]))
		return NULL;
// �����ȡ�ô��̿�ʧ�ܣ��򷵻�NULL ���˳���
	if (!(bh = bread(dir->i_dev,block)))
		return NULL;
// ��Ŀ¼�����ݿ���ѭ���������δʹ�õ�Ŀ¼�������Ŀ¼��ṹָ��de ָ����ٻ�������ݿ�
// ��ʼ����Ҳ����һ��Ŀ¼�
	i = 0;
	de = (struct dir_entry *) bh->b_data;
	while (1) {
// �����ǰ�б��Ŀ¼���Ѿ�������ǰ���ݿ飬���ͷŸ����ݿ飬��������һ����̿�block�����
// ����ʧ�ܣ��򷵻�NULL���˳���
		if ((char *)de >= BLOCK_SIZE+bh->b_data) {
			brelse(bh);
			bh = NULL;
			block = create_block(dir,i/DIR_ENTRIES_PER_BLOCK);
			if (!block)
				return NULL;
// �����ȡ���̿鷵�ص�ָ��Ϊ�գ��������ÿ������
			if (!(bh = bread(dir->i_dev,block))) {
				i += DIR_ENTRIES_PER_BLOCK;
				continue;
			}
// ������Ŀ¼��ṹָ��de ־��ÿ�ĸ��ٻ������ݿ鿪ʼ����
			de = (struct dir_entry *) bh->b_data;
		}
// �����ǰ��������Ŀ¼�����i*Ŀ¼�ṹ��С�Ѿ������˸�Ŀ¼��ָ���Ĵ�Сi_size����˵���õ�i
// ��Ŀ¼�δʹ�ã����ǿ���ʹ���������ǶԸ�Ŀ¼���������(�ø�Ŀ¼���i �ڵ�ָ��Ϊ��)����
// ���¸�Ŀ¼�ĳ���ֵ(����һ��Ŀ¼��ĳ��ȣ�����Ŀ¼��i �ڵ����޸ı�־���ٸ��¸�Ŀ¼�ĸı�ʱ
// ��Ϊ��ǰʱ�䡣
		if (i*sizeof(struct dir_entry) >= dir->i_size) {
			de->inode=0;
			dir->i_size = (i+1)*sizeof(struct dir_entry);
			dir->i_dirt = 1;
			dir->i_ctime = CURRENT_TIME;
		}
// ����Ŀ¼���i �ڵ�Ϊ�գ����ʾ�ҵ�һ����δʹ�õ�Ŀ¼����Ǹ���Ŀ¼���޸�ʱ��Ϊ��ǰʱ�䡣
// �����û������������ļ�������Ŀ¼����ļ����ֶΣ�����Ӧ�ĸ��ٻ�������޸ı�־�����ظ�Ŀ¼
// ���ָ���Լ��ø��ٻ�������ָ�룬�˳���
		if (!de->inode) {
			dir->i_mtime = CURRENT_TIME;
			for (i=0; i < NAME_LEN ; i++)
				de->name[i]=(i<namelen)?get_fs_byte(name+i):0;
			bh->b_dirt = 1;
			*res_dir = de;
			return bh;
		}
// �����Ŀ¼���Ѿ���ʹ�ã�����������һ��Ŀ¼�
		de++;
		i++;
	}
// ִ�в������Ҳ��Linus ��д��δ���ʱ���ȸ���������find_entry()�Ĵ��룬�����޸ĵ�:)
	brelse(bh);
	return NULL;
}

/*
 *	get_dir()
 *
 * �ú������ݸ�����·��������������ֱ���ﵽ��˵�Ŀ¼��
 * ���ʧ���򷵻�NULL��
 */
//// ��Ѱָ��·������Ŀ¼��
// ������pathname - ·������
// ���أ�Ŀ¼��i �ڵ�ָ�롣ʧ��ʱ����NULL��
static struct m_inode * get_dir(const char * pathname)
{
	char c;
	const char * thisname;
	struct m_inode * inode;
	struct buffer_head * bh;
	int namelen,inr,idev;
	struct dir_entry * de;

// �������û���趨��i �ڵ㣬���߸ý��̸�i �ڵ������Ϊ0����ϵͳ����������
	if (!current->root || !current->root->i_count)
		panic("No root inode");
// ������̵ĵ�ǰ����Ŀ¼ָ��Ϊ�գ����߸õ�ǰĿ¼i �ڵ�����ü���Ϊ0��Ҳ��ϵͳ�����⣬������
	if (!current->pwd || !current->pwd->i_count)
		panic("No cwd inode");
// ����û�ָ����·�����ĵ�1 ���ַ���'/'����˵��·�����Ǿ���·��������Ӹ�i �ڵ㿪ʼ������
	if ((c=get_fs_byte(pathname))=='/') {
		inode = current->root;
		pathname++;
// ��������һ���ַ��������ַ������ʾ�����������·������Ӧ�ӽ��̵ĵ�ǰ����Ŀ¼��ʼ������
// ��ȡ���̵�ǰ����Ŀ¼��i �ڵ㡣
	} else if (c)
		inode = current->pwd;
// �����ʾ·����Ϊ�գ���������NULL���˳���
	else
		return NULL;	/* �յ�·�����Ǵ���� */
// ��ȡ�õ�i �ڵ����ü�����1��
	inode->i_count++;
	while (1) {
// ����i �ڵ㲻��Ŀ¼�ڵ㣬����û�пɽ���ķ�����ɣ����ͷŸ�i �ڵ㣬����NULL���˳���
		thisname = pathname;
		if (!S_ISDIR(inode->i_mode) || !permission(inode,MAY_EXEC)) {
			iput(inode);
			return NULL;
		}
// ��·������ʼ����������ַ���ֱ���ַ����ǽ�β��(NULL)������'/'����ʱnamelen �����ǵ�ǰ����
// Ŀ¼���ĳ��ȡ�������Ҳ��һ��Ŀ¼���������û�м�'/'���򲻻᷵�ظ����Ŀ¼��i �ڵ㣡
// ���磺/var/log/httpd����ֻ����log/Ŀ¼��i �ڵ㡣
		for(namelen=0;(c=get_fs_byte(pathname++))&&(c!='/');namelen++)
			/* nothing */ ;
// ���ַ��ǽ�β��NULL��������Ѿ�����ָ��Ŀ¼���򷵻ظ�i �ڵ�ָ�룬�˳���
		if (!c)
			return inode;
// ���ò���ָ��Ŀ¼���ļ�����Ŀ¼������ڵ�ǰ����Ŀ¼��Ѱ����Ŀ¼����û���ҵ���
// ���ͷŸ�i �ڵ㣬������NULL���˳���
		if (!(bh = find_entry(&inode,thisname,namelen,&de))) {
			iput(inode);
			return NULL;
		}
// ȡ����Ŀ¼���i �ڵ��inr ���豸��idev���ͷŰ�����Ŀ¼��ĸ��ٻ����͸�i �ڵ㡣
		inr = de->inode;
		idev = inode->i_dev;
		brelse(bh);
		iput(inode);
// ȡ�ڵ��inr ��i �ڵ���Ϣ����ʧ�ܣ��򷵻�NULL���˳�����������Ը���Ŀ¼��i �ڵ���в�����
		if (!(inode = iget(idev,inr)))
			return NULL;
	}
}

/*
 *	dir_namei()
 *
 * dir_namei()��������ָ��Ŀ¼����i �ڵ�ָ�룬�Լ������Ŀ¼�����ơ�
 */
// ������pathname - Ŀ¼·������namelen - ·�������ȡ�
// ���أ�ָ��Ŀ¼�����Ŀ¼��i �ڵ�ָ������Ŀ¼�����䳤�ȡ�
static struct m_inode * dir_namei(const char * pathname,
	int * namelen, const char ** name)
{
	char c;
	const char * basename;
	struct m_inode * dir;

// ȡָ��·�������Ŀ¼��i �ڵ㣬�������򷵻�NULL���˳���
	if (!(dir = get_dir(pathname)))
		return NULL;
// ��·����pathname ����������⣬�鴦���һ��'/'����������ַ����������䳤�ȣ��������
// ��Ŀ¼��i �ڵ�ָ�롣
	basename = pathname;
	while (c=get_fs_byte(pathname++))
		if (c=='/')
			basename=pathname;
	*namelen = pathname-basename-1;
	*name = basename;
	return dir;
}

/*
 *	namei()
 *
 * �ú��������򵥵���������ȡ��ָ��·�����Ƶ�i �ڵ㡣open��link ����ʹ������
 * �Լ�����Ӧ���������������޸�ģʽ'chmod'������������ú������㹻���ˡ�
 */
//// ȡָ��·������i �ڵ㡣
// ������pathname - ·������
// ���أ���Ӧ��i �ڵ㡣
struct m_inode * namei(const char * pathname)
{
	const char * basename;
	int inr,dev,namelen;
	struct m_inode * dir;
	struct buffer_head * bh;
	struct dir_entry * de;

// ���Ȳ���ָ��·�������Ŀ¼��Ŀ¼������i �ڵ㣬�������ڣ��򷵻�NULL���˳���
	if (!(dir = dir_namei(pathname,&namelen,&basename)))
		return NULL;
// ������ص�������ֵĳ�����0�����ʾ��·������һ��Ŀ¼��Ϊ���һ�
	if (!namelen)			/* ��Ӧ��'/usr/'����� */
		return dir;
// �ڷ��صĶ���Ŀ¼��Ѱ��ָ���ļ�����Ŀ¼���i �ڵ㡣��Ϊ������Ҳ��һ��Ŀ¼���������û
// �м�'/'���򲻻᷵�ظ����Ŀ¼��i �ڵ㣡���磺/var/log/httpd����ֻ����log/Ŀ¼��i �ڵ㡣
// ���dir_namei()������'/'���������һ�����ֵ���һ���ļ��������������������Ҫ����������
// ���ʹ��Ѱ��Ŀ¼��i �ڵ㺯��find_entry()���д���
	bh = find_entry(&dir,basename,namelen,&de);
	if (!bh) {
		iput(dir);
		return NULL;
	}
// ȡ��Ŀ¼���i �ڵ�ź�Ŀ¼���豸�ţ����ͷŰ�����Ŀ¼��ĸ��ٻ������Լ�Ŀ¼i �ڵ㡣
	inr = de->inode;
	dev = dir->i_dev;
	brelse(bh);
	iput(dir);
// ȡ��Ӧ�ںŵ�i �ڵ㣬�޸��䱻����ʱ��Ϊ��ǰʱ�䣬�������޸ı�־����󷵻ظ�i �ڵ�ָ�롣
	dir=iget(dev,inr);
	if (dir) {
		dir->i_atime=CURRENT_TIME;
		dir->i_dirt=1;
	}
	return dir;
}

/*
 *	open_namei()
 *
 * open()��ʹ�õ�namei ����- ����ʵ�����������Ĵ��ļ�����
 */
//// �ļ���namei ������
// ������pathname - �ļ�·������flag - �ļ��򿪱�־��mode - �ļ�����������ԣ�
// ���أ��ɹ�����0�����򷵻س����룻res_inode - ���صĶ�Ӧ�ļ�·�����ĵ�i �ڵ�ָ�롣
int open_namei(const char * pathname, int flag, int mode,
	struct m_inode ** res_inode)
{
	const char * basename;
	int inr,dev,namelen;
	struct m_inode * dir, *inode;
	struct buffer_head * bh;
	struct dir_entry * de;

// ����ļ��������ģʽ��־��ֻ��(0)�����ļ���0 ��־O_TRUNC ȴ��λ�ˣ����Ϊֻд��־��
	if ((flag & O_TRUNC) && !(flag & O_ACCMODE))
		flag |= O_WRONLY;
// ʹ�ý��̵��ļ�������������룬���ε�����ģʽ�е���Ӧλ����������ͨ�ļ���־��
	mode &= 0777 & ~current->umask;
	mode |= I_REGULAR;
// ����·����Ѱ�ҵ���Ӧ��i �ڵ㣬�Լ�����ļ������䳤�ȡ�
	if (!(dir = dir_namei(pathname,&namelen,&basename)))
		return -ENOENT;
// �������ļ�������Ϊ0(����'/usr/'����·���������)����ô���򿪲������Ǵ�������0��
// ���ʾ��һ��Ŀ¼����ֱ�ӷ��ظ�Ŀ¼��i �ڵ㣬���˳���
	if (!namelen) {			/* special case: '/usr/' etc */
		if (!(flag & (O_ACCMODE|O_CREAT|O_TRUNC))) {
			*res_inode=dir;
			return 0;
		}
// �����ͷŸ�i �ڵ㣬���س����롣
		iput(dir);
		return -EISDIR;
	}
// ��dir �ڵ��Ӧ��Ŀ¼��ȡ�ļ�����Ӧ��Ŀ¼��ṹde �͸�Ŀ¼�����ڵĸ��ٻ�������
	bh = find_entry(&dir,basename,namelen,&de);
// ����ø��ٻ���ָ��ΪNULL�����ʾû���ҵ���Ӧ�ļ�����Ŀ¼����ֻ�����Ǵ����ļ�������
	if (!bh) {
// ������Ǵ����ļ������ͷŸ�Ŀ¼��i �ڵ㣬���س�����˳���
		if (!(flag & O_CREAT)) {
			iput(dir);
			return -ENOENT;
		}
// ����û��ڸ�Ŀ¼û��д��Ȩ�������ͷŸ�Ŀ¼��i �ڵ㣬���س�����˳���
		if (!permission(dir,MAY_WRITE)) {
			iput(dir);
			return -EACCES;
		}
// ��Ŀ¼�ڵ��Ӧ���豸������һ����i �ڵ㣬��ʧ�ܣ����ͷ�Ŀ¼��i �ڵ㣬������û�пռ�����롣
		inode = new_inode(dir->i_dev);
		if (!inode) {
			iput(dir);
			return -ENOSPC;
		}
// ����ʹ�ø���i �ڵ㣬������г�ʼ���ã��ýڵ���û�id����Ӧ�ڵ����ģʽ�������޸ı�־��
		inode->i_uid = current->euid;
		inode->i_mode = mode;
		inode->i_dirt = 1;
// Ȼ����ָ��Ŀ¼dir �����һ��Ŀ¼�
		bh = add_entry(dir,basename,namelen,&de);
// ������ص�Ӧ�ú�����Ŀ¼��ĸ��ٻ�����ָ��ΪNULL�����ʾ���Ŀ¼�����ʧ�ܡ����ǽ���
// ��i �ڵ���������Ӽ�����1�����ͷŸ�i �ڵ���Ŀ¼��i �ڵ㣬���س����룬�˳���
		if (!bh) {
			inode->i_nlinks--;
			iput(inode);
			iput(dir);
			return -ENOSPC;
		}
// ��ʼ���ø���Ŀ¼���i �ڵ��Ϊ�����뵽��i �ڵ�ĺ��룻���ø��ٻ��������޸ı�־��Ȼ��
// �ͷŸø��ٻ��������ͷ�Ŀ¼��i �ڵ㡣������Ŀ¼���i �ڵ�ָ�룬�˳���
		de->inode = inode->i_num;
		bh->b_dirt = 1;
		brelse(bh);
		iput(dir);
		*res_inode = inode;
		return 0;
	}
// ��������Ŀ¼��ȡ�ļ�����Ӧ��Ŀ¼��ṹ�����ɹ�(Ҳ��bh ��ΪNULL)��ȡ����Ŀ¼���i �ڵ��
// �������ڵ��豸�ţ����ͷŸø��ٻ������Լ�Ŀ¼��i �ڵ㡣
	inr = de->inode;
	dev = dir->i_dev;
	brelse(bh);
	iput(dir);
// �����ռʹ�ñ�־O_EXCL ��λ���򷵻��ļ��Ѵ��ڳ����룬�˳���
	if (flag & O_EXCL)
		return -EEXIST;
// ���ȡ��Ŀ¼���Ӧi �ڵ�Ĳ���ʧ�ܣ��򷵻ط��ʳ����룬�˳���
	if (!(inode=iget(dev,inr)))
		return -EACCES;
// ����i �ڵ���һ��Ŀ¼�Ľڵ㲢�ҷ���ģʽ��ֻ����ֻд������û�з��ʵ����Ȩ�ޣ����ͷŸ�i
// �ڵ㣬���ط���Ȩ�޳����룬�˳���
	if ((S_ISDIR(inode->i_mode) && (flag & O_ACCMODE)) ||
	    !permission(inode,ACC_MODE(flag))) {
		iput(inode);
		return -EPERM;
	}
// ���¸�i �ڵ�ķ���ʱ���ֶ�Ϊ��ǰʱ�䡣
	inode->i_atime = CURRENT_TIME;
// ��������˽�0 ��־���򽫸�i �ڵ���ļ����Ƚ�Ϊ0��
	if (flag & O_TRUNC)
		truncate(inode);
// ��󷵻ظ�Ŀ¼��i �ڵ��ָ�룬������0���ɹ�����
	*res_inode = inode;
	return 0;
}

//// ϵͳ���ú���- ����һ�������ļ�����ͨ�ļ��ڵ�(node)��
// ��������Ϊfilename����mode ��dev ָ�����ļ�ϵͳ�ڵ�(��ͨ�ļ����豸�����ļ��������ܵ�)��
// ������filename - ·������mode - ָ��ʹ������Լ��������ڵ�����ͣ�dev - �豸�š�
// ���أ��ɹ��򷵻�0�����򷵻س����롣
int sys_mknod(const char * filename, int mode, int dev)
{
	const char * basename;
	int namelen;
	struct m_inode * dir, * inode;
	struct buffer_head * bh;
	struct dir_entry * de;
	
// ������ǳ����û����򷵻ط�����ɳ����롣
	if (!suser())
		return -EPERM;
// ����Ҳ�����Ӧ·����Ŀ¼��i �ڵ㣬�򷵻س����롣
	if (!(dir = dir_namei(filename,&namelen,&basename)))
		return -ENOENT;
// �����˵��ļ�������Ϊ0����˵��������·�������û��ָ���ļ������ͷŸ�Ŀ¼i �ڵ㣬����
// �����룬�˳���
	if (!namelen) {
		iput(dir);
		return -ENOENT;
	}
// ����ڸ�Ŀ¼��û��д��Ȩ�ޣ����ͷŸ�Ŀ¼��i �ڵ㣬���ط�����ɳ����룬�˳���
	if (!permission(dir,MAY_WRITE)) {
		iput(dir);
		return -EPERM;
	}
// �����Ӧ·�����������ļ�����Ŀ¼���Ѿ����ڣ����ͷŰ�����Ŀ¼��ĸ��ٻ��������ͷ�Ŀ¼
// ��i �ڵ㣬�����ļ��Ѿ����ڳ����룬�˳���
	bh = find_entry(&dir,basename,namelen,&de);
	if (bh) {
		brelse(bh);
		iput(dir);
		return -EEXIST;
	}
// ����һ���µ�i �ڵ㣬������ɹ������ͷ�Ŀ¼��i �ڵ㣬�����޿ռ�����룬�˳���
	inode = new_inode(dir->i_dev);
	if (!inode) {
		iput(dir);
		return -ENOSPC;
	}
// ���ø�i �ڵ������ģʽ�����Ҫ�������ǿ��豸�ļ��������ַ��豸�ļ�������i �ڵ��ֱ�ӿ�
// ָ��0 �����豸�š�
	inode->i_mode = mode;
	if (S_ISBLK(mode) || S_ISCHR(mode))
		inode->i_zone[0] = dev;
// ���ø�i �ڵ���޸�ʱ�䡢����ʱ��Ϊ��ǰʱ�䡣
	inode->i_mtime = inode->i_atime = CURRENT_TIME;
	inode->i_dirt = 1;
// ��Ŀ¼�������һ��Ŀ¼����ʧ��(������Ŀ¼��ĸ��ٻ�����ָ��ΪNULL)�����ͷ�Ŀ¼��
// i �ڵ㣻�������i �ڵ��������Ӽ�����λ�����ͷŸ�i �ڵ㡣���س����룬�˳���
	bh = add_entry(dir,basename,namelen,&de);
	if (!bh) {
		iput(dir);
		inode->i_nlinks=0;
		iput(inode);
		return -ENOSPC;
	}
// ���Ŀ¼���i �ڵ��ֶε�����i �ڵ�ţ��ø��ٻ��������޸ı�־���ͷ�Ŀ¼���µ�i �ڵ㣬
// �ͷŸ��ٻ���������󷵻�0(�ɹ�)��
	de->inode = inode->i_num;
	bh->b_dirt = 1;
	iput(dir);
	iput(inode);
	brelse(bh);
	return 0;
}

//// ϵͳ���ú���- ����Ŀ¼��
// ������pathname - ·������mode - Ŀ¼ʹ�õ�Ȩ�����ԡ�
// ���أ��ɹ��򷵻�0�����򷵻س����롣
int sys_mkdir(const char * pathname, int mode)
{
	const char * basename;
	int namelen;
	struct m_inode * dir, * inode;
	struct buffer_head * bh, *dir_block;
	struct dir_entry * de;

// ������ǳ����û����򷵻ط�����ɳ����롣
	if (!suser())
		return -EPERM;
// ����Ҳ�����Ӧ·����Ŀ¼��i �ڵ㣬�򷵻س����롣
	if (!(dir = dir_namei(pathname,&namelen,&basename)))
		return -ENOENT;
// �����˵��ļ�������Ϊ0����˵��������·�������û��ָ���ļ������ͷŸ�Ŀ¼i �ڵ㣬����
// �����룬�˳���
	if (!namelen) {
		iput(dir);
		return -ENOENT;
	}
// ����ڸ�Ŀ¼��û��д��Ȩ�ޣ����ͷŸ�Ŀ¼��i �ڵ㣬���ط�����ɳ����룬�˳���
	if (!permission(dir,MAY_WRITE)) {
		iput(dir);
		return -EPERM;
	}
// �����Ӧ·�����������ļ�����Ŀ¼���Ѿ����ڣ����ͷŰ�����Ŀ¼��ĸ��ٻ��������ͷ�Ŀ¼
// ��i �ڵ㣬�����ļ��Ѿ����ڳ����룬�˳���
	bh = find_entry(&dir,basename,namelen,&de);
	if (bh) {
		brelse(bh);
		iput(dir);
		return -EEXIST;
	}
// ����һ���µ�i �ڵ㣬������ɹ������ͷ�Ŀ¼��i �ڵ㣬�����޿ռ�����룬�˳���
	inode = new_inode(dir->i_dev);
	if (!inode) {
		iput(dir);
		return -ENOSPC;
	}
// �ø���i �ڵ��Ӧ���ļ�����Ϊ32(һ��Ŀ¼��Ĵ�С)���ýڵ����޸ı�־���Լ��ڵ���޸�ʱ��
// �ͷ���ʱ�䡣
	inode->i_size = 32;
	inode->i_dirt = 1;
	inode->i_mtime = inode->i_atime = CURRENT_TIME;
// Ϊ��i �ڵ�����һ���̿飬����ڵ��һ��ֱ�ӿ�ָ����ڸÿ�š��������ʧ�ܣ����ͷŶ�ӦĿ¼
// ��i �ڵ㣻��λ�������i �ڵ����Ӽ������ͷŸ��µ�i �ڵ㣬����û�пռ�����룬�˳���
	if (!(inode->i_zone[0]=new_block(inode->i_dev))) {
		iput(dir);
		inode->i_nlinks--;
		iput(inode);
		return -ENOSPC;
	}
// �ø��µ�i �ڵ����޸ı�־��
	inode->i_dirt = 1;
// ��������Ĵ��̿顣���������ͷŶ�ӦĿ¼��i �ڵ㣻�ͷ�����Ĵ��̿飻��λ�������i �ڵ�
// ���Ӽ������ͷŸ��µ�i �ڵ㣬����û�пռ�����룬�˳���
	if (!(dir_block=bread(inode->i_dev,inode->i_zone[0]))) {
		iput(dir);
		free_block(inode->i_dev,inode->i_zone[0]);
		inode->i_nlinks--;
		iput(inode);
		return -ERROR;
	}
// ��de ָ��Ŀ¼�����ݿ飬�ø�Ŀ¼���i �ڵ���ֶε����������i �ڵ�ţ������ֶε���"."��
	de = (struct dir_entry *) dir_block->b_data;
	de->inode=inode->i_num;
	strcpy(de->name,".");
// Ȼ��de ָ����һ��Ŀ¼��ṹ���ýṹ���ڴ���ϼ�Ŀ¼�Ľڵ�ź�����".."��
	de++;
	de->inode = dir->i_num;
	strcpy(de->name,"..");
	inode->i_nlinks = 2;
// Ȼ�����øø��ٻ��������޸ı�־�����ͷŸû�������
	dir_block->b_dirt = 1;
	brelse(dir_block);
// ��ʼ��������i �ڵ��ģʽ�ֶΣ����ø�i �ڵ����޸ı�־��
	inode->i_mode = I_DIRECTORY | (mode & 0777 & ~current->umask);
	inode->i_dirt = 1;
// ��Ŀ¼�������һ��Ŀ¼����ʧ��(������Ŀ¼��ĸ��ٻ�����ָ��ΪNULL)�����ͷ�Ŀ¼��
// i �ڵ㣻�������i �ڵ��������Ӽ�����λ�����ͷŸ�i �ڵ㡣���س����룬�˳���
	bh = add_entry(dir,basename,namelen,&de);
	if (!bh) {
		iput(dir);
		free_block(inode->i_dev,inode->i_zone[0]);
		inode->i_nlinks=0;
		iput(inode);
		return -ENOSPC;
	}
// ���Ŀ¼���i �ڵ��ֶε�����i �ڵ�ţ��ø��ٻ��������޸ı�־���ͷ�Ŀ¼���µ�i �ڵ㣬�ͷ�
// ���ٻ���������󷵻�0(�ɹ�)��
	de->inode = inode->i_num;
	bh->b_dirt = 1;
	dir->i_nlinks++;
	dir->i_dirt = 1;
	iput(dir);
	iput(inode);
	brelse(bh);
	return 0;
}

/*
 * ���ڼ��ָ����Ŀ¼�Ƿ�Ϊ�յ��ӳ���(����rmdir ϵͳ���ú���)��
 */
//// ���ָ��Ŀ¼�Ƿ��ǿյġ�
// ������inode - ָ��Ŀ¼��i �ڵ�ָ�롣
// ���أ�0 - �ǿյģ�1 - ���ա�
static int empty_dir(struct m_inode * inode)
{
	int nr,block;
	int len;
	struct buffer_head * bh;
	struct dir_entry * de;

// ����ָ��Ŀ¼������Ŀ¼��ĸ���(Ӧ��������2 ������"."��".."�����ļ�Ŀ¼��)��
	len = inode->i_size / sizeof (struct dir_entry);
// ���Ŀ¼���������2 �����߸�Ŀ¼i �ڵ�ĵ�1 ��ֱ�ӿ�û��ָ���κδ��̿�ţ�������Ӧ����
// �������������ʾ������Ϣ���豸dev ��Ŀ¼��������0(ʧ��)��
	if (len<2 || !inode->i_zone[0] ||
	    !(bh=bread(inode->i_dev,inode->i_zone[0]))) {
	    	printk("warning - bad directory on dev %04x\n",inode->i_dev);
		return 0;
	}
// ��de ָ���ж������̿����ݵĸ��ٻ������е�1 ��Ŀ¼�
	de = (struct dir_entry *) bh->b_data;
// �����1 ��Ŀ¼���i �ڵ���ֶ�ֵ�����ڸ�Ŀ¼��i �ڵ�ţ����ߵ�2 ��Ŀ¼���i �ڵ���ֶ�
// Ϊ�㣬��������Ŀ¼��������ֶβ��ֱ����"."��".."������ʾ��������Ϣ���豸dev ��Ŀ¼��
// ������0��
	if (de[0].inode != inode->i_num || !de[1].inode || 
	    strcmp(".",de[0].name) || strcmp("..",de[1].name)) {
	    	printk("warning - bad directory on dev %04x\n",inode->i_dev);
		return 0;
	}
// ��nr ����Ŀ¼����ţ�de ָ�������Ŀ¼�
	nr = 2;
	de += 2;
// ѭ������Ŀ¼�����е�Ŀ¼��(len-2 ��)������û��Ŀ¼���i �ڵ���ֶβ�Ϊ0(��ʹ��)��
	while (nr<len) {
// ����ÿ���̿��е�Ŀ¼���Ѿ�����꣬���ͷŸô��̿�ĸ��ٻ���������ȡ��һ�麬��Ŀ¼���
// ���̿顣����Ӧ��û��ʹ��(���Ѿ����ã����ļ��Ѿ�ɾ����)�����������һ�飬�������������
// ������0��������de ָ���������׸�Ŀ¼�
		if ((void *) de >= (void *) (bh->b_data+BLOCK_SIZE)) {
			brelse(bh);
			block=bmap(inode,nr/DIR_ENTRIES_PER_BLOCK);
			if (!block) {
				nr += DIR_ENTRIES_PER_BLOCK;
				continue;
			}
			if (!(bh=bread(inode->i_dev,block)))
				return 0;
			de = (struct dir_entry *) bh->b_data;
		}
// �����Ŀ¼���i �ڵ���ֶβ�����0�����ʾ��Ŀ¼��Ŀǰ����ʹ�ã����ͷŸø��ٻ�������
// ����0���˳���
		if (de->inode) {
			brelse(bh);
			return 0;
		}
// ��������û�в�ѯ���Ŀ¼�е�����Ŀ¼��������⡣
		de++;
		nr++;
	}
// ������˵����Ŀ¼��û���ҵ����õ�Ŀ¼��(��Ȼ����ͷ��������)���򷵻ػ�����������1��
	brelse(bh);
	return 1;
}

//// ϵͳ���ú���- ɾ��ָ�����Ƶ�Ŀ¼��
// ������ name - Ŀ¼��(·����)��
// ���أ�����0 ��ʾ�ɹ������򷵻س���š�
int sys_rmdir(const char * name)
{
	const char * basename;
	int namelen;
	struct m_inode * dir, * inode;
	struct buffer_head * bh;
	struct dir_entry * de;

// ������ǳ����û����򷵻ط�����ɳ����롣
	if (!suser())
		return -EPERM;
// ����Ҳ�����Ӧ·����Ŀ¼��i �ڵ㣬�򷵻س����롣
	if (!(dir = dir_namei(name,&namelen,&basename)))
		return -ENOENT;
// �����˵��ļ�������Ϊ0����˵��������·�������û��ָ���ļ������ͷŸ�Ŀ¼i �ڵ㣬����
// �����룬�˳���
	if (!namelen) {
		iput(dir);
		return -ENOENT;
	}
// ����ڸ�Ŀ¼��û��д��Ȩ�ޣ����ͷŸ�Ŀ¼��i �ڵ㣬���ط�����ɳ����룬�˳���
	if (!permission(dir,MAY_WRITE)) {
		iput(dir);
		return -EPERM;
	}
// �����Ӧ·�����������ļ�����Ŀ¼����ڣ����ͷŰ�����Ŀ¼��ĸ��ٻ��������ͷ�Ŀ¼
// ��i �ڵ㣬�����ļ��Ѿ����ڳ����룬�˳�������dir �ǰ���Ҫ��ɾ��Ŀ¼����Ŀ¼i �ڵ㣬de
// ��Ҫ��ɾ��Ŀ¼��Ŀ¼��ṹ��
	bh = find_entry(&dir,basename,namelen,&de);
	if (!bh) {
		iput(dir);
		return -ENOENT;
	}
// ȡ��Ŀ¼��ָ����i �ڵ㡣���������ͷ�Ŀ¼��i �ڵ㣬���ͷź���Ŀ¼��ĸ��ٻ�����������
// ����š�
	if (!(inode = iget(dir->i_dev, de->inode))) {
		iput(dir);
		brelse(bh);
		return -EPERM;
	}
// ����Ŀ¼����������ɾ����־���ҽ��̵���Ч�û�id �����ڸ�i �ڵ���û�id�����ʾû��Ȩ��ɾ
// ����Ŀ¼�������ͷŰ���Ҫɾ��Ŀ¼����Ŀ¼i �ڵ�͸�Ҫɾ��Ŀ¼��i �ڵ㣬�ͷŸ��ٻ�������
// ���س����롣
	if ((dir->i_mode & S_ISVTX) && current->euid &&
	    inode->i_uid != current->euid) {
		iput(dir);
		iput(inode);
		brelse(bh);
		return -EPERM;
	}
// ���Ҫ��ɾ����Ŀ¼���i �ڵ���豸�Ų����ڰ�����Ŀ¼���Ŀ¼���豸�ţ����߸ñ�ɾ��Ŀ¼��
// �������Ӽ�������1(��ʾ�з������ӵ�)������ɾ����Ŀ¼�������ͷŰ���Ҫɾ��Ŀ¼����Ŀ¼
// i �ڵ�͸�Ҫɾ��Ŀ¼��i �ڵ㣬�ͷŸ��ٻ����������س����롣
	if (inode->i_dev != dir->i_dev || inode->i_count>1) {
		iput(dir);
		iput(inode);
		brelse(bh);
		return -EPERM;
	}
// ���Ҫ��ɾ��Ŀ¼��Ŀ¼��i �ڵ�Ľڵ�ŵ��ڰ�������ɾ��Ŀ¼��i �ڵ�ţ����ʾ��ͼɾ��"."
// Ŀ¼�������ͷŰ���Ҫɾ��Ŀ¼����Ŀ¼i �ڵ�͸�Ҫɾ��Ŀ¼��i �ڵ㣬�ͷŸ��ٻ�����������
// �����롣
	if (inode == dir) {	/* ���ǲ�����ɾ��"."��������ɾ��"../dir"*/
		iput(inode);
		iput(dir);
		brelse(bh);
		return -EPERM;
	}
// ��Ҫ��ɾ����Ŀ¼��i �ڵ�����Ա����ⲻ��һ��Ŀ¼�����ͷŰ���Ҫɾ��Ŀ¼����Ŀ¼i �ڵ��
// ��Ҫɾ��Ŀ¼��i �ڵ㣬�ͷŸ��ٻ����������س����롣
	if (!S_ISDIR(inode->i_mode)) {
		iput(inode);
		iput(dir);
		brelse(bh);
		return -ENOTDIR;
	}
// �����豻ɾ����Ŀ¼���գ����ͷŰ���Ҫɾ��Ŀ¼����Ŀ¼i �ڵ�͸�Ҫɾ��Ŀ¼��i �ڵ㣬�ͷ�
// ���ٻ����������س����롣
	if (!empty_dir(inode)) {
		iput(inode);
		iput(dir);
		brelse(bh);
		return -ENOTEMPTY;
	}
// �����豻ɾ��Ŀ¼��i �ڵ��������������2������ʾ������Ϣ��
	if (inode->i_nlinks != 2)
		printk("empty directory has nlink!=2 (%d)",inode->i_nlinks);
// �ø��豻ɾ��Ŀ¼��Ŀ¼���i �ڵ���ֶ�Ϊ0����ʾ��Ŀ¼���ʹ�ã����ú��и�Ŀ¼��ĸ���
// ���������޸ı�־�����ͷŸû�������
	de->inode = 0;
	bh->b_dirt = 1;
	brelse(bh);
// �ñ�ɾ��Ŀ¼��i �ڵ��������Ϊ0������i �ڵ����޸ı�־��
	inode->i_nlinks=0;
	inode->i_dirt=1;
// ��������ɾ��Ŀ¼����Ŀ¼��i �ڵ����ü�����1���޸���ı�ʱ����޸�ʱ��Ϊ��ǰʱ�䣬����
// �ýڵ����޸ı�־��
	dir->i_nlinks--;
	dir->i_ctime = dir->i_mtime = CURRENT_TIME;
	dir->i_dirt=1;
// ����ͷŰ���Ҫɾ��Ŀ¼����Ŀ¼i �ڵ�͸�Ҫɾ��Ŀ¼��i �ڵ㣬����0(�ɹ�)��
	iput(dir);
	iput(inode);
	return 0;
}

//// ϵͳ���ú���- ɾ���ļ����Լ�����Ҳɾ������ص��ļ���
// ���ļ�ϵͳɾ��һ�����֡������һ���ļ������һ�����ӣ�����û�н������򿪸��ļ�������ļ�
// Ҳ����ɾ�������ͷ���ռ�õ��豸�ռ䡣
// ������name - �ļ�����
// ���أ��ɹ��򷵻�0�����򷵻س���š�
int sys_unlink(const char * name)
{
	const char * basename;
	int namelen;
	struct m_inode * dir, * inode;
	struct buffer_head * bh;
	struct dir_entry * de;

// ����Ҳ�����Ӧ·����Ŀ¼��i �ڵ㣬�򷵻س����롣
	if (!(dir = dir_namei(name,&namelen,&basename)))
		return -ENOENT;
// �����˵��ļ�������Ϊ0����˵��������·�������û��ָ���ļ������ͷŸ�Ŀ¼i �ڵ㣬
// ���س����룬�˳���
	if (!namelen) {
		iput(dir);
		return -ENOENT;
	}
// ����ڸ�Ŀ¼��û��д��Ȩ�ޣ����ͷŸ�Ŀ¼��i �ڵ㣬���ط�����ɳ����룬�˳���
	if (!permission(dir,MAY_WRITE)) {
		iput(dir);
		return -EPERM;
	}
// �����Ӧ·�����������ļ�����Ŀ¼����ڣ����ͷŰ�����Ŀ¼��ĸ��ٻ��������ͷ�Ŀ¼
// ��i �ڵ㣬�����ļ��Ѿ����ڳ����룬�˳�������dir �ǰ���Ҫ��ɾ��Ŀ¼����Ŀ¼i �ڵ㣬de
// ��Ҫ��ɾ��Ŀ¼��Ŀ¼��ṹ��
	bh = find_entry(&dir,basename,namelen,&de);
	if (!bh) {
		iput(dir);
		return -ENOENT;
	}
// ȡ��Ŀ¼��ָ����i �ڵ㡣���������ͷ�Ŀ¼��i �ڵ㣬���ͷź���Ŀ¼��ĸ��ٻ�������
// ���س���š�
	if (!(inode = iget(dir->i_dev, de->inode))) {
		iput(dir);
		brelse(bh);
		return -ENOENT;
	}
// �����Ŀ¼����������ɾ����־�����û����ǳ����û������ҽ��̵���Ч�û�id �����ڱ�ɾ���ļ�
// ��i �ڵ���û�id�����ҽ��̵���Ч�û�id Ҳ������Ŀ¼i �ڵ���û�id����û��Ȩ��ɾ�����ļ�
// �������ͷŸ�Ŀ¼i �ڵ�͸��ļ���Ŀ¼���i �ڵ㣬�ͷŰ�����Ŀ¼��Ļ����������س���š�
	if ((dir->i_mode & S_ISVTX) && !suser() &&
	    current->euid != inode->i_uid &&
	    current->euid != dir->i_uid) {
		iput(dir);
		iput(inode);
		brelse(bh);
		return -EPERM;
	}
// �����ָ���ļ�����һ��Ŀ¼����Ҳ����ɾ�����ͷŸ�Ŀ¼i �ڵ�͸��ļ���Ŀ¼���i �ڵ㣬
// �ͷŰ�����Ŀ¼��Ļ����������س���š�
	if (S_ISDIR(inode->i_mode)) {
		iput(inode);
		iput(dir);
		brelse(bh);
		return -EPERM;
	}
// �����i �ڵ���������Ѿ�Ϊ0������ʾ������Ϣ��������Ϊ1��
	if (!inode->i_nlinks) {
		printk("Deleting nonexistent file (%04x:%d), %d\n",
			inode->i_dev,inode->i_num,inode->i_nlinks);
		inode->i_nlinks=1;
	}
// �����ļ�����Ŀ¼���е�i �ڵ���ֶ���Ϊ0����ʾ�ͷŸ�Ŀ¼������ð�����Ŀ¼��Ļ�����
// ���޸ı�־���ͷŸø��ٻ�������
	de->inode = 0;
	bh->b_dirt = 1;
	brelse(bh);
// ��i �ڵ����������1�������޸ı�־�����¸ı�ʱ��Ϊ��ǰʱ�䡣����ͷŸ�i �ڵ��Ŀ¼��i ��
// �㣬����0(�ɹ�)��
	inode->i_nlinks--;
	inode->i_dirt = 1;
	inode->i_ctime = CURRENT_TIME;
	iput(inode);
	iput(dir);
	return 0;
}

//// ϵͳ���ú���- Ϊ�ļ�����һ���ļ�����
// Ϊһ���Ѿ����ڵ��ļ�����һ��������(Ҳ��ΪӲ����- hard link)��
// ������oldname - ԭ·������newname - �µ�·������
// ���أ����ɹ��򷵻�0�����򷵻س���š�
int sys_link(const char * oldname, const char * newname)
{
	struct dir_entry * de;
	struct m_inode * oldinode, * dir;
	struct buffer_head * bh;
	const char * basename;
	int namelen;

// ȡԭ�ļ�·������Ӧ��i �ڵ�oldinode�����Ϊ0�����ʾ�������س���š�
	oldinode=namei(oldname);
	if (!oldinode)
		return -ENOENT;
// ���ԭ·������Ӧ����һ��Ŀ¼�������ͷŸ�i �ڵ㣬���س���š�
	if (S_ISDIR(oldinode->i_mode)) {
		iput(oldinode);
		return -EPERM;
	}
// ������·���������Ŀ¼��i �ڵ㣬�����������ļ������䳤�ȡ����Ŀ¼��i �ڵ�û���ҵ���
// ���ͷ�ԭ·������i �ڵ㣬���س���š�
	dir = dir_namei(newname,&namelen,&basename);
	if (!dir) {
		iput(oldinode);
		return -EACCES;
	}
// �����·�����в������ļ��������ͷ�ԭ·����i �ڵ����·����Ŀ¼��i �ڵ㣬���س���š�
	if (!namelen) {
		iput(oldinode);
		iput(dir);
		return -EPERM;
	}
// �����·����Ŀ¼���豸����ԭ·�������豸�Ų�һ������Ҳ���ܽ������ӣ������ͷ���·����
// Ŀ¼��i �ڵ��ԭ·������i �ڵ㣬���س���š�
	if (dir->i_dev != oldinode->i_dev) {
		iput(dir);
		iput(oldinode);
		return -EXDEV;
	}
// ����û�û������Ŀ¼��д��Ȩ�ޣ���Ҳ���ܽ������ӣ������ͷ���·����Ŀ¼��i �ڵ�
// ��ԭ·������i �ڵ㣬���س���š�
	if (!permission(dir,MAY_WRITE)) {
		iput(dir);
		iput(oldinode);
		return -EACCES;
	}
// ��ѯ����·�����Ƿ��Ѿ����ڣ�������ڣ���Ҳ���ܽ������ӣ������ͷŰ������Ѵ���Ŀ¼���
// ���ٻ��������ͷ���·����Ŀ¼��i �ڵ��ԭ·������i �ڵ㣬���س���š�
	bh = find_entry(&dir,basename,namelen,&de);
	if (bh) {
		brelse(bh);
		iput(dir);
		iput(oldinode);
		return -EEXIST;
	}
// ����Ŀ¼�����һ��Ŀ¼���ʧ�����ͷŸ�Ŀ¼��i �ڵ��ԭ·������i �ڵ㣬���س���š�
	bh = add_entry(dir,basename,namelen,&de);
	if (!bh) {
		iput(dir);
		iput(oldinode);
		return -ENOSPC;
	}
// �����ʼ���ø�Ŀ¼���i �ڵ�ŵ���ԭ·������i �ڵ�ţ����ð���������Ŀ¼��ĸ��ٻ�����
// ���޸ı�־���ͷŸû��������ͷ�Ŀ¼��i �ڵ㡣
	de->inode = oldinode->i_num;
	bh->b_dirt = 1;
	brelse(bh);
	iput(dir);
// ��ԭ�ڵ��Ӧ�ü�����1���޸���ı�ʱ��Ϊ��ǰʱ�䣬������i �ڵ����޸ı�־������ͷ�ԭ
// ·������i �ڵ㣬������0(�ɹ�)��
	oldinode->i_nlinks++;
	oldinode->i_ctime = CURRENT_TIME;
	oldinode->i_dirt = 1;
	iput(oldinode);
	return 0;
}
