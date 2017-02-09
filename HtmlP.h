#pragma once
#include "HtmlEventTarget.h"
class HtmlP :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlP, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY

	

	HtmlP();
	virtual ~HtmlP();

	static bool InitClass(const HandleContext&context);
};

