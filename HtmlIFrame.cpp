#include "stdafx.h"
#include "HtmlIFrame.h"
//http://www.w3school.com.cn/tags/tag_iframe.asp

IMPLEMENT_CUSTOM_CLASS(HtmlIFrame, HtmlEventTarget)
HtmlIFrame::HtmlIFrame()
{
}


HtmlIFrame::~HtmlIFrame()
{
}

bool HtmlIFrame::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLIFrameElement", HtmlEventTarget::GetPrototype_s());
}
