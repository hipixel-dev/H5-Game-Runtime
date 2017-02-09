#pragma once
#include "HtmlEventTarget.h"
class HtmlScript :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlScript, HtmlEventTarget)
		std::string m_src;
public:
	BEGIN_CUSTOM_FUNCTION


		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("src")
		DEFINE_CUSTOM_PROPERTY("innerText")
		END_CUSTOM_PROPERTY

	HtmlScript();
	virtual ~HtmlScript();


	static bool InitClass(const HandleContext&context);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	bool LoadScript(const HandleContext&context, const std::string& name, bool bAsync, bool bIsText);
};

