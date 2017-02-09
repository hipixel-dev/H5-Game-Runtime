#pragma once
#include "HtmlEventTarget.h"

#include "OpenglFBO.h"

#include "OpenglContext.h"


class CTexture
{
public:
	GLuint m_nTexID = 0;
	int*m_pRefCount = 0;
	int m_nWidth = 0;
	int m_nHeight = 0;
	explicit CTexture()
	{
		
	}
	explicit CTexture(GLuint nTexID)
	{
		assert(nTexID);
		m_nTexID = nTexID;
		m_pRefCount = new int;
		*m_pRefCount = 1;
	}
	explicit CTexture(const CTexture&src)
	{
		*this = src;

	}
	bool IsNull() const
	{
		return m_pRefCount == nullptr;
	}
	void Release()
	{
		if (!m_pRefCount)return;
		(*m_pRefCount)--;
		if (*m_pRefCount == 0)
		{
			delete m_pRefCount;
			if (m_nTexID)
				glDeleteTextures(1, &m_nTexID);
			
		}
		m_pRefCount = 0;
		m_nTexID = 0;
	}
	~CTexture()
	{
		Release();
	}
	CTexture&operator=(const CTexture&src)
	{
		Release();
		if (src.IsNull())return *this;
		m_nTexID = src.m_nTexID;
		m_pRefCount = src.m_pRefCount;
		(*m_pRefCount)++;
		m_nWidth = src.m_nWidth;
		m_nHeight = src.m_nHeight;
		return *this;
	}
	operator GLuint()
	{
		return m_nTexID;
	}
};




class RoundList
{
public:
	struct _ROUNDLIST//使用循环链表
	{
		DXPOINT pt;
		int index;
		float ang = 0;//上次计算出来与相邻两个点组成的夹角(1-cosa)
		bool bNeedRecalc = true;//需要重新计算ang

		_ROUNDLIST*prev;
		_ROUNDLIST*next;
	};
	int m_nCount = 0;
	_ROUNDLIST*pHead = nullptr;
	_ROUNDLIST*Append(const DXPOINT&pt, int index)//添加到尾部（即pHead的上一节点）
	{
		_ROUNDLIST*pitem = new _ROUNDLIST;
		pitem->pt = pt;
		pitem->index = index;
		if (!pHead)
		{
			pHead = pitem;
			pHead->prev = pHead;
			pHead->next = pHead;
		}
		else
		{
			pHead->prev->next = pitem;
			pitem->prev = pHead->prev;
			pitem->next = pHead;
			pHead->prev = pitem;
		}
		m_nCount++;
		return pitem;
	}
	~RoundList()
	{
		Clear();
	}
	void Clear()
	{
		if (!pHead)return;
		_ROUNDLIST*p = pHead;
		while (true)
		{
			_ROUNDLIST*q = p->next;
			delete p;
			p = q;
			if (p == pHead)
			{
				break;
			}
		}
		pHead = nullptr;
		m_nCount = 0;
	}
	void Remove(_ROUNDLIST*pItem)
	{
		if (m_nCount > 1)
		{
			pItem->prev->next = pItem->next;
			pItem->next->prev = pItem->prev;
			if (pHead == pItem)
			{
				pHead = pItem->next;
			}
			delete pItem;
			m_nCount--;
		}
		else
		{
			delete pHead;
			pHead = nullptr;
			m_nCount = 0;
		}
	}
};




class CANVASPATH
{
public:
	struct _INDEX
	{
		GLushort a;
		GLushort b;
		GLushort c;
	};
	std::vector<DXPOINT> m_pointPath;
	std::vector<_INDEX>m_indexList;

	void AddPoint(const DXPOINT&pt)
	{
		m_pointPath.push_back(pt);
	}

	bool CreatePathMesh()//根据m_pathList生成m_indexList
	{
		m_indexList.clear();
		//将path生成网格
		/*推进波前法：1、按顺序查找锐角，连接锐角两边的顶点（添加索引），从查找列表中去掉顶点，直到找不到锐角
		2、查找角度最小的钝角，连接两边顶点，重复1
		3、查找列表.size()<3结束

		*/
		if (m_pointPath.size() < 3)return false;

		RoundList poslist;
		int n = m_pointPath.size();
		if (n > 2 && m_pointPath[n - 1] == m_pointPath[0])//最后一个点跟第一个点重合，去掉
			n--;

		for (int i = 0; i <n; i++)
		{
			DXPOINT&pt = m_pointPath[i];
			poslist.Append(pt, i);
		}

		//判断顺时针还是逆时针:所有向量旋转角度加起来<0顺时针，否则逆时针
		bool bClockwise;
		float angles = 0;
		RoundList::_ROUNDLIST*pitem = poslist.pHead;
		for (int i = 0; i < poslist.m_nCount; i++)
		{
			DXPOINT v1 = pitem->prev->pt - pitem->pt;
			DXPOINT v2 = pitem->next->pt - pitem->pt;
			assert(v1 != v2);
			float d = v1*v2;
			float len = v1.GetLength()*v2.GetLength();
			float ang = acosf(d / len);
			DXPOINT3 vcross = v1.cross(v2);
			if (vcross.z < 0)ang = -ang;
			angles += ang;
			pitem = pitem->next;
		}
		bClockwise = angles < 0;

		pitem = poslist.pHead;

		n = 0;
		while (poslist.m_nCount>3)
		{
			DXPOINT v1 = pitem->prev->pt - pitem->pt;
			DXPOINT v2 = pitem->next->pt - pitem->pt;
			float ang;
			if (pitem->bNeedRecalc)
			{
				float d = v1*v2;
				float len = v1.GetLength()*v2.GetLength();
				ang = 1 - (d / len);//相当于1-cosa，0－180度时范围0-2，180－360度时范围2－4
				DXPOINT3 vcross = v1.cross(v2);
				if ((vcross.z > 0 && bClockwise) || (vcross.z<0 && !bClockwise))ang = 4 - ang;
				pitem->ang = ang;
				pitem->bNeedRecalc = false;
			}
			else
			{
				ang = pitem->ang;
			}
			if (ang<1)
			{
				//锐角，连接v1v2组成三角形
				_INDEX idx;
				idx.a = pitem->index;
				if (bClockwise)
				{//索引顺序为逆时针
					idx.b = pitem->prev->index;
					idx.c = pitem->next->index;
				}
				else
				{
					idx.c = pitem->prev->index;
					idx.b = pitem->next->index;
				}
				m_indexList.push_back(idx);
				pitem->prev->bNeedRecalc = true;
				pitem->next->bNeedRecalc = true;
				//将本点从列表中移除
				RoundList::_ROUNDLIST* p = pitem->next;
				poslist.Remove(pitem);
				pitem = p;
				n = 0;
			}
			else
			{
				pitem = pitem->next;
				n++;
			}
			if (n >= poslist.m_nCount)//循环一圈没有找到锐角，可随机取相邻两个点连接成三角形
			{
				_INDEX idx;
				idx.a = pitem->index;
				if (bClockwise)
				{
					idx.b = pitem->prev->index;
					idx.c = pitem->next->index;
				}
				else
				{
					idx.c = pitem->prev->index;
					idx.b = pitem->next->index;
				}
				m_indexList.push_back(idx);
				pitem->prev->bNeedRecalc = true;
				pitem->next->bNeedRecalc = true;
				RoundList::_ROUNDLIST* p = pitem->next;
				poslist.Remove(pitem);
				pitem = p;
				n = 0;

			}
		}
		//添加最后3个顶点
		_INDEX idx;
		idx.a = poslist.pHead->index;
		if (bClockwise)
		{
			idx.b = poslist.pHead->prev->index;
			idx.c = poslist.pHead->next->index;
		}
		else
		{
			idx.c = poslist.pHead->prev->index;
			idx.b = poslist.pHead->next->index;
		}
		m_indexList.push_back(idx);
		return true;
	}
};

class CANVASPATHS
{
public:
	std::vector<CANVASPATH*>m_PathList;//每个图形保存一个数组，如调用两次rect()会产生两个图形
	CANVASPATH*NewPath()
	{
		if (m_PathList.size() > 0 && m_PathList[m_PathList.size() - 1]->m_pointPath.size() == 0)//最后路径为空，不用重复创建
			return m_PathList[m_PathList.size() - 1];
		CANVASPATH*plist = new CANVASPATH;
		m_PathList.push_back(plist);
		return plist;
	}
	CANVASPATH*GetCurPath()
	{
		if (m_PathList.size() == 0)return NewPath();
		return m_PathList[m_PathList.size() - 1];
	}
	void Clear()
	{
		for (int i = 0; i < m_PathList.size(); i++)
		{
			delete m_PathList[i];
		}
		m_PathList.clear();
	}
	~CANVASPATHS()
	{
		Clear();
	}
	CANVASPATHS&operator=(const CANVASPATHS&src)
	{
		Clear();
		for (int i = 0; i < src.m_PathList.size(); i++)
		{
			CANVASPATH*p = new CANVASPATH;
			*p = *src.m_PathList[i];
			m_PathList.push_back(p);
		}
		return *this;
	}
	bool IsEmpty()
	{
		if (m_PathList.empty())return true;
		if (m_PathList[0]->m_pointPath.empty())return true;
		return false;
	}
};


class HtmlCanvasRenderingContext2D;

class HtmlCanvas :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlCanvas, HtmlEventTarget)
public:
	static HtmlCanvas*s_prevCanvas;//上次调用的canvas
	OpenglFBO m_fbo;
//	OpenGlState m_glState;
	OpenGl2::GLSTATE m_glState;

	GLuint m_texBackground=0;//style里设置的背景图
	ImageData m_imgBackground;
	
	CANVASPATHS m_pointPath;





	struct CANVASSTATE//save/restore保存的状态
	{
		MATRIX m_matrix;
		//H5的变换不同于OPENGL，变换只对坐标系进行，因此需单独记录变换后的原点坐标、变换后的旋转角度、相对变换后的坐标系的缩放
		DXPOINT m_ptOrigin;//当前原点
		float m_fRotate = 0;//当前旋转角度
		float m_fScaleX = 1;//缩放
		float m_fScaleY = 1;
		float m_globalAlpha = 1;
		std::string m_lineCap = "butt";
		std::string m_lineJoin = "miter";
		float m_lineWidth = 1;
		float m_miterLimit = 10;

		std::string m_globalCompositeOperation = "source-over";

//		CTexture m_clipTexture;
		CANVASPATHS m_clipPath;

		CANVASSTATE()
		{

		}
		void Reset()
		{
			m_matrix.Identity();
			m_ptOrigin.x = m_ptOrigin.y = 0;
			m_fRotate = 0;
			m_fScaleX = m_fScaleY = 1;
//			m_clipTexture.Release();
			m_clipPath.Clear();
		}
		~CANVASSTATE()
		{
			
		}
		CANVASSTATE*Clone()
		{
			CANVASSTATE*ret = new CANVASSTATE;
			*ret = *this;
			return ret;
		}
	};
	CANVASSTATE*m_curState = nullptr;
	std::list<CANVASSTATE*>m_saveState;





	float WindowPointToGlPointX(float x);
	float WindowPointToGlPointY(float y);
	float GlPointToWindowPointX(float x);
	float GlPointToWindowPointY(float y);
	DXPOINT WindowPointToGlPoint(const DXPOINT &vPos);



	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("getContext", &HtmlCanvas::getContext, 1)
		DEFINE_CUSTOM_FUNCTION("getImageData", &HtmlCanvas::getImageData, 1)
		DEFINE_CUSTOM_FUNCTION("getBoundingClientRect", &HtmlCanvas::getBoundingClientRect, 1)


		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("width")
		DEFINE_CUSTOM_PROPERTY("height")
		DEFINE_CUSTOM_PROPERTY("clientWidth")
		DEFINE_CUSTOM_PROPERTY("clientHeight")
		DEFINE_CUSTOM_PROPERTY("offsetLeft")
		DEFINE_CUSTOM_PROPERTY("offsetTop")


		END_CUSTOM_PROPERTY





	HtmlCanvas();
	virtual ~HtmlCanvas();

	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args) override;
	virtual bool OnSetProperty(SetPropertyArgs&args) override;
	virtual void OnFinalize(FinalizeArgs&args) override;
	virtual bool OnConstructor(IFunctionCallbackArgs&args) override;
	virtual void OnAddToParent(const HandleContext&context, HtmlElement*pParent);
	virtual void OnRemoveFromParent(const HandleContext&context, HtmlElement*pParent);

	bool DrawFBO();

	bool getContext(IFunctionCallbackArgs&args);
	bool getImageData(IFunctionCallbackArgs&args);
	bool getBoundingClientRect(IFunctionCallbackArgs&args);
	bool Setid(const HandleContext& context, const char*szID);
//	void SetboundingClientRect(const HandleContext&context, int x, int y, int width, int height);

	void DrawImage(GLuint nTexID, int nTexWidth, int nTexHeight, int sx, int sy, int sWidth, int sHeight, int x, int y, int nWidth, int nHeight, MATRIX*pMat);



	virtual bool SetBackgroundImage(const std::string &backgroundImage);
	void DrawBackGround();

	virtual void OnMeasure(int parentX, int parentY, int parentWidth, int parentHeight);
	void Clip();

};

