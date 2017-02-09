#pragma once
#include "JSVMInterface.h"
class HtmlElement;

enum COORDTYPE//坐标类型
{
	CT_NOVALUE,//未在style中设置
	CT_PX,//px
	CT_PERCENT,//%
};
struct HTMLCOORD
{
	std::string strvalue;
	double value = 0;
	COORDTYPE typ = CT_PX;
	bool SetValue(const std::string& szValue)
	{
		strvalue = szValue;
		char*p = 0;
		value = strtod(szValue.c_str(), &p);
		if (!_stricmp(p, "px"))
		{
			typ = CT_PX;
		}
		else if (!_stricmp(p, "%"))
		{
			typ = CT_PERCENT;
		}
		else
		{
			typ = CT_PX;
		}
		return true;
	}

};
void Trim(char*&str);
class HtmlStyle :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlStyle, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION


		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("left")
		DEFINE_CUSTOM_PROPERTY("top")
		DEFINE_CUSTOM_PROPERTY("width")
		DEFINE_CUSTOM_PROPERTY("height")
		DEFINE_CUSTOM_PROPERTY("fontFamily")
		DEFINE_CUSTOM_PROPERTY("fontSize")
		DEFINE_CUSTOM_PROPERTY("backgroundImage")
		DEFINE_CUSTOM_PROPERTY("cssText")
		DEFINE_CUSTOM_PROPERTY("transform")
		DEFINE_CUSTOM_PROPERTY("transformOrigin")
		END_CUSTOM_PROPERTY



	

	HTMLCOORD m_left;
	HTMLCOORD m_top;
	HTMLCOORD m_width;
	HTMLCOORD m_height;
	HtmlElement* m_pParent = nullptr;

	struct _TRANSFORM
	{
		int xTran=0;
		int yTran=0;
		float fRot=0;
		float xScale=1;
		float yScale=1;
		int xOrig = 0;
		int yOrig = 0;
		void initial()
		{
			xTran = yTran = xOrig=yOrig=0;
			fRot = 0;
			xScale = yScale = 1;
		}
		void ParseTransform(const std::string&str)
		{
			if (str == "initial")
			{
				initial();
				return;
			}
			std::vector<std::string>paralist;
			char*pbuf = strdup(str.c_str());
			char*p = pbuf;
			while (*p)
			{
				char*q = p;
				while (*q)
				{
					if (*q != '(')q++;
					else break;
				}
				std::string szFunName;
				szFunName.resize(q - p);
				memcpy((char*)szFunName.data(), p, q - p);
				szFunName = trim(szFunName);
				p = q + 1;
				q = p;
				paralist.clear();
				while (*q)
				{
					if (*q == ',' || *q == ')')
					{
						char ch = *q;
						*q = 0;
						Trim(p);
						paralist.push_back(p);
						q++;
						p = q;
						if (ch == ')')
						{
							break;
						}
					}
					q++;
				}
				p = q;
				if (szFunName == "translate")
				{
					if (paralist.size() == 2)
					{
						xTran = strtol(paralist[0].c_str(), 0, 10);
						yTran = strtol(paralist[1].c_str(), 0, 10);
					}
				}
				else if (szFunName == "rotate")
				{
					if (paralist.size() == 1)
					{
						char*d;
						fRot = strtod(paralist[0].c_str(),&d);
						if (!strcmp(d, "deg"))
						{
							fRot = fRot * PI / 180.0;
						}
					}
				}
				else if (szFunName == "scale")
				{
					if (paralist.size() == 2)
					{
						xScale = strtod(paralist[0].c_str(), 0);
						yScale = strtod(paralist[1].c_str(), 0);
					}
				}
			}


			free(pbuf);
		}
		void ParseTransformOrigin(const std::string&str)
		{

		}
	};
	_TRANSFORM m_Transform;


	std::string m_fontFamily;
	std::string m_fontSize;
	std::string m_backgroundImage;
	std::string m_styleText;
	HtmlStyle();
	virtual ~HtmlStyle();

	static bool InitClass(const HandleContext&context);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	FontInfo GetFontInfo(const HandleContext&context);

	bool SetStyle(const HandleContext&context, const char*szText, HtmlElement*pElement);
	bool SetStyle(const HandleContext&context, const char*szName, std::vector<std::string>&values, HtmlElement*pElement);
	void SetParent(const HandleContext&context, HtmlElement*pElement);
	HandleObject GetParentElement(const HandleContext&context);
	void ResetStyle(const HandleContext& context);
};

