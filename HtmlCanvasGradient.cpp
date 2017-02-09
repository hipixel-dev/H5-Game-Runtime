#include "stdafx.h"
#include "HtmlCanvasGradient.h"

IMPLEMENT_CUSTOM_CLASS(HtmlCanvasGradient, CustomClassBase)
HtmlCanvasGradient::HtmlCanvasGradient()
{
}


HtmlCanvasGradient::~HtmlCanvasGradient()
{
}

bool HtmlCanvasGradient::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HTMLGradient", HandleObject());
}
