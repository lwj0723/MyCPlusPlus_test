#pragma once
#include <queue>
#include <condition_variable>
#include <mutex>

/*
20210602
使用condition_variable, 实现含生产者和消费者的消息队列;
消费: 队列空阻塞;
生产: 队列超过最大值阻塞;
*/

#define  Max_Queue_Size 10

template <typename T>
class MyQueue
{
public:
	MyQueue(){}
	MyQueue(const MyQueue& e) = delete;
	MyQueue& operator=(const MyQueue& e) = delete;

public:
	T Get()
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		m_cond.wait(locker, [this] {
			return !m_dataQueue.empty();
		});

		T val = m_dataQueue.front();
		m_dataQueue.pop();
		m_cond.notify_all();//consider more threads
		return val;
	}

	void Push(T &val)
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		if (m_dataQueue.size() > Max_Queue_Size) {
			m_cond.wait(locker, [this] {
				return m_dataQueue.size() < Max_Queue_Size;
			});
		}
		m_dataQueue.push(val);
		m_cond.notify_all();
		return;
	}

private:
	std::mutex m_mutex;
	std::condition_variable m_cond;
	const int m_size = Max_Queue_Size;
	std::queue<T> m_dataQueue;
};

//////////////////////////////////////////////////////////////////////////
