#ifndef _CONST_H
#define _CONST_H

#define BUFFER_END 0x200000	// ���建��ʹ���ڴ��ĩ��(������û��ʹ�øó���)��

// i �ڵ����ݽṹ��i_mode �ֶεĸ���־λ��
#define I_TYPE 0170000		// ָ��i �ڵ����͡�
#define I_DIRECTORY 0040000	// ��Ŀ¼�ļ���
#define I_REGULAR 0100000	// �����ļ�������Ŀ¼�ļ��������ļ���
#define I_BLOCK_SPECIAL 0060000	// ���豸�����ļ���
#define I_CHAR_SPECIAL 0020000	// �ַ��豸�����ļ���
#define I_NAMED_PIPE 0010000	// �����ܵ���
#define I_SET_UID_BIT 0004000	// ��ִ��ʱ������Ч�û�id ���͡�
#define I_SET_GID_BIT 0002000	// ��ִ��ʱ������Ч��id ���͡�

#endif
