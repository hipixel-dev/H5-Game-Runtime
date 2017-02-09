#pragma once
#include "HtmlCanvasGradient.h"
#include "OpenglFBO.h"

class HtmlLinearGradient :
	public HtmlCanvasGradient
{
	DEFINE_CUSTOM_CLASS(HtmlLinearGradient, HtmlCanvasGradient)


	GLuint m_tex = 0;

	int m_x0;
	int m_y0;
	int m_x1;
	int m_y1;
	struct COLORSTOP
	{
		float stop;
		uint32_t color;
		bool operator ==(const COLORSTOP&src)
		{
			return stop == src.stop&&color == src.color;
		}
		bool operator!=(const COLORSTOP&src)
		{
			return !operator==(src);
		}
	};
	std::vector<COLORSTOP>m_stopList;

	ImageData imgdata;


	static std::unordered_map<HtmlLinearGradient*, HtmlLinearGradient*>s_gradientCache;//cocos2d每次都会createLinearGradient，因此使用缓存




public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("addColorStop", &HtmlLinearGradient::addColorStop,2)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY
	HtmlLinearGradient();
	virtual ~HtmlLinearGradient();
	static bool InitClass(const HandleContext&context);
	
	bool addColorStop(IFunctionCallbackArgs&args);

	GLuint GetTexture();
	bool equal(HtmlLinearGradient* src);
};

