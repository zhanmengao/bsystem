/* passed
 *  linux/fs/exec.c
 *
 *  (C) 1991  Linus Torvalds
 */
#include <set_seg.h>

/*
 * #!��ʼ�ĳ����ⲿ������tytso ʵ�ֵġ�
 */

/*
 * ����ʱ��������1991.12.1 ʵ�ֵ�- ֻ�轫ִ���ļ�ͷ���ֶ����ڴ������
 * ������ִ���ļ������ؽ��ڴ档ִ���ļ���i �ڵ㱻���ڵ�ǰ���̵Ŀ�ִ���ֶ���
 * ("current-.h>executable")����ҳ�쳣�����ִ���ļ���ʵ�ʼ��ز����Լ���������
 *
 * �ҿ�����һ���Ժ���˵��linux �������޸ģ�ֻ���˲���2 Сʱ�Ĺ���ʱ�����ȫ
 * ʵ����������ش���
 */

#include <errno.h>// �����ͷ�ļ�������ϵͳ�и��ֳ���š�(Linus ��minix ��������)��
#include <string.h>// �ַ���ͷ�ļ�����Ҫ������һЩ�й��ַ���������Ƕ�뺯����
#include <sys/stat.h>// �ļ�״̬ͷ�ļ��������ļ����ļ�ϵͳ״̬�ṹstat{}�ͳ�����
#include <a.out.h>// a.out ͷ�ļ���������a.out ִ���ļ���ʽ��һЩ�ꡣ

#include <linux/fs.h>// �ļ�ϵͳͷ�ļ��������ļ���ṹ��file,buffer_head,m_inode �ȣ���
#include <linux/sched.h>// ���ȳ���ͷ�ļ�������������ṹtask_struct����ʼ����0 �����ݣ�
						// ����һЩ�й��������������úͻ�ȡ��Ƕ��ʽ��ຯ������䡣
#include <linux/kernel.h>// �ں�ͷ�ļ�������һЩ�ں˳��ú�����ԭ�ζ��塣
#include <linux/mm.h>// �ڴ����ͷ�ļ�������ҳ���С�����һЩҳ���ͷź���ԭ�͡�
#include <asm/segment.h>// �β���ͷ�ļ����������йضμĴ���������Ƕ��ʽ��ຯ����

extern int sys_exit(int exit_code);// �����˳�ϵͳ���á�
extern int sys_close(int fd);// �ļ��ر�ϵͳ���á�

/*
 * MAX_ARG_PAGES �������³������������ͻ�������ʹ�õ��ڴ����ҳ����
 * 32 ҳ�ڴ�Ӧ���㹻�ˣ���ʹ�û����Ͳ���(env+arg)�ռ���ܺϴﵽ128kB!
 */
#define MAX_ARG_PAGES 32

/*
 * create_tables()���������û��ڴ��н������������Ͳ����ַ������ɴ�
 * ����ָ����������ǵĵ�ַ�ŵ�"��ջ"�ϣ�Ȼ�󷵻���ջ��ָ��ֵ��
 */
//// �����û���ջ�д��������Ͳ�������ָ���
// ������p - �����ݶ�Ϊ���Ĳ����ͻ�����Ϣƫ��ָ�룻argc - ����������envc -������������
// ���أ���ջָ�롣
static unsigned long * create_tables(char * p,int argc,int envc)
{
	unsigned long *argv,*envp;
	unsigned long * sp;

// ��ջָ������4 �ֽڣ�1 �ڣ�Ϊ�߽�Ѱַ�ģ����������sp Ϊ4 ����������
	sp = (unsigned long *) (0xfffffffc & (unsigned long) p);
// sp �����ƶ����ճ���������ռ�õĿռ���������û�������ָ��envp ָ��ô���
	sp -= envc+1;
	envp = sp;
// sp �����ƶ����ճ������в���ָ��ռ�õĿռ����������argv ָ��ָ��ô���
// ����ָ���1��sp ������ָ�����ֽ�ֵ��
	sp -= argc+1;
	argv = sp;
// ����������ָ��envp �������в���ָ���Լ������в�������ѹ���ջ��
	put_fs_long((unsigned long)envp,--sp);
	put_fs_long((unsigned long)argv,--sp);
	put_fs_long((unsigned long)argc,--sp);
// �������и�����ָ�����ǰ��ճ�������Ӧ�ط���������һ��NULL ָ�롣
	while (argc-->0) {
		put_fs_long((unsigned long) p,argv++);
		while (get_fs_byte(p++)) /* nothing */ ;// p ָ��ǰ��4 �ֽڡ�
	}
	put_fs_long(0,argv);
// ������������ָ�����ǰ��ճ�������Ӧ�ط���������һ��NULL ָ�롣
	while (envc-->0) {
		put_fs_long((unsigned long) p,envp++);
		while (get_fs_byte(p++)) /* nothing */ ;
	}
	put_fs_long(0,envp);
	return sp;		// ���ع���ĵ�ǰ�¶�ջָ�롣
}

/*
 * count()�������������в���/���������ĸ�����
 */
//// �������������
// ������argv - ����ָ�����飬���һ��ָ������NULL��
// ���أ�����������
static int count(char ** argv)
{
	int i=0;
	char ** tmp;

	if (tmp = argv)
		while (get_fs_long((unsigned long *) (tmp++)))
			i++;

	return i;
}

/*
 * 'copy_string()'�������û��ڴ�ռ俽�������ͻ����ַ������ں˿���ҳ���ڴ��С�
 * ��Щ�Ѿ���ֱ�ӷŵ����û��ڴ��еĸ�ʽ��
 *
 * ��TYT(Tytso)��1991.12.24 ���޸ģ�������from_kmem �������ò���ָ�����ַ�����
 * �ַ��������������û��λ����ں˶Ρ�
 * 
 * from_kmem     argv *        argv **
 *    0          �û��ռ�      �û��ռ�
 *    1          �ں˿ռ�      �û��ռ�
 *    2          �ں˿ռ�      �ں˿ռ�
 * 
 * ������ͨ�������fs �μĴ����������ġ����ڼ���һ���μĴ�������̫������
 * ���Ǿ����������set_fs()������ʵ�ڱ�Ҫ��
 */
//// ����ָ�������Ĳ����ַ����������ͻ����ռ䡣
// ������argc - ����ӵĲ���������argv - ����ָ�����飻page - �����ͻ����ռ�ҳ��ָ�����顣
//       p -�ڲ�����ռ��е�ƫ��ָ�룬ʼ��ָ���Ѹ��ƴ���ͷ����from_kmem - �ַ�����Դ��־��
// ��do_execve()�����У�p ��ʼ��Ϊָ�������(128kB)�ռ�����һ�����ִ��������ַ���
// ���Զ�ջ������ʽ���������и��ƴ�ŵģ����p ָ���ʼ��ָ������ַ�����ͷ����
// ���أ������ͻ����ռ䵱ǰͷ��ָ�롣
static unsigned long copy_strings(int argc,char ** argv,unsigned long *page,
		unsigned long p, int from_kmem)
{
	char *tmp, *pag;
	int len, offset = 0;
	unsigned long old_fs, new_fs;

	if (!p)
		return 0;	/* ƫ��ָ����֤ */
// ȡds �Ĵ���ֵ��new_fs��������ԭfs �Ĵ���ֵ��old_fs��
	new_fs = get_ds();
	old_fs = get_fs();
// ����ַ������ַ������������ں˿ռ䣬������fs �μĴ���ָ���ں����ݶΣ�ds����
	if (from_kmem==2)
		set_fs(new_fs);
// ѭ��������������������һ����������ʼ���ƣ����Ƶ�ָ��ƫ�Ƶ�ַ����
	while (argc-- > 0) {
// ����ַ������û��ռ���ַ����������ں˿ռ䣬������fs �μĴ���ָ���ں����ݶΣ�ds����
		if (from_kmem == 1)
			set_fs(new_fs);
// �����һ��������ʼ���������ȡfs �������һ����ָ�뵽tmp�����Ϊ�գ������������
		if (!(tmp = (char *)get_fs_long(((unsigned long *)argv)+argc)))
			panic("argc is wrong");
// ����ַ������û��ռ���ַ����������ں˿ռ䣬��ָ�fs �μĴ���ԭֵ��
		if (from_kmem == 1)
			set_fs(old_fs);
// ����ò����ַ�������len����ʹtmp ָ��ò����ַ���ĩ�ˡ�
		len=0;		/* ����֪��������NULL �ֽڽ�β�� */
		do {
			len++;
		} while (get_fs_byte(tmp++));
// ������ַ������ȳ�����ʱ�����ͻ����ռ��л�ʣ��Ŀ��г��ȣ���ָ�fs �μĴ���������0��
		if (p-len < 0) {	/* ���ᷢ��-��Ϊ��128kB �Ŀռ� */
			set_fs(old_fs);
			return 0;
		}
// ����fs ���е�ǰָ���Ĳ����ַ������ǴӸ��ַ���β����ʼ���ơ�
		while (len) {
			--p; --tmp; --len;
// �����տ�ʼִ��ʱ��ƫ�Ʊ���offset ����ʼ��Ϊ0�������offset-1<0��˵�����״θ����ַ�����
// ���������p ָ����ҳ���ڵ�ƫ��ֵ�����������ҳ�档
			if (--offset < 0) {
				offset = p % PAGE_SIZE;
// ����ַ������ַ����������ں˿ռ䣬��ָ�fs �μĴ���ԭֵ��
				if (from_kmem==2)
					set_fs(old_fs);
// �����ǰƫ��ֵp ���ڵĴ��ռ�ҳ��ָ��������page[p/PAGE_SIZE]==0����ʾ��Ӧҳ�滹�����ڣ�
// ���������µ��ڴ����ҳ�棬����ҳ��ָ������ָ�����飬����Ҳʹpag ָ�����ҳ�棬�����벻
// ������ҳ���򷵻�0��
				if (!(pag = (char *) page[p/PAGE_SIZE]) &&
				    !(pag = (char *) page[p/PAGE_SIZE] = 
				      (char *) get_free_page())) 
					return 0;
// ����ַ������ַ������������ں˿ռ䣬������fs �μĴ���ָ���ں����ݶΣ�ds����
				if (from_kmem==2)
					set_fs(new_fs);

			}
// ��fs ���и��Ʋ����ַ�����һ�ֽڵ�pag+offset ����
			*(pag + offset) = get_fs_byte(tmp);
		}
	}
// ����ַ������ַ����������ں˿ռ䣬��ָ�fs �μĴ���ԭֵ��
	if (from_kmem==2)
		set_fs(old_fs);
// ��󣬷��ز����ͻ����ռ����Ѹ��Ʋ�����Ϣ��ͷ��ƫ��ֵ��
	return p;
}

//// �޸ľֲ����������е���������ַ�Ͷ��޳������������ͻ����ռ�ҳ����������ݶ�ĩ�ˡ�
// ������text_size - ִ���ļ�ͷ����a_text �ֶθ����Ĵ���γ���ֵ��
//       page - �����ͻ����ռ�ҳ��ָ�����顣
// ���أ����ݶ��޳�ֵ(64MB)��
static unsigned long change_ldt(unsigned long text_size,unsigned long * page)
{
	unsigned long code_limit,data_limit,code_base,data_base;
	int i;

// ����ִ���ļ�ͷ��a_text ֵ��������ҳ�泤��Ϊ�߽�Ĵ�����޳������������ݶγ���Ϊ64MB��
	code_limit = text_size+PAGE_SIZE -1;
	code_limit &= 0xFFFFF000;
	data_limit = 0x4000000;
// ȡ��ǰ�����оֲ��������������������д���λ�ַ������λ�ַ�����ݶλ�ַ��ͬ��
	code_base = get_base(current->ldt[1]);
	data_base = code_base;
// �������þֲ����д���κ����ݶ��������Ļ�ַ�Ͷ��޳���
	set_base(current->ldt[1],code_base);
	set_limit(current->ldt[1],code_limit);
	set_base(current->ldt[2],data_base);
	set_limit(current->ldt[2],data_limit);
/* Ҫȷ��fs �μĴ�����ָ���µ����ݶ� */
// fs �μĴ����з���ֲ������ݶ���������ѡ���(0x17)��
//	__asm__("pushl $0x17\n\tpop %%fs"::);
	_asm {
		push 0x17
		pop fs
	}
// �������ͻ����ռ��Ѵ�����ݵ�ҳ�棨������MAX_ARG_PAGES ҳ��128kB���ŵ����ݶ����Ե�ַ��
// ĩ�ˡ��ǵ��ú���put_page()���в����ģ�mm/memory.c, 197����
	data_base += data_limit;
	for (i=MAX_ARG_PAGES-1 ; i>=0 ; i--) {
		data_base -= PAGE_SIZE;
		if (page[i])						// �����ҳ����ڣ�
			put_page(page[i],data_base);	// �ͷ��ø�ҳ�档
	}
	return data_limit;		// ��󷵻����ݶ��޳�(64MB)��
}

/*
 * 'do_execve()'����ִ��һ���³���
 */
//// execve()ϵͳ�жϵ��ú��������ز�ִ���ӽ��̣��������򣩡�
// �ú���ϵͳ�жϵ���(int 0x80)���ܺ�__NR_execve ���õĺ�����
// ������eip - ָ���ջ�е���ϵͳ�жϵĳ������ָ��eip �����μ�kernel/system_call.s ����
// ��ʼ���ֵ�˵����tmp - ϵͳ�жϵ��ñ�����ʱ�ķ��ص�ַ�����ã�
//                 filename - ��ִ�г����ļ�����argv - �����в���ָ�����飻
//                 envp - ��������ָ�����顣
// ���أ�������óɹ����򲻷��أ��������ó���ţ�������-1��
int do_execve(unsigned long * eip,long tmp,char * filename,
	char ** argv, char ** envp)
{
	struct m_inode * inode;				// �ڴ���I �ڵ�ָ��ṹ������
	struct buffer_head * bh;			// ���ٻ����ͷָ�롣
	struct exec ex;						// ִ���ļ�ͷ�����ݽṹ������
	unsigned long page[MAX_ARG_PAGES];	// �����ͻ����ַ����ռ��ҳ��ָ�����顣
	int i,argc,envc;
	int e_uid, e_gid;					// ��Ч�û�id ����Ч��id��
	int retval;							// ����ֵ��
	int sh_bang = 0;					// �����Ƿ���Ҫִ�нű�������롣
// �����ͻ����ַ����ռ��е�ƫ��ָ�룬��ʼ��Ϊָ��ÿռ�����һ�����ִ���
	unsigned long p=PAGE_SIZE*MAX_ARG_PAGES-4;

// eip[1]����ԭ����μĴ���cs�����е�ѡ������������ں˶�ѡ�����Ҳ���ں˲��ܵ��ñ�������
	if ((0xffff & eip[1]) != 0x000f)
		panic("execve called from supervisor mode");
// ��ʼ�������ͻ������ռ��ҳ��ָ�����飨����
	for (i=0 ; i<MAX_ARG_PAGES ; i++)	/* clear page-table */
		page[i]=0;
// ȡ��ִ���ļ��Ķ�Ӧi �ڵ�š�
	if (!(inode=namei(filename)))		/* get executables inode */
		return -ENOENT;
// ������������ͻ�������������
	argc = count(argv);
	envc = count(envp);
	
// ִ���ļ������ǳ����ļ��������ǳ����ļ����ó������룬��ת��exec_error2(��347 ��)��
restart_interp:
	if (!S_ISREG(inode->i_mode)) {	/* must be regular file */
		retval = -EACCES;
		goto exec_error2;
	}
// ��鱻ִ���ļ���ִ��Ȩ�ޡ�����������(��Ӧi �ڵ��uid ��gid)�����������Ƿ���Ȩִ������
	i = inode->i_mode;
	e_uid = (i & S_ISUID) ? inode->i_uid : current->euid;
	e_gid = (i & S_ISGID) ? inode->i_gid : current->egid;
	if (current->euid == inode->i_uid)
		i >>= 6;
	else if (current->egid == inode->i_gid)
		i >>= 3;
	if (!(i & 1) &&
	    !((inode->i_mode & 0111) && suser())) {
		retval = -ENOEXEC;
		goto exec_error2;
	}
// ��ȡִ���ļ��ĵ�һ�����ݵ����ٻ����������������ó����룬��ת��exec_error2 ��ȥ����
	if (!(bh = bread(inode->i_dev,inode->i_zone[0]))) {
		retval = -EACCES;
		goto exec_error2;
	}
// �����ִ���ļ���ͷ�ṹ���ݽ��д���������ex ָ��ִ��ͷ���ֵ����ݽṹ��
	ex = *((struct exec *) bh->b_data);	/* ��ȡִ��ͷ���� */
// ���ִ���ļ���ʼ�������ֽ�Ϊ'#!'������sh_bang ��־û����λ������ű��ļ���ִ�С�
	if ((bh->b_data[0] == '#') && (bh->b_data[1] == '!') && (!sh_bang)) {
		/*
		 * �ⲿ�ִ����'#!'�Ľ��ͣ���Щ���ӣ���ϣ���ܹ�����-TYT
		 */

		char buf[1023], *cp, *interp, *i_name, *i_arg;
		unsigned long old_fs;

// ����ִ�г���ͷһ���ַ�'#!'������ַ�����buf �У����к��нű������������
		strncpy(buf, bh->b_data+2, 1022);
// �ͷŸ��ٻ����͸�ִ���ļ�i �ڵ㡣
		brelse(bh);
		iput(inode);
// ȡ��һ�����ݣ���ɾ����ʼ�Ŀո��Ʊ����
		buf[1022] = '\0';
		if (cp = strchr(buf, '\n')) {
			*cp = '\0';
			for (cp = buf; (*cp == ' ') || (*cp == '\t'); cp++);
		}
// ������û���������ݣ�������ó����룬��ת��exec_error1 ����
		if (!cp || *cp == '\0') {
			retval = -ENOEXEC; /* No interpreter name found */
			goto exec_error1;
		}
// ����͵õ��˿�ͷ�ǽű�����ִ�г������Ƶ�һ�����ݡ�
		interp = i_name = cp;
// ����������С�����ȡ��һ���ַ�������Ӧ���ǽű����ͳ�������iname ָ������ơ�
		i_arg = 0;
		for ( ; *cp && (*cp != ' ') && (*cp != '\t'); cp++) {
 			if (*cp == '/')
				i_name = cp+1;
		}
// ���ļ��������ַ�����Ӧ���ǲ���������i_arg ָ��ô���
		if (*cp) {
			*cp++ = '\0';
			i_arg = cp;
		}
		/*
		 * OK�������Ѿ����������ͳ�����ļ����Լ�(��ѡ��)������
		 */
// ��sh_bang ��־û�����ã�����������������ָ�������Ļ����������Ͳ������������ͻ����ռ��С�
		if (sh_bang++ == 0) {
			p = copy_strings(envc, envp, page, p, 0);
			p = copy_strings(--argc, argv+1, page, p, 0);
		}
		/*
		 * ƴ��(1) argv[0]�зŽ��ͳ��������
		 * (2) (��ѡ��)���ͳ���Ĳ���
		 * (3) �ű����������
		 *
		 * ������������д���ģ��������û������Ͳ����Ĵ�ŷ�ʽ��ɵġ�
		 */
// ���ƽű������ļ����������ͻ����ռ��С�
		p = copy_strings(1, &filename, page, p, 1);
// ���ƽ��ͳ���Ĳ����������ͻ����ռ��С�
		argc++;
		if (i_arg) {
			p = copy_strings(1, &i_arg, page, p, 2);
			argc++;
		}
// ���ƽ��ͳ����ļ����������ͻ����ռ��С����������ó����룬��ת��exec_error1��
		p = copy_strings(1, &i_name, page, p, 2);
		argc++;
		if (!p) {
			retval = -ENOMEM;
			goto exec_error1;
		}
		/*
		 * OK������ʹ�ý��ͳ����i �ڵ��������̡�
		 */
// ����ԭfs �μĴ�����ԭָ���û����ݶΣ���������ָ���ں����ݶΡ�
		old_fs = get_fs();
		set_fs(get_ds());
// ȡ���ͳ����i �ڵ㣬����ת��restart_interp �����´���
		if (!(inode=namei(interp))) { /* get executables inode */
			set_fs(old_fs);
			retval = -ENOENT;
			goto exec_error1;
		}
		set_fs(old_fs);
		goto restart_interp;
	}
// �ͷŸû�������
	brelse(bh);
// �����ִ��ͷ��Ϣ���д���
// �����������������ִ�г������ִ���ļ���������ҳ��ִ���ļ�(ZMAGIC)�����ߴ����ض�λ����
// ����a_trsize ������0�����������ض�λ��Ϣ���Ȳ�����0�����ߴ����+���ݶ�+�Ѷγ��ȳ���50MB��
// ����i �ڵ�����ĸ�ִ���ļ�����С�ڴ����+���ݶ�+���ű���+ִ��ͷ���ֳ��ȵ��ܺ͡�
	if (N_MAGIC(ex) != ZMAGIC || ex.a_trsize || ex.a_drsize ||
		ex.a_text+ex.a_data+ex.a_bss>0x3000000 ||
		inode->i_size < ex.a_text+ex.a_data+ex.a_syms+N_TXTOFF(ex)) {
		retval = -ENOEXEC;
		goto exec_error2;
	}
// ���ִ���ļ�ִ��ͷ���ֳ��Ȳ�����һ���ڴ���С��1024 �ֽڣ���Ҳ����ִ�С�תexec_error2��
	if (N_TXTOFF(ex) != BLOCK_SIZE) {
		printk("%s: N_TXTOFF != BLOCK_SIZE. See a.out.h.", filename);
		retval = -ENOEXEC;
		goto exec_error2;
	}
// ���sh_bang ��־û�����ã�����ָ�������Ļ��������ַ����Ͳ����������ͻ����ռ��С�
// ��sh_bang ��־�Ѿ����ã�������ǽ����нű����򣬴�ʱ��������ҳ���Ѿ����ƣ������ٸ��ơ�
	if (!sh_bang) {
		p = copy_strings(envc,envp,page,p,0);
		p = copy_strings(argc,argv,page,p,0);
// ���p=0�����ʾ��������������ռ�ҳ���Ѿ���ռ�������ɲ����ˡ�ת����������
		if (!p) {
			retval = -ENOMEM;
			goto exec_error2;
		}
	}
/* OK�����濪ʼ��û�з��صĵط��� */
// ���ԭ����Ҳ��һ��ִ�г������ͷ���i �ڵ㣬���ý���executable �ֶ�ָ���³���i �ڵ㡣
	if (current->executable)
		iput(current->executable);
	current->executable = inode;
// �帴λ�����źŴ�������������SIG_IGN ������ܸ�λ�������322 ��323 ��֮�������һ��
// if ��䣺if (current->sa[I].sa_handler != SIG_IGN)������Դ�����е�һ��bug��
	for (i=0 ; i<32 ; i++)
		current->sigaction[i].sa_handler = NULL;
// ����ִ��ʱ�ر�(close_on_exec)�ļ����λͼ��־���ر�ָ���Ĵ��ļ�������λ�ñ�־��
	for (i=0 ; i<NR_OPEN ; i++)
		if ((current->close_on_exec>>i)&1)
			sys_close(i);
	current->close_on_exec = 0;
// ����ָ���Ļ���ַ���޳����ͷ�ԭ���������κ����ݶ�����Ӧ���ڴ�ҳ��ָ�����ڴ�鼰ҳ����
	free_page_tables(get_base(current->ldt[1]),get_limit(0x0f));
	free_page_tables(get_base(current->ldt[2]),get_limit(0x17));
// ������ϴ�����ʹ����Э��������ָ����ǵ�ǰ���̣������ÿգ�����λʹ����Э�������ı�־��
	if (last_task_used_math == current)
		last_task_used_math = NULL;
	current->used_math = 0;
// ����a_text �޸ľֲ�������������ַ�Ͷ��޳������������ͻ����ռ�ҳ����������ݶ�ĩ�ˡ�
// ִ���������֮��p ��ʱ�������ݶ���ʼ��Ϊԭ���ƫ��ֵ����ָ������ͻ����ռ����ݿ�ʼ����
// Ҳ��ת����Ϊ��ջ��ָ�롣
	p += change_ldt(ex.a_text,page)-MAX_ARG_PAGES*PAGE_SIZE;
// create_tables()�����û���ջ�д��������Ͳ�������ָ��������ظö�ջָ�롣
	p = (unsigned long) create_tables((char *)p,argc,envc);
// �޸ĵ�ǰ���̸��ֶ�Ϊ��ִ�г������Ϣ������̴����βֵ�ֶ�end_code = a_text�����������
// ��β�ֶ�end_data = a_data + a_text������̶ѽ�β�ֶ�brk = a_text + a_data + a_bss��
	current->brk = ex.a_bss +
		(current->end_data = ex.a_data +
		(current->end_code = ex.a_text));
// ���ý��̶�ջ��ʼ�ֶ�Ϊ��ջָ�����ڵ�ҳ�棬���������ý��̵��û�id ����id��
	current->start_stack = p & 0xfffff000;
	current->euid = e_uid;
	current->egid = e_gid;
// ��ʼ��һҳbss �����ݣ�ȫΪ�㡣
	i = ex.a_text+ex.a_data;
	while (i&0xfff)
		put_fs_byte(0,(char *) (i++));
// ��ԭ����ϵͳ�жϵĳ����ڶ�ջ�ϵĴ���ָ���滻Ϊָ����ִ�г������ڵ㣬������ջָ���滻
// Ϊ��ִ�г���Ķ�ջָ�롣����ָ�������Щ��ջ���ݲ�ʹ��CPU ȥִ���µ�ִ�г�����˲���
// ���ص�ԭ����ϵͳ�жϵĳ�����ȥ�ˡ�
	eip[0] = ex.a_entry;		/* eip��ħ���������� :-) */
	eip[3] = p;			/* esp����ջָ�� */
	return 0;
exec_error2:
	iput(inode);
exec_error1:
	for (i=0 ; i<MAX_ARG_PAGES ; i++)
		free_page(page[i]);
	return(retval);
}
