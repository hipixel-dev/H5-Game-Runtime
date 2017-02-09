#include "stdafx.h"
#include "HtmlRect.h"

IMPLEMENT_CUSTOM_CLASS(HtmlRect, CustomClassBase)
HtmlRect::HtmlRect()
{
}


HtmlRect::~HtmlRect()
{
}

bool HtmlRect::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HTMLRect", HandleObject());
}

bool HtmlRect::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "left"))
	{
		args.SetRetValue(ValueBase().SetInt(m_left));
		return true;
	}
	else if (!PROPCMP(name, "top"))
	{
		args.SetRetValue(ValueBase().SetInt(m_top));
		return true;
	}
	else if (!PROPCMP(name, "width"))
	{
		args.SetRetValue(ValueBase().SetInt(m_width));
		return true;
	}
	else if (!PROPCMP(name, "height"))
	{
		args.SetRetValue(ValueBase().SetInt(m_height));
		return true;
	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlRect::OnSetProperty(SetPropertyArgs&args)
{
	return CustomClassBase::OnSetProperty(args);
}
