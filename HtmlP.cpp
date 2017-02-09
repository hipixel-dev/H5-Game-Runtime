#include "stdafx.h"
#include "HtmlP.h"

IMPLEMENT_CUSTOM_CLASS(HtmlP, HtmlEventTarget)
HtmlP::HtmlP()
{
}


HtmlP::~HtmlP()
{
}

bool HtmlP::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLP", HtmlEventTarget::GetPrototype_s());
}
