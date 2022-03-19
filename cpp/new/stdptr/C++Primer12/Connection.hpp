#pragma once
#include<iostream>

struct Connection
{
};
void Conn(Connection* conn)
{
	std::cout<< conn << "Connection 连接" << std::endl;
	throw "连接失败";
}
void DisConn(Connection* conn)
{
	std::cout << conn << "Connection 断开连接" << std::endl;
}
void DeleteConn(Connection* conn)
{
	std::cout << conn << "Connection 断开连接" << std::endl;
	delete conn;
}

void mainConnect()
{
	//老方法，在Catch里边清理资源
	Connection conn;
	try
	{
		Conn(&conn);
	}
	catch (...)
	{
		DisConn(&conn);
	}

	//分配一个删除器给智能指针
	shared_ptr<Connection> connPtr(new Connection(), DeleteConn);
	//抛异常，由智能指针清理资源
	try
	{
		Conn(connPtr.get());
	}
	catch (...)
	{
		connPtr.reset();
	}

	unique_ptr<Connection, decltype(DeleteConn)*> uConnect(new Connection(), DeleteConn);
	//抛异常，由智能指针清理资源
	try
	{
		Conn(uConnect.get());
	}
	catch (...)
	{
		uConnect.reset();
	}
}