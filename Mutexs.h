#pragma once

class CMutexs
{
public:
	pthread_mutex_t m_mutex;
	CMutexs();
	~CMutexs();
	void lock();
	void unlock();
};

