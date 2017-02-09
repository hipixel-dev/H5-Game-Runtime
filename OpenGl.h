#pragma once
//#include <gl/glew.h>
#include "Include/esUtil.h"
// #ifdef _DEBUG
// #pragma comment(lib,"lib/esUtil_d.lib")
// #else
// #pragma comment(lib,"lib/esUtil.lib")
// #endif
#include <map>


#ifdef WIN32
#pragma comment(lib,"OpenGL32.lib")
//#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"libEGL.lib")
#pragma comment(lib,"libGLESv2.lib")
#endif
struct MATRIX;

class CImgData;
struct _IMGINFO;
typedef struct
{
	// Handle to a program object
	GLuint programObject;

	// Attribute locations
	GLint  positionLoc;
	GLint  texCoordLoc;

	// Sampler location
	GLint samplerLoc;

	// Texture handle
	GLuint textureId;

} UserData;
class COpenGl
{
public:
	static COpenGl _instance;
	static COpenGl*GetInstance()
	{
		return &_instance;
	}
	bool m_bCanvasMode = false;

	ESContext esContext;
	UserData userData;

// 	HGLRC m_hGl;
// 	HDC m_hDC;
	int m_nWidth=0;
	int m_nHeight=0;

	GLuint pBackBuffer;


// 	std::map<std::string, GLTEXIMAGE*>m_TexFileMap;//纹理缓存
// 	CLRUMap<GLTEXTTEXIMAGEKEY>m_TexStringMap;

	COpenGl();
	~COpenGl();
	
//	bool CreateFromHDC(HDC hdc, int width, int height);
	bool CreateFromHwnd(HWND hwnd, int width, int height);
	bool BeginRender();
	void EndRender();
	bool IsInit();
	void SetSize(int width, int height);
//	bool InitialGL();
	bool SetupPixFormat(HDC hdc);


	GLuint LoadProgram(const char *vertShaderSrc, const char *fragShaderSrc);



// 	//将H5或windows坐标系转成opengl坐标系
// 	float WindowPointToGlPointX(float x);
// 	float WindowPointToGlPointY(float y);
// 	//将opengl坐标系转成H5或windows坐标系
// 	float GlPointToWindowPointX(float x);
// 	float GlPointToWindowPointY(float y);

	static EGLBoolean CreateEGLContext(EGLNativeWindowType hWnd, EGLDisplay* eglDisplay, EGLContext* eglContext, EGLSurface* eglSurface, EGLint attribList[]);
	void DeleteProgram(GLuint program);
};

