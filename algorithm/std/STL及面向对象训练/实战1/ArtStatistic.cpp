#include "ArtStatistic.h"


ArtStatistic::ArtStatistic()
{
	//ûɶ�����ġ�����
}


ArtStatistic::~ArtStatistic()
{
	//������ĳ���Ա���ǹر��ļ��ˣ�Ҫ�ǵ�������ƨ�ɣ����߰�������
	this->closeFile();
}

//�ļ�����
bool ArtStatistic::openFile(string filePath)
{
	this->_fs.open(filePath, ios::in);
	if (this->_fs.is_open())
	{
		return true;
	}
	return false;
}
void ArtStatistic::closeFile()
{
	if (this->_fs.is_open())//��ֹ�ļ��������رճ���
	{
		this->_fs.close();
	}
}
//ͳ��
void ArtStatistic::statistic()
{
	const std::streamsize lineSize = 1024*16;
	char buf[lineSize];
	//ͳ�����³���
	while (!this->_fs.eof())
	{
		//���ж�ȡ
		this->_fs.getline(buf, lineSize);
		cout << buf << endl;
		Spliter spliter(buf);
		while (!spliter.eos() && strlen(buf) > 0)
		{
			this->addWord(this->toLowerString(spliter.getWord()) );
		}
	}
}

void ArtStatistic::addWord(string &word)
{
	map<string, int>::iterator it = this->_words.find(word);
	if (it == this->_words.end())  //����ʱ��в�����������ʾ����
	{
		this->_words.insert(pair<string, int>(word, 1));
	}
	else	//������ڣ���ô����������
	{
		it->second++;
	}
}

void ArtStatistic::outDatas()
{
	cout << "The Words:" << endl << "==========================" << endl;
	map<string, int>::iterator it = this->_words.begin();
	while (it != this->_words.end())
	{
		cout << it->first << ":" << it->second << endl;
		it++;
	}
	cout << "==========================" << endl << "words number:" << this->_words.size() << endl;
}