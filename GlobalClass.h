#pragma once
#include "JSVMInterface.h"
#include "HtmlEventTarget.h"
//给global对象增加用户定义函数、变量，
class GlobalClass :
	public HtmlEventTarget//window只继承了HtmlEventTarget但不继承HtmlEventTarget的基类
{
	DEFINE_CUSTOM_CLASS(GlobalClass, HtmlEventTarget)

	GlobalClass();
public:

// 	struct INTEVAL
// 	{
// 		PersistentValue*pFunc=nullptr;//当clearInteval后pFunc会变为空
// 		int nInteval;//间隔毫秒数（传入的参数）
// 		uint32_t nNextCallTime = 0;//下一次调用的时间，每次执行更新
// 	};
// 
// 	std::vector<INTEVAL>m_IntevalList;



	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("_CallGC", &GlobalClass::_CallGC,0 )


		DEFINE_CUSTOM_FUNCTION("alert", &GlobalClass::alert, 1)
		DEFINE_CUSTOM_FUNCTION("setInterval", &GlobalClass::setInterval, 2)
		DEFINE_CUSTOM_FUNCTION("clearInterval", &GlobalClass::clearInterval, 1)
		DEFINE_CUSTOM_FUNCTION("blur", &GlobalClass::blur, 0)
		DEFINE_CUSTOM_FUNCTION("setTimeout", &GlobalClass::setTimeout, 2)
		DEFINE_CUSTOM_FUNCTION("clearTimeout", &GlobalClass::clearTimeout, 1)
		DEFINE_CUSTOM_FUNCTION("close", &GlobalClass::close, 0)
		DEFINE_CUSTOM_FUNCTION("confirm", &GlobalClass::confirm, 1)
		DEFINE_CUSTOM_FUNCTION("createPopup", &GlobalClass::createPopup, 0)
		DEFINE_CUSTOM_FUNCTION("focus", &GlobalClass::focus, 0)
		DEFINE_CUSTOM_FUNCTION("moveBy", &GlobalClass::moveBy, 2)
		DEFINE_CUSTOM_FUNCTION("moveTo", &GlobalClass::moveTo, 2)
		DEFINE_CUSTOM_FUNCTION("open", &GlobalClass::open, 4)
		DEFINE_CUSTOM_FUNCTION("print", &GlobalClass::print, 0)
		DEFINE_CUSTOM_FUNCTION("prompt", &GlobalClass::prompt, 2)
		DEFINE_CUSTOM_FUNCTION("resizeBy", &GlobalClass::resizeBy,2)
		DEFINE_CUSTOM_FUNCTION("resizeTo", &GlobalClass::resizeTo,2)
		DEFINE_CUSTOM_FUNCTION("scrollBy", &GlobalClass::scrollBy, 2)
		DEFINE_CUSTOM_FUNCTION("scrollTo", &GlobalClass::scrollTo, 2)
		DEFINE_CUSTOM_FUNCTION("scroll", &GlobalClass::scroll, 2)
		DEFINE_CUSTOM_FUNCTION("getComputedStyle", &GlobalClass::getComputedStyle, 1)
		DEFINE_CUSTOM_FUNCTION("requestAnimationFrame", &GlobalClass::requestAnimationFrame, 1)
		DEFINE_CUSTOM_FUNCTION("cancelAnimationFrame", &GlobalClass::cancelAnimationFrame, 1)

		DEFINE_CUSTOM_FUNCTION("postMessage", &GlobalClass::postMessage, 1)


		DEFINE_CUSTOM_FUNCTION("decodeURIComponent", &GlobalClass::decodeURIComponent, 1)

		DEFINE_CUSTOM_FUNCTION("callJSBFunction", &GlobalClass::callJSBFunction, 1)

	END_CUSTOM_FUNCTION
	BEGIN_CUSTOM_PROPERTY
	DEFINE_CUSTOM_PROPERTY("innerHeight")
	DEFINE_CUSTOM_PROPERTY("innerWidth")
	END_CUSTOM_PROPERTY

	static GlobalClass *_instance;//set after InitClass called
	static GlobalClass*GetInstance()
	{
		return _instance;
	}

	virtual ~GlobalClass();
	static bool InitClass(const HandleContext&context);

	virtual bool _InitClass(const HandleContext&context, const char*szName, const HandleObject&ParentProto, bool bHookAllProperties);
	virtual bool OnGetProperty(GetPropertyArgs&args) ;
	virtual bool OnSetProperty(SetPropertyArgs&args) ;
	virtual void OnFinalize(FinalizeArgs&args) ;
	virtual bool OnConstructor(IFunctionCallbackArgs&args) ;


	bool _CallGC(IFunctionCallbackArgs&args);



	bool setInterval(IFunctionCallbackArgs&args);
	bool clearInterval(IFunctionCallbackArgs&args);
	void ExecIntevals(const HandleContext&context);

	bool alert(IFunctionCallbackArgs&args);

	bool blur(IFunctionCallbackArgs&args);
	bool setTimeout(IFunctionCallbackArgs&args);
	bool clearTimeout(IFunctionCallbackArgs&args);
	bool close(IFunctionCallbackArgs&args);
	bool confirm(IFunctionCallbackArgs&args);
	bool createPopup(IFunctionCallbackArgs&args);
	bool focus(IFunctionCallbackArgs&args);
	bool moveBy(IFunctionCallbackArgs&args);
	bool moveTo(IFunctionCallbackArgs&args);
	bool open(IFunctionCallbackArgs&args);
	bool print(IFunctionCallbackArgs&args);
	bool prompt(IFunctionCallbackArgs&args);
	bool resizeBy(IFunctionCallbackArgs&args);
	bool resizeTo(IFunctionCallbackArgs&args);
	bool scrollBy(IFunctionCallbackArgs&args);
	bool scrollTo(IFunctionCallbackArgs&args);
	bool scroll(IFunctionCallbackArgs&args);
	bool getComputedStyle(IFunctionCallbackArgs&args);
	bool requestAnimationFrame(IFunctionCallbackArgs&args);
	bool cancelAnimationFrame(IFunctionCallbackArgs&args);
	bool postMessage(IFunctionCallbackArgs&args);

	bool decodeURIComponent(IFunctionCallbackArgs&args);
	bool callJSBFunction(IFunctionCallbackArgs&args);


	void ExecTimeout(const HandleContext&context);
	

};

