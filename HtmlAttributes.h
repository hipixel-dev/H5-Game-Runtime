#pragma once
#include "JSVMInterface.h"
class HtmlElement;
class HtmlAttributes :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlAttributes, CustomClassBase)
public:
	HtmlElement*m_pParent = nullptr;



	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("getNamedItem", &HtmlAttributes::getNamedItem, 1)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY
	HtmlAttributes();
	virtual ~HtmlAttributes();
	static bool InitClass(const HandleContext&context);
	void SetParent(const HandleContext&context, const HandleObject&objparent);
	bool getNamedItem(IFunctionCallbackArgs&args);

	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
};

