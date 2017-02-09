#include "stdafx.h"
#include "HtmlTextarea.h"
IMPLEMENT_CUSTOM_CLASS(HtmlTextarea, HtmlEventTarget)

HtmlTextarea::HtmlTextarea()
{
}


HtmlTextarea::~HtmlTextarea()
{
}

bool HtmlTextarea::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLTextArea", HtmlEventTarget::GetPrototype_s());
}
