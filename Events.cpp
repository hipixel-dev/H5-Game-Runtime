#include "stdafx.h"
#include "Events.h"


CEvents::CEvents()
{
#ifdef _WIN32
	m_hEvent = CreateEvent(0, true, false, 0);
#else
	int ret=sem_init(&m_hEvent, 0, 0);
#endif
}


CEvents::~CEvents()
{
#ifdef _WIN32
	CloseHandle(m_hEvent);
#else
	sem_close(&m_hEvent);
#endif
}

void CEvents::SetEvent()
{
#ifdef _WIN32
	::SetEvent(m_hEvent);
#else
	int i = 0;
	sem_getvalue(&m_hEvent, &i);
	if (i==0)
		sem_post(&m_hEvent);
#endif
}

void CEvents::ResetEvent()
{
#ifdef _WIN32
	::ResetEvent(m_hEvent);
#else
	sem_trywait(&m_hEvent);
#endif
}

void CEvents::Wait()
{
#ifdef _WIN32
	WaitForSingleObject(m_hEvent, -1);
#else
	sem_wait(&m_hEvent);
#endif
}
