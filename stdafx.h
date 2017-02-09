// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#ifdef _WIN32
#include <windows.h>
//#include "Include\GLES2\gl2.h"
#include "Include\esUtil.h"
#include <tchar.h>

#endif
// C 运行时头文件
//#define __STDC_LIMIT_MACROS
#include "mozilla/Char16.h"
#include <stddef.h>
#include <stdlib.h>
//#include <malloc.h>
#include <memory.h>
#include <inttypes.h>
#include <stdint.h>
#include <algorithm>



#include "jsapi.h"
#include "jsfriendapi.h"
#include "js/CallArgs.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <math.h>
#include <errno.h>


#ifdef _WIN32
#include <direct.h>
#include <ws2tcpip.h>
#include "Include/esUtil.h"

#define pthread_mutex_t CRITICAL_SECTION
void pthread_mutex_init(pthread_mutex_t*mutex, int i);
void pthread_mutex_destroy(pthread_mutex_t*mutex);
void pthread_mutex_unlock(pthread_mutex_t*mutex);
void pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t*mutex);
#endif

#ifdef ANDROID
#include<pthread.h>
#include<semaphore.h>
#include <GLES2/gl2.h>
#include <unistd.h>
#endif

#if defined(_IOS)
#include <sys/time.h>
#include <pthread/pthread.h>
#include <unistd.h>
#endif

#include "Macros.h"
#ifndef RGB
#define RGB(r,g,b) ((uint32_t)(((uint8_t)(r)|((uint16_t)((uint8_t)(g))<<8))|(((uint32_t)(uint8_t)(b))<<16)))
#endif

#define GetAValue(rgb) ((uint8_t)(rgb>>24))
#define PI 3.141592658975

uint32_t GetTime();





#include "JSVMInterface.h"
#include "OpenGl2.h"
// TODO:  在此处引用程序需要的其他头文件


extern OpenGl2 gl;
