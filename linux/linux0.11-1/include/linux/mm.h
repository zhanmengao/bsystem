#ifndef _MM_H
#define _MM_H

#define PAGE_SIZE 4096		// �����ڴ�ҳ��Ĵ�С(�ֽ���)��

// ȡ����ҳ�溯��������ҳ���ַ��ɨ��ҳ��ӳ������mem_map[]ȡ����ҳ�档
extern unsigned long get_free_page (void);
// ��ָ�������ַ������һҳ�档��ҳĿ¼��ҳ���з���ָ��ҳ����Ϣ��
extern unsigned long put_page (unsigned long page, unsigned long address);
// �ͷ������ַaddr ��ʼ��һҳ���ڴ档�޸�ҳ��ӳ������mem_map[]�����ô�����Ϣ��
extern void free_page (unsigned long addr);

#endif
