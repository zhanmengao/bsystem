#include "ArtStatistic.h"


ArtStatistic::ArtStatistic()
{
	//没啥好做的。。。
}


ArtStatistic::~ArtStatistic()
{
	//调用类的程序员忘记关闭文件了，要记得替他擦屁股，悲催啊。。。
	this->closeFile();
}

//文件操作
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
	if (this->_fs.is_open())//防止文件被反复关闭出错
	{
		this->_fs.close();
	}
}
//统计
void ArtStatistic::statistic()
{
	const std::streamsize lineSize = 1024*16;
	char buf[lineSize];
	//统计文章长度
	while (!this->_fs.eof())
	{
		//按行读取
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
	if (it == this->_words.end())  //如果词表中不存在这个单词就添加
	{
		this->_words.insert(pair<string, int>(word, 1));
	}
	else	//如果存在，那么计数器增加
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