#pragma once
#include "JSVMInterface.h"
class HtmlElement;
class HtmlRect :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlRect, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION


		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY

		int m_left = 0;
	int m_top = 0;
	int m_width = 0;
	int m_height = 0;

	HtmlElement*m_pParent = nullptr;

	HtmlRect();
	virtual ~HtmlRect();
	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);

};

