#pragma once
#include "HtmlEventTarget.h"

class HtmlDocument :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlDocument, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("createElement", &HtmlDocument::createElement, 1)
		DEFINE_CUSTOM_FUNCTION("write", &HtmlDocument::write, 1)
		DEFINE_CUSTOM_FUNCTION("getElementsByTagName", &HtmlDocument::getElementsByTagName, 1)
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("clientWidth")
		DEFINE_CUSTOM_PROPERTY("clientHeight")
		DEFINE_CUSTOM_PROPERTY("scrollLeft")
		DEFINE_CUSTOM_PROPERTY("scrollTop")
		DEFINE_CUSTOM_PROPERTY("hidden")
		DEFINE_CUSTOM_PROPERTY("URL")
		DEFINE_CUSTOM_PROPERTY("cookie")
		END_CUSTOM_PROPERTY

		int m_nWidth=0;
	int m_nHeight=0;
	
	bool m_bCookieLoaded = false;
	std::map<std::string, std::string>m_cookie;

	HtmlDocument();
	virtual ~HtmlDocument();

	static bool InitClass(const HandleContext&context);

	virtual bool OnGetProperty(GetPropertyArgs&args) ;

	virtual bool OnSetProperty(SetPropertyArgs&args) ;

	virtual void OnFinalize(FinalizeArgs&args) ;

	virtual bool OnConstructor(IFunctionCallbackArgs&args) ;

	bool createElement(IFunctionCallbackArgs&args);
	bool write(IFunctionCallbackArgs&args);
	bool getElementsByTagName(IFunctionCallbackArgs&args);
	void Measure(const HandleContext&context);

	virtual int GetWidth();
	virtual int GetHeight();
	virtual int GetClientWidth();
	virtual int GetClientHeight();
	void SaveCookie();
	std::string GetCookieStr();
	void SetCookieStr(std::string val);
	void LoadCookie();
};

