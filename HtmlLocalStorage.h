#pragma once
#include "JSVMInterface.h"
#include "Markup.h"
#include "HtmlSessionStorage.h"
class HtmlLocalStorage :
	public HtmlSessionStorage
{
	DEFINE_CUSTOM_CLASS(HtmlLocalStorage, HtmlSessionStorage)
//	std::unordered_map<std::string, std::string>m_keymap;


public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("getItem", &HtmlLocalStorage::getItem,1)
		DEFINE_CUSTOM_FUNCTION("setItem", &HtmlLocalStorage::setItem, 1)
		DEFINE_CUSTOM_FUNCTION("removeItem", &HtmlLocalStorage::removeItem, 1)
		DEFINE_CUSTOM_FUNCTION("clear", &HtmlLocalStorage::clear, 0)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY


	HtmlLocalStorage();
	virtual ~HtmlLocalStorage();




	static bool InitClass(const HandleContext&context);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);

	bool getItem(IFunctionCallbackArgs&args);
	bool setItem(IFunctionCallbackArgs&args);
	bool removeItem(IFunctionCallbackArgs&args);
	bool clear(IFunctionCallbackArgs&args);


	bool LoadFromFile();
	bool SavaToFile();
};

