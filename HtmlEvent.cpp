#include "stdafx.h"
#include "HtmlEvent.h"
#include "HtmlEventTarget.h"
#include "OpenglFBO.h"

IMPLEMENT_CUSTOM_CLASS(HtmlEvent, CustomClassBase)
HtmlEvent::HtmlEvent()
{
}


HtmlEvent::~HtmlEvent()
{
}

bool HtmlEvent::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "_EVENT", HandleObject());
}

bool HtmlEvent::stopPropagation(IFunctionCallbackArgs&args)
{
	return true;
}

bool HtmlEvent::preventDefault(IFunctionCallbackArgs&args)
{
	return true;
}


void HtmlEvent::Init(const HandleContext&context, JSVM::_TouchEvent* pEvent,const HandleObject&target)
{
	bool ret;
	LocalObject objthis(context, GetJsObject());
	HtmlEventTarget*pTarget = dynamic_cast<HtmlEventTarget*>(FindCustomClass(target));
	LocalObject obtarget(context, target);
	CustomClassAutoThisObj cc(pTarget, obtarget);
	DXPOINT pt(0, 0);// = pTarget->GetXYInScreen();

	objthis.SetProperty("altKey", ValueBase().SetBoolean(pEvent->altKey));
	objthis.SetProperty("ctrlKey", ValueBase().SetBoolean(pEvent->ctrlKey));
	objthis.SetProperty("shiftKey", ValueBase().SetBoolean(pEvent->shiftKey));
	switch (pEvent->nTouchType)
	{
	case JSVM::_TouchEvent::TT_START:
		objthis.SetProperty("type", "touchstart"); break;
	case JSVM::_TouchEvent::TT_MOVE:
		objthis.SetProperty("type", "touchmove"); break;
	case JSVM::_TouchEvent::TT_END:
		objthis.SetProperty("type", "touchend"); break;
	}
	


	LocalObject objTouches(context, objthis.GetProperty("touches").GetObject());
	if (objTouches.IsNull())
	{
		objTouches.CreateArrayObject(pEvent->touches.size());
		objthis.SetProperty("touches", objTouches);
	}
	else
	{
		assert(objTouches.IsArray());
		objTouches.SetArrayLength(context, pEvent->touches.size());
	}
	

	for (int i = 0; i < pEvent->touches.size(); i++)
	{
		JSVM::_Touch&touch = pEvent->touches[i];
		LocalObject obItem(context, objTouches.GetElement(i).GetObject());
		if (obItem.IsNull())
		{
			obItem.CreateJsObject();
			objTouches.SetElement(i, obItem);
		}
		obItem.SetProperty("target", target);
		obItem.SetProperty("clientX", ValueBase().SetInt(touch.clientX - pt.x));//相对窗口客户区坐标
		obItem.SetProperty("clientY", ValueBase().SetInt(touch.clientY-pt.y));
		obItem.SetProperty("identifier", ValueBase().SetInt(touch.identifier));
		obItem.SetProperty("pageX", ValueBase().SetInt(touch.pageX));
		obItem.SetProperty("pageY", ValueBase().SetInt(touch.pageY));
		obItem.SetProperty("screenX", ValueBase().SetInt(touch.screenX));//屏幕坐标
		obItem.SetProperty("screenY", ValueBase().SetInt(touch.screenY));
	

	}
	//目前暂时让targetTouches和touches相同
	objthis.SetProperty("targetTouches", objTouches);

	objTouches = objthis.GetProperty("changedTouches").GetObject();
	if (objTouches.IsNull())
	{
		objTouches.CreateArrayObject(pEvent->changedTouches.size());
		objthis.SetProperty("changedTouches", objTouches);
	}
	else
	{
		assert(objTouches.IsArray());
		objTouches.SetArrayLength(pEvent->changedTouches.size());
	}
	for (int i = 0; i < pEvent->changedTouches.size(); i++)
	{
		JSVM::_Touch&touch = pEvent->changedTouches[i];
		LocalObject obItem(context, objTouches.GetElement(i).GetObject());
		if (obItem.IsNull())
		{
			obItem.CreateJsObject();
			objTouches.SetElement(i, obItem);
		}
		obItem.SetProperty("target", target);
		obItem.SetProperty("clientX", ValueBase().SetInt(touch.clientX-pt.x));
		obItem.SetProperty("clientY", ValueBase().SetInt(touch.clientY-pt.y));
		obItem.SetProperty("identifier", ValueBase().SetInt(touch.identifier));
		obItem.SetProperty("pageX", ValueBase().SetInt(touch.pageX));
		obItem.SetProperty("pageY", ValueBase().SetInt(touch.pageY));
		obItem.SetProperty("screenX", ValueBase().SetInt(touch.screenX));
		obItem.SetProperty("screenY", ValueBase().SetInt(touch.screenY));

	}


}

bool HtmlEvent::stopImmediatePropagation(IFunctionCallbackArgs&args)
{
	return true;
}
