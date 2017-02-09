#pragma once
#include "HtmlEventTarget.h"
class HtmlLink :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlLink, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY
	HtmlLink();
	virtual ~HtmlLink();

	static bool InitClass(const HandleContext&context);
};

