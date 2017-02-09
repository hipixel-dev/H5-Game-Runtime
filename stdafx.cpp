// stdafx.cpp : 只包括标准包含文件的源文件
// XiaobaiEngine.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO:  在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#ifndef _WIN32
char * _itoa(int _Val, char * _DstBuf, int _Radix)
{
	if (_Radix == 10)
		sprintf(_DstBuf, "%d", _Val);
	else if (_Radix == 16)
		sprintf(_DstBuf, "%x", _Val);
	else assert(0);
	return _DstBuf;
}


#endif

uint32_t GetTime()
{
#ifdef _WIN32
	return GetTickCount();
#else 
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

#ifdef _WIN32
void pthread_mutex_init(pthread_mutex_t*mutex, int i)
{
	InitializeCriticalSection(mutex);
}
void pthread_mutex_destroy(pthread_mutex_t*mutex)
{
	DeleteCriticalSection(mutex);
}
void pthread_mutex_unlock(pthread_mutex_t*mutex)
{
	LeaveCriticalSection(mutex);
}
void pthread_mutex_lock(pthread_mutex_t *mutex)
{
	EnterCriticalSection(mutex);
}
int pthread_mutex_trylock(pthread_mutex_t*mutex)
{
	return !TryEnterCriticalSection(mutex);
}
#endif