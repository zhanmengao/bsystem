#pragma once


// β�÷������������ڲ����б�֮��������������
template <typename It>
auto fcn(It beg, It end) -> decltype(*beg)
{
	// ��������
	return *beg;    // ����������һ��Ԫ�ص�����
}

// Ϊ��ʹ��ģ�������Ա�������� typename
template <typename It>
auto fcn2(It beg, It end) -> typename remove_reference<decltype(*beg)>::type
{
	// ��������
	return *beg;    // ����������һ��Ԫ�صĿ���
}