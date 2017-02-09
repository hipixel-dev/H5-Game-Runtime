#include "stdafx.h"
#include "HtmlEventTarget.h"
#include "HtmlElement.h"
#include "HtmlEvent.h"
#include "OpenglContext.h"
#include "HtmlCanvas.h"
IMPLEMENT_CUSTOM_CLASS(HtmlEventTarget, HtmlElement)


HtmlEventTarget::HtmlEventTarget()
{
}


HtmlEventTarget::~HtmlEventTarget()
{
// 	{
// 		for (std::list<EVENT_LISTENER_INFO>::iterator it = m_eventList.begin(); it != m_eventList.end(); ++it)
// 		{
// 			delete it->value;
// 		}
// 		m_eventList.clear();
// 	}
// 	{
// 		for (static std::list<EVENT_WILL_EXEC>::iterator it = s_eventWillExec.begin(); it != s_eventWillExec.end();)
// 		{
// 			if (it->pHtmlObj == this)
// 			{
// 				std::list<EVENT_WILL_EXEC>::iterator it2 = it;
// 				++it;
// 				s_eventWillExec.erase(it2);
// 
// 			}
// 			else ++it;
// 		}
// 	}
}

bool HtmlEventTarget::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "visibility"))
	{
		if (m_bVisibility)args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "visible"));
		else args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "hidden"));
		return true;
	}
	else if (!PROPCMP(name, "display"))
	{
		if (m_bDisplay)args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "block"));
		else args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "none"));
		return true;
	}

	return HtmlElement::OnGetProperty(args);
}

bool HtmlEventTarget::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "visibility"))
	{
		std::string &&val = args.GetValue().GetString(args.GetContext());
		if (!_stricmp(val.c_str(), "visible"))
		{
			m_bVisibility = true;
		}
		else
		{
			m_bVisibility = false;
		}
		return true;
	}
	else if (!PROPCMP(name, "display"))
	{
		std::string &&val = args.GetValue().GetString(args.GetContext());
		if (!_stricmp(val.c_str(),"none"))
		{
			m_bDisplay = false;
		}
		else
		{
			m_bDisplay = true;
		}
		return true;
	}
	else if (!PROPCMP(name, "ontouchstart"))
	{
		if(args.GetValue().IsObject())
		{
			CustomClassAutoThisObj cc(this, args);
			JSVM::GetInstance()->AddTouchEventObject(GetJsObject());
		}
	}
	else if (!PROPCMP(name, "ontouchmove"))
	{
	}
	else if (!PROPCMP(name, "ontouchend"))
	{
	}
	return HtmlElement::OnSetProperty(args);
}

void HtmlEventTarget::OnFinalize(FinalizeArgs&args)
{
	JSVM::GetInstance()->RemoveTouchEventObject(args.GetThisObject());
	HtmlElement::OnFinalize(args);
}

bool HtmlEventTarget::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret= HtmlElement::OnConstructor(args);
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject objthis(args.getContext(), GetJsObject());
	objthis.SetProperty(args.getContext(), "ontouchstart", ValueBase().SetNull());
	objthis.SetProperty(args.getContext(), "ontouchmove", ValueBase().SetNull());
	objthis.SetProperty(args.getContext(), "ontouchend", ValueBase().SetNull());
	objthis.SetProperty(args.getContext(), "touch", ValueBase().SetNull());

	return ret;
}

bool HtmlEventTarget::InitClass(const HandleContext&context)
{
	if (HtmlElement::GetPrototype_s().IsNull())
	{
		HtmlElement::InitClass(context);
	}
	return InitClass_s(context, "__HtmlEventTarget", HtmlElement::GetPrototype_s());
}

bool HtmlEventTarget::addEventListener(IFunctionCallbackArgs&args)
{
//	PrintDescribeScriptedCaller(args.getContext());
	CHECK_ARGS_COUNT_MIN(2, "HtmlEventTarget::addEventListener");

	std::string &&szName = args[0].GetString(args.getContext());
	
	bool ret = AddEventListener(args.getContext(), szName, args[1]);

	return ret;
}

bool HtmlEventTarget::removeEventListener(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(2, "HtmlEventTarget::removeEventListener");
	std::string &&szName = args[0].GetString(args.getContext());
	bool ret = RemoveEventListener(args.getContext(), szName, args[1]);
	return ret;
}

bool HtmlEventTarget::dispatchEvent(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlEventTarget::AddEventListener(const HandleContext& context, const std::string& szName,const ValueBase&vFunc,bool bAddTail)
{

	LocalObject objthis(context, GetJsObject());
	LocalObject objlist(context, objthis["__EventListener"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		objthis.SetProperty("__EventListener", objlist);

	}
	assert(objlist.IsArray());
	int nCount = objlist.GetArrayLength();
	//检查重复
	for (int i = 0; i < nCount; i++)
	{
		LocalValue item(context,objlist.GetElement(i));

		if (item.IsUndefined())
			continue;
		LocalObject obitem(context, item.GetObject());
		std::string &&name = obitem.GetProperty(context, "name").GetString(context);

		if (name == szName)
		{
			LocalValue fun(context, obitem.GetProperty(context, "fun"));
			if (fun == vFunc)return true;
		}
	}
	LocalObject obitem(context);
	obitem.CreateJsObject();
	obitem.SetProperty("name", szName.c_str());
	obitem.SetProperty("fun", vFunc);
	bool ret= objlist.SetElement(nCount, obitem);




	if (ret)
	{
		if (szName == "touchstart" || szName == "touchmove" || szName == "touchend")//触控事件，把本对象添加到JSVM的触控里
		{
			JSVM::GetInstance()->AddTouchEventObject(objthis);
		}
	}



	return ret;



	//检查重复
// 	for (std::list<EVENT_LISTENER_INFO>::iterator it = m_eventList.begin(); it != m_eventList.end(); ++it)
// 	{
// 		if (it->szName == szName&&*it->value == vFunc)return true;
// 	}
// 	EVENT_LISTENER_INFO eli;
// 	eli.szName = szName;
// 	eli.value = new PersistentValue(context, vFunc);
// 	if (bAddTail)m_eventList.push_back(eli);
// 	else m_eventList.push_front(eli);



	return true;

}

bool HtmlEventTarget::RemoveEventListener(const HandleContext& context, const std::string szName,const ValueBase&vFunc)
{
	LocalObject objthis(context, GetJsObject());
	LocalObject objlist(context, objthis["__EventListener"].GetObject());
	if (objlist.IsNull())
	{
		return true;
	}
	assert(objlist.IsArray());
	int nCount = objlist.GetArrayLength();

	for (int i = 0; i < nCount; i++)
	{
		LocalValue item(context,objlist.GetElement(i));
	
			
		if (item.IsUndefined())
			continue;
		LocalObject obitem(context, item.GetObject());
		std::string &&name = obitem.GetProperty(context, "name").GetString(context);

		if (name == szName)
		{
			LocalValue fun(context, obitem.GetProperty(context, "fun"));
			if (fun == vFunc)
			{
				return objlist.RemoveElement(i);
				
			}
		}
	}

	if (szName == "touchstart" || szName == "touchmove" || szName == "touchend")//触控事件
	{
		if (objlist.GetArrayLength() == 0)
		{
			JSVM::GetInstance()->RemoveTouchEventObject(objthis);
		}
	}

// 	for (std::list<EVENT_LISTENER_INFO>::iterator it = m_eventList.begin(); it != m_eventList.end(); ++it)
// 	{
// 		if (it->szName == szName&&*it->value == vFunc)
// 		{
// 			delete it->value;
// 			m_eventList.erase(it);
// 			return true;
// 		}
// 	}
	return true;
}

// void HtmlEventTarget::ExecEventListenerAsyncFromThread(const char*eventName)
// {
// 
// }


void HtmlEventTarget::ExecEventListenerAsync(const HandleContext& context, const char*eventName)
{
	LocalObject glob(context, JSVM::GetInstance()->GetGlobal());
	LocalObject objlist(context, glob["__ExecEventAsync"].GetObject());
	LocalObject objthis(context, GetJsObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__ExecEventAsync", objlist);
	}
	assert(objlist.IsArray());
	int nCount = objlist.GetArrayLength();
	LocalObject obitem(context);
	obitem.CreateJsObject();
	obitem.SetProperty("thisobj", objthis);
	obitem.SetProperty("name", eventName);
	objlist.SetElement(nCount, obitem);

	

// 	if (eventName == "error")
// 	{
// //		assert(0);
// 	}
// 	EVENT_WILL_EXEC ev;
// 	ev.szEventName = eventName;
// 	ev.pHtmlObj = this;
// 	s_eventWillExec.push_back(ev);
}

int HtmlEventTarget::ExecEventListener(const HandleContext& context, const char*eventName, ValueBase*pArg )
{
	int n = 0;
	LocalObject objthis(context, GetJsObject());
	LocalObject objlist(context, objthis["__EventListener"].GetObject());
	if (!objlist.IsNull())
	{
		assert(objlist.IsArray());
		int nCount = objlist.GetArrayLength();


		for (int i = nCount - 1; i >= 0; i--)// execute events from bottom to top in case of deleting event while execution
		{
			LocalValue item(context, objlist.GetElement(i));
			if (!item.IsObject())
				continue;
			LocalObject obitem(context, item.GetObject());
			std::string &&name = obitem.GetProperty(context, "name").GetString(context);
			if (name == eventName)
			{
				LocalValue fun(context, obitem.GetProperty(context, "fun"));
				if (fun.IsUndefined() || fun.IsNull())
					continue;

				IFunctionCallJsArgs args(context);
				if (pArg)
				{
					args.SetParams(&objthis, 1, pArg);
				}
				else
				{
					args.SetParams(&objthis, 0);
				}
				LocalValue vRet(context);
				bool bOK = JSVM::GetInstance()->executeFunction(context, fun, args, vRet);
				if (bOK)n++;
			}

		}
	}
//	if (n==0)// try attribution begin with "on"
	{
		std::string szname = (std::string)"on" + eventName;
		LocalValue vevent(context, objthis.GetProperty(szname.c_str()));
		if (vevent.IsObject())
		{
			IFunctionCallJsArgs args(context);
			if (pArg)
			{
				args.SetParams(&objthis, 1, pArg);
			}
			else
			{
				args.SetParams(&objthis, 0);
			}
			LocalValue vRet(context);
			bool bOK = JSVM::GetInstance()->executeFunction(context, vevent, args, vRet);
			n++;
		}
		else if (vevent.IsString())
		{
			std::string &&code = vevent.GetString(context);
			bool bOK = JSVM::GetInstance()->RunScriptText(code.c_str(), code.c_str());
			n++;
		}
	}




	
// 	LocalObject objthis(context, GetJsObject());
// 
// 	
// 	for (std::list<EVENT_LISTENER_INFO>::iterator it = m_eventList.begin(); it != m_eventList.end(); )
// 	{
// 		std::list<EVENT_LISTENER_INFO>::iterator it2 = it;
// 		it2++;//先++防止executeFunction时事件被删除，目前只能防止删除当前事件
// 		if (it->szName == eventName)
// 		{
// 			
// 			IFunctionCallJsArgs args;
// 			args.SetParams(&objthis, 0);
// 			LocalValue vRet(context);
// 			bool bOK=JSVM::GetInstance()->executeFunction(context, *it->value, args, vRet);
// 			if (bOK)n++;
// 		}
// 		it = it2;
// 	}
	return n;
}


void HtmlEventTarget::ExecTouchEvent(const HandleContext&context, JSVM::_TouchEvent* pevent)
{
	const char*eventName = "";
	if (pevent->nTouchType == JSVM::_TouchEvent::TT_START)eventName = "touchstart";
	else if (pevent->nTouchType == JSVM::_TouchEvent::TT_MOVE)eventName = "touchmove";
	else if (pevent->nTouchType == JSVM::_TouchEvent::TT_END)eventName = "touchend";
	else assert(0);
    
	LocalObject objthis(context, GetJsObject());
	LocalObject objevent(context, objthis.GetProperty("__touchevent").GetObject());
	if (objevent.IsNull())
	{
		objevent = HtmlEvent::CreateObject(context);
		objthis.SetProperty("__touchevent", objevent);
	}
	HtmlEvent*pe = (HtmlEvent*)FindCustomClass(objevent);
	CustomClassAutoThisObj cc(pe, objevent);
	pe->Init(context, pevent,objthis);

	LocalValue vevent(context, objevent);
	ExecEventListener(context, eventName, &vevent);
	if (pevent->nTouchType == JSVM::_TouchEvent::TT_END)
	{// add a click event
		ExecEventListener(context, "click", &vevent);
	}
}

DXPOINT HtmlEventTarget::GetXYInScreen()//取得元素在屏幕上的坐标
{
	assert(0);
//	assert(dynamic_cast<HtmlCanvas*>(this));
//	return DXPOINT((OpenglContext::GetWidth() - m_width)*0.5f, 0);
	return DXPOINT(0, 0);
}


//std::list<HtmlEventTarget::EVENT_WILL_EXEC> HtmlEventTarget::s_eventWillExec;
