#pragma once
#include "HtmlEventTarget.h"
class HtmlInput :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlInput, HtmlEventTarget)
		CTRLHANDLE* m_ctrl = nullptr;
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("blur", &HtmlInput::blur, 0)
		DEFINE_CUSTOM_FUNCTION("focus", &HtmlInput::focus, 0)
		DEFINE_CUSTOM_FUNCTION("select", &HtmlInput::select, 0)
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("accept")
		DEFINE_CUSTOM_PROPERTY("align")
		DEFINE_CUSTOM_PROPERTY("alt")
		DEFINE_CUSTOM_PROPERTY("autocomplete")
		DEFINE_CUSTOM_PROPERTY("autofocus")
		DEFINE_CUSTOM_PROPERTY("checked")
		DEFINE_CUSTOM_PROPERTY("disabled")
		DEFINE_CUSTOM_PROPERTY("form")
		DEFINE_CUSTOM_PROPERTY("formaction")
		DEFINE_CUSTOM_PROPERTY("formenctype")
		DEFINE_CUSTOM_PROPERTY("formmethod")
		DEFINE_CUSTOM_PROPERTY("formnovalidate")
		DEFINE_CUSTOM_PROPERTY("formtarget")
		DEFINE_CUSTOM_PROPERTY("list")
		DEFINE_CUSTOM_PROPERTY("max")
		DEFINE_CUSTOM_PROPERTY("maxlength")
		DEFINE_CUSTOM_PROPERTY("min")
		DEFINE_CUSTOM_PROPERTY("multiple")
		DEFINE_CUSTOM_PROPERTY("name")
		DEFINE_CUSTOM_PROPERTY("pattern")
		DEFINE_CUSTOM_PROPERTY("placeholder")
		DEFINE_CUSTOM_PROPERTY("readonly")
		DEFINE_CUSTOM_PROPERTY("required")
		DEFINE_CUSTOM_PROPERTY("size")
		DEFINE_CUSTOM_PROPERTY("src")
		DEFINE_CUSTOM_PROPERTY("step")
		DEFINE_CUSTOM_PROPERTY("type")
		DEFINE_CUSTOM_PROPERTY("value")

		END_CUSTOM_PROPERTY




	HtmlInput();
	virtual ~HtmlInput();

	static bool InitClass(const HandleContext&context);

	virtual bool OnGetProperty(GetPropertyArgs&args) override;

	virtual bool OnSetProperty(SetPropertyArgs&args) override;

	virtual bool OnConstructor(IFunctionCallbackArgs&args) override;

	virtual void OnAddToParent(const HandleContext&context, HtmlElement*pParent) override;

	virtual void OnRemoveFromParent(const HandleContext&context, HtmlElement*pParent) override;

	virtual void OnSetStyle(const HandleContext&context, const char*szName) override;
	virtual void OnMeasure(int parentX, int parentY, int parentWidth, int parentHeight);

	bool blur(IFunctionCallbackArgs&args);
	bool focus(IFunctionCallbackArgs&args);
	bool select(IFunctionCallbackArgs&args);
};

