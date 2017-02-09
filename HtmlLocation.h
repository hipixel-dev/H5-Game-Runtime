#pragma once
#include "JSVMInterface.h"
class HtmlLocation :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlLocation, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("reload", &HtmlLocation::reload,1)
		DEFINE_CUSTOM_FUNCTION("assign", &HtmlLocation::assign, 1)
		DEFINE_CUSTOM_FUNCTION("replace", &HtmlLocation::replace, 1)
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("href")
		DEFINE_CUSTOM_PROPERTY("search")
		END_CUSTOM_PROPERTY

		

	HtmlLocation();
	virtual ~HtmlLocation();
	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);

	bool reload(IFunctionCallbackArgs&args);
	bool assign(IFunctionCallbackArgs&args);
	bool replace(IFunctionCallbackArgs&args);
};

