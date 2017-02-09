#include "stdafx.h"
#include "HtmlLink.h"
IMPLEMENT_CUSTOM_CLASS(HtmlLink, HtmlEventTarget)

HtmlLink::HtmlLink()
{
}


HtmlLink::~HtmlLink()
{
}

bool HtmlLink::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLLink", HtmlEventTarget::GetPrototype_s());
}
