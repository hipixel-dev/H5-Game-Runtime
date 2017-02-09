#pragma once
#include "JSVMInterface.h"
class HtmlSessionStorage :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlSessionStorage, CustomClassBase)
	
public:
	std::unordered_map<std::string, std::string>m_keymap;
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("getItem", &HtmlSessionStorage::getItem, 1)
		DEFINE_CUSTOM_FUNCTION("setItem", &HtmlSessionStorage::setItem, 1)
		DEFINE_CUSTOM_FUNCTION("removeItem", &HtmlSessionStorage::removeItem, 1)
		DEFINE_CUSTOM_FUNCTION("clear", &HtmlSessionStorage::clear, 0)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY

	HtmlSessionStorage();
	virtual ~HtmlSessionStorage();

	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);

	bool getItem(IFunctionCallbackArgs&args);
	bool setItem(IFunctionCallbackArgs&args);
	bool removeItem(IFunctionCallbackArgs&args);
	bool clear(IFunctionCallbackArgs&args);
};

