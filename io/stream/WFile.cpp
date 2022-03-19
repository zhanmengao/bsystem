#include <Windows.h>
#include <iostream>
#include <tchar.h>
 
using namespace std;
 
int mainWfile()
{
	{
		//����һ�����ļ�
		HANDLE hfile = CreateFile(_T("first.cpp"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hfile == INVALID_HANDLE_VALUE)
		{
			cout << "�����ļ�ʧ��" << endl;
		}
		else
		{
			cout << "first.cpp�����ɹ�" << endl;
		}
		CloseHandle(hfile);
	}

	{
		// ��ֻд��ʽ���Ѵ��ڵ��ļ�
		HANDLE hfile = CreateFile(_T("first.cpp"), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hfile)
		{
			cout << "open file error" << endl;
		}
		DWORD plocation_len;
		//�����ļ�ָ��(���ļ�ʱ�ļ�ָ��Ĭ�ϴ����ļ���ͷλ��,���Ҫ������д��������Ҫ�����ļ�ָ��λ��)
		plocation_len = SetFilePointer(hfile, 0, NULL, FILE_END);
		cout << "�ļ���ͷ���ļ�ָ���ֽ���:" << plocation_len << endl;
		//���ļ���д������
		char buffer[] = { "//hello my first create file!!!" };						//����buffer������Ҫд������
		DWORD writelength;																		 //����writelength,������ʵ��д��ĳ���,���Դ�ӡ����
		bool write_ret = WriteFile(hfile, &buffer, (DWORD)strlen(buffer), &writelength, NULL);
		if (write_ret == 0)
		{
			cout << " write file fault" << endl;
		}
		cout << "ʵ��д�볤�� " << writelength << endl;
		cout << "action over" << endl;

		//�����ļ�ָ��
		plocation_len = SetFilePointer(hfile, 0, NULL, FILE_END);
		cout << "�ļ���ͷ���ļ�ָ���ֽ���:" << plocation_len << endl;

		//���ļ���д������
		char buffer1[] = { "//ni hao zhong guo!" };
		DWORD writelength1;                                    //����writelength,������ʵ��д��ĳ���,���Դ�ӡ����
		bool write_ret1 = WriteFile(hfile, &buffer1, (DWORD)strlen(buffer1), &writelength1, NULL);
		if (write_ret1 == 0)
		{
			cout << " write file fault" << endl;
		}
		cout << "ʵ��д�볤�� " << writelength1 << endl;
		cout << "action1 over" << endl;
		CloseHandle(hfile);
		//�����ļ�ָ��(���ļ�ָ�����õ��ļ�ͷ)
		//plocation_len = SetFilePointer(hfile,0,NULL,FILE_BEGIN);
		//cout << "�ļ���ͷ���ļ�ָ���ֽ���:" << plocation_len << endl;
	}

   
	{
		HANDLE hFile = CreateFile(_T("first.cpp"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			cout << "open file error" << endl;
		}
		//��ȡ�ļ���С
		int file_length1 = GetFileSize(hFile, NULL);
		cout << "�ļ���СΪ" << file_length1 << endl;
		//���ļ��ж�����
		char * pbuffer11 = (char*)malloc(file_length1 + 1);  //������������Ŷ�ȡ������
		DWORD rel_read_len1 = 0;
		bool read_ret = ReadFile(hFile, pbuffer11, file_length1, &rel_read_len1, NULL);
		{
			if (read_ret == 0)
				cout << "read file error" << endl;
		}
		pbuffer11[file_length1] = '\0';
		cout << "�����ļ�ȫ������:" << pbuffer11 << endl;
		cout << "ʵ�ʶ����ĳ���" << rel_read_len1 << endl;
		free(pbuffer11);
		//�ر��ļ������
		CloseHandle(hFile);
		if (DeleteFile(_T("first.cpp")))
		{
			cout << "first.cpp��ɾ��" << endl;
		}
	}

     
	return 0;
}