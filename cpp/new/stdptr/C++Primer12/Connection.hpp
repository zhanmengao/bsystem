#pragma once
#include<iostream>

struct Connection
{
};
void Conn(Connection* conn)
{
	std::cout<< conn << "Connection ����" << std::endl;
	throw "����ʧ��";
}
void DisConn(Connection* conn)
{
	std::cout << conn << "Connection �Ͽ�����" << std::endl;
}
void DeleteConn(Connection* conn)
{
	std::cout << conn << "Connection �Ͽ�����" << std::endl;
	delete conn;
}

void mainConnect()
{
	//�Ϸ�������Catch���������Դ
	Connection conn;
	try
	{
		Conn(&conn);
	}
	catch (...)
	{
		DisConn(&conn);
	}

	//����һ��ɾ����������ָ��
	shared_ptr<Connection> connPtr(new Connection(), DeleteConn);
	//���쳣��������ָ��������Դ
	try
	{
		Conn(connPtr.get());
	}
	catch (...)
	{
		connPtr.reset();
	}

	unique_ptr<Connection, decltype(DeleteConn)*> uConnect(new Connection(), DeleteConn);
	//���쳣��������ָ��������Դ
	try
	{
		Conn(uConnect.get());
	}
	catch (...)
	{
		uConnect.reset();
	}
}