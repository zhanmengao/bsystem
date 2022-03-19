#include<iostream>
#include <string>
#include <vector>
using namespace std;

//�滻�ո�
void ReplaceBlank(string &str, const string& regex, const string &replacement)
{
	//�����ҵ�����regex
}
int* getNext(const string& zi);
int KMP(const string &fu, const string& zi,int pos=0)
{
	if (pos >= fu.length())
	{
		return -1;
	}
	//��ȡNext����
	int *next = getNext(zi);
	//��������
	int i = pos;
	//�Ӵ�����
	int j = 0;
	//���ش�pos��ʼ��һ��ƥ�����Ϣ
	while (i<fu.length())
	{
		if (fu[i] == zi[j])
		{
			i++;
			j++;
			if (j >= zi.length())
			{
				return i - j;
			}
		}
		//��ƥ��->j��˷
		else if(j>0){
			j = next[j - 1];
		}
		else{
			i++;
		}
	}
	return -1;
}
int* getNext(const string& zi)
{
	int *next = new int[zi.length()];
	//ǰ׺û�������Ϊ0����һ�������Ϊ1
	next[0] = 0;
	//ǰ׺
	int top = 0;
	//��׺
	int bottom = 1;
	//����zi
	while (top < zi.length())
	{
		if (zi[top]==zi[bottom])
		{
			next[top] = bottom + 1;
			//���Ƽ����Ƚ�
			top++;
			bottom++;
		}
		else if(top>0){
			//��˷
			bottom = next[bottom - 1];
		}
		else{
			next[bottom] = 0;
			top++;
		}
	}
	return next;
}
void main255()
{
	vector<int> v1;
	cout << sizeof(v1)<<endl;
}