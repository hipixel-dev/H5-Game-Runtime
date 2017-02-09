#pragma once
#include "JSVMInterface.h"
#include "HtmlElement.h"
class HtmlTextNode :
	public HtmlElement
{
	DEFINE_CUSTOM_CLASS(HtmlTextNode, HtmlElement)

public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("appendData", &HtmlTextNode::appendData,1)
		DEFINE_CUSTOM_FUNCTION("deleteData", &HtmlTextNode::deleteData, 2)
		DEFINE_CUSTOM_FUNCTION("insertData", &HtmlTextNode::insertData, 2)
		DEFINE_CUSTOM_FUNCTION("replaceData", &HtmlTextNode::replaceData, 3)
		DEFINE_CUSTOM_FUNCTION("splitText", &HtmlTextNode::splitText, 1)
		DEFINE_CUSTOM_FUNCTION("substringData", &HtmlTextNode::substringData, 2)

		
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("data")
		DEFINE_CUSTOM_PROPERTY("length")
//		DEFINE_CUSTOM_PROPERTY("nodeValue")
		END_CUSTOM_PROPERTY
	HtmlTextNode();
	virtual ~HtmlTextNode();
	static bool InitClass(const HandleContext&context);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);

	bool appendData(IFunctionCallbackArgs&args);
	bool deleteData(IFunctionCallbackArgs&args);
	bool insertData(IFunctionCallbackArgs&args);
	bool replaceData(IFunctionCallbackArgs&args);
	bool splitText(IFunctionCallbackArgs&args);
	bool substringData(IFunctionCallbackArgs&args);
};

