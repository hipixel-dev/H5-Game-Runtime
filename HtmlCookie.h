#pragma once
#include "JSVMInterface.h"
class HtmlCookie :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlCookie, CustomClassBase)
public:

	BEGIN_CUSTOM_FUNCTION

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
	
		END_CUSTOM_PROPERTY



	HtmlCookie();
	virtual ~HtmlCookie();
	static bool InitClass(const HandleContext&context);

};

