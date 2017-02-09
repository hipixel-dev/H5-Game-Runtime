#pragma once
#include "JSVMInterface.h"
//触控事件等事件带的event参数
class HtmlEvent :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlEvent, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("stopPropagation", &HtmlEvent::stopPropagation,0)
		DEFINE_CUSTOM_FUNCTION("preventDefault", &HtmlEvent::preventDefault, 0)
		DEFINE_CUSTOM_FUNCTION("stopImmediatePropagation", &HtmlEvent::stopImmediatePropagation, 0)
		END_CUSTOM_FUNCTION
		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY


	HtmlEvent();
	virtual ~HtmlEvent();

	static bool InitClass(const HandleContext&context);

	bool stopPropagation(IFunctionCallbackArgs&args);
	bool preventDefault(IFunctionCallbackArgs&args);
	bool stopImmediatePropagation(IFunctionCallbackArgs&args);
	void Init(const HandleContext&context, JSVM::_TouchEvent* pEvent, const HandleObject&target);
	
};

