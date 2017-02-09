#pragma once
#include "HtmlElement.h"
class HtmlStyleElement :
	public HtmlElement
{
	DEFINE_CUSTOM_CLASS(HtmlStyleElement, HtmlElement)
public:
	HtmlStyleElement();
	virtual ~HtmlStyleElement();
	static bool InitClass(const HandleContext&context);
};

