#pragma once

#include "HtmlElement.h"
#include <memory>
#include "OpenglFBO.h"
//响应事件的基类
class HtmlEventTarget:public HtmlElement
{
	DEFINE_CUSTOM_CLASS(HtmlEventTarget, HtmlElement)
public:
// 	struct EVENT_LISTENER_INFO
// 	{
// 		std::string szName;
// 		PersistentValue*value;
// 	};
// 	std::list<EVENT_LISTENER_INFO>m_eventList;//事件执行时可能会removeEventListener，使用list可能会有问题

// 	struct EVENT_WILL_EXEC//异步执行事件的每个事件
// 	{
// 		HtmlEventTarget*pHtmlObj = nullptr;
// 		std::string szEventName;
// 	};
// 	static std::list<EVENT_WILL_EXEC>s_eventWillExec;//队列
	



	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("addEventListener", &HtmlEventTarget::addEventListener, 3)
		DEFINE_CUSTOM_FUNCTION("removeEventListener", &HtmlEventTarget::removeEventListener, 3)
		DEFINE_CUSTOM_FUNCTION("dispatchEvent", &HtmlEventTarget::dispatchEvent, 1)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("visibility")
		DEFINE_CUSTOM_PROPERTY("display")
		DEFINE_CUSTOM_PROPERTY("ontouchstart")
		DEFINE_CUSTOM_PROPERTY("ontouchmove")
		DEFINE_CUSTOM_PROPERTY("ontouchend")
		END_CUSTOM_PROPERTY




	bool m_bVisibility = true;
	bool m_bDisplay = true;






	HtmlEventTarget();
	virtual ~HtmlEventTarget();
	static bool InitClass(const HandleContext&context);

	virtual bool OnGetProperty(GetPropertyArgs&args) ;

	virtual bool OnSetProperty(SetPropertyArgs&args) ;

	virtual void OnFinalize(FinalizeArgs&args) ;

	virtual bool OnConstructor(IFunctionCallbackArgs&args) ;

	bool addEventListener(IFunctionCallbackArgs&args);
	bool removeEventListener(IFunctionCallbackArgs&args);
	bool dispatchEvent(IFunctionCallbackArgs&args);
	bool AddEventListener(const HandleContext& context, const  std::string& szName, const ValueBase&vFunc, bool bAddTail=true);
	bool RemoveEventListener(const HandleContext& context, const  std::string szName,const ValueBase&vFunc);

	virtual void ExecEventListenerAsync(const HandleContext& context, const char*eventName);//在下一次mainloop时执行事件
//	virtual void ExecEventListenerAsyncFromThread(const char*eventName);//在下一次mainloop时执行事件（子线程里调用）
	virtual int ExecEventListener(const HandleContext& context, const char*eventName,ValueBase*pArg=nullptr);//立刻执行事件，返回成功执行事件的次数
	void ExecTouchEvent(const HandleContext&context, JSVM::_TouchEvent* pevent);
	DXPOINT GetXYInScreen();
	
};

