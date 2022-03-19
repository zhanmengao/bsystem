#pragma once
#include<fstream>

class CppFile
{
public:
	//in 读方式打开			out 写方式打开
	//app 每次写操作前均定位到文件末尾
	//ate 打开文件后定位到文件末尾
	//trunc 覆盖写（写模式默认）
	//binary 字节流
	bool Open(const char* filePath, std::ios::openmode model = std::ios::app)
	{
		Close();
		fs.open(filePath,std::ios::in | std::ios::out | model);
		return fs.is_open();
	}
	std::streamsize Read(char* buffer, size_t nSize)
	{
		fs.read(buffer, nSize);
		return fs.gcount();
	}
	std::streamsize ReadLine(char* buffer, size_t nSize)
	{
		fs.getline(buffer, nSize);
		return fs.gcount();
	}
	bool End()
	{
		return fs.eof();
	}
	std::fstream::pos_type Pos()
	{
		return fs.tellg();
	}
	std::streamsize FileSize()
	{
		std::streampos pos = fs.tellg();
		Seek(0, std::ios::end);
		std::streampos size = fs.tellg();
		//还原
		Seek(pos);
		return size;
	}
	void Seek(std::fstream::pos_type offset, std::ios_base::seekdir way = std::ios::beg)
	{
		fs.seekg(offset, way);
	}
	std::streamsize Write(char* buffer, size_t nSize)
	{
		fs.write(buffer, nSize);
		return fs.gcount();
	}
	//可以使用>> <<，遇到空格或者换行等停止
	std::fstream& stream()
	{
		return fs;
	}
	void Close()
	{
		if (fs.is_open())
		{
			fs.close();
		}
	}
private:
	std::fstream fs;
};

/*
ofstream流，以ios::app打开（或者“ios::app|ios::out”）,如果没有文件，那么生成空文件；如果有文件，那么在文件尾追加。
以ios::app|ios::in打开，不管有没有文件，都是失败。
以ios::ate打开（或者”ios::ate|ios::out”），如果没有文件，那么生成空文件；如果有文件，那么清空该文件
以ios::ate|ios::in打开，如果没有文件，那么打开失败；如果有文件，那么定位到文件尾，并可以写文件，但是不能读文件

ifstream流，以ios::app打开（“ios::app|ios::out”）,不管有没有文件，打开都是失败。
以ios::ate打开（“ios::ate|ios::out”）,如果没有文件，打开失败
如果有文件，打开成功，并定位到文件 尾，但是不能写文件

fstream流，默认是ios::in，所以如果没有文件，ios::app和ios::ate都是失败，
以ios::app|ios::out,如果没有文件则创建文件，如果有文件，则在文件尾追加
以ios::ate|ios::out打开，如果没有文件则创建文件，如果有，则清空文件。
以ios::ate|ios::out|ios::in打开，如果没有文件，则打开失败，有文件则定位到文件尾
*/
