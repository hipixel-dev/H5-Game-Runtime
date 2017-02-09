#include "stdafx.h"
#include "HtmlScreen.h"
#include "OpenglContext.h"

IMPLEMENT_CUSTOM_CLASS(HtmlScreen, CustomClassBase)
HtmlScreen::HtmlScreen()
{
}


HtmlScreen::~HtmlScreen()
{
}

bool HtmlScreen::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HTMLScreen", HandleObject());
}

bool HtmlScreen::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "availTop"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(name, "availLeft"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(name, "availHeight"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetHeight()));
		return true;
	}
	else if (!PROPCMP(name, "availWidth"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetWidth()));
		return true;
	}
	else if (!PROPCMP(name, "colorDepth"))
	{
		args.SetRetValue(ValueBase().SetInt(32));
		return true;
	}
	else if (!PROPCMP(name, "left"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(name, "top"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(name, "pixelDepth"))
	{
		args.SetRetValue(ValueBase().SetInt(32));
		return true;
	}
	else if (!PROPCMP(name, "width"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetWidth()));
		return true;
	}
	else if (!PROPCMP(name, "height"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetHeight()));
		return true;
	
	}
	return CustomClassBase::OnGetProperty(args);
}
