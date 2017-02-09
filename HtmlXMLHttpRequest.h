#pragma once
#include "JSVMInterface.h"
class HtmlXMLHttpRequest :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlXMLHttpRequest, CustomClassBase)
	std::string m_szMethod;
	
	RESPONSETYPE m_ResponseType = RST_TEXT;
	std::string m_szURL;
	bool m_bAsync=false;
	int m_readyState=0;
//	char* m_pData=nullptr;
//	int m_nDataLen = 0;
	int m_status = 0;
	std::string m_szContentType;//post≤≈”√µΩ
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("open", &HtmlXMLHttpRequest::open,3)
		DEFINE_CUSTOM_FUNCTION("send", &HtmlXMLHttpRequest::send, 1)
		DEFINE_CUSTOM_FUNCTION("setRequestHeader", &HtmlXMLHttpRequest::setRequestHeader, 2)
		DEFINE_CUSTOM_FUNCTION("abort", &HtmlXMLHttpRequest::abort, 0)
		DEFINE_CUSTOM_FUNCTION("getAllResponseHeaders", &HtmlXMLHttpRequest::getAllResponseHeaders, 0)
		DEFINE_CUSTOM_FUNCTION("getResponseHeader", &HtmlXMLHttpRequest::getResponseHeader, 1)
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("readyState")
		DEFINE_CUSTOM_PROPERTY("status")
		DEFINE_CUSTOM_PROPERTY("responseType")
		END_CUSTOM_PROPERTY


	HtmlXMLHttpRequest();
	virtual ~HtmlXMLHttpRequest();


	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	bool open(IFunctionCallbackArgs&args);
	bool send(IFunctionCallbackArgs&args);
	bool setRequestHeader(IFunctionCallbackArgs&args);
	bool abort(IFunctionCallbackArgs&args);
	bool getAllResponseHeaders(IFunctionCallbackArgs&args);
	bool getResponseHeader(IFunctionCallbackArgs&args);
	bool onreadystatechange(const HandleContext&context);

};

