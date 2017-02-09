#pragma once
#include "HtmlEventTarget.h"
class HtmlLI :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlLI, HtmlEventTarget)
public:
	HtmlLI();
	virtual ~HtmlLI();
	static bool InitClass(const HandleContext&context);

};

