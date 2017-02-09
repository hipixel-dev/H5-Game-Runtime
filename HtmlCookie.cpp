#include "stdafx.h"
#include "HtmlCookie.h"
IMPLEMENT_CUSTOM_CLASS(HtmlCookie, CustomClassBase)

HtmlCookie::HtmlCookie()
{
}


HtmlCookie::~HtmlCookie()
{
}

bool HtmlCookie::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HTMLCookie", HandleObject());
}
