#include "stdafx.h"
#include "Mutexs.h"


CMutexs::CMutexs()
{
	pthread_mutex_init(&m_mutex, 0);
}


CMutexs::~CMutexs()
{
	pthread_mutex_destroy(&m_mutex);

}

void CMutexs::lock()
{
	pthread_mutex_lock(&m_mutex);
}

void CMutexs::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}


