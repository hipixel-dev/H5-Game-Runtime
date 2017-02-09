#include "stdafx.h"
#include "HtmlLI.h"
IMPLEMENT_CUSTOM_CLASS(HtmlLI, HtmlEventTarget)

HtmlLI::HtmlLI()
{
}


HtmlLI::~HtmlLI()
{
}

bool HtmlLI::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLLIElement", HtmlEventTarget::GetPrototype_s());
}
