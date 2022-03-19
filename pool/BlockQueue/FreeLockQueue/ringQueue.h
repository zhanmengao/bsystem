#pragma once
#include<atomic>

/*
RingBuffer�������ߺ�������ģ���г��õ����ݽṹ�������߽�����׷�ӵ�����β�ˣ����ﵽ�����β��ʱ���������ƻص������ͷ����
�����ߴ�����ͷ��ȡ�����ݣ������������β��ʱ���������ƻص�����ͷ����

���ֻ��һ�������ߺ�һ�������ߣ����λ����������������ʣ����λ�������д��indexֻ���������߷��ʲ��޸ģ�
ֻҪ�������ڸ���indexǰ���µ�ֵ���浽�������У��������߽�ʼ�տ���һ�µ����ݽṹ��
��ȡindexҲֻ���������߷��ʲ��޸ģ�������ֻҪ��ȡ�����ݺ���¶�index���������߽�ʼ�տ���һ�µ����ݽṹ��
*/


template <class T>
class RingBuffer
{
public:
	RingBuffer(unsigned size) : m_size(size), m_front(0), m_rear(0)
	{
		m_data = new T[size];
	}

	~RingBuffer()
	{
		delete[] m_data;
		m_data = NULL;
	}

	inline bool isEmpty() const
	{
		return m_front == m_rear;
	}

	inline bool isFull() const
	{
		return m_front == (m_rear + 1) % m_size;
	}

	//д���������ж϶���ʱ��Ϊ�����������δ��������д���ݣ�д�����ݺ����޸�write_index��
	bool push(const T& value)
	{
		if (isFull())
		{
			return false;
		}
		m_data[m_rear] = value;
		m_rear = (m_rear + 1) % m_size;
		return true;
	}

	bool push(const T* value)
	{
		if (isFull())
		{
			return false;
		}
		m_data[m_rear] = *value;
		m_rear = (m_rear + 1) % m_size;
		return true;
	}

	//�����������ж϶����Ƿ�Ϊ�գ�������в�Ϊ�գ����ȶ����ݣ��������޸�read_index��
	inline bool pop(T& value)
	{
		if (isEmpty())
		{
			return false;
		}
		value = m_data[m_front];
		m_front = (m_front + 1) % m_size;
		return true;
	}
	inline unsigned int front()const
	{
		return m_front;
	}

	inline unsigned int rear()const
	{
		return m_rear;
	}
	inline unsigned int size()const
	{
		return m_size.load();
	}
private:
	std::atomic<unsigned int>	m_size;
	std::atomic<int> m_front;
	std::atomic<int> m_rear;
	T* m_data;								// ���ݻ�����
};