#pragma once
#include<atomic>

/*
RingBuffer是生产者和消费者模型中常用的数据结构，生产者将数据追加到数组尾端，当达到数组的尾部时，生产者绕回到数组的头部；
消费者从数组头端取走数据，当到达数组的尾部时，消费者绕回到数组头部。

如果只有一个生产者和一个消费者，环形缓冲区可以无锁访问，环形缓冲区的写入index只允许生产者访问并修改，
只要生产者在更新index前将新的值保存到缓冲区中，则消费者将始终看到一致的数据结构；
读取index也只允许消费者访问并修改，消费者只要在取走数据后更新读index，则生产者将始终看到一致的数据结构。
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

	//写操作。先判断队列时否为满，如果队列未满，则先写数据，写完数据后再修改write_index。
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

	//读操作。先判断队列是否为空，如果队列不为空，则先读数据，读完再修改read_index。
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
	T* m_data;								// 数据缓冲区
};