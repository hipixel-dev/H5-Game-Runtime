#pragma once

#include "HtmlEventTarget.h"
class HtmlDiv :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlDiv, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION
	
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
//		DEFINE_CUSTOM_PROPERTY("innerHTML")
		
		END_CUSTOM_PROPERTY


	HtmlDiv();
	virtual ~HtmlDiv();

	static bool InitClass(const HandleContext&context);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
};

