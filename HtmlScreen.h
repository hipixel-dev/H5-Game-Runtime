#pragma once
#include "JSVMInterface.h"
class HtmlScreen :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlScreen, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("availTop")
		DEFINE_CUSTOM_PROPERTY("availLeft")
		DEFINE_CUSTOM_PROPERTY("availHeight")
		DEFINE_CUSTOM_PROPERTY("availWidth")
		DEFINE_CUSTOM_PROPERTY("colorDepth")
		DEFINE_CUSTOM_PROPERTY("left")
		DEFINE_CUSTOM_PROPERTY("top")
		DEFINE_CUSTOM_PROPERTY("pixelDepth")

		DEFINE_CUSTOM_PROPERTY("width")
		DEFINE_CUSTOM_PROPERTY("height")
		END_CUSTOM_PROPERTY


	HtmlScreen();
	virtual ~HtmlScreen();



	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
};

