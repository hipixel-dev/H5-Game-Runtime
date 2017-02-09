#include "stdafx.h"
#include "HtmlStyleElement.h"

IMPLEMENT_CUSTOM_CLASS(HtmlStyleElement, HtmlElement)
HtmlStyleElement::HtmlStyleElement()
{
}


HtmlStyleElement::~HtmlStyleElement()
{
}

bool HtmlStyleElement::InitClass(const HandleContext&context)
{
	if (HtmlElement::GetPrototype_s().IsNull())
	{
		HtmlElement::InitClass(context);
	}
	return InitClass_s(context, "HTMLStyleElement", HtmlElement::GetPrototype_s());
}
