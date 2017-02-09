
#include "stdafx.h"
#include "GlobalClass.h"
#include "Macros.h"
#include "HtmlLocation.h"
#include "OpenglContext.h"
#include "HtmlDocument.h"
#include "SystemInterface.h"
#include "JSVMInterface.h"
#include "HtmlScreen.h"

IMPLEMENT_CUSTOM_CLASS(GlobalClass, HtmlEventTarget)


GlobalClass *GlobalClass::_instance = nullptr;


GlobalClass::GlobalClass()
{
}


GlobalClass::~GlobalClass()
{
}

bool GlobalClass::InitClass(const HandleContext&context)
{
	return InitClass_s(context, nullptr, HandleObject(),false);
}

bool GlobalClass::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "innerWidth"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetWidth()));
		return true;
	}
	else if (!PROPCMP(name,"innerHeight"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetHeight()));
		return true;
    }
	bool ret= HtmlEventTarget::OnGetProperty(args);
// 	if (args.bCallDefaultProperty)
// 	{
// 		if (!strncmp(name, "__", 2))
// 			return ret;
// 
// 		if (args.CallGetPropertyFunc().IsUndefined())
// 		{//访问的属性不存在,使用getElementByID
// 			LocalObject objdoc(args.GetContext(), args.GetThisObject().GetProperty(args.GetContext(),"document").GetObject());
// 			HtmlDocument*pDoc = dynamic_cast<HtmlDocument*>(FindCustomClass(objdoc));
// 			CustomClassAutoThisObj cc(pDoc, objdoc);
// 			LocalObject obj(args.GetContext(), pDoc->FindChildByPropertyValue(args.GetContext(), "id", name, true));
// 			if(!obj.IsNull())args.SetRetValue(obj);
// 			else args.SetRetValue(ValueBase());
// 			return true;
// 		}
// 	}
	return ret;
}

bool GlobalClass::OnSetProperty(SetPropertyArgs&args)
{
	return HtmlEventTarget::OnSetProperty(args);
}

void GlobalClass::OnFinalize(FinalizeArgs&args)
{
	HtmlEventTarget::OnFinalize(args);
	_instance = nullptr;
}

bool GlobalClass::OnConstructor(IFunctionCallbackArgs&args)
{

	return true;
}

bool GlobalClass::_InitClass(const HandleContext&context, const char*szName, const HandleObject&ParentProto, bool bHookAllProperties)
{
	if (_instance)return true;
	JSContext*cx = (JSContext*)context.Get();
	JS::RootedObject global(cx, JS::CurrentGlobalOrNull(cx));
	assert(!_instance);
	_instance = new GlobalClass;
	JS_SetPrivate(global, _instance);
//	_instance->m_pObj = global;

// 	CUSTOMFUNCTION*pFuncs;
// 	int nFuncs = CustomFunction(pFuncs);
// 	for (int i = 0; i < nFuncs; i++)
// 	{
// 		JS::RootedFunction func(cx, JS_DefineFunction(cx, global, pFuncs[i].szName, js_function, pFuncs[i].nArgs, 0));
// 		JS::Heap<JSFunction*>fun(func);
// 		CustomClassBase::s_mapFuncToNative[fun] = pFuncs[i].pFunc;
// 	}

	CustomClassInfo*clsinfo = _instance->GetCustomClassInfo();
	CUSTOMFUNCTION*pFuncs;
	int nFuncs = CustomFunction(pFuncs);
	for (int i = 0; i < nFuncs; i++)
	{
		JS::RootedId funid(cx);
		JS::RootedString jsstr(cx, JS_NewStringCopyZ(cx, pFuncs[i].szName));
		JS_StringToId(cx, jsstr, &funid);
		JS::RootedFunction func(cx, JS_DefineFunctionById(cx, global, funid, js_function, pFuncs[i].nArgs, 0));
		pFuncs[i].id = funid;
		clsinfo->s_mapFuncToNative[funid.get().asBits] = &pFuncs[i];

	}


	CUSTOMPROPERTY*pProps;
	bool bOK;
	int nProp = CustomProperty(pProps);
	for (int i = 0; i < nProp; i++)
	{
		JS::RootedId propid(cx);
		JS::RootedString jsstr(cx, JS_NewStringCopyZ(cx, pProps[i].szName));

		bOK = JS_StringToId(cx, jsstr, &propid);
		bOK = JS_DefinePropertyById(cx, global, propid, JS::RootedValue(cx, JS::NullValue()), JSPROP_ENUMERATE | JSPROP_PERMANENT);
		pProps[i].id = propid;
		s_mapPropToNative[propid.get().asBits] = &pProps[i];
		//		bOK = JS_DefineProperty(cx, proto, pProps[i].szName, JS::RootedValue(cx, JS::NullValue()), JSPROP_ENUMERATE | JSPROP_GETTER | JSPROP_SETTER, JSOP_WRAPPER(JS_CAST_NATIVE_TO(js_GetProperty, JSPropertyOp)),JSOP_WRAPPER(JS_CAST_NATIVE_TO(js_SetProperty, JSStrictPropertyOp)));
	}





	//因为global先于HtmlEventTarget创建，所以手动添加HtmlEventTarget方法
	nFuncs = HtmlEventTarget::CustomFunction(pFuncs);
	for (int i = 0; i < nFuncs; i++)
	{
		JS::RootedId funid(cx);
		JS::RootedString jsstr(cx, JS_NewStringCopyZ(cx, pFuncs[i].szName));
		JS_StringToId(cx, jsstr, &funid);
		JS::RootedFunction func(cx, JS_DefineFunctionById(cx, global, funid, js_function, pFuncs[i].nArgs, 0));
		pFuncs[i].id = funid;
		clsinfo->s_mapFuncToNative[funid.get().asBits] = &pFuncs[i];
//		s_mapFuncToNative[funid.get().asBits] = &pFuncs[i];
// 		JS::RootedFunction func(cx, JS_DefineFunction(cx, global, pFuncs[i].szName, js_function, pFuncs[i].nArgs, 0));
// 		JS::Heap<JSFunction*>fun(func);
// 		CustomClassBase::s_mapFuncToNative[fun] = pFuncs[i].pFunc;
	}

	GetCustomClassInfo()->bHookAllProperties = bHookAllProperties;

	//添加window成员变量
	HandleObject hglobal(global);
	hglobal.SetProperty(context, "window", ValueBase(hglobal));
	hglobal.SetProperty(context, "parent", ValueBase(hglobal));

	hglobal.SetProperty(context,"ontouchstart", ValueBase().SetNull());
	hglobal.SetProperty(context,"ontouchmove", ValueBase().SetNull());
	hglobal.SetProperty(context,"ontouchend", ValueBase().SetNull());

	IFunctionCallbackArgs args(cx, 0, 0);

	OnConstructor(args);
	
	return true;
}

bool GlobalClass::setInterval(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(2, "GlobalClass::setInterval");

	LocalObject glob(args.getContext(), GetJsObject());
	LocalObject objlist(args.getContext(), glob["__Intervals"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__Intervals", objlist);
	}
	assert(objlist.IsArray());

	int nCount = objlist.GetArrayLength();
	int nInteval = args[1].GetInt();
	//查找重复
	int iEmpty = -1;
	for (int i = 0; i < nCount; i++)
	{
		LocalObject obitem(args.getContext(), objlist.GetElement(i).GetObject());
		if (obitem.IsNull())
		{
			if(iEmpty<0)iEmpty = i;
			continue;
		}
		LocalValue vFunItem(args.getContext(), obitem.GetProperty("fun"));
		if (vFunItem == args[0])//定时器指向同一个函数，修改时间
		{
			obitem.SetProperty("inteval", args[1]);
			obitem.SetProperty("nextTime", ValueBase().SetInt(nInteval + GetTime()));
			args.setReturnValue(ValueBase().SetInt(i + 1));
			return true;
		}
	}
	LocalObject obitem(args.getContext());
	obitem.CreateJsObject();
	obitem.SetProperty("inteval", args[1]);
	obitem.SetProperty("nextTime",ValueBase().SetInt(nInteval + GetTime()));
	obitem.SetProperty("fun", args[0]);

	if (iEmpty >= 0)
	{
		objlist.SetElement(iEmpty, obitem);
		args.setReturnValue(ValueBase().SetInt(iEmpty + 1));
	}
	else
	{
		objlist.SetElement(nCount, obitem);
		args.setReturnValue(ValueBase().SetInt(nCount + 1));
	}


	return true;


// 	INTEVAL it;
// 	it.nInteval = args[1].GetInt();
// 	for (size_t i = 0; i < m_IntevalList.size(); i++)
// 	{
// 		if (m_IntevalList[i].pFunc&&*m_IntevalList[i].pFunc == args[0])//定时器指向同一个函数，修改时间
// 		{
// 			m_IntevalList[i].nInteval = it.nInteval;
// 			m_IntevalList[i].nNextCallTime = it.nInteval + GetTime();
// 			args.setReturnValue(ValueBase().SetInt(i + 1));
// 			return true;
// 		}
// 	}
// 
// 
// 	it.nNextCallTime = GetTime() + it.nInteval;
// 	it.pFunc = new PersistentValue(args.getContext(), args[0]);
// 	m_IntevalList.push_back(it);
// 	args.setReturnValue(ValueBase().SetInt(m_IntevalList.size()));
	return true;
}

bool GlobalClass::clearInterval(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "GlobalClass::clearInterval");
	
	LocalObject glob(args.getContext(), GetJsObject());
	LocalObject objlist(args.getContext(), glob["__Intervals"].GetObject());
	if (objlist.IsNull())
	{
		return true;
	}
	assert(objlist.IsArray());
	int i = args[0].GetInt();
	int nCount = objlist.GetArrayLength();
	if (nCount > i)return true;
	objlist.SetElement(i-1, ValueBase());



// 	int i = args[0].GetInt();
// 	if (m_IntevalList[i-1].pFunc == nullptr)return true;
// 	delete m_IntevalList[i-1].pFunc;
// 	m_IntevalList[i-1].pFunc = nullptr;
	return true;
}

void GlobalClass::ExecIntevals(const HandleContext&context)
{
	LocalObject glob(context, GetJsObject());
	LocalObject objlist(context, glob["__Intervals"].GetObject());
	if (objlist.IsNull())
	{
		return;
	}
	assert(objlist.IsArray());

	int nCount = objlist.GetArrayLength();
	int nTimeNow = GetTime();
	bool ret;
	for (int i = 0; i < nCount; i++)
	{
		LocalObject obitem(context, objlist.GetElement(i).GetObject());
		if (obitem.IsNull())continue;
		int inteval = obitem.GetProperty("inteval").GetInt();
		int nextTime = obitem.GetProperty("nextTime").GetInt();
		if (nextTime <= nTimeNow)
		{
			LocalValue vfun(context, obitem.GetProperty("fun"));
			IFunctionCallJsArgs args(context);
			args.SetParams(&glob, 0);
			LocalValue vRet(context);
			if (vfun.IsObject())
			{
				ret = JSVM::executeFunction(context, vfun, args, vRet);
			}
			else if (vfun.IsString())
			{
				ret = JSVM::GetInstance()->RunScriptText(vfun.GetString(context).c_str(), "");
			}
			else assert(0);


			nextTime = nTimeNow+ inteval;
			obitem.SetProperty("nextTime", ValueBase().SetInt(nextTime));
		}
	}


// 	LocalObject objthis(context, GetJsObject());
// 	uint32_t nNow = GetTime();
// 	for (size_t i = 0; i < m_IntevalList.size(); i++)
// 	{
// 		if (!m_IntevalList[i].pFunc)continue;
// 		if (m_IntevalList[i].nNextCallTime <= nNow)
// 		{
// 			m_IntevalList[i].nNextCallTime = nNow + m_IntevalList[i].nInteval;
// 			IFunctionCallJsArgs args;
// 			args.SetParams(&objthis, 0);
// 			LocalValue vRet(context);
// 			bool ret=JSVM::executeFunction(context, *m_IntevalList[i].pFunc, args, vRet);
// 			
// 		}
// 	}
}

bool GlobalClass::alert(IFunctionCallbackArgs&args)
{
    //TODO(CJH): for debug
    std::string &&str = args[0].GetString(args.getContext());
    printf("\n%s", str.c_str());
    
    /*
	CHECK_ARGS_COUNT(1, "alert");
	std::string &&str = args[0].GetString(args.getContext());
	JSVM::GetInstance()->GetSystemInterface()->ShowAlert(str.c_str());
	*/
    return true;
}

bool GlobalClass::blur(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::setTimeout(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(2, "GlobalClass::setTimeout");
	LocalObject glob(args.getContext(), GetJsObject());
	LocalObject objlist(args.getContext(), glob["__SetTimeout"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__SetTimeout", objlist);
	}
	assert(objlist.IsArray());
	uint32_t nTime = GetTime();
	int nMilsec = args[1].GetInt();
	int nCount = objlist.GetArrayLength();
	int iEmpty = -1;
	for (int i = 0; i < nCount; i++)
	{
		LocalObject objitem(args.getContext(), objlist.GetElement(i).GetObject());
		if (objitem.IsNull())
		{
			iEmpty = i;
			break;
		}
	}
	if (iEmpty == -1)iEmpty = nCount;
	LocalObject objitem(args.getContext());
	objitem.CreateJsObject();
	objitem.SetProperty("code", args[0]);
	objitem.SetProperty("time", ValueBase().SetInt(nTime + nMilsec));
	if (args.length() > 2)//有参数
	{
		LocalObject objpara(args.getContext());
		objpara.CreateArrayObject(args.length() - 2);
		for (int i = 2; i < args.length(); i++)
		{
			objpara.SetElement(i, args[i + 2]);
		}
		objitem.SetProperty("param", objpara);
	}
	objlist.SetElement(iEmpty, objitem);
	args.setReturnValue(ValueBase().SetInt(iEmpty + 1));

	return true;
}

bool GlobalClass::clearTimeout(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "GlobalClass::clearTimeout");
	LocalObject glob(args.getContext(), GetJsObject());
	LocalObject objlist(args.getContext(), glob["__SetTimeout"].GetObject());
	if (objlist.IsNull())
	{
		return true;
	}
	assert(objlist.IsArray());
	if (args[0].IsNumber())
	{
		int idx = args[0].GetInt() - 1;
		int len = objlist.GetArrayLength();
		if (idx >= len)return true;
		objlist.SetElement(idx, ValueBase());
	}
	else if (args[0].IsObject())
	{
		int len = objlist.GetArrayLength();
		for (int i = 0; i < len; i++)
		{
			LocalObject objitem(args.getContext(), objlist.GetElement(i).GetObject());
			if (objitem.IsNull())continue;
			if (objitem.GetProperty("code") == args[0])
			{
				objlist.SetElement(i, ValueBase());
				return true;
			}
		}
	}
	return true;
}


void GlobalClass::ExecTimeout(const HandleContext&context)
{
	LocalObject glob(context, GetJsObject());
	LocalObject objlist(context, glob["__SetTimeout"].GetObject());
	if (objlist.IsNull())
	{
		return;
	}
	assert(objlist.IsArray());

	int nCount = objlist.GetArrayLength();
	int nTimeNow = GetTime();

	for (int i = 0; i < nCount; i++)
	{
		LocalObject obitem(context, objlist.GetElement(i).GetObject());
		if (obitem.IsNull())continue;
		int nextTime = obitem.GetProperty("time").GetInt();
		if (nextTime <= nTimeNow)
		{
			objlist.SetElement(i, ValueBase());
			LocalValue vCode(context, obitem.GetProperty("code"));
			IFunctionCallJsArgs args(context);
			LocalObject objparam(context, obitem.GetProperty("param").GetObject());
			args.SetParams(&glob, 0);
			if (objparam.IsArray())
			{
				int nParam = objparam.GetArrayLength();
				for (int j = 0; j < nParam; j++)
				{
					args.AddParams(objparam.GetElement(j));
				}
			}
			LocalValue vRet(context);
			bool ret = JSVM::executeFunction(context, vCode, args, vRet);
			
		}
	}

}


bool GlobalClass::close(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::confirm(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::createPopup(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::focus(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::moveBy(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::moveTo(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::open(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::print(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::prompt(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::resizeBy(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::resizeTo(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::scrollBy(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool GlobalClass::scrollTo(IFunctionCallbackArgs&args)
{
//	assert(0);
	return true;
}

bool GlobalClass::getComputedStyle(IFunctionCallbackArgs&args)
{

	return true;
}

bool GlobalClass::requestAnimationFrame(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "requestAnimationFrame");

	long ret = JSVM::GetInstance()->requestAnimationFrame(args[0]);

	args.setReturnValue(ValueBase().SetInt(ret));

	return true;
}

bool GlobalClass::cancelAnimationFrame(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "cancelAnimationFrame");
	long id = args[0].GetInt();
	JSVM::GetInstance()->cancelAnimationFrame(id);
	return true;
}

bool GlobalClass::_CallGC(IFunctionCallbackArgs&args)
{
	JSVM::GetInstance()->CallGC();
	return true;
}

bool GlobalClass::scroll(IFunctionCallbackArgs&args)
{
	return true;
}

bool GlobalClass::postMessage(IFunctionCallbackArgs&args)
{
//	CHECK_ARGS_COUNT_MIN(2, "postMessage");
	return true;

}
long strtol16(const char * _Str, char ** _EndPtr)
{
	const char*p = _Str;
	while (isxdigit(*p)&&(p-_Str<2))p++;
	if (p == _Str)return 0;
	char*tmp = new char[p - _Str + 1];
	memcpy(tmp, _Str, p - _Str);
	tmp[p - _Str] = 0;
	
	long ret = strtol(tmp, _EndPtr, 16);
	
	if (_EndPtr)
	{
		*_EndPtr = (char*)_Str+(*_EndPtr-tmp);
	}
	delete[]tmp;
	return ret;
}
bool GlobalClass::decodeURIComponent(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "decodeURIComponent");
	std::string str = args[0].GetString(args.getContext());
	//UTF8解码
	CArrayBuffer buf;
	char*p = (char*)str.data();
	while (*p)
	{
		if (*p == '%')
		{
			p++;
			char*q;
			uint8_t b = strtol16(p, &q);
			buf.AddData(&b, 1);
			p = q;
		}
		else
		{
			buf.AddData(p, 1);
			p++;
		}
	}


// 	char*ctx;
// 	char*p = strtok_s((char*)str.c_str(), "%", &ctx);
// 	CArrayBuffer buf;
// 	while (p)
// 	{
// 		uint8_t b = strtol(p, 0, 16);
// 		buf.AddData(&b, 1);
// 		p = strtok_s(nullptr, "%", &ctx);
// 
// 	}
	if (buf.GetLength() > 0)
	{

		str=JSVM::Utf8ToString((char*)buf.GetBuffer());
	
		args.setReturnValue(LocalValue(args.getContext()).SetString(str.c_str()));
		return true;
	}
	else
	{
		args.setReturnValue(LocalValue(args.getContext()).SetString(""));
		return true;
	}
}

bool GlobalClass::callJSBFunction(IFunctionCallbackArgs&args)//调用用户自定义JSB绑定函数,参数和返回值都是字符串
{
	CHECK_ARGS_COUNT_MIN(1, "callJSBFunction");
	std::string szFunName = args[0].GetString(args.getContext());
	int nArg = args.length() - 1;
	std::vector<std::string>paraList;
	for (int i = 0; i < nArg; i++)
	{
		paraList.push_back(args[i + 1].GetString(args.getContext()));
	}
	SystemInterface*psi = JSVM::GetInstance()->GetSystemInterface();
	std::string szRet = psi->callJSBFunction(szFunName, paraList);
	args.setReturnValue(LocalValue(args.getContext()).SetString(szRet.c_str()));
	return true;

}




