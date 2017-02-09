#pragma once
#include "JSVMInterface.h"
class HtmlNavigator :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlNavigator, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("javaEnabled", &HtmlNavigator::javaEnabled,0)
		DEFINE_CUSTOM_FUNCTION("taintEnabled", &HtmlNavigator::taintEnabled, 0)
	END_CUSTOM_FUNCTION

	BEGIN_CUSTOM_PROPERTY

	END_CUSTOM_PROPERTY





	HtmlNavigator();
	virtual ~HtmlNavigator();

	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);

	virtual bool javaEnabled(IFunctionCallbackArgs&args);
	virtual bool taintEnabled(IFunctionCallbackArgs&args);


};

