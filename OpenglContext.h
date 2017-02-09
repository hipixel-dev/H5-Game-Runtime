#pragma once
#include "OpenglFBO.h"
#include "ImageDecoderInterface.h"
//每个canvas离屏渲染到自己独立的纹理中，所有canvas执行完毕后才统一将离屏纹理渲染出来
class OpenglContext
{
public:
	struct VIEWPORT
	{
		int x = 0;
		int y = 0;
		int width = 0;
		int height = 0;
	};
	static VIEWPORT s_rcViewPort;
// 
// 
// 	static float WindowPointToGlPointX(float x)
// 	{
// 		return 2.0f*x / s_rcViewPort.width - 1;
// 	}
// 	static float WindowPointToGlPointY(float y)
// 	{
// 		return -2.0f*y / s_rcViewPort.height + 1;
// 	}
// 	static float GlPointToWindowPointX(float x)
// 	{
// 		return (x + 1)*s_rcViewPort.width*0.5f;
// 	}
// 	static float GlPointToWindowPointY(float y)
// 	{
// 		return s_rcViewPort.height - (y + 1)*s_rcViewPort.height*0.5f;
// 	}

	class SingleColorProgram//单色填充shader
	{
	public:
		GLuint m_Program = 0;
		bool Init();
		void Uninit();
		operator GLuint()
		{
			return m_Program;
		}
		void SetColor(float r, float g, float b, float a);
		void SetVertexPosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
	};
	class LinearGradientProgram//线性渐变填充色的shader
	{
	public:
		GLuint m_Program = 0;
		bool Init();
		void Uninit();
		operator GLuint()
		{
			return m_Program;
		}
		void SetTextureUnit(GLint i);//渐变色的宽度1像素纹理
		void SetPosition(DXPOINT A, DXPOINT B, float viewportWidth, float viewportHeight);//纹理的起点、终点坐标
		void SetAlpha(float fAlpha);
		void SetVertexPosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
	};


	class TextureProgram//普通的使用一张纹理的shader
	{
	public:
		GLuint m_Program = 0;
		bool Init();
		void Uninit();
		operator GLuint();
		void SetVertexPosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
		void SetTexturePosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
		void SetTextureUnit(GLint i);
		void SetAlpha(float fAlpha);
		void SetMatrix(MATRIX* mat);
	};
	

// 	class FillColorTextProgram//使用填充色显示文字图片的shader
// 	{
// 	public:
// 		GLuint m_Program = 0;
// 		bool Init();
// 		void Uninit();
// 		operator GLuint()
// 		{
// 			return m_Program;
// 		}
// 		void SetVertexPosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
// 		void SetTexturePosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
// 		void SetTextureUnit(GLint i);
// 		void SetMatrix(MATRIX* mat);
// 		enum FILLSTYLE
// 		{
// 			FS_COLOR,//不透明区域填充单色
// 		};
// 		void SetFillColorStyle(FILLSTYLE style)
// 		{
// 
// 		}
// 
// 	};

	static SingleColorProgram s_SingleColorProgram;//单色填充
	static TextureProgram s_TextureProgram;//纹理填充
	static LinearGradientProgram s_LinearGradientProgram;//线性渐变填充
	static bool InitProgram();
	static bool Cleanup();
	static void SetViewPort(int x, int y, int width, int height);
	static int GetX();
	static int GetY();
	static int GetWidth();
	static int GetHeight();
//	static DXPOINT WindowPointToGlPoint(const DXPOINT &vPos);
	static GLuint CreateGlTexID(const ImageData&imgData);
};