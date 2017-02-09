#pragma once
#include "HtmlEventTarget.h"

class HtmlIFrame :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlIFrame, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION


		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY


		END_CUSTOM_PROPERTY
	HtmlIFrame();
	virtual ~HtmlIFrame();

	static bool InitClass(const HandleContext&context);
};

