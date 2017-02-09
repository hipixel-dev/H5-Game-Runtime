#ifndef MACROS_H_
#define MACROS_H_

#define NS_XB_BEGIN                     namespace xiaobai {
#define NS_XB_END                       }
#define USING_NS_XB                     using namespace xiaobai
#define NS_XB                           ::xiaobai


/// @name Xiaobai debug
#if !defined(XB_DEBUG) || XB_DEBUG == 0
#define XBLOG(...)       do {} while (0)
#define XBLOGINFO(...)   do {} while (0)
#define XBLOGERROR(...)  do {} while (0)
#define XBLOGWARN(...)   do {} while (0)

#elif XB_DEBUG == 1
#define XBLOG(format, ...)        utils::log(format, ##__VA_ARGS__)
#define XBLOGERROR(format,...)  utils::log(format, ##__VA_ARGS__)
#define XBLOGINFO(format,...)   do {} while (0)
#define XBLOGWARN(...) __LOGWITHFUNCTION(__VA_ARGS__)

#elif XB_DEBUG > 1
#define XBLOG(format, ...)      utils::log(format, ##__VA_ARGS__)
#define XBLOGERROR(format,...)  utils::log(format, ##__VA_ARGS__)
#define XBLOGINFO(format,...)   utils::log(format, ##__VA_ARGS__)
#define XBLOGWARN(...) __LOGWITHFUNCTION(__VA_ARGS__)
#endif // XB_DEBUG

/// when define returns true it means that our architecture uses big endian
#define HOST_IS_BIG_ENDIAN (bool)(*(unsigned short *)"\0\xff" < 0x100) 
#define SWAP32(i)  ((i & 0x000000ff) << 24 | (i & 0x0000ff00) << 8 | (i & 0x00ff0000) >> 8 | (i & 0xff000000) >> 24)
#define SWAP16(i)  ((i & 0x00ff) << 8 | (i &0xff00) >> 8)   
#define SWAP_INT32_LITTLE_TO_HOST(i) ((HOST_IS_BIG_ENDIAN == true)? SWAP32(i) : (i) )
#define SWAP_INT16_LITTLE_TO_HOST(i) ((HOST_IS_BIG_ENDIAN == true)? SWAP16(i) : (i) )
#define SWAP_INT32_BIG_TO_HOST(i)    ((HOST_IS_BIG_ENDIAN == true)? (i) : SWAP32(i) )
#define SWAP_INT16_BIG_TO_HOST(i)    ((HOST_IS_BIG_ENDIAN == true)? (i):  SWAP16(i) )

#if !defined(XB_DEBUG) || XB_DEBUG == 0
#define CHECK_GL_ERROR_DEBUG()
#else
#define CHECK_GL_ERROR_DEBUG() \
do { \
	GLenum __error = glGetError(); \
    if (__error) { \
	    XBLOG("OpenGL error 0x%04X in %s %s %d\n", __error, __FILE__, __FUNCTION__, __LINE__); \
	} \
} while (false)
#endif

#if defined(_MSC_VER)
#define FORMAT_PRINTF_SIZE_T "%08lX"
#else
#define FORMAT_PRINTF_SIZE_T "%08zX"
#endif

/** @def RANDOM_0_1
returns a random float between 0 and 1
*/
#define RANDOM_0_1() rand_0_1()

/** @def BLEND_SRC
default gl blend src function. Compatible with premultiplied alpha images.
*/
#define BLEND_SRC GL_ONE
#define BLEND_DST GL_ONE_MINUS_SRC_ALPHA

#define SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)
#define SAFE_RELEASE(p)          do { if(p) { (p)->release(); } } while(0)
#define SAFE_RELEASE_NULL(p)     do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SAFE_RETAIN(p)           do { if(p) { (p)->retain(); } } while(0)

#define BREAK_IF(cond)           if(cond) break

#ifdef _WIN32
#define CHECK_ARGS_COUNT(NARGS,TAG) \
/*	OutputDebugString(TAG);OutputDebugString("\n");*/\
if (args.length() != NARGS)\
{\
	PrintDescribeScriptedCaller(args.getContext());__asm{ int 3}\
/*	JS_ReportError((JSContext*)args.getContext().Get(), TAG##" : wrong number of arguments: %d, was expecting %d", args.length(), NARGS);*/\
	return false;\
}
#define CHECK_ARGS_COUNT_MIN(NARGS,TAG) \
/*OutputDebugString(TAG);OutputDebugString("\n");*/\
if (args.length() < NARGS)\
{\
	PrintDescribeScriptedCaller(args.getContext());__asm{ int 3}\
/*	JS_ReportError((JSContext*)args.getContext().Get(), TAG##" : wrong number of arguments: %d, was expecting %d", args.length(), NARGS);*/\
	return false;\
}

#define ReportError(CTX,format,...) {PrintDescribeScriptedCaller(CTX);char str[1000];sprintf(str,format, ##__VA_ARGS__);OutputDebugString(str);__asm{ int 3}}//JS_ReportError((JSContext*)CTX.Get(),format, ##__VA_ARGS__);

#define GLCHECKERR {int err = glGetError();if(err){ReportError(args.getContext(),"opengl错误:%x\n",err);}}
//#define GLCHECKERR

#elif defined(_IOS)
#define CHECK_ARGS_COUNT(NARGS,TAG) \
/*OutputDebugStringA(TAG);OutputDebugStringA("\n");*/\
if (args.length() != NARGS)\
{\
    PrintDescribeScriptedCaller(args.getContext());\
    /*	JS_ReportError((JSContext*)args.getContext().Get(), TAG##" : wrong number of arguments: %d, was expecting %d", args.length(), NARGS);*/\
    return false;\
}

#define CHECK_ARGS_COUNT_MIN(NARGS,TAG)
#define ReportError(CTX,format,...) {PrintDescribeScriptedCaller(CTX);char str[1000];printf(str,format, ##__VA_ARGS__);assert(false);}
#define GLCHECKERR {int err = glGetError();if(err){printf("!!!====opengl error:%x\n",err);assert(false);}else printf("====gl operation in func: %s\n", __FUNCTION__);}
#define GLCHECKERR {int err = glGetError();if(err){printf("!!!====opengl error:%x\n",err);assert(false);}}

#else
#define CHECK_ARGS_COUNT(NARGS,TAG)
#define CHECK_ARGS_COUNT_MIN(NARGS,TAG)
#define ReportError(CTX,format,...) 
#define GLCHECKERR
#endif

#define GL_CHECK_FRAMEBUFFER_STATUS \
GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);\
assert( status == GL_FRAMEBUFFER_COMPLETE);

#if defined(_WIN32)
#define assert(n) if(!(n)){__asm int 3}
#define usleep(n) Sleep((n)/1000)
typedef std::wstring wstring16;
#endif

#if defined(ANDROID)
#include <sys/stat.h>

#include<android/log.h>
#define LOGD(LOG,...)  __android_log_print(ANDROID_LOG_DEBUG, LOG, __VA_ARGS__) // 定义LOGD类型  
#define LOGI(LOG,...)  __android_log_print(ANDROID_LOG_INFO, LOG, __VA_ARGS__) // 定义LOGI类型  
#define LOGW(LOG,...)  __android_log_print(ANDROID_LOG_WARN, LOG, __VA_ARGS__) // 定义LOGW类型  
#define LOGE(LOG,...)  __android_log_print(ANDROID_LOG_ERROR, LOG, __VA_ARGS__) // 定义LOGE类型  
#define LOGF(LOG,...)  __android_log_print(ANDROID_LOG_FATAL, LOG, __VA_ARGS__) // 定义LOGF类型  

#define assert(n) if(!(n)){__android_log_print(ANDROID_LOG_ERROR,"xbengine","assert("#n":%s:%d\n",__FILE__,__LINE__); }

#define strnicmp(a,b,c) strncasecmp(a,b,c)
#define htonll(a) htonq(a)
#define OutputDebugStringA( lpOutputString ) __android_log_print(ANDROID_LOG_ERROR,"xbengine","%s",lpOutputString);

typedef std::basic_string<jschar, std::char_traits<jschar>, std::allocator<jschar> >wstring16;
#endif

#if defined(_IOS)
#include <assert.h>
#define OutputDebugStringA( lpOutputString )  printf(lpOutputString);

#include <string>
typedef signed   char int8_t;
typedef unsigned char uint8_t;
typedef signed   short int16_t;
typedef unsigned short uint16_t;
typedef signed   int int32_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;
typedef long long int int64_t;
typedef std::basic_string<char16_t, std::char_traits<char16_t>, std::allocator<char16_t> >wstring16;

#include <pthread/pthread.h>

#endif

#if defined(ANDROID) || defined(_IOS)
#define _stricmp strcasecmp
#define _strdup strdup
#define strtok_s strtok_r

#define BYTE uint8_t
#define WORD uint16_t
#define DWORD uint32_t
#define ULONGLONG unsigned long long
//#define BOOL int
#define TRUE 1
#define FALSE 0
#define MAX_PATH 260
#define HWND unsigned int
#define HFONT unsigned int
#define HANDLE unsigned int
#define LPVOID void*

char * _itoa(int _Val, char * _DstBuf, int _Radix);
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define GetRValue(rgb) (uint8_t(rgb))
#define GetGValue(rgb) (uint8_t(((uint16_t)(rgb)) >> 8))
#define GetBValue(rgb) (uint8_t((rgb)>>16))
#define _mkdir(n) mkdir(n,S_IRWXU|S_IRWXG|S_IRWXO)
#define _strnicmp(a,b,c) strncasecmp(a,b,c)
#define stricmp(a,b) strcasecmp(a,b)
#define strnicmp(a,b,c) strncasecmp(a,b,c)

#define closesocket(n) close(n)


#endif

#endif // MACROS_H_