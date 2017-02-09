#include "stdafx.h"
#include "HtmlLinearGradient.h"
#include "HTML5Color.h"
#include "OpenglContext.h"
IMPLEMENT_CUSTOM_CLASS(HtmlLinearGradient, HtmlCanvasGradient)

HtmlLinearGradient::HtmlLinearGradient()
{
}


HtmlLinearGradient::~HtmlLinearGradient()
{
	if (m_tex)glDeleteTextures(1, &m_tex);
	s_gradientCache.erase(this);
}

bool HtmlLinearGradient::InitClass(const HandleContext&context)
{
	if (HtmlCanvasGradient::GetPrototype_s().IsNull())
	{
		HtmlCanvasGradient::InitClass(context);
	}
	return InitClass_s(context, "HTMLLinearGradient", HtmlCanvasGradient::GetPrototype_s());
}

bool HtmlLinearGradient::addColorStop(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "addColorStop");
	COLORSTOP cs;
	cs.stop = args[0].GetNumber();
	cs.color = CHTML5Color::GetColorValue(args[1].GetString(args.getContext()));
	m_stopList.push_back(cs);
	if (m_tex)
	{
		glDeleteTextures(1, &m_tex);
		m_tex = 0;
	}
	return true;
}

struct _RGBA
{
	_RGBA()
	{

	}
	_RGBA(uint32_t col)
	{
		nColor = col;
	}
	union{
		uint32_t nColor;
		struct
		{
			uint8_t r;
			uint8_t g;
			uint8_t b;
			uint8_t a;
		}rgba;
	};
	_RGBA&operator=(uint32_t col)
	{
		nColor = col;
		return*this;
	}
};
GLuint HtmlLinearGradient::GetTexture()
{
	
	if (m_tex)return m_tex;
	else
	{
		for (std::unordered_map<HtmlLinearGradient*, HtmlLinearGradient*>::iterator it = s_gradientCache.begin(); it != s_gradientCache.end(); ++it)
		{
			HtmlLinearGradient*pg = it->second;
			if (pg->equal(this))
			{
				return pg->m_tex;
			}
		}


		DXPOINT A(m_x0, m_y0);
		DXPOINT B(m_x1, m_y1);
		DXPOINT vAB = B - A;

		imgdata.Cleanup();
		
		imgdata.nWidth = vAB.GetLength();
		imgdata.nHeight = 1;
		imgdata.pData = new uint32_t[imgdata.nWidth];
		//m_stopList先排序



		imgdata.pData[0] = m_stopList[0].color;
		COLORSTOP prevcs = m_stopList[0];
		int prevx = 0;
		for (int i = 0; i < m_stopList.size();i++)
		{
			int curx = (m_stopList[i].stop*(imgdata.nWidth-1) + 0.5f);//stop点的坐标
			_RGBA curcolor = m_stopList[i].color;
			//跟上一个stop点之间做插值
			_RGBA precolor = prevcs.color;
			
			for (int x = prevx + 1; x < curx; x++)
			{
				_RGBA col;
				float percent;
				if (curx == prevx)percent = 0.5f;
				else percent= float(x - prevx) / (curx - prevx);
				col.rgba.r = min(precolor.rgba.r*(1 - percent) + curcolor.rgba.r*percent, 255);
				col.rgba.g = min(precolor.rgba.g*(1 - percent) + curcolor.rgba.g*percent, 255);
				col.rgba.b = min(precolor.rgba.b*(1 - percent) + curcolor.rgba.b*percent, 255);
				col.rgba.a = min(precolor.rgba.a*(1 - percent) + curcolor.rgba.a*percent, 255);
				imgdata.pData[x] = col.nColor;
			}
			imgdata.pData[curx] = curcolor.nColor;
			prevcs = m_stopList[i];
			prevx = curx;
		}
		m_tex=OpenglContext::CreateGlTexID(imgdata);
		s_gradientCache[this] = this;
	}
	return m_tex;
}

bool HtmlLinearGradient::equal(HtmlLinearGradient* src)
{
	bool ret = m_x0 == src->m_x0&&m_x1 == src->m_x1&&m_y0 == src->m_y0&&m_y1 == src->m_y1;
	if (!ret)return false;
	if (m_stopList.size() != src->m_stopList.size())return false;
	for (int i = 0; i < m_stopList.size(); i++)
	{
		if (m_stopList[i] != src->m_stopList[i])return false;
	}
	return true;
}

std::unordered_map<HtmlLinearGradient*, HtmlLinearGradient*> HtmlLinearGradient::s_gradientCache;
