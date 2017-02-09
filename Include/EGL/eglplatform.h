/* -*- mode: c; tab-width: 8; -*- */
/* vi: set sw=4 ts=8: */
/* Platform-specific types and definitions for egl.h */

#ifndef __eglplatform_h_
#define __eglplatform_h_

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#endif

#if defined(_WIN32)
typedef HDC     EGLNativeDisplayType;
typedef HBITMAP EGLNativePixmapType;
typedef HWND    EGLNativeWindowType;

#elif defined(__ANDROID__) || defined(ANDROID)

#include <android/native_window.h>

struct egl_native_pixmap_t;

typedef struct ANativeWindow*           EGLNativeWindowType;
typedef struct egl_native_pixmap_t*     EGLNativePixmapType;
typedef void*                           EGLNativeDisplayType;
#endif

#ifndef EGLAPIENTRY
#define EGLAPIENTRY __stdcall
#endif

#ifndef EGLAPI
#ifdef __EGL_EXPORTS
#    define EGLAPI __declspec(dllexport)
#else
#    define EGLAPI __declspec(dllimport)
#endif
#endif

#if defined(_WIN32) || defined(__ANDROID__) || defined(ANDROID)
/* EGL 1.2 types, renamed for consistency in EGL 1.3 */
typedef EGLNativeDisplayType NativeDisplayType;
typedef EGLNativePixmapType  NativePixmapType;
typedef EGLNativeWindowType  NativeWindowType;
#endif

#endif /* __eglplatform_h */