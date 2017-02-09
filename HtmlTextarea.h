#pragma once
#include "HtmlEventTarget.h"
class HtmlTextarea :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlTextarea, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION


		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("autofocus")
		DEFINE_CUSTOM_PROPERTY("cols")
		DEFINE_CUSTOM_PROPERTY("disabled")
		DEFINE_CUSTOM_PROPERTY("form")
		DEFINE_CUSTOM_PROPERTY("maxlength")
		DEFINE_CUSTOM_PROPERTY("name")
		DEFINE_CUSTOM_PROPERTY("placeholder")
		DEFINE_CUSTOM_PROPERTY("readonly")
		DEFINE_CUSTOM_PROPERTY("required")
		DEFINE_CUSTOM_PROPERTY("rows")
		DEFINE_CUSTOM_PROPERTY("wrap")
	

		END_CUSTOM_PROPERTY


	HtmlTextarea();
	virtual ~HtmlTextarea();

	static bool InitClass(const HandleContext&context);

};

