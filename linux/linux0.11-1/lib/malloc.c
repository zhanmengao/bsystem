/*
 * malloc.c - Linux ��ͨ���ں��ڴ���亯����
 * 
 * ��Theodore Ts'o ����(tytso@mit.edu), 11/29/91
 *
 * �ú�������д�ɾ����ܵؿ죬�Ӷ����Դ��жϲ���ô˺�����
 *
 * ���ƣ�ʹ�øú���һ�����ܷ��������ڴ���4k��Ҳ��Linux ���ڴ�ҳ��Ĵ�С��
 *
 * ��д�ú�������ѭ��һ�������ÿҳ(����Ϊһ���洢Ͱ)��������Ҫ���ɶ���Ĵ�С��
 * ��һҳ�ϵ����ж����ͷź󣬸�ҳ�Ϳ��Է���ͨ�ÿ����ڴ�ء���malloc()������
 * ʱ������Ѱ������Ҫ�����С�Ĵ洢Ͱ�����Ӹô洢Ͱ�з���һ���ڴ档
 *
 * ÿ���洢Ͱ����һ����Ϊ������õĴ洢Ͱ�����������м�¼��ҳ�����ж��ٶ�������
 * ʹ���Լ���ҳ�Ͽ����ڴ���б�����洢Ͱ����һ�����洢Ͱ������Ҳ�Ǵ洢��ʹ��
 * get_free_page()���뵽��ҳ���ϵģ�������洢Ͱ��ͬ���ǣ�Ͱ��������ռ�õ�ҳ��
 * �����ٻ��ͷŸ�ϵͳ�����˵���һ��ϵͳ��Լֻ��Ҫ1 ��2 ҳ��Ͱ������ҳ�棬��Ϊһ
 * ��ҳ����Դ��256 ��Ͱ������(��Ӧ1MB �ڴ�Ĵ洢Ͱҳ��)�����ϵͳΪͰ��������
 * ��������ڴ棬��ô�϶�ϵͳʲô�ط���������  :-)
 *
 * ע�⣡malloc()��free()���߹ر����жϵĴ��벿�ֶ�������get_free_page()��
 *       free_page()��������ʹmalloc()��free()���԰�ȫ�ر����жϳ����е���
 *       (��������룬������NFS �ȱ����뵽Linux ��ʱ�Ϳ�����Ҫ���ֹ���)����ǰ
 *       ���Ǽ���get_free_page()��free_page()�ǿ��԰�ȫ�����жϼ�������ʹ�õģ�
 *       ����һ�������˷�ҳ����֮��Ϳ��ܲ��ǰ�ȫ�ġ�������������������ô����
 *       ����Ҫ�޸�malloc()����Ԥ�ȷ��䡱��ҳ���õ��ڴ棬���malloc()��free()
 *       �����жϳ����е���ʱ�Ϳ��԰�ȫ��ʹ����Щҳ�档
 *
 *       ������Ҫ���ǵ�����get_free_page()��Ӧ��˯�ߣ������˯�ߵĻ�����Ϊ�˷�ֹ
 *       �κξ���������������Ҫ��ϸ�ذ���˳�򡣹ؼ��������malloc()�ǿ��������
 *       �����õĻ�����ô�ͻ���ڲ���Ҫ��ҳ�汻��ϵͳ��ȡ�ߵĻ��ᡣ��������Ͱ����
 *       ����ҳ�棬��Щ�����ҳ�����ջ��ͷŸ�ϵͳ�����Բ�������������������á�
 */
#include <set_seg.h>

#include <linux/kernel.h>// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/mm.h>// �ڴ����ͷ�ļ�������ҳ���С�����һЩҳ���ͷź���ԭ�͡�
#include <asm/system.h>// ϵͳͷ�ļ������������û��޸�������/�ж��ŵȵ�Ƕ��ʽ���ꡣ

// �洢Ͱ�������ṹ��
struct bucket_desc {	/* 16 bytes */
	void			*page;			// ��Ͱ��������Ӧ���ڴ�ҳ��ָ�롣
	struct bucket_desc	*next;		// ��һ��������ָ�롣
	void			*freeptr;		// ָ��Ͱ�п����ڴ�λ�õ�ָ�롣
	unsigned short		refcnt;		// ���ü�����
	unsigned short		bucket_size;// ����������Ӧ�洢Ͱ�Ĵ�С��
};

// �洢Ͱ������Ŀ¼�ṹ��
struct _bucket_dir {	/* 8 bytes */
	int			size;			// �ô洢Ͱ�Ĵ�С(�ֽ���)��
	struct bucket_desc	*chain;	// �ô洢ͰĿ¼���Ͱ����������ָ�롣
};

/*
 * ���������Ǵ�ŵ�һ��������С�洢Ͱ������ָ��ĵط���  
 *
 * ���Linux �ں˷��������ָ����С�Ķ�����ô���Ǿ�ϣ������ָ���Ĵ�С�ӵ�
 * ���б�(����)�У���Ϊ��������ʹ�ڴ�ķ������Ч�����ǣ���Ϊһҳ�����ڴ�ҳ��
 * ���������б���ָ����С�����ж���������Ҫ����������Ĳ��Բ�����
 */
// �洢ͰĿ¼�б�(����)��
struct _bucket_dir bucket_dir[] = {
	{ 16,	(struct bucket_desc *) 0},// 16 �ֽڳ��ȵ��ڴ�顣
	{ 32,	(struct bucket_desc *) 0},// 32 �ֽڳ��ȵ��ڴ�顣
	{ 64,	(struct bucket_desc *) 0},// 64 �ֽڳ��ȵ��ڴ�顣
	{ 128,	(struct bucket_desc *) 0},// 128 �ֽڳ��ȵ��ڴ�顣
	{ 256,	(struct bucket_desc *) 0},// 256 �ֽڳ��ȵ��ڴ�顣
	{ 512,	(struct bucket_desc *) 0},// 512 �ֽڳ��ȵ��ڴ�顣
	{ 1024,	(struct bucket_desc *) 0},// 1024 �ֽڳ��ȵ��ڴ�顣
	{ 2048, (struct bucket_desc *) 0},// 2048 �ֽڳ��ȵ��ڴ�顣
	{ 4096, (struct bucket_desc *) 0},// 4096 �ֽ�(1 ҳ)�ڴ档
	{ 0,    (struct bucket_desc *) 0}};   /* End of list marker */

/*
 * �����Ǻ��п���Ͱ�������ڴ�������
 */
struct bucket_desc *free_bucket_desc = (struct bucket_desc *) 0;

/*
 * ������ӳ������ڳ�ʼ��һҳͰ������ҳ�档
 */
//// ��ʼ��Ͱ��������
// ��������Ͱ��������������free_bucket_desc ָ���һ������Ͱ��������
static _inline void init_bucket_desc()
{
	struct bucket_desc *bdesc, *first;
	int	i;
	
// ����һҳ�ڴ棬���ڴ��Ͱ�����������ʧ�ܣ�����ʾ��ʼ��Ͱ������ʱ�ڴ治��������Ϣ��������
	first = bdesc = (struct bucket_desc *) get_free_page();
	if (!bdesc)
		panic("Out of memory in init_bucket_desc()");
// ���ȼ���һҳ�ڴ��пɴ�ŵ�Ͱ������������Ȼ����佨����������ָ�롣
	for (i = PAGE_SIZE/sizeof(struct bucket_desc); i > 1; i--) {
		bdesc->next = bdesc+1;
		bdesc++;
	}
	/*
	 * �����������ģ�Ŀ����Ϊ�˱�����get_free_page()˯��ʱ���ӳ����ֱ� 
	 * ���ö�����ľ���������
	 */
// ������Ͱ������ָ��free_bucket_desc ���������С�
	bdesc->next = free_bucket_desc;
	free_bucket_desc = first;
}

//// ���䶯̬�ڴ溯����
// ������len - ������ڴ�鳤�ȡ�
// ���أ�ָ�򱻷����ڴ��ָ�롣���ʧ���򷵻�NULL��
void *malloc(unsigned int len)
{
	struct _bucket_dir	*bdir;
	struct bucket_desc	*bdesc;
	void			*retval;

/*
 * �������������洢ͰĿ¼bucket_dir ��Ѱ���ʺ������Ͱ��С��
 */
// �����洢ͰĿ¼��Ѱ���ʺ������ڴ���С��Ͱ�������������Ŀ¼���Ͱ�ֽ�
// ������������ֽ��������ҵ��˶�Ӧ��ͰĿ¼�
	for (bdir = bucket_dir; bdir->size; bdir++)
		if (bdir->size >= len)
			break;
// �������������Ŀ¼��û���ҵ����ʴ�С��Ŀ¼��������������ڴ���С̫�󣬳����˸�
// ����ķ�������(�Ϊ1 ��ҳ��)��������ʾ������Ϣ��������
	if (!bdir->size) {
		printk("malloc called with impossibly large argument (%d)\n",
			len);
		panic("malloc: bad arg");
	}
	/*
	 * �����������������п��пռ��Ͱ��������
	 */
	cli();	/* Ϊ�˱�����־������������ȹ��ж� */
// ������ӦͰĿ¼�����������������Ҿ��п��пռ��Ͱ�����������Ͱ�������Ŀ����ڴ�ָ��
// freeptr ��Ϊ�գ����ʾ�ҵ�����Ӧ��Ͱ��������
	for (bdesc = bdir->chain; bdesc; bdesc = bdesc->next) 
		if (bdesc->freeptr)
			break;
	/*
	 * ���û���ҵ����п��пռ��Ͱ����������ô���Ǿ�Ҫ�½���һ����Ŀ¼�����������
	 */
	if (!bdesc) {
		char		*cp;
		int		i;

// ��free_bucket_desc ��Ϊ��ʱ����ʾ��һ�ε��øó������������������г�ʼ����
// free_bucket_desc ָ���һ������Ͱ��������
		if (!free_bucket_desc)	
			init_bucket_desc();
// ȡfree_bucket_desc ָ��Ŀ���Ͱ������������free_bucket_desc ָ����һ������Ͱ��������
		bdesc = free_bucket_desc;
		free_bucket_desc = bdesc->next;
// ��ʼ�����µ�Ͱ������������������������0��Ͱ�Ĵ�С���ڶ�ӦͰĿ¼�Ĵ�С������һ�ڴ�ҳ�棬
// ����������ҳ��ָ��page ָ���ҳ�棻�����ڴ�ָ��Ҳָ���ҳ��ͷ����Ϊ��ʱȫΪ���С�
		bdesc->refcnt = 0;
		bdesc->bucket_size = bdir->size;
		bdesc->page = bdesc->freeptr = (void *) cp = (void *)get_free_page();
// ��������ڴ�ҳ�����ʧ�ܣ�����ʾ������Ϣ��������
		if (!cp)
			panic("Out of memory in kernel malloc()");
		/* �ڸ�ҳ�����ڴ��н������ж������� */
// �Ը�ͰĿ¼��ָ����Ͱ��СΪ���󳤶ȣ��Ը�ҳ�ڴ���л��֣���ʹÿ������Ŀ�ʼ4 �ֽ�����
// ��ָ����һ�����ָ�롣
		for (i=PAGE_SIZE/bdir->size; i > 1; i--) {
			*((char **) cp) = cp + bdir->size;
			cp += bdir->size;
		}
// ���һ������ʼ����ָ������Ϊ0(NULL)��
// Ȼ���ø�Ͱ����������һ������ָ���ֶ�ָ���ӦͰĿ¼��ָ��chain ��ָ������������ͰĿ¼��
// chain ָ���Ͱ��������Ҳ���������������뵽����������ͷ����
		*((char **) cp) = 0;
		bdesc->next = bdir->chain; /* OK, link it in! */
		bdir->chain = bdesc;
	}
// ����ָ�뼴���ڸ���������Ӧҳ��ĵ�ǰ����ָ�롣Ȼ������ÿ��пռ�ָ��ָ����һ�����ж���
// ��ʹ�������ж�Ӧҳ���ж������ü�����1��
	retval = (void *) bdesc->freeptr;
	bdesc->freeptr = *((void **) retval);
	bdesc->refcnt++;
// ��󿪷��жϣ�������ָ������ڴ�����ָ�롣
	sti();	/* OK�����������ְ�ȫ�� */
	return(retval);
}

/*
 * �������ͷ��ӳ��������֪���ͷŶ���Ĵ�С����free_s()��ʹ�ø���Ϣ����
 * ��Ѱ��ӦͰ���������ٶȡ�
 * 
 * ���ǽ�����һ���꣬ʹ��"free(x)"��Ϊ"free_s(x, 0)"��
 */
//// �ͷŴ洢Ͱ����
// ������obj - ��Ӧ����ָ�룻size - ��С��
void free_s(void *obj, int size)
{
	void		*page;
	struct _bucket_dir	*bdir;
	struct bucket_desc	*bdesc, *prev;

	/* ����ö������ڵ�ҳ�� */
	page = (void *)  ((unsigned long) obj & 0xfffff000);
	/* ���������洢ͰĿ¼�������ӵ�Ͱ��������Ѱ�Ҹ�ҳ�� */
	for (bdir = bucket_dir; bdir->size; bdir++) {
		prev = 0;
		/* �������size ��0�������������϶���false */
		if (bdir->size < size)
			continue;
// ������ӦĿ¼�������ӵ����������������Ҷ�Ӧҳ�档���ĳ������ҳ��ָ�����page ���ʾ�ҵ�
// ����Ӧ������������ת��found����������������ж�Ӧpage������������ָ��prev ָ�����������
		for (bdesc = bdir->chain; bdesc; bdesc = bdesc->next) {
			if (bdesc->page == page) 
				goto found;
			prev = bdesc;
		}
	}
// �������˶�ӦĿ¼���������������û���ҵ�ָ����ҳ�棬����ʾ������Ϣ��������
	panic("Bad address passed to kernel free_s()");
found:
// �ҵ���Ӧ��Ͱ�����������ȹ��жϡ�Ȼ�󽫸ö����ڴ��������п���������У�
// ��ʹ���������Ķ������ü�����1��
	cli(); /* Ϊ�˱��⾺������ */
	*((void **)obj) = bdesc->freeptr;
	bdesc->freeptr = obj;
	bdesc->refcnt--;
// ������ü����ѵ���0�������ǾͿ����ͷŶ�Ӧ���ڴ�ҳ��͸�Ͱ��������
	if (bdesc->refcnt == 0) {
		/*
		 * ������Ҫȷ��prev ��Ȼ����ȷ�ģ���ĳ�����³���ж�������
		 * ���п��ܲ����ˡ�
		 */
// ���prev �Ѿ���������������������ǰһ����������������������ǰ��������ǰһ����������
		if ((prev && (prev->next != bdesc)) ||
		    (!prev && (bdir->chain != bdesc)))
			for (prev = bdir->chain; prev; prev = prev->next)
				if (prev->next == bdesc)
					break;
// ����ҵ���ǰһ�����������������������ɾ����ǰ��������
		if (prev)
			prev->next = bdesc->next;
// ���prev==NULL����˵����ǰһ���������Ǹ�Ŀ¼���׸���������Ҳ��Ŀ¼����chain Ӧ��ֱ��
// ָ��ǰ������bdesc�������ʾ���������⣬����ʾ������Ϣ����������ˣ�Ϊ�˽���ǰ������
// ��������ɾ����Ӧ����chain ָ����һ����������
		else {
			if (bdir->chain != bdesc)
				panic("malloc bucket chains corrupted");
			bdir->chain = bdesc->next;
		}
// �ͷŵ�ǰ���������������ڴ�ҳ�棬�����������������������������ʼ����
		free_page((unsigned long) bdesc->page);
		bdesc->next = free_bucket_desc;
		free_bucket_desc = bdesc;
	}
// ���жϣ����ء�
	sti();
	return;
}

