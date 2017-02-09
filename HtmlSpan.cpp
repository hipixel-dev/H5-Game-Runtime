#include "stdafx.h"
#include "HtmlSpan.h"
IMPLEMENT_CUSTOM_CLASS(HtmlSpan, HtmlEventTarget)

HtmlSpan::HtmlSpan()
{
}


HtmlSpan::~HtmlSpan()
{
}

bool HtmlSpan::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLSpan", HtmlEventTarget::GetPrototype_s());
}
