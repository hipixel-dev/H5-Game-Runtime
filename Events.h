#pragma once
#ifndef _WIN32
#include <semaphore.h>
#endif
class CEvents
{
public:
#ifdef _WIN32
	HANDLE m_hEvent = 0;
#else
	sem_t m_hEvent;
#endif
	CEvents();
	~CEvents();
	void SetEvent();
	void ResetEvent();
	void Wait();
};

