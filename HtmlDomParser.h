#pragma once
#include "JSVMInterface.h"
#include "Markup.h"
class HtmlElement;
class HtmlDomParser :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlDomParser, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("parseFromString", &HtmlDomParser::parseFromString,1)
	END_CUSTOM_FUNCTION

	BEGIN_CUSTOM_PROPERTY

	END_CUSTOM_PROPERTY






	HtmlDomParser();
	virtual ~HtmlDomParser();



	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);

	virtual bool parseFromString(IFunctionCallbackArgs&args);
	static int CreateObjFromXml(HtmlElement*pParent, const HandleContext&context, CMarkup&xml);
	static HandleObject CreateObjFromXml(const HandleContext&context, const char*szXml);
};

