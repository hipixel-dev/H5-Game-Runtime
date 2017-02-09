#pragma once
#include "HtmlEventTarget.h"
class HtmlSpan :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlSpan, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY

	HtmlSpan();
	virtual ~HtmlSpan();
	static bool InitClass(const HandleContext&context);
};

