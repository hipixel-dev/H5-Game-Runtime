#pragma once
#include "JSVMInterface.h"
class HtmlCanvasGradient :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlCanvasGradient, CustomClassBase)
public:

	BEGIN_CUSTOM_FUNCTION
	

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		
		END_CUSTOM_PROPERTY



	HtmlCanvasGradient();
	virtual ~HtmlCanvasGradient();
	static bool InitClass(const HandleContext&context);
};

