#include "stdafx.h"
#include "HtmlStyle.h"
#include "OpenglContext.h"
#include "HtmlElement.h"
IMPLEMENT_CUSTOM_CLASS(HtmlStyle, CustomClassBase)

HtmlStyle::HtmlStyle()
{
	m_width.typ = CT_NOVALUE;
	m_width.value = 0;
	m_height.typ = CT_NOVALUE;
	m_height.value = 0;
}


HtmlStyle::~HtmlStyle()
{
}

bool HtmlStyle::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HtmlStyle", HandleObject());
}

bool HtmlStyle::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = CustomClassBase::OnConstructor(args);
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject objthis(args.getContext(), args.getReturnValue().GetObject());

	objthis.SetProperty(args.getContext(), "className", LocalValue(args.getContext()).SetString(args.getContext(), ""));


// 	objthis.SetProperty(args.getContext(), "x", ValueBase().SetInt(0));
// 	objthis.SetProperty(args.getContext(), "y", ValueBase().SetInt(0));
// 	objthis.SetProperty(args.getContext(), "width", ValueBase().SetInt(0));
// 	objthis.SetProperty(args.getContext(), "height", ValueBase().SetInt(0));
// 	objthis.SetProperty(args.getContext(), "clientLeft", ValueBase().SetInt(0));
// 	objthis.SetProperty(args.getContext(), "clientTop", ValueBase().SetInt(0));
// 	objthis.SetProperty(args.getContext(), "clientHeight", ValueBase().SetInt(0));
// 	objthis.SetProperty(args.getContext(), "clientWidth", ValueBase().SetInt(0));

	return ret;
}

bool HtmlStyle::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "left"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_left.strvalue.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "top"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_top.strvalue.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "width"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_width.strvalue.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "height"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_height.strvalue.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "fontFamily"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_fontFamily.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "fontSize"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_fontSize.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "backgroundImage"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_backgroundImage.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "transform"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "transformOrigin"))
	{
		assert(0);
	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlStyle::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "left"))
	{
		m_left.SetValue(args.GetValue().GetString(args.GetContext()));
		m_pParent->OnSetStyle(args.GetContext(),name);
		return true;
	}
	else if (!PROPCMP(name, "top"))
	{
		m_top.SetValue(args.GetValue().GetString(args.GetContext()));
		m_pParent->OnSetStyle(args.GetContext(), name);
		return true;
	}
	else if (!PROPCMP(name, "width"))
	{
		m_width.SetValue(args.GetValue().GetString(args.GetContext()));
		m_pParent->OnSetStyle(args.GetContext(), name);
		return true;
	}
	else if (!PROPCMP(name, "height"))
	{
		m_height.SetValue(args.GetValue().GetString(args.GetContext()));
		m_pParent->OnSetStyle(args.GetContext(), name);
		return true;
	}
	else if (!PROPCMP(name, "fontFamily"))
	{
		m_fontFamily = args.GetValue().GetString(args.GetContext());
		m_pParent->OnSetStyle(args.GetContext(), "font");
		return true;
	}
	else if (!PROPCMP(name, "fontSize"))
	{
		m_fontSize = args.GetValue().GetString(args.GetContext());
		m_pParent->OnSetStyle(args.GetContext(), "font");
		return true;
	}
	else if (!PROPCMP(name, "backgroundImage"))
	{
		CustomClassAutoThisObj cc(this, args);
		m_backgroundImage = args.GetValue().GetString(args.GetContext());
		LocalObject objele(args.GetContext(), GetParentElement(args.GetContext()));
		if (objele.IsNull())return true;
		HtmlElement*pelement = dynamic_cast<HtmlElement*>(FindCustomClass(objele));
		if (!pelement)return true;
		CustomClassAutoThisObj cc2(pelement, objele);
		if (!_strnicmp(m_backgroundImage.c_str(), "url(", 4))
		{
			int i = m_backgroundImage.rfind(")");
			std::string str = m_backgroundImage.substr(4, i - 4);
			pelement->SetBackgroundImage(str);
		}
		else
		{
			assert(0);
			pelement->SetBackgroundImage(m_backgroundImage);
		}
		return true;
	}
	else if (!PROPCMP(name, "cssText"))
	{
		std::string&&val = args.GetValue().GetString(args.GetContext());
		SetStyle(args.GetContext(), val.c_str(), m_pParent);
		
	}
	else if (!PROPCMP(name, "transform"))
	{
		std::string&&val = args.GetValue().GetString(args.GetContext());
		m_Transform.ParseTransform(val);
		m_pParent->OnSetStyle(args.GetContext(), name);
		return true;
	}
	else if (!PROPCMP(name, "transformOrigin"))
	{
		std::string&&val = args.GetValue().GetString(args.GetContext());
		m_Transform.ParseTransformOrigin(val);
		m_pParent->OnSetStyle(args.GetContext(), name);
		return true;
	}
	return CustomClassBase::OnSetProperty(args);
}

FontInfo HtmlStyle::GetFontInfo(const HandleContext&context)
{
//	LocalObject objthis(context, GetJsObject());
	FontInfo finfo;
	finfo.szFontName = m_fontFamily;// objthis.GetProperty("fontFamily").GetString(context);
//	std::string &&szSize = m_fontSize;// objthis.GetProperty("fontSize").GetString(context);
	finfo.nFontSize = atoi(m_fontSize.c_str());
	return finfo;
}

bool HtmlStyle::SetStyle(const HandleContext&context, const char*szName, std::vector<std::string>&values, HtmlElement*pElement)
{
//	LocalObject objelement(context, pElement->GetJsObject());
	if (!_stricmp(szName, "width"))
	{
		m_width.SetValue(values[0]);
		pElement->OnSetStyle(context, "width");
// 		if (m_width.typ == CT_PX)
// 		{
// 			objelement.SetProperty("width", ValueBase().SetInt(m_width.value));
// 		}
// 		else if(m_width.typ==CT_PERCENT)
// 		{
// 			
// 			objelement.SetProperty("width", ValueBase().SetInt(pElement->GetParentWidth(context)*m_width.value*0.01));
// 		}
	}
	else if (!_stricmp(szName, "height"))
	{
		m_height.SetValue(values[0]);
		pElement->OnSetStyle(context, "height");
// 		if (m_height.typ == CT_PX)
// 		{
// 			objelement.SetProperty("height", ValueBase().SetInt(m_height.value));
// 		}
// 		else if(m_height.typ==CT_PERCENT)
// 		{
// 			objelement.SetProperty("height", ValueBase().SetInt(pElement->GetParentHeight(context)*m_height.value*0.01));
// 		}
	}
	else if (!_stricmp(szName, "font"))
	{
		bool bItalic = false;
		bool bBold = false;
		/*
		fontStyle
		fontVariant
		fontWeight
		fontSize/lineHeight
		fontFamily
*/
		for (int i = 0; i < values.size(); i++)
		{
			std::string &val=values[i];
			//目前暂时带px的为字体大小，最后一个单词为字体名称，如何解析还待研究
			if (val.find("px")!=val.npos)
			{
				this->m_fontSize = val;
				break;
			}
			
		}
		m_fontFamily = values[values.size() - 1];
		pElement->OnSetStyle(context,"font");
	}
	else if (!_stricmp(szName, "transform"))
	{
		int dd = 3;
	}

	return true;
}

char*TrimLeft(char*p)
{
	while (*p && (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n'))p++;
	return p;
}

bool HtmlStyle::SetStyle(const HandleContext&context, const char*szText, HtmlElement*pElement)
{
	ResetStyle(context);
	m_styleText = szText;
	char*ptext = _strdup(szText);

	std::vector<std::string>values;
	char*ctxline = 0;
	char*p = strtok_s(ptext, ";", &ctxline);
	while (p)
	{
		char*ctxitem = 0;
		char*pName = strtok_s(p, ":", &ctxitem);
		
		assert(pName);
		pName = TrimLeft(pName);
		values.clear();
		p = strtok_s(nullptr, " \t", &ctxitem);
		while (p)
		{
			values.push_back(p);
			p = strtok_s(nullptr, " \t", &ctxitem);
		}
		
		if (!SetStyle(context, pName, values, pElement))break;


		p = strtok_s(nullptr, ";", &ctxline);
	}






	free(ptext);
	return true;
}

void HtmlStyle::SetParent(const HandleContext&context, HtmlElement*pElement)
{
	m_pParent = pElement;
	pElement->m_pStyle = this;
 	LocalObject objthis(context, GetJsObject());
 	LocalObject objele(context,pElement->GetJsObject());
 	objthis.SetProperty("__parent", objele);
 	objele.SetProperty("style", GetJsObject());
}

void HtmlStyle::ResetStyle(const HandleContext& context)
{
	m_left.value = m_top.value = 0;
	m_left.typ = m_top.typ = CT_PX;
	m_width.typ = CT_NOVALUE;
	m_width.value = 0;
	m_height.typ = CT_NOVALUE;
	m_height.value = 0;
}

HandleObject HtmlStyle::GetParentElement(const HandleContext&context)
{
	return GetJsObject().GetProperty(context, "__parent").GetObject();
}

