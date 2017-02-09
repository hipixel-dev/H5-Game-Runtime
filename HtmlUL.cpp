#include "stdafx.h"
#include "HtmlUL.h"

IMPLEMENT_CUSTOM_CLASS(HtmlUL, HtmlEventTarget)
HtmlUL::HtmlUL()
{
}


HtmlUL::~HtmlUL()
{
}

bool HtmlUL::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLULElement", HtmlEventTarget::GetPrototype_s());
}
