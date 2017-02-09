#pragma once

#if defined(_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

struct DXPOINT3
{
	DXPOINT3()
	{

	}
	DXPOINT3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float x;
	float y;
	float z;
};
struct DXPOINT
{
	float x;
	float y;

	DXPOINT(){ x = 0; y = 0; };
	DXPOINT(std::initializer_list<int>lis)
	{
		const int*p = lis.begin();
		x = p[0];
		y = p[1];
	}
	DXPOINT(std::initializer_list<float>lis)
	{
		const float*p = lis.begin();
		x = p[0];
		y = p[1];

	}
	DXPOINT(float X, float Y)
	{
		x = X;
		y = Y;
	}
// 	DXPOINT(int X, int Y)
// 	{
// 		new(this)DXPOINT((float)X, (float)Y);
// 	}

	float GetLength()
	{
		return sqrtf(x*x + y*y);
	}
	float operator*(const DXPOINT&src)//点乘
	{
		return x*src.x + y*src.y;
	}
	DXPOINT3 cross(const DXPOINT&src)//叉乘
	{
		return DXPOINT3(y - src.y, src.x - x, x*src.y - y*src.x);
	}

	DXPOINT Rotate(float angle)//旋转是顺时针
	{
		float cosa = cosf(angle);
		float sina = sinf(angle);
		float X = x*cosa - y*sina;
		float Y = x*sina + y*cosa;
		return DXPOINT(X, Y);
	}
	DXPOINT operator+(const DXPOINT &pt)
	{
		return DXPOINT(x + pt.x, y + pt.y);
	}
	DXPOINT operator-(const DXPOINT&pt)
	{
		return DXPOINT(x - pt.x, y - pt.y);
	}
	DXPOINT&operator+=(const DXPOINT &pt)
	{
		x += pt.x;
		y += pt.y;
		return *this;
	}
	bool operator==(const DXPOINT&pt)
	{
		return x == pt.x&&y == pt.y;
	}
	bool operator!=(const DXPOINT&pt)
	{
		return !(*this == pt);
	}
	// 	DXPOINT operator*(const MATRIX &mat)
	// 	{
	// 		DXPOINT ret;
	// 		ret.x = x*mat._11 + y*mat._21 + 1 * mat._31;
	// 		ret.y = x*mat._12 + y*mat._22 + 1 * mat._32;
	// 		return ret;
	// 	}
};

struct MATRIX {
	union {
		struct {
			float        _11, _12, _13;
			float        _21, _22, _23;
			float        _31, _32, _33;

		};
		float m[3][3];
	};
	MATRIX operator *(const MATRIX&mat) const
	{
		MATRIX ret;
		ret._11 = _11*mat._11 + _12*mat._21 + _13*mat._31;
		ret._12 = _11*mat._12 + _12*mat._22 + _13*mat._32;
		ret._13 = _11*mat._13 + _12*mat._23 + _13*mat._33;
		ret._21 = _21*mat._11 + _22*mat._21 + _23*mat._31;
		ret._22 = _21*mat._12 + _22*mat._22 + _23*mat._32;
		ret._23 = _21*mat._13 + _22*mat._23 + _23*mat._33;
		ret._31 = _31*mat._11 + _32*mat._21 + _33*mat._31;
		ret._32 = _31*mat._12 + _32*mat._22 + _33*mat._32;
		ret._33 = _31*mat._13 + _32*mat._23 + _33*mat._33;
		return ret;
	}
//	union {
// 		struct {
// 			float        _11, _12, _13,_14;
// 			float        _21, _22, _23,_24;
// 			float        _31, _32, _33,_34;
// 			float        _41, _42, _43, _44;
// 		};
// 		float m[4][4];
// 	};
// 	MATRIX operator *(const MATRIX&mat) const
// 	{
// 		MATRIX ret;
// 		ret._11 = _11*mat._11 + _12*mat._21 + _13*mat._31 + _14*mat._41;
// 		ret._12 = _11*mat._12 + _12*mat._22 + _13*mat._32 + _14*mat._42;
// 		ret._13 = _11*mat._13 + _12*mat._23 + _13*mat._33 + _14*mat._43;
// 		ret._13 = _11*mat._14 + _12*mat._24 + _13*mat._34 + _14*mat._44;
// 		ret._21 = _21*mat._11 + _22*mat._21 + _23*mat._31 + _24*mat._41;
// 		ret._22 = _21*mat._12 + _22*mat._22 + _23*mat._32 + _24*mat._42;
// 		ret._23 = _21*mat._13 + _22*mat._23 + _23*mat._33 + _24*mat._43;
// 		ret._24 = _21*mat._14 + _22*mat._24 + _23*mat._34 + _24*mat._44;
// 		ret._31 = _31*mat._11 + _32*mat._21 + _33*mat._31 + _34*mat._41;
// 		ret._32 = _31*mat._12 + _32*mat._22 + _33*mat._32 + _34*mat._42;
// 		ret._33 = _31*mat._13 + _32*mat._23 + _33*mat._33 + _34*mat._43;
// 		ret._34 = _31*mat._14 + _32*mat._24 + _33*mat._34 + _34*mat._44;
// 		ret._41 = _41*mat._11 + _42*mat._21 + _43*mat._31 + _44*mat._41;
// 		ret._42 = _41*mat._12 + _42*mat._22 + _43*mat._32 + _44*mat._42;
// 		ret._43 = _41*mat._13 + _42*mat._23 + _43*mat._33 + _44*mat._43;
// 		ret._44 = _41*mat._14 + _42*mat._24 + _43*mat._34 + _44*mat._44;
// 
// 		return ret;
// 	}
	MATRIX&operator*=(MATRIX&mat)
	{
		*this = *this*mat;
		return *this;
	}
	DXPOINT operator*(DXPOINT&pt) const
	{
		DXPOINT ret;
		ret.x = _11*pt.x + _12*pt.y + _13;
		ret.y = _21*pt.x + _22*pt.y + _23;
		return ret;
	}
	void Identity()
	{
		memset(this, 0, sizeof(MATRIX));
		_11 = _22 = _33 = 1;
	}

	void SetScale(float x, float y)
	{
		Identity();
		_11 = x;
		_22 = y;
	}
	void SetRotate(float angle)
	{
		Identity();
		float cosa = cosf(angle);
		float sina = sinf(angle);
		_11 = cosa;
		_12 = -sina;
		_21 = sina;
		_22 = cosa;
	}
	void SetTranslate(float x, float y)
	{
		Identity();
		_13 = x;
		_23 = y;
	}

	MATRIX()
	{
		Identity();
	}
};






//OPENGL离屏渲染
class OpenglFBO
{
public:
	static std::list<OpenglFBO*>s_fboList;


	int m_width=1024;
	int m_height=1024;

	GLuint m_FrameBuffer=0;
	GLuint m_FboTexture=0;
	GLuint m_depthbuffer=0;

// 	struct VIEWPORTPARAM
// 	{
// 		int x=0;
// 		int y=0;
// 		int width=0;
// 		int height=0;
// 		void SetViewPort()
// 		{
// 			glViewport(x, y, width, height);
// 		}
// 	};
// 	VIEWPORTPARAM m_ViewportParam;//切换时需要保存viewport,因此由JS调过来的glViewport一律调用本函数
// 	void glViewportFBO(GLint x, GLint y, GLsizei width, GLsizei height)
// 	{
// 		m_ViewportParam.x = x;
// 		m_ViewportParam.y = y;
// 		m_ViewportParam.width = width;
// 		m_ViewportParam.height = height;
// 		glViewport(x, y, width, height);
// 	}

	OpenglFBO();
	~OpenglFBO();
	bool IsInited();
	bool Init(int width,int height);
	bool Uninit();
	bool BeginFBO();
	bool EndFBO();
	bool DrawFBO(int x,int y);
	void GetImageData(int x, int y, int width, int height, uint8_t* pData);//pData的大小为width*height*4
};

