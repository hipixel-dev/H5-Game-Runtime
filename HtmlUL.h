#pragma once
#include "HtmlEventTarget.h"
class HtmlUL :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlUL, HtmlEventTarget)
public:
	HtmlUL();
	virtual ~HtmlUL();
	static bool InitClass(const HandleContext&context);

};

