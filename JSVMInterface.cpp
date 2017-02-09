#include "stdafx.h"
#include "JSVMInterface.h"
#include "jsapi.h"
#include "js/TypeDecls.h"
#include "GlobalClass.h"
#include "HtmlDocument.h"
#include "js/OldDebugAPI.h"
#include "HtmlCanvas.h"
#include "HtmlLocation.h"
#include "HtmlNavigator.h"
#include "HtmlWebGLRenderingContext.h"
#include "HtmlXMLHttpRequest.h"
#include "HtmlAudio.h"
#include "HtmlDomParser.h"
#include "HtmlScript.h"

#include "HtmlLocalStorage.h"
#include "google-gumbo-parser/src/gumbo.h"
#include "HtmlDiv.h"
#include "HtmlImgClass.h"
#include "HtmlInput.h"
#include "HtmlAudioContext.h"
#include "HtmlConsole.h"

#if defined(ANDROID)
#include "../proj.android/jni/libiconv/include/iconv.h"
#elif defined(_IOS)
#include <iconv.h>
#endif
#include "HtmlLink.h"
#include "HtmlLI.h"
#include "HtmlUL.h"
#include "HtmlSpan.h"
#include "HtmlP.h"
#include "HtmlStyleElement.h"
#include "HtmlScreen.h"

#ifndef _WIN32
//使用libiconv转码
int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return 0;
	memset(outbuf, 0, outlen);
	int nlen = outlen;
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1) return 0;
	iconv_close(cd);
	return nlen-outlen;
}
#endif
int jscharlen(const jschar*str)
{
#ifdef _WIN32
	return wcslen(str);
#else

	const jschar*p = str;
	while (*p)p++;
	return p - str;
#endif

}


std::list<CLASSFUNC> *CustomClassBase::s_InitList=nullptr;

//std::map<JS::Heap<JSObject*>, CustomClassBase*> CustomClassBase::s_mapCustomObj;

//std::map<JS::Heap<JSFunction*>, CustomFuncCallback> CustomClassBase::s_mapFuncToNative;
//IMPLEMENT_CUSTOM_CLASS(CustomClassBase)




std::string trim(const std::string& str)
{
	std::string::size_type pos = str.find_first_not_of(' ');
	if (pos == std::string::npos)
	{
		return str;
	}
	std::string::size_type pos2 = str.find_last_not_of(' ');
	if (pos2 != std::string::npos)
	{
		return str.substr(pos, pos2 - pos + 1);
	}
	return str.substr(pos);
}





// 
// void* PersistentObject::Get() const
// {
// 	JS::PersistentRootedObject*pHeapObj = (JS::PersistentRootedObject*)m_pObject;
// 	return pHeapObj->get();
// }
// 
// void PersistentObject::Set(JSObject*pObject)
// {
// 	JS::PersistentRootedObject*pHeapObj = (JS::PersistentRootedObject*)m_pObject;
// 	*pHeapObj = (JSObject*)pObject;
// }
// 
// PersistentObject::PersistentObject(const HandleContext&context, JSObject*pObj /*= nullptr*/) :HandleObject(nullptr)
// {
// 	m_cx = (JSContext*)context.Get();
// 	JS::PersistentRootedObject*pHeapObj = new JS::PersistentRootedObject((JSContext*)context.Get());
// 	if (pObj)*pHeapObj = (JSObject*)pObj;
// 	m_pObject = pHeapObj;
// 
// 	
// 	//PersistentRootedObject似乎不能防止被GC，因此添加到global
// 
// 	JSVM::GetInstance()->AddValueToGlobal(JS::RootedValue(m_cx, OBJECT_TO_JSVAL(pObj)));
// 
// 	
// }
// 
// PersistentObject::PersistentObject(const HandleContext&context, const HandleObject&obj) :HandleObject(nullptr)
// {
// 	new(this)PersistentObject(context, (JSObject*)obj.Get());
// }
// 
// PersistentObject::~PersistentObject()
// {
// 	JS::PersistentRootedObject*pHeapObj = (JS::PersistentRootedObject*)m_pObject;
// 
// 	JSVM::GetInstance()->RemoveValueFromGlobal(JS::RootedValue(m_cx, OBJECT_TO_JSVAL((JSObject*)pHeapObj->get())));
// 
// 	delete pHeapObj;
// 	m_pObject = nullptr;
// }




HandleContext::HandleContext(JSContext*pContext)
{
	m_pCtx = (JSContext*)pContext;
}

HandleContext::~HandleContext()
{

}

JSContext* HandleContext::Get() const
{
	return m_pCtx;
}



LocalObject::LocalObject(const HandleContext&context, JSObject*pObj/*=nullptr*/) :HandleObject(nullptr),m_pObject(context.Get(),pObj)
{
	m_cx = context;
}

LocalObject::LocalObject(const HandleContext&context,const HandleObject&obj) : HandleObject(nullptr), m_pObject(context.Get(), obj.Get())
{
	m_cx = context;
}

LocalObject::LocalObject(const LocalObject&obj) : HandleObject(), m_pObject(obj.m_cx.Get(),obj.Get())
{
	m_cx = obj.m_cx;
}

JSObject* LocalObject::Get() const
{
	return m_pObject;
}

void LocalObject::Set(JSObject*pObject)
{
	m_pObject = pObject;
}


LocalObject::~LocalObject()
{

}
long long nCreate = 0;
bool LocalObject::CreateJsObject(const HandleContext&context) //创建一个JS对象并赋值到this
{
	nCreate++;
	JSContext*cx = (JSContext*)context.Get();
	m_pObject = JS_NewObject(cx, nullptr, JS::RootedObject(cx), JS::RootedObject(cx));
	return true;
}

bool LocalObject::CreateJsObject()
{
	return CreateJsObject(m_cx);
}

bool LocalObject::CreateArrayObject(const HandleContext&context, int nLength)
{
	JSContext*cx = (JSContext*)context.Get();

	m_pObject = JS_NewArrayObject(cx, nLength);
	
	return true;
}

bool LocalObject::CreateArrayObject(int nLength)
{
	return CreateArrayObject(m_cx, nLength);
}

bool LocalObject::CreatePrimitiveArray(const HandleContext&context, int nLength, ARRAYTYPE type, void*pData /*= nullptr*/)
{
	JSContext*cx = (JSContext*)context.Get();
	JS::RootedObject*pObj = &m_pObject;
	switch (type)
	{
	case AT_INT8:
	{
		*pObj = JS_NewInt8Array(cx, nLength);
		if (pData)
		{
			int8_t*p = JS_GetInt8ArrayData(*pObj);
			memcpy(p, pData, nLength);
		}
		return true;
		break;
	}
	case AT_UINT8:
	{
		*pObj = JS_NewUint8Array(cx, nLength);
		if (pData)
		{
			uint8_t*p = JS_GetUint8ArrayData(*pObj);
			memcpy(p, pData, nLength);
		}
		return true;
		break;
	}
	case AT_INT16:
	{
		*pObj = JS_NewInt16Array(cx, nLength);
		if (pData)
		{
			int16_t*p = JS_GetInt16ArrayData(*pObj);
			memcpy(p, pData, nLength * 2);
		}
		return true;
		break;
	}
	case AT_UINT16:
	{
		*pObj = JS_NewUint16Array(cx, nLength);
		if (pData)
		{
			uint16_t*p = JS_GetUint16ArrayData(*pObj);
			memcpy(p, pData, nLength * 2);
		}
		return true;
		break;
	}
	case AT_INT32:
	{
		*pObj = JS_NewInt32Array(cx, nLength);
		if (pData)
		{
			int32_t*p = JS_GetInt32ArrayData(*pObj);
			memcpy(p, pData, nLength * 4);
		}
		return true;
		break;
	}
	case AT_UINT32:
	{
		*pObj = JS_NewUint32Array(cx, nLength);
		if (pData)
		{
			uint32_t*p = JS_GetUint32ArrayData(*pObj);
			memcpy(p, pData, nLength * 4);
		}
		return true;
		break;
	}
	case AT_FLOAT32:
	{
		*pObj = JS_NewFloat32Array(cx, nLength);
		if (pData)
		{
			float*p = JS_GetFloat32ArrayData(*pObj);
			memcpy(p, pData, nLength * 4);
		}
		return true;
		break;
	}
	case AT_FLOAT64:
	{
		*pObj = JS_NewFloat64Array(cx, nLength);
		if (pData)
		{
			double*p = JS_GetFloat64ArrayData(*pObj);
			memcpy(p, pData, nLength * 8);
		}
		return true;
		break;
	}
	case AT_BOOLEAN8:
	{
		*pObj = JS_NewArrayObject(cx, nLength);
		if (pData)
		{
			unsigned char *p = (unsigned char*)pData;
			JS::RootedValue v(cx);
			for (int i = 0; i < nLength; i++)
			{
				v.setBoolean(p[i]!=0);
				JS_SetElement(cx, *pObj, i, v);
			}
		}
		break;
	}
	case AT_NOARRAY:return false;
	}
	return false;
}

LocalObject::ARRAYTYPE LocalObject::GetPrimitiveArrayType(const HandleContext&context)
{
	JSContext*cx = (JSContext*)context.Get();
	JS::RootedObject*pObj = &m_pObject;

	if (JS_IsInt8Array(*pObj))return AT_INT8;
	if (JS_IsUint8Array(*pObj))return AT_UINT8;
	if (JS_IsInt16Array(*pObj))return AT_INT16;
	if (JS_IsUint16Array(*pObj))return AT_UINT16;
	if (JS_IsInt32Array(*pObj))return AT_INT32;
	if (JS_IsUint32Array(*pObj))return AT_UINT32;
	if (JS_IsFloat32Array(*pObj))return AT_FLOAT32;
	if (JS_IsFloat64Array(*pObj))return AT_FLOAT64;
	return AT_NOARRAY;
}

bool LocalObject::SetElement(const HandleContext&context, int index, const ValueBase&val)
{
	JSContext*cx = (JSContext*)context.Get();

	return JS_SetElement(cx, m_pObject, index, JS::RootedValue(cx, *val.Get()));
}

bool LocalObject::SetElement(int index, const ValueBase&val)
{
	return SetElement(m_cx, index, val);
}

ValueBase LocalObject::GetElement(const HandleContext&context, int index)
{
	JSContext*cx = (JSContext*)context.Get();
	JS::RootedValue vp(cx);
	JS_GetElement(cx, m_pObject, index, &vp);
	return ValueBase(vp);
}

ValueBase LocalObject::GetElement(int index)
{
	return GetElement(m_cx, index);
}

int LocalObject::GetArrayLength(const HandleContext&context)
{
	JSContext*cx = (JSContext*)context.Get();
	uint32_t len = 0;
	JS_GetArrayLength(cx,m_pObject, &len);
	return len;
}

bool LocalObject::IsArray(const HandleContext&context)
{
	if (IsNull())return false;
	JSContext*cx = (JSContext*)context.Get();
	bool bOK= JS_IsArrayObject(cx, m_pObject);
	bool bOK1 = JS_IsArrayBufferObject(m_pObject);
	bool bOK2 = JS_IsArrayBufferViewObject(m_pObject);
	return bOK|bOK1|bOK2;
}

void* LocalObject::GetArrayData(const HandleContext&context)
{
	if (JS_IsArrayBufferViewObject(m_pObject))
	{
		return JS_GetArrayBufferViewData(m_pObject);
	}
	else if (JS_IsArrayBufferObject(m_pObject))
	{
		return JS_GetArrayBufferData(m_pObject);

	}
	return nullptr;
}

int LocalObject::GetArrayByteLength(const HandleContext&context)
{
	
	if (JS_IsArrayBufferViewObject(m_pObject))
	{
		return JS_GetArrayBufferViewByteLength(m_pObject);
	}
	else if (JS_IsArrayBufferObject(m_pObject))
	{

		return JS_GetArrayBufferByteLength(m_pObject);
	}
	return 0;
}

bool LocalObject::SetArrayLength(const HandleContext&context, int nLen)
{
	return JS_SetArrayLength((JSContext*)context.Get(), m_pObject, nLen);
}

bool LocalObject::SetArrayLength(int nLen)
{
	return SetArrayLength(m_cx, nLen);
}

// ArrayViewItem LocalObject::operator[](int iElement)//数组下标访问
// {
// 	if (!IsArray(m_cx))return ArrayViewItem(m_cx);
// 	return ArrayViewItem(m_cx,GetElement(m_cx, iElement),*this,iElement);
// }
ValueBase LocalObject::operator[](const char*name)//属性访问
{
	return GetProperty(m_cx, name);
}

CustomClassBase* LocalObject::GetNativeObj()
{
	return CustomClassBase::FindCustomClass(*this);
}

bool LocalObject::SetProperty(const char*name, const ValueBase&value)
{
	return HandleObject::SetProperty(m_cx, name, value);
}

bool LocalObject::SetProperty(const char*name, const char*str, bool bAutoToNumber)
{
	int type = 0;//0:字符串，1：int，2：double,3:bool
	int nDot = 0;
	if (bAutoToNumber)
	{
		if (!strcmp(str, "true"))
		{
			return HandleObject::SetProperty(m_cx, name,ValueBase().SetBoolean(true));
		}
		else if (!strcmp(str, "false"))
		{
			return HandleObject::SetProperty(m_cx, name, ValueBase().SetBoolean(false));
		}
		int len = strlen(str);
		for (int i = 0; i < len; i++)
		{
			if ((str[i]<'0' || str[i]>'9'))
			{
				if (str[i] != '.')
				{
					type = 0;
					goto line1;
				}
				else
				{
					if (nDot > 0)
					{
						type = 0;
						goto line1;
					}
					nDot++;
				}
			}
		}
		if (nDot == 1)type = 2;
		else if (nDot == 0)type = 1;
	}
line1:
	switch (type)
	{
	case 0:return HandleObject::SetProperty(m_cx, name, LocalValue(m_cx).SetString(m_cx, str));
	case 1:return HandleObject::SetProperty(m_cx, name, ValueBase().SetInt(atoi(str)));
	case 2:return HandleObject::SetProperty(m_cx, name, ValueBase().SetNumber(strtod(str, 0)));
	default:
		assert(0);
		return false;
	}
	
}

bool LocalObject::RemoveElement(int iElement)
{
	int len = GetArrayLength();
	if (iElement >= len)return false;
	for (int i = iElement; i < len - 1; i++)
	{
		SetElement(i, GetElement(i + 1));
	}
	SetArrayLength(m_cx, len - 1);
	return true;
}

ValueBase LocalObject::GetProperty(const HandleContext&context, const char*name)
{
	return HandleObject::GetProperty(context, name);
}

ValueBase LocalObject::GetProperty(const char*name)
{
	return HandleObject::GetProperty(m_cx, name);
}


HandleObject::HandleObject()
{

}

HandleObject::HandleObject(JSObject*pObject)
{
	m_pObject = pObject;
}

HandleObject::HandleObject(const HandleObject&obj)
{
	new(this)HandleObject((JSObject*)obj.Get());
}

HandleObject::~HandleObject()
{
	
}

JSObject* HandleObject::Get() const
{
	return m_pObject;
}

void HandleObject::Set(JSObject*pObject)
{
	m_pObject = pObject;
}

void HandleObject::SetObj(const HandleObject&obj)
{
	Set((JSObject*)obj.Get());
}

bool HandleObject::SetProperty(const HandleContext&context, const char*name, const ValueBase&value)
{
	JSContext*cx = (JSContext*)context.Get();
	JSObject*pobj = (JSObject*)Get();
	if (value.IsUndefined())
	{
		return JS_DeleteProperty((JSContext*)context.Get(), JS::RootedObject(cx, pobj), name);
	}
	const jsval&pval = *value.Get();
	bool bOK = JS_SetProperty((JSContext*)context.Get(), JS::RootedObject(cx, pobj), name, JS::RootedValue(cx, pval));
//  	std::wstring wname = JSVM::StringToUnicode(name);
//  	bool bOK = JS_SetUCProperty((JSContext*)context.Get(), JS::RootedObject(cx, pobj), wname.c_str(), wname.length(), JS::RootedValue(cx, pval));
	return bOK;
}

ValueBase HandleObject::GetProperty(const HandleContext&context, const char*name)
{
	JSContext*cx = (JSContext*)context.Get();
	JS::RootedValue vp(cx);
	JSObject*pobj = (JSObject*)Get();
	bool ret = JS_GetProperty(cx, JS::RootedObject(cx, pobj), name,  &vp);
//  	std::wstring wname = JSVM::StringToUnicode(name);
//  	bool ret = JS_GetUCProperty(cx, JS::RootedObject(cx, pobj), wname.c_str(),wname.length(), &vp);
	if (!ret)return ValueBase();
	return ValueBase(vp);
}

bool HandleObject::IsNull()const
{
	return Get() == nullptr;
}

bool HandleObject::operator==(const HandleObject&src)
{
	return Get() == src.Get();
}

int HandleObject::GetProperties(const HandleContext&context,ObjectProperties&props)
{
	bool ok;
//	assert(0);
	JSContext*cx = (JSContext*)context.Get();
	JS::RootedObject obj(cx, (JSObject*)Get());


	
	JS::AutoIdArray vecid(cx, JS_Enumerate(cx, obj));

	size_t len = vecid.length();



	JS::Rooted<JSPropertyDescriptor>desc(cx);
	JS::RootedValue vid(cx);
	for (size_t i = 0; i < len; i++)
	{
		ok=JS_GetPropertyDescriptorById(cx, obj, JS::RootedId(cx, vecid[i]), &desc);
		ObjectProperties::PROPERTY prop;
		prop.index = i;
		prop.pParent = &props;
		props.m_values.append(ValueBase(desc.value()));

		ok=JS_IdToValue(cx, vecid[i], &vid);
		LocalValue vstr(context, vid.get());
		std::string&&str = vstr.GetString(context);
// 		JSString *tmp = JS::ToString(cx, vid);
// 		char*str = JS_EncodeString(cx, tmp);
		prop.isEnumerate = true; //(pda.array[i].flags&JSPD_ENUMERATE) != 0;
		prop.isPremanent = false;// (pda.array[i].flags&JSPD_PERMANENT) != 0;
		prop.isReadOnly =  false;// (pda.array[i].flags&JSPD_READONLY) != 0;
		props.GetProps()[str] = prop;
//		JS_free(cx,str);
	}

// 	JSPropertyDescArray pda;
// 	std::string nam;
// 	bool ok = JS_GetPropertyDescArray(cx, obj, &pda);
// 	for (uint32_t i = 0; i < pda.length; i++)
// 	{
// 		nam = ValueBase(pda.array[i].id).GetString(context);
// 		ObjectProperties::PROPERTY prop;
// 		prop.pValue = new LocalValue(context, pda.array[i].value);
// 		prop.isEnumerate = (pda.array[i].flags&JSPD_ENUMERATE) != 0;
// 		prop.isPremanent = (pda.array[i].flags&JSPD_PERMANENT) != 0;
// 		prop.isReadOnly = (pda.array[i].flags&JSPD_READONLY) != 0;
// 		props.GetProps()[nam] = prop;
// 		
// 	}

	return props.GetProps().size();
}

bool HandleObject::operator!=(const HandleObject&src)
{
	return !(*this == src);
}

HandleObject& HandleObject::operator=(const HandleObject&src)
{
	SetObj(src);
	return *this;
}
int ValueBase::GetStringLength(const HandleContext &context) const
{
	JSContext*cx = (JSContext*)context.Get();
	const jsval*pval = Get();
	if (IsString())
	{
		JSString *tmp = JS::ToString(cx, JS::RootedValue(cx, *pval));
		return JS_GetStringLength(tmp);
	}
	else
	{
		assert(0);
		JSString *tmp = JS::ToString(cx, JS::RootedValue(cx, *pval));
		return JS_GetStringLength(tmp);

	}
}


std::string ValueBase::GetString(const HandleContext &context) const
{
	JSContext*cx = (JSContext*)context.Get();
	std::string ret;
	const jsval*pval=Get();
	if (pval->isUndefined() || pval->isNull())return "";
	JSString *tmp = JS::ToString(cx, JS::RootedValue(cx, *pval));
	//因spidermonkey会将<=0xff的unicode字符缩为char储存，所以在这里将它展开为双字节UNICODE
	if (!JS_StringIsFlat(tmp))
		JS_FlattenString(cx, tmp);//执行完后再JS_StringHasLatin1Chars才会准确，否则可能不准确导致读不到字符串
	if (JS_StringHasLatin1Chars(tmp))
	{
		
		char*str = JS_EncodeString(cx, JS::RootedString(cx, tmp));
		int len = strlen(str);
		jschar* wstr = new jschar[len + 1];
		for (int i = 0; i < len; i++)
		{
			wstr[i] = (uint8_t)str[i];
		}
		wstr[len] = 0;
		ret = JSVM::UnicodeToString(wstr);

		delete[]wstr;
		JS_free(cx,str);
		return ret;
	}
	else 
	{
//		PrintDescribeScriptedCaller(context);
		const jschar*pch = JS_GetTwoByteExternalStringChars(tmp);
		ret = JSVM::GetInstance()->UnicodeToString(pch);
		return ret;
	}
// #ifdef _WIN32
// 
// 
// 	char*str = JS_EncodeString(cx, JS::RootedString(cx, tmp));
// 	ret = str;
// 	//UTF8转ASCII
// // 	int wlen=MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
// // 	WCHAR *pwstr = new WCHAR[wlen];
// // 	MultiByteToWideChar(CP_UTF8, 0, str, -1, pwstr, wlen);
// // 	int clen=WideCharToMultiByte(0, 0, pwstr, -1, 0, 0, 0, 0);
// // 	char*pstr = new char[clen];
// // 	WideCharToMultiByte(0, 0, pwstr, -1, pstr, clen, 0, 0);
// // 	ret = pstr;
// // 	delete[]pwstr;
// // 	delete[]pstr;
// #else
// 	char*str = JS_EncodeStringToUTF8(cx, JS::RootedString(cx,tmp));
// 	ret = str;
// #endif
// 	JS_free(cx, str);
// 	return ret;
}

ValueBase& ValueBase::SetString(const HandleContext &cx, const char*str)
{
	
// 	OutputDebugString(str);
// 	OutputDebugString("\n");
	//字符串一律转UNICODE
	wstring16 wstr = JSVM::StringToUnicode(str);
	auto emptyStr = JS_NewUCStringCopyZ((JSContext*)cx.Get(), wstr.c_str());
//	auto emptyStr = JS_NewStringCopyZ((JSContext*)cx.Get(), str);
	Set(STRING_TO_JSVAL(emptyStr));
	OnValueSet();
	return *this;
}

ValueBase& ValueBase::SetObject(const HandleObject&obj)
{
	Get()->setObject(*(JSObject*)obj.Get());
	OnValueSet();
	return *this;
}

ValueBase& ValueBase::SetBoolean(bool val)
{
	Get()->setBoolean(val);
	OnValueSet();
	return *this;
}

ValueBase& ValueBase::SetNumber(double val)
{
	Get()->setDouble(val);
	OnValueSet();
	return *this;
}

ValueBase& ValueBase::SetInt(int32_t val)
{
	Get()->setInt32(val);
	OnValueSet();
	return *this;
}

HandleObject ValueBase::GetObject() const
{
	if (!IsObject())return HandleObject();
	return HandleObject(&Get()->toObject());
}

bool ValueBase::GetBoolean() const
{
	return Get()->toBoolean();
}

double ValueBase::GetNumber() const
{
	if (IsNull() || IsUndefined())return 0;
	double ret= Get()->toNumber();
	if (isnan(ret))
	{
//		PrintDescribeScriptedCaller(JSVM::GetInstance()->GetContext());
		return 0;
	}
	return ret;
}

int32_t ValueBase::GetInt() const
{
	if (IsInt32())return Get()->toInt32();
	else if (IsNumber())return GetNumber();
	else if (IsNull())return 0;
	else if (IsString())
	{
		std::string&&str = GetString(JSVM::GetInstance()->GetContext());
		return atoi(str.c_str());
	}
	else assert(0);
	return 0;
}

bool ValueBase::IsObject() const
{
	return Get()->isObject();
}

bool ValueBase::IsNumber() const
{
	return Get()->isNumber();
}

bool ValueBase::IsString() const
{
	return Get()->isString();
}

bool ValueBase::IsInt32() const
{
	return Get()->isInt32();
}

bool ValueBase::IsUndefined() const
{

	return Get()->isUndefined();
}

bool ValueBase::IsNull() const
{
	return Get()->isNull();
}

ValueBase::~ValueBase()
{

}



ValueBase::ValueBase(const jsval&val)
{
	m_pVal = val;

}

ValueBase::ValueBase( const HandleObject&obj)
{
	if (obj.IsNull())
	{
		m_pVal.setNull();
	}
	else
		m_pVal.setObject(*obj.Get());
}

// ValueBase::ValueBase(bool b)
// {
// 	m_pVal = BOOLEAN_TO_JSVAL(b);
// 
// }

ValueBase::ValueBase(const ValueBase&src)
{
	m_pVal =*src.Get();
}


jsval* ValueBase::Get() const
{
	return (jsval*)&m_pVal;
}

bool ValueBase::operator==(const ValueBase&src)
{
	return *Get() == *src.Get();

}

bool ValueBase::operator!=(const ValueBase&src)
{
	return !operator==(src);
}

ValueBase& ValueBase::SetNull()
{
	Get()->setNull();
	OnValueSet();
	return *this;
}

ValueBase& ValueBase::SetUndefined()
{
	Get()->setUndefined();
	OnValueSet();
	return *this;
}

size_t ValueBase::GetID(const HandleContext&context)
{
	JS::RootedId id(context.Get());
	bool bOK = JS_ValueToId(context.Get(), JS::RootedValue(context.Get(), *Get()),&id);
	if (!bOK)return 0;
	return id.get().asBits;
}




LocalValue::LocalValue(const HandleContext&context, const jsval&pValue) :ValueBase(), m_pVal(context.Get(), pValue)
{
	m_cx = context;

}

LocalValue::LocalValue(const HandleContext&context,const ValueBase&value) :ValueBase(), m_pVal(context.Get(), *value.Get())
{
	m_cx = context;
}

LocalValue::LocalValue(const HandleContext&context,const HandleObject&value) : ValueBase(), m_pVal(context.Get(), OBJECT_TO_JSVAL(value.Get()))
{
	m_cx = context;
}

LocalValue::LocalValue(const LocalValue&src) : m_pVal(src.m_cx.Get())
{
	m_cx = src.m_cx;
}

LocalValue::~LocalValue()
{

}

jsval* LocalValue::Get()const 
{
	return (jsval*)m_pVal.address();
}

bool LocalValue::GetIDValue(size_t id)
{
	jsid i;
	i.asBits = id;
	return JS_IdToValue(m_cx.Get(), i, &m_pVal);
}

int32_t LocalValue::GetInt() const
{
	if (IsString())
	{
		std::string &&str = GetString(m_cx);
		return atoi(str.c_str());
	}
	else return ValueBase::GetInt();
}

double LocalValue::GetNumber() const
{
	if (IsString())
	{
		std::string &&str = GetString(m_cx);
		return strtod(str.c_str(), 0);
	}
	else return ValueBase::GetNumber();
}

LocalValue& LocalValue::SetString(const char*str)
{
	SetString(m_cx, str);
	return *this;
}

IFunctionCallbackArgs::IFunctionCallbackArgs()
{
}
IFunctionCallbackArgs::IFunctionCallbackArgs(const HandleContext& cx, int argc, void* args)
{
	this->m_cx = cx;
	this->args = CallArgsFromVp(argc, (jsval*)args);
}

IFunctionCallbackArgs::~IFunctionCallbackArgs()
{

}

const bool IFunctionCallbackArgs::hasReturn()
{
	return m_return;
}

void IFunctionCallbackArgs::setReturnValue(const ValueBase&val)
{
	args.rval().set(*val.Get());
	m_return = true;
}

HandleObject IFunctionCallbackArgs::thisObj()
{
	return HandleObject(&args.thisv().toObject());
}

HandleObject IFunctionCallbackArgs::callee() const
{
	return HandleObject(&args.callee());
}

ValueBase IFunctionCallbackArgs::operator[](int i) const
{
	return ValueBase(args[i].get());
}

int IFunctionCallbackArgs::length() const
{
	return args.length();
}

const HandleContext& IFunctionCallbackArgs::getContext() const
{
	return m_cx;
}

ValueBase IFunctionCallbackArgs::getReturnValue()
{
	return ValueBase(args.rval().get());
}

// bool CustomClassTemplate::CreatePrototype()
// {
// 	JSClass jsclass;
// 	jsclass.name = m_szName.c_str();
// 	jsclass.addProperty = JS_PropertyStub;
// 	jsclass.delProperty = JS_DeletePropertyStub;
// 	jsclass.getProperty = js_GetProperty;
// 	jsclass.setProperty = js_SetProperty;
// 	jsclass.enumerate = JS_EnumerateStub;
// 	jsclass.resolve = JS_ResolveStub;
// 	jsclass.convert = JS_ConvertStub;
// 	jsclass.finalize = js_finalize;
// 	jsclass.flags = JSCLASS_HAS_PRIVATE;
// 
// 	
// 	JSContext*cx = (JSContext*)m_Context.Get();
// 	JS::RootedObject global(cx, JS::CurrentGlobalOrNull(cx));
// 
// 	JS::RootedObject proto(cx);
// 	proto= JS_InitClass(
// 		cx, global,
// 		JS::RootedObject(cx,(JSObject*)m_ParentProto.Get()),
// 		&jsclass,
// 		js_constructor, 0, // constructor
// 		NULL,
// 		NULL,
// 		NULL, // no static properties
// 		NULL);
// 	if (!proto)return false;
// 	m_prototype = proto;
// 	s_TemplateMap[m_prototype] = this;
// 
// 	return true;
// }
// 
// bool CustomClassTemplate::js_GetProperty(JSContext *cx, JS::Handle<JSObject*> obj, JS::Handle<jsid> id, JS::MutableHandle<JS::Value> vp)
// {
// 
// 
// 	JS::RootedObject proto(cx, JS_GetObjectPrototype(cx, obj));
// 	CustomClassTemplate*pThis = FindTemplate(proto);
// 	if (!pThis||!pThis->m_pCallback)
// 	{
// 		return JS_PropertyStub(cx, obj, id, vp);
// 	}
// 
// 
// 	JS::RootedValue property(cx);
// 	JS_IdToValue(cx, id, &property);
// 	
// 	ValueBase val(&property.get());
// 	std::string name = val.GetString(cx);
// 	
// 
// 
// 	GetPropertyArgs args;
// 	args.szName = name;
// 	HandleObject handleObject(obj.get());
// 	args.pObj = &handleObject;
// 	MyMutableHandleValue retValue(cx, &vp);
// 	
// 	bool ret = pThis->m_pCallback->OnGetProperty(args);
// 	return ret;
// }
// 
// bool CustomClassTemplate::js_SetProperty(JSContext *cx, JS::Handle<JSObject*> obj, JS::Handle<jsid> id, bool strict, JS::MutableHandle<JS::Value> vp)
// {
// 	JS::RootedObject proto(cx, JS_GetObjectPrototype(cx, obj));
// 	CustomClassTemplate*pThis = FindTemplate(proto);
// 	if (!pThis || !pThis->m_pCallback)
// 	{
// 		return JS_PropertyStub(cx, obj, id, vp);
// 	}
// 
// 
// 	JS::RootedValue property(cx);
// 	JS_IdToValue(cx, id, &property);
// 	ValueBase val(&property.get());
// 	std::string name = val.GetString(cx);
// 
// 	SetPropertyArgs args;
// 	args.szName = name;
// 	HandleObject handleObject(obj.get());
// 	args.pObj = &handleObject;
// 	MyMutableHandleValue v(cx, &vp);
// 	args.pValue = &v;
// 	bool ret = pThis->m_pCallback->OnSetProperty(args);
// 	return ret;
// }
// 
// bool CustomClassTemplate::js_constructor(JSContext *cx, uint32_t argc, jsval *vp)
// {
// 	JS::CallArgs args = CallArgsFromVp(argc, vp);
// 	JSFunction* func = JS_ValueToFunction(cx, args.calleev()); // 被调用的函数
// 	assert(func);
// 	if (!func)
// 	{
// 		JS_ReportError(cx, "SM commonCtorCallback : Couldn't convert callee value to JSFunction type!");
// 		return false;
// 	}
// 
// 	// 获取函数名称（在这里应该是创建类的名称）
// 	JSString* jsStr = JS_GetFunctionId(func);

// }
// 
// void CustomClassTemplate::js_finalize(JSFreeOp *fop, JSObject *obj)
// {
// 
// }
// 
// CustomClassTemplate* CustomClassTemplate::FindTemplate(JS::HandleObject proto)
// {
// 	JS::Heap<JSObject*> obj(proto);
// 	std::map<JS::Heap<JSObject*>, CustomClassTemplate*>::iterator it = s_TemplateMap.find(obj);
// 	if (it == s_TemplateMap.end())return nullptr;
// 	return it->second;
// }
// 
// std::map<JS::Heap<JSObject*>, CustomClassTemplate*> CustomClassTemplate::s_TemplateMap;


CustomClassBase::CustomClassBase()
{

}

CustomClassBase::~CustomClassBase()
{
	delete m_pObj;

}

bool CustomClassBase::_InitClass(const HandleContext&context, const char*szName, const HandleObject&ParentProto, bool bHookAllProperties)
{
	if (!GetPrototype().IsNull())return true;
	JSClass*pJsClass = GetJsClass();
	pJsClass->name = szName;
	pJsClass->addProperty = JS_PropertyStub;
	pJsClass->delProperty = JS_DeletePropertyStub;
	pJsClass->getProperty = /*JS_PropertyStub;*/js_GetProperty;
	pJsClass->setProperty = /*JS_StrictPropertyStub;*/js_SetProperty;
	pJsClass->enumerate = js_EnumerateStub;
	pJsClass->resolve = JS_ResolveStub;
	pJsClass->convert = JS_ConvertStub;
	pJsClass->finalize = js_finalize;
	pJsClass->flags = JSCLASS_HAS_PRIVATE;
	pJsClass->construct = GetConstructor();
	
	JSContext*cx = (JSContext*)context.Get();
	JS::RootedObject global(cx, JS::CurrentGlobalOrNull(cx));


// 	CUSTOMPROPERTY*pProps;
// 	bool bOK;
// 	int nProp = CustomProperty(pProps);
// 	JSPropertySpec *properties = new JSPropertySpec[nProp + 1];
// 	for (int i = 0; i < nProp; i++)
// 	{
// 		properties[i] = { pProps[i].szName, JSPROP_ENUMERATE | JSPROP_GETTER | JSPROP_SETTER |  JSPROP_SHARED|JSPROP_NATIVE_ACCESSORS ,
// 			JSOP_WRAPPER(JS_CAST_NATIVE_TO(&CustomClassBase::js_GetProperty2, JSPropertyOp)),
// 			JSOP_WRAPPER(JS_CAST_NATIVE_TO(&CustomClassBase::js_SetProperty2, JSStrictPropertyOp)) };
// 
// 	}
// 	properties[nProp] = JS_PS_END;
	
	
	bool bOK;

	JS::RootedObject proto(cx);
	proto = JS_InitClass(
		cx, global,
		JS::RootedObject(cx, (JSObject*)ParentProto.Get()),
		pJsClass,
		GetConstructor(), 0, // constructor
		NULL,//properties,
		NULL,
		NULL, // no static properties
		NULL);
//	delete[]properties;
	if (!proto)return false;
	SetPrototype(proto);
	CustomClassInfo*clsinfo = GetCustomClassInfo();
	CUSTOMFUNCTION*pFuncs;
	int nFuncs = CustomFunction(pFuncs);
	for (int i = 0; i < nFuncs; i++)
	{
		JS::RootedId funid(cx);
		JS::RootedString jsstr(cx, JS_NewStringCopyZ(cx, pFuncs[i].szName));
		bOK = JS_StringToId(cx, jsstr, &funid);
		JS::RootedFunction func(cx, JS_DefineFunctionById(cx, proto, funid, js_function, pFuncs[i].nArgs, 0));
		pFuncs[i].id = funid;
		
		clsinfo->s_mapFuncToNative[funid.get().asBits] = &pFuncs[i];

//		JS::RootedFunction func(cx, JS_DefineFunction(cx, proto, pFuncs[i].szName, js_function, pFuncs[i].nArgs, 0));
// 		JS::Heap<JSFunction*>fun(func);
// 		s_mapFuncToNative[fun] = pFuncs[i].pFunc;
	}


	CUSTOMPROPERTY*pProps;
	
	int nProp = CustomProperty(pProps);
	for (int i = 0; i < nProp; i++)
	{
		JS::RootedId propid(cx);
		JS::RootedString jsstr(cx, JS_NewStringCopyZ(cx, pProps[i].szName));
		
		bOK=JS_StringToId(cx, jsstr,&propid);
		bOK = JS_DefinePropertyById(cx, proto, propid, JS::RootedValue(cx, JS::NullValue()), JSPROP_ENUMERATE | JSPROP_PERMANENT);
		pProps[i].id = propid;
		s_mapPropToNative[propid.get().asBits] = &pProps[i];
//		bOK = JS_DefineProperty(cx, proto, pProps[i].szName, JS::RootedValue(cx, JS::NullValue()), JSPROP_ENUMERATE | JSPROP_GETTER | JSPROP_SETTER, JSOP_WRAPPER(JS_CAST_NATIVE_TO(js_GetProperty, JSPropertyOp)),JSOP_WRAPPER(JS_CAST_NATIVE_TO(js_SetProperty, JSStrictPropertyOp)));
	}
	GetCustomClassInfo()->bHookAllProperties = bHookAllProperties;
	return true;
}
// bool CustomClassBase::js_GetProperty2(JSContext *cx, uint32_t argc, jsval *vp)
// {
// 	return true;
// }
// 
// bool CustomClassBase::js_SetProperty2(JSContext *cx, uint32_t argc, jsval *vp)
// {
// 	HandleContext hcx(cx);
// 	JS::CallArgs cargs = CallArgsFromVp(argc, vp);
// 
// 	LocalObject hobj(hcx, &cargs.thisv().toObject());
// 	CustomClassBase*pThis = FindCustomClass(hobj);
// 	if (!pThis)return true;
// 	
// 	int nargs = cargs.length();
// 	JSFunction* func = JS_ValueToFunction(cx, cargs.calleev());
// 	JSString* jsStr = JS_GetFunctionId(func);
// 	char*pstr = JS_EncodeString(cx, jsStr);
// 	SetPropertyArgs args;
// 	
// 	args.pContext = &hcx;
// 	int t = GetTickCount();
// 	for (int i = 0; i < 100000; i++)
// 	{
// //		LocalValue hobj(hcx, cargs.thisv());
// 		JS::HandleValue hv= cargs.thisv();
// 	}
// 	int tt = GetTickCount() - t;
// 
// 
// 	args.pObj = &hobj;
// 	MyMutableHandleValue val(hcx,&cargs[0]);
// 	args.pValue = &val;
// 	args.szName = pstr;
// //	PrintDescribeScriptedCaller(cx);
// 	bool ret = pThis->OnSetProperty(args);
// 	if (args.bCallDefaultProperty)
// 		assert(0);
// 		//return JS_StrictPropertyStub(cx, obj, id, strict, vp);
// 
// 	JS_free(cx,pstr);
// 	return ret;
// }
// 


bool CustomClassBase::js_GetProperty(JSContext *cx, JS::Handle<JSObject*> obj, JS::Handle<jsid> id, JS::MutableHandle<JS::Value> vp)
{
	CustomClassBase*pThis = FindCustomClass(obj);
	if (!pThis)return JS_PropertyStub(cx, obj, id, vp);
	CUSTOMPROPERTY*pProp;

	
	MapPropToNativeIt it;
	if (!pThis->GetCustomClassInfo()->bHookAllProperties)
	{
		it = s_mapPropToNative.find(id.get().asBits);
		if (it == s_mapPropToNative.end())return JS_PropertyStub(cx, obj, id, vp);


		GetPropertyArgs args;
		HandleContext hcx(cx);
		args.pContext = &hcx;
		JSHandleObject hobj(&obj);
		args.pObj = &hobj;
		args.pRetValue = &vp;
		args.szName = it->second->szName;
		args.pid = &id;
//		PrintDescribeScriptedCaller(cx);
		bool ret = pThis->OnGetProperty(args);
		if (args.bCallDefaultProperty)
			return JS_PropertyStub(cx, obj, id, vp);
		return ret;
	}
	else
	{
		JS::RootedValue vname(cx);
		JS_IdToValue(cx,id, &vname);
		JSString *tmp = JS::ToString(cx, vname);
		char*szName = JS_EncodeString(cx, tmp);
		GetPropertyArgs args;
		HandleContext hcx(cx);
		args.pContext = &hcx;
		JSHandleObject hobj(&obj);
		args.pObj = &hobj;
		args.pRetValue = &vp;
		args.szName = szName;
		args.pid = &id;
		bool ret = pThis->OnGetProperty(args);
		JS_free(cx,szName);
		if (args.bCallDefaultProperty)
			return JS_PropertyStub(cx, obj, id, vp);
		return ret;
	}
}

bool CustomClassBase::js_SetProperty(JSContext *cx, JS::Handle<JSObject*> obj, JS::Handle<jsid> id, bool strict, JS::MutableHandle<JS::Value> vp)
{
	CustomClassBase*pThis = FindCustomClass(obj);
	if (!pThis)return JS_StrictPropertyStub(cx ,obj, id,strict, vp);
	
	MapPropToNativeIt it;
	if (!pThis->GetCustomClassInfo()->bHookAllProperties)
	{
		it = s_mapPropToNative.find(id.get().asBits);
		if (it == s_mapPropToNative.end())return JS_StrictPropertyStub(cx, obj, id, strict, vp);



		SetPropertyArgs args;
		HandleContext hcx(cx);
		args.pContext = &hcx;
		JSHandleObject hobj(&obj);
		args.pObj = &hobj;
		MyMutableHandleValue val(cx, &vp);
		args.pValue = &val;
		args.szName = it->second->szName;
//		PrintDescribeScriptedCaller(cx);
		bool ret = pThis->OnSetProperty(args);
		if (args.bCallDefaultProperty)
			return JS_StrictPropertyStub(cx, obj, id, strict, vp);
		return ret;
	}
	else
	{
		JS::RootedValue vname(cx);
		JS_IdToValue(cx, id, &vname);
		JSString *tmp = JS::ToString(cx, vname);
		char*szName = JS_EncodeString(cx, tmp);

		SetPropertyArgs args;
		HandleContext hcx(cx);
		args.pContext = &hcx;
		JSHandleObject hobj(&obj);
		args.pObj = &hobj;
		MyMutableHandleValue val(cx, &vp);
		args.pValue = &val;
		args.szName = szName;
		//	PrintDescribeScriptedCaller(cx);
		bool ret = pThis->OnSetProperty(args);
		JS_free(cx, szName);
		if (args.bCallDefaultProperty)
			return JS_StrictPropertyStub(cx, obj, id, strict, vp);
		return ret;
	}
}

void CustomClassBase::js_finalize(JSFreeOp *fop, JSObject *obj)
{
	CustomClassBase*pThis = FindCustomClass(obj);
	if (!pThis)return;
	FinalizeArgs args;
	HandleObject hobj(obj);
	args.pObj = &hobj;
// 	OutputDebugStringA("OnFinalize:");
// 	OutputDebugStringA(pThis->GetThisClassName());
	pThis->OnFinalize(args);
//	s_mapCustomObj.erase(JS::Heap<JSObject*>(obj));
//	OutputDebugStringA("::ok\n");
	delete pThis;
}




bool CustomClassBase::OnGetProperty(GetPropertyArgs&args)
{
	args.bCallDefaultProperty = true;
	return true;
}

bool CustomClassBase::OnSetProperty(SetPropertyArgs&args)
{
	args.bCallDefaultProperty = true;
	return true;
}

void CustomClassBase::OnFinalize(FinalizeArgs&args)
{
	
}

bool CustomClassBase::OnConstructor(IFunctionCallbackArgs&args)
{
	LocalObject obj(args.getContext(), CreateJsObject(args.getContext()));
	JSObject*pObj = (JSObject*)obj.Get();
	JS_SetPrivate(pObj, this);
	assert(!m_pObj);
//	m_pObj = new JS::RootedObject((JSContext*)args.getContext().Get(), pObj);
//	m_pObj = JS::Heap<JSObject*>(pObj);
//	s_mapCustomObj[JS::Heap<JSObject*>(pObj)] = this;
	args.setReturnValue(ValueBase(obj));
	return true;
}

bool CustomClassBase::js_function(JSContext *cx, uint32_t argc, jsval *vp)
{
	bool bOK;
	JS::CallArgs args = CallArgsFromVp(argc, vp);
	JSFunction* func = JS_ValueToFunction(cx, JS::RootedValue(cx, args.calleev())); // 被调用的函数
	JS::RootedString fstr(cx,JS_GetFunctionId(func));
	JS::RootedId funid(cx);
	bOK=JS_StringToId(cx, fstr, &funid);
    
	LocalObject thisobj(HandleContext(cx), &args.thisv().toObject());
	if (thisobj.IsNull())//全局调用没有this
	{
		thisobj = JSVM::GetInstance()->GetGlobal();
	}
	CustomClassBase*pThis = FindCustomClass(thisobj.Get());
	assert(pThis);
	if (!pThis)
		return false;
	CUSTOMFUNCTION*pFunc = nullptr;
	CustomClassInfo*clsinfo = pThis->GetCustomClassInfo();
	do
	{
		CustomClassInfo::MapFunToNativeIt it = clsinfo->s_mapFuncToNative.find(funid.get().asBits);
		if (it != clsinfo->s_mapFuncToNative.end())
		{
			pFunc = it->second;
			break;
		}
		clsinfo = clsinfo->pParentClassInfo;
	} while (clsinfo);
	if (!pFunc)
	{
		char*str = JS_EncodeString(cx,fstr);
		JS_free(cx, str);
		return false;
	}

	IFunctionCallbackArgs funargs(cx,argc,vp);
	
	
	CustomClassAutoThisObj cc(pThis,thisobj);
//	PrintDescribeScriptedCaller(HandleContext(cx),pFunc->szName);
	bool ret = pThis->OnFunctionCall(funargs,pFunc);
//	bool ret=(pThis->*callback)(funargs);
	if (!funargs.hasReturn())
	{
		funargs.setReturnValue(ValueBase());
	}
	return ret;
}

CustomClassBase* CustomClassBase::FindCustomClass(JSObject* Obj)
{
	if (!Obj)return GlobalClass::GetInstance();
	CustomClassBase*ret = (CustomClassBase*)JS_GetPrivate(Obj);
// 	if (!ret)
// 	{
// 		std::map<JS::Heap<JSObject*>, CustomClassBase*>::iterator it = s_mapCustomObj.find(JS::Heap<JSObject*>(Obj));
// 		if (it == s_mapCustomObj.end())return NULL;
// 		return it->second;
// 	}
	return ret;
}

void CustomClassBase::CleanUp()
{
// 	for (std::map<JS::Heap<JSObject*>, CustomClassBase*>::iterator it = s_mapCustomObj.begin(); it != s_mapCustomObj.end(); ++it)
// 	{
// 		delete it->second;
// 	}
// 	s_mapCustomObj.clear();
	delete s_InitList;
	s_InitList = nullptr;
}

bool CustomClassBase::OnFunctionCall(IFunctionCallbackArgs&args, CUSTOMFUNCTION*pFunc)
{
	return (this->*(pFunc->pFunc))(args);
}

bool CustomClassBase::ProtectObjNoGC( bool bProtect)
{
	char key[20];
	sprintf(key, "__%d", this);
	HandleContext context = JSVM::GetInstance()->GetContext();
	LocalObject global(context, JSVM::GetInstance()->GetGlobal());
	LocalObject objnogc(context, global.GetProperty("__ObjNoGCList").GetObject());
	if (bProtect)
	{
		if (objnogc.IsNull())
		{
			objnogc.CreateJsObject();
			global.SetProperty("__ObjNoGCList", objnogc);
		}

	
		objnogc.SetProperty(key, GetJsObject());
		m_bProtectObjNoGC = true;
	}
	else
	{
		if (objnogc.IsNull())
			return true;
		objnogc.SetProperty(key, ValueBase());
		m_bProtectObjNoGC = false;
	}

	return true;
}

HandleObject CustomClassBase::GetObjFromNoGC()
{
	char key[20];
	sprintf(key, "__%d", this);
	HandleContext context = JSVM::GetInstance()->GetContext();
	LocalObject global(context, JSVM::GetInstance()->GetGlobal());
	LocalObject objnogc(context, global.GetProperty("__ObjNoGCList").GetObject());
	
	if (objnogc.IsNull())
		return HandleObject();
	return objnogc.GetProperty(key).GetObject();
		
	


}

bool CustomClassBase::js_EnumerateStub(JSContext *cx, JS::HandleObject obj)
{
	return JS_EnumerateStub(cx, obj);
}

CustomClassInfo CustomClassBase::s_CustomClassBase_ClassInfo("CustomClassBase",nullptr);

std::unordered_map<size_t, CUSTOMPROPERTY*> CustomClassBase::s_mapPropToNative;

//std::unordered_map<size_t, CUSTOMFUNCTION*> CustomClassBase::s_mapFuncToNative;










//////////////////////////////////////////////////////////////////////////
class SM_JSPrincipals : public JSPrincipals
{
public:
	explicit SM_JSPrincipals(int rc = 0)
		: JSPrincipals()
	{
		refcount = rc;
	}
};

static SM_JSPrincipals shellTrustedPrincipals(1);

static bool CheckObjectAccess(JSContext *cx)
{
	return true;
}

static JSSecurityCallbacks securityCallbacks = {
	CheckObjectAccess,
	nullptr
};

void(*oldreportError)(JSContext *cx, const char *message, JSErrorReport *report);
#define JSB_MAX_STACK_QUOTA 500000
void reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
	char str[2000];
	sprintf(str,"%s(%u):%s\n",
		report->filename ? report->filename : "<no filename=\"filename\">",
		(unsigned int)report->lineno,
		message);
	JSVM::GetInstance()->GetSystemInterface()->ShowErr(str);
//	oldreportError(cx, message, report);

};

void PrintDescribeScriptedCaller(const HandleContext&context,const char*str)
{

	JSContext*cx = (JSContext*)context.Get();
	JS::RootedScript sc(cx);
	unsigned int lineno;
	JS::AutoFilename files;
	const char*szFile = "";
	bool ok = JS::DescribeScriptedCaller(cx, &files, &lineno);
	if (ok)
		szFile = files.get();
	int len = 0;
	if (szFile )len= strlen(szFile);
	else szFile = "";

	char*tmp = new char[len + 100];
	if (!str)
		sprintf(tmp, "%s(%d)\n", szFile, lineno);
	else
		sprintf(tmp, "%s(%d) %s\n", szFile, lineno, str);
	OutputDebugStringA(tmp);
	delete[]tmp;

// 	JS::RootedObject stackp(cx);
// 	ok=CaptureCurrentStack(cx, &stackp);
// 	if (stackp != NULL)
// 	{
// 		JS_objecti
// 	}
}

JSClass JSVM::global_class = {
	"global", JSCLASS_GLOBAL_FLAGS | JSCLASS_HAS_PRIVATE,
	JS_PropertyStub, JS_DeletePropertyStub,&CustomClassBase::js_GetProperty, &CustomClassBase::js_SetProperty,
	JS_EnumerateStub, JS_ResolveStub, JS_ConvertStub, &CustomClassBase::js_finalize,
	nullptr, nullptr, nullptr,
	JS_GlobalObjectTraceHook
};
// spidermonkey创建JS Global Object 
JSObject* JSVM::sm_newGlobalObject(JSContext* jsCx)
{
	JS::CompartmentOptions options;
	options.setVersion(JSVERSION_LATEST);
	JS::RootedObject glob(jsCx, JS_NewGlobalObject(jsCx, &global_class, &shellTrustedPrincipals, JS::DontFireOnNewGlobalHook, options));
	if (!glob) {
		return nullptr;
	}
	JSAutoCompartment ac(jsCx, glob);
	bool ok = true;
	ok = JS_InitStandardClasses(jsCx, glob);
	if (ok)
		JS_InitReflect(jsCx, glob);
	if (!ok)
		return nullptr;

	JS_FireOnNewGlobalObject(jsCx, glob);

	return glob;
}
JSVM::JSVM()
: m_touchIDtoErase(0)
{
	pthread_mutex_init(&m_lock, 0);
}

JSVM::~JSVM()
{
	pthread_mutex_destroy(&m_lock);
}
#ifdef _DEBUG
unsigned long nGC = 0;
void JSVM::OnGCCallBack(JSRuntime *rt, JSGCStatus status, void *data)
{
	char str[12];
	sprintf(str, "GC:%u\n", nGC);
	OutputDebugStringA(str);
	nGC++;
}
#endif
bool JSVM::initEnvironment()
{

	m_bLoaded = false;
	if (this->m_szRootPath.length() == 0)
	{
		
		char path[MAX_PATH];
		getcwd(path, sizeof(path));
		char*p = path;
		while (*p){ if (*p == '\\')*p = '/'; p++; }
		m_szRootPath = path;
#ifdef _WIN32
		m_szCachePath = (std::string)path+"/cache";
		m_szExternalPath = path;
#else
//		m_szCachePath = "cache";//(std::string)path+"/cache";
#endif
	}
	
	m_bdisposeEnvironment = false;
	// init the engine
	if (!JS_Init())
		return false;

	m_rt = JS_NewRuntime(32 * 1024L * 1024L);

	JS_SetGCParameter(m_rt, JSGC_MAX_BYTES, 0xffffffff);

	JS_SetTrustedPrincipals(m_rt, &shellTrustedPrincipals);
	JS_SetSecurityCallbacks(m_rt, &securityCallbacks);
	JS_SetNativeStackQuota(m_rt, JSB_MAX_STACK_QUOTA);
#ifdef _DEBUG
	JS_SetGCCallback(m_rt, &JSVM::OnGCCallBack,this);
#endif
	m_cx = JS_NewContext(m_rt, 8192);

	JS::RuntimeOptionsRef(m_rt).setIon(true);
	JS::RuntimeOptionsRef(m_rt).setBaseline(true);
	
	oldreportError=JS_SetErrorReporter(m_cx, reportError);
// #if defined(JS_GC_ZEAL) && defined(_DEBUG)
// 	// 更加频繁的触发GC
// 	JS_SetGCZeal(m_cx, 2, JS_DEFAULT_ZEAL_FREQ);
// #endif
	JSAutoRequest ar(m_cx);
	m_globalObj = sm_newGlobalObject(m_cx);

	JSAutoCompartment ac(m_cx, m_globalObj);
    
	std::list<CLASSFUNC>*s_InitList = CustomClassBase::GetInitList();
	for (std::list<CLASSFUNC>::iterator it = s_InitList->begin(); it != s_InitList->end(); ++it)
	{
		it->pInitClass(m_cx);
	}
	OnInitFinish();
	return true;
}

void JSVM::disposeEnvironment()
{
	m_bLoaded = false;
	m_bdisposeEnvironment = true;
	std::list<CLASSFUNC>*s_InitList = CustomClassBase::GetInitList();
	for (std::list<CLASSFUNC>::iterator it = s_InitList->begin(); it != s_InitList->end(); ++it)
	{
		it->pDisposeClass();
	}
	m_VisibleCanvas.clear();
    m_TouchStateList.clear();
	for (std::list<_EVENTQUEUE*>::iterator it = m_eventQueue.begin(); it != m_eventQueue.end(); ++it)
	{
		delete *it;
	}
	m_eventQueue.clear();
	
	for (std::list<RunOnUIThread*>::iterator it = m_RunOnUIThreadList.begin(); it != m_RunOnUIThreadList.end(); ++it)
	{
		delete *it;
	}
	m_RunOnUIThreadList.clear();
// 	for (std::list<ASYNCFUN*>::iterator it = m_asyncFunList.begin(); it != m_asyncFunList.end(); ++it)
// 	{
// 		delete *it;
// 	}
// 	m_asyncFunList.clear();

	if (m_cx)
	{

		JS_DestroyContext(m_cx);
		m_cx = nullptr;
	}

	if (m_rt)
	{
		JS_DestroyRuntime(m_rt);
		m_rt = nullptr;
		JS_ShutDown();
	}

	

}

bool JSVM::runScript(const char* path)
{
	JSVMAutoContext ac(this);


// 	OutputDebugStringA(path);
// 	OutputDebugStringA("\n");

	std::string &&szpath = GetFullPath(path);
	HttpRequestInterface*pReq = GetHttpRequestInterface();
	char*pCode=nullptr;
	int nLen=0;
	int httpret = pReq->SendGetRequest(szpath.c_str(),RST_TEXT, "", pCode, nLen);
	if (httpret != 200)return false;
	if (!pCode)return true;
	bool ret = RunScriptText(pCode, GetCacheFullPath(szpath.c_str()).c_str());
// 	bool isUtf8 = IsTextUtf8(pCode, nLen);
// 	bool ret;
// #ifdef _WIN32
// 	//转ANSI
// 	if (isUtf8)
// 	{
// 		
// 		std::string szcode=Utf8ToString(pCode);
// 		ret = RunScriptText(szcode.c_str(), GetCacheFullPath(szpath.c_str()).c_str());
// 	}
// 	else
// 	{
// 		ret = RunScriptText(pCode, GetCacheFullPath(szpath.c_str()).c_str());
// 	}
// #else
// 	if (isUtf8)
// 	{
// 		ret = RunScriptText(pCode, szpath.c_str());
// 	}
// 	else
// 	{
// 		ret = RunScriptText(pCode, szpath.c_str());
// 	}
// #endif
	delete[]pCode;
	return ret;











// 	JS::RootedScript script(m_cx, compileScript(path));
// 	if (!script)return false;
// 
// 	JS::RootedValue rval(m_cx);
// 	bool evaluatedOK = JS_ExecuteScript(m_cx, JS::RootedObject(m_cx,m_globalObj), script, &rval);
// 	if (false == evaluatedOK) {
// 		XBLOG("(evaluatedOK == JS_FALSE)");
// 		JS_ReportPendingException(m_cx);
// 		return false;
// 	}
	

	return true;
}

bool JSVM::evalString(const char* string)
{
//	JSAutoCompartment ac(m_cx, m_globalObj);
	bool bOK=JS_EvaluateScript(m_cx, JS::RootedObject(m_cx, m_globalObj), string, strlen(string), "JSVM::evalString", 1);
	return bOK;
}

JSScript* JSVM::compileScript(const char *path)
{
	if (!path) {
		return nullptr;
	}
	std::string szPath = GetFullPath(path);

//	JSAutoCompartment ac(m_cx, m_globalObj);

	JS::RootedScript script(m_cx);

	JS::CompileOptions op(m_cx);
	
	op.setUTF8(true);
	op.setFileAndLine(szPath.c_str(), 1);

	bool ok = false;

	ok = JS::Compile(m_cx, JS::RootedObject(m_cx, m_globalObj), op, szPath.c_str(), &script);
	if (!ok)return nullptr;
	return script;
}

void JSVM::OnInitFinish()
{

	//创建window.document对象

	LocalObject objdoc(GetContext(), HtmlDocument::CreateObject(GetContext()));
	LocalObject global(GetContext(), GetGlobal());

	global.SetProperty(GetContext(), "document", ValueBase(objdoc));
	objdoc.SetProperty("defaultView", global);

	LocalObject objlocation(GetContext(), HtmlLocation::CreateObject(GetContext()));
	global.SetProperty(GetContext(), "location",objlocation);
	objdoc.SetProperty(GetContext(), "location", objlocation);
	
	global.SetProperty(GetContext(), "navigator", HtmlNavigator::CreateObject(GetContext()));
//	global.SetProperty(GetContext(), "WebGLRenderingContext", HtmlWebGLRenderingContext::GetPrototype_s());
	global.SetProperty(GetContext(), "XMLHttpRequest", HtmlXMLHttpRequest::GetPrototype_s());
//	global.SetProperty(GetContext(), "AudioContext", HtmlAudio::GetPrototype_s());
	global.SetProperty(GetContext(), "DomParser", HtmlDomParser::GetPrototype_s());

	global.SetProperty(GetContext(), "pageXOffset", ValueBase().SetInt(0));
	global.SetProperty(GetContext(), "pageYOffset", ValueBase().SetInt(0));

	global.SetProperty(GetContext(), "screen", HtmlScreen::CreateObject(GetContext()));

// 	LocalObject objaudiocontext(GetContext(),HtmlAudioContext::CreateObject(GetContext()));
// 	global.SetProperty("AudioContext", objaudiocontext);





	RunScriptText("var Image=function(){return document.createElement(\"img\");};", nullptr);
//	RunScriptText("var CONSOLE=function(){this.log=function(str){};};var console=new CONSOLE();", nullptr);
	RunScriptText("var _PrintStack=function(){var stack = [],caller = arguments.callee.caller;var n=0;while (caller&&n++<10) {stack.push(caller);caller = caller && caller.caller;};alert(stack);};", nullptr);



	//////////////////////////////////////////////////////////////////////////
// 	LocalObject pCanvas(GetContext(),HtmlCanvas::CreateObject(GetContext()));
// 	CustomClassAutoThisObj cc(pCanvas.GetNativeObj(), pCanvas);
// 	((HtmlCanvas*)pCanvas.GetNativeObj())->Setid(GetContext(), "gameCanvas");
// 	CustomClassAutoThisObj cdoc(objdoc.GetNativeObj(), objdoc);
// 	((HtmlDocument*)objdoc.GetNativeObj())->AppendChild(GetContext(), pCanvas);
	LocalObject objls(GetContext(), HtmlLocalStorage::CreateObject(GetContext()));
	global.SetProperty("localStorage", objls);
	LocalObject objss(GetContext(), HtmlSessionStorage::CreateObject(GetContext()));
	global.SetProperty("sessionStorage", objss);
	LocalObject objconsole(GetContext(), HtmlConsole::CreateObject(GetContext()));
	global.SetProperty("console", objconsole);


	

	
	


}
// LONGLONG ltime1 = 0;
// LONGLONG ltime2 = 0;
// LONGLONG ltime3 = 0;
// LONGLONG ltime4 = 0;
// LONGLONG ltime5 = 0;
// LONGLONG ltime6 = 0;
// LONGLONG ltime7 = 7;
//LARGE_INTEGER freq;
void JSVM::ExecEvents()
{
	LocalObject global(GetContext(), GetGlobal());
	GlobalClass*pWindow = (GlobalClass*)CustomClassBase::FindCustomClass(global);
	CustomClassAutoThisObj cc(pWindow, global);
//	MeasureElements();

	ExecAsyncTask();

	//异步加载JS
	int nExec = 0;
	nExec+=ExecRunScript();

	nExec+=ExecEventAsync();

	nExec+=ExecAsyncFun();

	if (nExec==0&&!m_bLoaded)
	{
		m_bLoaded = true;
		OnLoad();
	}


	ExecEventQueue();

// 
// 	//定时器事件

	pWindow->ExecIntevals(GetContext());

	pWindow->ExecTimeout(GetContext());

	ExecAnimationFrame();

	
}

bool JSVM::executeFunction(const HandleContext&context,const ValueBase&vFunc, IFunctionCallJsArgs&args, LocalValue&vRet)
{
    if (vFunc.IsString())
	{
		std::string &&str = vFunc.GetString(context);
		return GetInstance()->RunScriptText(str.c_str(), "");
	}
	bool bRet = false;

	JSContext* cx = (JSContext*)context.Get();
	JS::RootedValue temp_retval(cx);

	JS::RootedObject obj(cx, (JSObject*)args.pObjThis->Get());


//	int argc = args.m_ParamList.size();
// 	JS::AutoValueVector ve(cx);
// 	for (int i = 0; i < argc; i++)
// 	{
// 		ve.append(*args.m_ParamList[i]->Get());
// 
// 	}
	JS::HandleValueArray vp(args.m_ParamList.m_jsvector);
	
	JS::RootedValue jsret(cx);
	JS::RootedValue vfun(cx, *vFunc.Get());

	bRet = JS_CallFunctionValue(cx, obj, vfun, vp, &jsret);
	vRet.Set(jsret);
	return bRet;
}

bool JSVM::executeFunction(const HandleContext&context, const std::string szName, IFunctionCallJsArgs&args, LocalValue&vRet)
{
	bool bRet = false;
	bool hasAction;
	JSContext* cx = (JSContext*)context.Get();
	JS::RootedValue temp_retval(cx);
	JS::RootedObject obj(cx, (JSObject*)args.pObjThis->Get());

	do
	{
//		JSAutoCompartment ac(cx, obj);

		if (JS_HasProperty(cx, obj, szName.c_str(), &hasAction) && hasAction) {
			if (!JS_GetProperty(cx, obj, szName.c_str(), &temp_retval)) {
				break;
			}
			if (temp_retval == JSVAL_VOID) {
				break;
			}
// 			int argc = args.m_ParamList.size();
// 
// 			JS::AutoValueVector ve(cx);
// 			for (int i = 0; i < argc; i++)
// 			{
// 				ve.append(*args.m_ParamList[i]->Get());
// 
// 			}
			JS::HandleValueArray vp(args.m_ParamList.m_jsvector);

			JS::RootedValue jsret(cx);
			bRet = JS_CallFunctionName(cx, obj, szName.c_str(), vp, &jsret);
			vRet.Set(jsret);

		}
	} while (0);
	assert(bRet);
	return bRet;
}

void JSVM::MainLoop()
{
	if (m_bdisposeEnvironment)return;
	JSVMAutoContext ac(this);
	
//	glViewport(OpenglContext::s_rcViewPort.x, OpenglContext::s_rcViewPort.y, OpenglContext::s_rcViewPort.width, OpenglContext::s_rcViewPort.height);

    ExecEvents();
    // audio play update
    AudioInterface* audioInt = GetAudioInterface();
    if (audioInt) {
        GetAudioInterface()->update();
    }
    
	JS_MaybeGC(m_cx);
}
// 
// int JSVM::GetVisibleCanvas(std::vector<HtmlCanvas*>&canvasList)
// {
// 
// 	JSAutoCompartment ac(m_cx, m_globalObj);
// 	LocalObject objDoc(GetContext(),GetGlobal().GetProperty(GetContext(), "document").GetObject());
// 	HtmlElement*pDoc = dynamic_cast<HtmlElement*>(CustomClassBase::FindCustomClass(objDoc));
// 	if (!pDoc)return 0;
// 	ObjectList objlist(GetContext());
// 	CustomClassAutoThisObj cc(pDoc, objDoc);
// 	int ret= pDoc->GetElementsByTagName(GetContext(), "canvas", objlist);
// 
// 	for (size_t i = 0; i < objlist.objList.size(); i++)
// 	{
// 		HtmlCanvas*pCanvas = dynamic_cast<HtmlCanvas*>(CustomClassBase::FindCustomClass(objlist.objList[i]));
// 		if (pCanvas)
// 		{
// 			if (pCanvas->m_glState.m_viewport.width>0||pCanvas->m_glState.m_viewport.height>0)
// 				canvasList.push_back(pCanvas);
// 		}
// 	}
// 	return canvasList.size();
// }

bool JSVM::RunScriptText(const char*szText, const char*szFilename)
{
	JSAutoCompartment ac(m_cx, m_globalObj);
// 	if (szFilename)
// 	{
// 		OutputDebugStringA(szFilename);
// 		OutputDebugStringA("\n");
// 	}
	//JS引擎一律使用UNICODE，否则JS里字符串长度是按1汉字2字符处理可能会乱码
	wstring16 wszText = StringToUnicode(szText);
	JS::RootedValue rval(m_cx);
	bool ret = JS_EvaluateUCScript(m_cx, JS::RootedObject(m_cx, m_globalObj), wszText.c_str(), wszText.length(), szFilename, 1, &rval);
	return ret;
}

void JSVM::executeFunctionAsync(const HandleContext& context,const HandleObject&objthis,const ValueBase& fun)
{
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__executeFunctionAsync"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__executeFunctionAsync", objlist);
	}
	assert(objlist.IsArray());
	int n = objlist.GetArrayLength();
	LocalObject item(context);
	item.CreateJsObject();
	item.SetProperty("objthis", objthis);
	item.SetProperty("func",fun);
	objlist.SetElement(n, item);

	
// 	ASYNCFUN*pfun = new ASYNCFUN;
// 	pfun->pFun = new PersistentValue(context, fun);
// 	m_asyncFunList.push_back(pfun);
}

int JSVM::ExecAsyncFun()
{
	int nRet = 0;
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__executeFunctionAsync"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__executeFunctionAsync", objlist);
	}
	assert(objlist.IsArray());
	int nCount = objlist.GetArrayLength();

	for (int i = 0; i < nCount; i++)
	{
		LocalObject item(GetContext(),objlist.GetElement(i).GetObject());
		if(item.IsNull())continue;
		LocalValue vfunc(GetContext(), item.GetProperty("func"));
		LocalObject objthis(GetContext(), item.GetProperty("objthis").GetObject());
		objlist.SetElement(i, ValueBase());
		IFunctionCallJsArgs args(GetContext());
		args.SetParams(&objthis, 0);
		LocalValue vret(GetContext());
		bool ret=executeFunction(GetContext(), vfunc, args, vret);
		nRet++;
		if (i == nCount - 1)
		{
			int nCount2 = objlist.GetArrayLength();
			if (nCount != nCount2)nCount = nCount2;
			else break;
		}
// 		if (i < nCount - 1)
// 			return nRet;//因一次全部执行完可能导致加载进度不动，因此每次mainloop只执行一个
// 		else
// 			break;
	}
	objlist.SetArrayLength(GetContext(), 0);

	return nRet;
	//异步调用
	// 	while (m_asyncFunList.size()>0)
	// 	{
	// 		ASYNCFUN*pfun = m_asyncFunList.front();
	// 		m_asyncFunList.pop_front();
	// 		IFunctionCallJsArgs args;
	// 		args.SetParams(&global, 0);
	// 		LocalValue vret(GetContext());
	// 		bool ret=executeFunction(GetContext(), *pfun->pFun, args, vret);
	// 		delete pfun;
	// 	}
}

void JSVM::OnLoad()
{
	JSVMAutoContext ac(this);
	LocalObject golb(GetContext(), GetGlobal());
	GlobalClass*pWindow = (GlobalClass*)CustomClassBase::FindCustomClass(golb);
	CustomClassAutoThisObj cc(pWindow, golb);
	pWindow->ExecEventListenerAsync(GetContext(), "load");
	LocalObject objdoc(GetContext(), golb.GetProperty("document").GetObject());
	HtmlDocument*pDoc = dynamic_cast<HtmlDocument*>(CustomClassBase::FindCustomClass(objdoc));
	CustomClassAutoThisObj cc2(pDoc, objdoc);
	pDoc->ExecEventListenerAsync(GetContext(), "load");
}

long JSVM::requestAnimationFrame(const ValueBase&vFun)
{
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__AnimationFrame"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__AnimationFrame", objlist);
	}
	assert(objlist.IsArray());
	

	int n = objlist.GetArrayLength();
	int iEmpty = -1;
	//查找重复
	for (int i = 0; i < n; i++)
	{
		LocalValue item(GetContext(), objlist.GetElement(i));
		if (item.IsUndefined())
		{
			if (iEmpty < 0)iEmpty = i;
			continue;
		}
		if (item == vFun)
		{
			return i + 1;
		}
	}


	if (iEmpty >= 0)
	{
		objlist.SetElement(iEmpty, vFun);
		return iEmpty + 1;
	}

	objlist.SetElement(n, vFun);
	return n + 1;

// 	for (size_t i = 0; i < m_aniframeList.size(); i++)
// 	{
// 		if (m_aniframeList[i] == 0)
// 		{
// 			m_aniframeList[i] = vFun.GetID(GetContext());
// 			return i + 1;
// 		}
// 	}
// 
// 	m_aniframeList.push_back(vFun.GetID(GetContext()));
// 	return m_aniframeList.size();
// 	return 0;
}

bool JSVM::cancelAnimationFrame(long id)
{
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__AnimationFrame"].GetObject());
	if (objlist.IsNull())
	{
		return true;
	}
	int n = objlist.GetArrayLength();
	if (id > n)return false;
	objlist.SetElement(id - 1, ValueBase());

// 	if (m_aniframeList.size() < id)return false;
// 	m_aniframeList[id - 1] = 0;
	return true;
}

void JSVM::RunScriptTextAsync(const char*szText, const char*szFilename, const HandleObject&jsobj)
{
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__runScriptAsync"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__runScriptAsync", objlist);
	}
	int n = objlist.GetArrayLength();
	LocalObject al(GetContext());
	al.CreateJsObject();
	al.SetProperty("filename", szFilename);
	al.SetProperty("thisobj", jsobj);
	al.SetProperty("jstext", LocalValue(GetContext()).SetString(GetContext(), szText));
	objlist.SetElement(n, al);
}

void JSVM::runScriptAsync(const char * szFilename,const HandleObject&jsobj)
{
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__runScriptAsync"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__runScriptAsync", objlist);
	}
	int n = objlist.GetArrayLength();
	LocalObject al(GetContext());
	al.CreateJsObject();
	al.SetProperty("filename", szFilename);
	al.SetProperty("thisobj", jsobj);
	objlist.SetElement(n, al);

// 	ASYNCLOADJS al;
// 	al.jsFile = szFilename;
// 	al.pScriptObj = new PersistentObject(GetContext(), jsobj);
// 	m_asyncScriptList.push_back(al);
}


int JSVM::ExecRunScript()
{
	int nRet = 0;
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__runScriptAsync"].GetObject());
	if (objlist.IsNull())
	{
		return 0;
	}
	assert(objlist.IsArray());
	int nCount = objlist.GetArrayLength();
	int i = 0;
	bool bOK;
	for (; i < nCount;i++)//全部执行完再一次性清除
	{
		LocalObject al(GetContext(),objlist.GetElement(i).GetObject());

		std::string &&filename = al["filename"].GetString(GetContext());
		LocalObject thisobj(GetContext(), al["thisobj"].GetObject());
		
		LocalValue vjstext(GetContext(), al["jstext"]);
		if (vjstext.IsUndefined())
		{
			bOK = runScript(filename.c_str());
		}
		else
		{
			std::string &&jstext = vjstext.GetString(GetContext());
			bOK=RunScriptText(jstext.c_str(), jstext.c_str());
		}
		HtmlScript*pScript = dynamic_cast<HtmlScript*>(CustomClassBase::FindCustomClass(thisobj));
		if (pScript)
		{
			CustomClassAutoThisObj cc(pScript, thisobj);
			if (bOK)
				pScript->ExecEventListener(GetContext(), "load");
			else
				pScript->ExecEventListener(GetContext(), "error");
		}
		nRet++;
		if (i == nCount - 1)
		{
			int nCount2 = objlist.GetArrayLength();
			if (nCount == nCount2)break;
			else nCount = nCount2;
		}
	

	}
	objlist.SetArrayLength(GetContext(), 0);
	return nRet;

	// 	while (m_asyncScriptList.size() > 0)
	// 	{
	// 
	// 		ASYNCLOADJS al=m_asyncScriptList.front();
	// 		m_asyncScriptList.pop_front();
	// 		bool bOK=runScript(al.jsFile.c_str());
	// 
	// 		HtmlScript*pScript = dynamic_cast<HtmlScript*>(CustomClassBase::FindCustomClass(*al.pScriptObj));
	// 		if (pScript)
	// 		{
	// 			
	// 			if (bOK)
	// 				pScript->ExecEventListener(GetContext(), "load");
	// 			else
	// 				pScript->ExecEventListener(GetContext(), "error");
	// 		}
	// 		delete al.pScriptObj;
	// 	}
}


void JSVM::AddValueToGlobal( JS::HandleValue val)
{

	JS::RootedValue vlist(m_cx);
	JS::RootedObject objlist(m_cx);
	JS::RootedObject global(m_cx, m_globalObj);
	bool bOK = JS_GetProperty(m_cx, global, "__PersistentObjects", &vlist);
	if (vlist.isUndefined())
	{
		objlist = JS_NewArrayObject(m_cx, 0);

		bOK = JS_SetProperty(m_cx, global, "__PersistentObjects", JS::RootedValue(m_cx, OBJECT_TO_JSVAL(objlist)));
	}
	else objlist = vlist.toObjectOrNull();


	uint32_t len = 0;
	bOK = JS_GetArrayLength(m_cx, objlist, &len);
	JS::RootedValue item(m_cx);
	for (uint32_t i = 0; i < len; i++)
	{
		bOK = JS_GetElement(m_cx, objlist, i, &item);
		if (!bOK)continue;
		if (item.isUndefined())
		{
			bOK = JS_SetElement(m_cx, objlist, len, val);
			return;
		}
	}
	bOK = JS_SetElement(m_cx, objlist, len, val);

}
void JSVM::RemoveValueFromGlobal( JS::HandleValue val)
{
	JS::RootedValue vlist(m_cx);
	JS::RootedObject objlist(m_cx);
	JS::RootedObject global(m_cx, m_globalObj);
	bool bOK = JS_GetProperty(m_cx, global, "__PersistentObjects", &vlist);
	if (vlist.isUndefined())
	{
		return;
	}
	else objlist = vlist.toObjectOrNull();


	uint32_t len = 0;
	bOK = JS_GetArrayLength(m_cx, objlist, &len);
	JS::RootedValue item(m_cx);
	for (uint32_t i = 0; i < len; i++)
	{
		bOK = JS_GetElement(m_cx, objlist, i, &item);
		if (!bOK)continue;
		if (item.isUndefined())continue;
		if (item == val)
		{
			JS_SetElement(m_cx, objlist, i, JS::RootedValue(m_cx, JS::UndefinedValue()));
			return;
		}
	}
}

int JSVM::ExecEventAsync()
{
	int nRet = 0;
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__ExecEventAsync"].GetObject());
	if (objlist.IsNull())
	{
		return 0;
	}
	assert(objlist.IsArray());
	int nCount = objlist.GetArrayLength();
	int i = 0;
	for (; i < nCount; i++)//全部执行完再一次性清除
	{
		LocalObject obitem(GetContext(), objlist.GetElement(i).GetObject());
		if (obitem.IsNull())continue;
		objlist.SetElement(i, ValueBase());
		LocalObject obj(GetContext(),obitem.GetProperty("thisobj").GetObject());
		std::string &&szEventName = obitem.GetProperty("name").GetString(GetContext());
		HtmlEventTarget*pEvent = dynamic_cast<HtmlEventTarget*>(CustomClassBase::FindCustomClass(obj));
		CustomClassAutoThisObj cc(pEvent, obj);
		pEvent->ExecEventListener(GetContext(), szEventName.c_str());
		nRet++;
		if (i == nCount - 1)
		{
			int nCount2 = objlist.GetArrayLength();
			if (nCount2>nCount)
				nCount = nCount2;
			else break;
		}
		if (i < nCount - 1)
			return nRet;//因一次全部执行完可能导致加载进度不动，因此每次mainloop只执行一个
		else
			break;
	}
	objlist.SetArrayLength(0);
	return nRet;

	//异步事件
// 	while (HtmlEventTarget::s_eventWillExec.size() > 0)
// 	{
// 		HtmlEventTarget::EVENT_WILL_EXEC ev = HtmlEventTarget::s_eventWillExec.front();
// 		HtmlEventTarget::s_eventWillExec.pop_front();
// 		ev.pHtmlObj->ExecEventListener(GetContext(), ev.szEventName.c_str());
// 
// 	}
}

void JSVM::ExecAnimationFrame()
{
	LocalObject glob(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), glob["__AnimationFrame"].GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		glob.SetProperty("__AnimationFrame", objlist);
	}
	assert(objlist.IsArray());
	int n = objlist.GetArrayLength();
	for (int i = 0; i < n; i++)
	{
		LocalValue item(GetContext(), objlist.GetElement(i));
		if (item.IsUndefined())continue;
		//requestAnimationFrame执行一次后会自动cancel
		objlist.SetElement(i, ValueBase());

		uint32_t t = GetTime();

		IFunctionCallJsArgs args(GetContext());
		ValueBase para;
		para.SetInt(t);
		args.SetParams(&glob,1,&para);
		LocalValue vret(GetContext());
		bool ret = executeFunction(GetContext(), item, args, vret);
		if (i == n - 1)
		{
			int n2 = objlist.GetArrayLength();
			if (n2 == n)break;
			n = n2;
		}
	}

//帧动画
// 	LocalObject global(GetContext(), GetGlobal());
// 	LocalValue vfun(GetContext());
// 	for (size_t i = 0; i < m_aniframeList.size(); i++)
// 	{
// 		if (m_aniframeList[i]!=0)
// 		{
// 			bool ok=vfun.GetIDValue(m_aniframeList[i]);
// 			assert(ok);
// 			if (!ok)continue;
// 			IFunctionCallJsArgs args;
// 			args.SetParams(&global, 0);
// 			LocalValue vret(GetContext());
// 			bool ret = executeFunction(GetContext(), vfun, args, vret);
// 		}
// 	}
}

AudioInterface* JSVM::CreateAudioInterface()
{
	return GetAudioInterface()->Create();
}

void JSVM::OnTouchStart(long touchID,int x, int y, int screenX, int screenY, bool bAlt, bool bShift, bool bCtrl)
{
	m_balt = bAlt;
	m_bshift = bShift;
	m_bctrl = bCtrl;
	
	assert(m_TouchStateList.size() <= MAX_TOUCHES);
	_TOUCHSTATE ts;
	ts.bEnable = true;
	ts.bStateChange = true;
	ts.clientx = x;
	ts.clienty = y;
	ts.screenx = screenX;
	ts.screeny = screenY;
	m_TouchStateList[touchID] = ts;
}

void JSVM::OnTouchMove(long touchID, int x, int y, int screenX, int screenY, bool bAlt, bool bShift, bool bCtrl)
{
	m_balt = bAlt;
	m_bshift = bShift;
	m_bctrl = bCtrl;

	_TOUCHSTATE&ts = m_TouchStateList[touchID];
	if (!ts.bEnable)
	{
        printf("\njsvm: touch move, touch(%ld) is DISabled!", touchID);
		return;
	}

	ts.bStateChange = true;
	ts.clientx = x;
	ts.clienty = y;
	ts.screenx = screenX;
	ts.screeny = screenY;
}

void JSVM::OnTouchEnd(long touchID, int x, int y, int screenX, int screenY, bool bAlt, bool bShift, bool bCtrl)
{
    m_balt = bAlt;
	m_bshift = bShift;
	m_bctrl = bCtrl;
	_TOUCHSTATE&ts = m_TouchStateList[touchID];
	if (!ts.bEnable)
	{
		return;
	}
	ts.bEnable = false;
	ts.bStateChange = true;
	ts.clientx = x;
	ts.clienty = y;
	ts.screenx = screenX;
	ts.screeny = screenY;
    m_touchIDtoErase = touchID;
}

void JSVM::SendOnTouchEvent(_TouchEvent::TOUCHTYPE touchtype)
{
	_TouchEvent* tevent = new _TouchEvent;
	tevent->altKey = m_balt;
	tevent->ctrlKey = m_bctrl;
	tevent->shiftKey = m_bshift;
	tevent->nTouchType = touchtype;
	bool bSend = false;
	//for (int i = 0; i < MAX_TOUCHES; i++)
    for (auto itor = m_TouchStateList.begin(); itor != m_TouchStateList.end(); ++itor)
    {
		_TOUCHSTATE&ts = itor->second;
		if (ts.bEnable)
		{
			_Touch touch;
			touch.clientX = ts.clientx;
			touch.clientY = ts.clienty;
			touch.pageX = ts.clientx;
			touch.pageY = ts.clienty;
			touch.screenX = ts.screenx;
			touch.screenY = ts.screeny;
			touch.identifier = itor->first;
			tevent->touches.push_back(touch);
			bSend = true;
		}
		if (ts.bStateChange)
		{
			_Touch touch;
			touch.clientX = ts.clientx;
			touch.clientY = ts.clienty;
			touch.pageX = ts.clientx;
			touch.pageY = ts.clienty;
			touch.screenX = ts.screenx;
			touch.screenY = ts.screeny;
			touch.identifier = itor->first;
			tevent->changedTouches.push_back(touch);
			ts.bStateChange = false;
			bSend = true;
		}
	}
    
    if (touchtype == _TouchEvent::TT_END) {
        m_TouchStateList.erase(m_touchIDtoErase);
        m_touchIDtoErase = 0;
    }
    
	if (!bSend)
	{
		delete tevent;
		return;
	}
	m_eventQueue.push_back(tevent);
}

void JSVM::ExecEventQueue()
{
	while (m_eventQueue.size() > 0)
	{
		_EVENTQUEUE*pq = m_eventQueue.front();
		m_eventQueue.pop_front();
		switch (pq->eventType)
		{
		case ET_TOUCH:
		{
			_TouchEvent*pevent = dynamic_cast<_TouchEvent*>(pq);
			//目前只对第一个注册了touch响应触控事件
			LocalObject objlist(GetContext(), GetGlobal().GetProperty(GetContext(),"__TouchEventObject").GetObject());
			if (objlist.IsNull())break;
			if (!objlist.IsArray())break;

			int nCount = objlist.GetArrayLength();
			
			for (int i = 0; i < nCount; i++)
			{
				LocalObject objitem(GetContext(), objlist.GetElement(i).GetObject());
				
				if (objitem.IsNull())continue;
				HtmlEventTarget*pItem = dynamic_cast<HtmlEventTarget*>(CustomClassBase::FindCustomClass(objitem));
				assert(pItem);
				CustomClassAutoThisObj cc2(pItem, objitem);
				pItem->ExecTouchEvent(GetContext(), pevent);
				
				
			}


// 			LocalObject objDoc(GetContext(), GetGlobal().GetProperty(GetContext(), "document").GetObject());
// 			HtmlElement*pDoc = dynamic_cast<HtmlElement*>(CustomClassBase::FindCustomClass(objDoc));
// 			if (!pDoc)break;
// 			ObjectList objlist(GetContext());
// 			CustomClassAutoThisObj cc(pDoc, objDoc);
// 			int ret = pDoc->GetElementsByTagName(GetContext(), "canvas", objlist);
// 
// 			for (size_t i = 0; i < objlist.objList.size(); i++)
// 			{
// 				LocalObject obj(GetContext(), objlist.objList[i]);
// 				HtmlCanvas*pCanvas = dynamic_cast<HtmlCanvas*>(CustomClassBase::FindCustomClass(obj));
// 				if (pCanvas)
// 				{
// 					if (pCanvas->m_glState.m_viewport.width > 0 || pCanvas->m_glState.m_viewport.height > 0)
// 					{
// 						CustomClassAutoThisObj cc2(pCanvas, obj);
// 						pCanvas->ExecTouchEvent(GetContext(),pevent);
// 
// 						break;
// 					}
// 				}
// 			}
			break;
		}
		default:
			assert(0);
			break;
		}
		delete pq;
	}
}

void JSVM::SetRootPath(const std::string&path)
{
	m_szRootPath = path;
	JSVMAutoContext ac(this);
	LocalObject localStorage(GetContext(), GetGlobal().GetProperty(GetContext(),"localStorage").GetObject());
	HtmlLocalStorage *pls = (HtmlLocalStorage*)CustomClassBase::FindCustomClass(localStorage);
	CustomClassAutoThisObj cc(pls, localStorage);
	pls->LoadFromFile();
}

void JSVM::CallGC()
{
	JS_GC(m_rt);
}
// void outlogfile(const char*str)
// {
// 	FILE*f = fopen("xb.log", "a");
// 	fseek(f, 0, SEEK_END);
// 	fprintf(f, "%s\n", str);
// 	fclose(f);
// }
bool JSVM::LoadHtml(LocalObject&objParent, GumboNode*pNode,bool bFromInnerText)//bFromInnerText:是否是从innerText加载，TRUE则将GUMBO_TAG_HTML、GUMBO_TAG_BODY设为传入的objParent
{
	assert(!objParent.IsNull());
	GumboVector *children;
	int i;
	
	if (pNode->type == GUMBO_NODE_TEXT)
	{
		objParent.SetProperty("innerText", Utf8ToString(pNode->v.text.text).c_str());
		return true;
	}

	/* 如果当前节点不是一个元素的话直接返回 */
	if (pNode->type != GUMBO_NODE_ELEMENT) return false;

	bool bNeedAppendChild = true;
	LocalObject curObj(GetContext());
	//加载到objParent
	switch (pNode->v.element.tag)
	{
	case GUMBO_TAG_HTML:
	{
		bNeedAppendChild = false;
		curObj = objParent;
		break;
	}
	case GUMBO_TAG_BODY://document
	case GUMBO_TAG_HEAD://head可能用script，也作为document处理
	{
		bNeedAppendChild = false;
		if (bFromInnerText)
		{
			curObj = objParent;
		}
		else
		{
			curObj = GetGlobal().GetProperty(GetContext(), "document").GetObject();
		}
		break;
	}
	case GUMBO_TAG_DIV://DIV
	{
		curObj = HtmlDiv::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_SCRIPT://SCRIPT
	{
		curObj = HtmlScript::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_IMG:
	{
		curObj = HtmlImgClass::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_CANVAS:
	{
		curObj = HtmlCanvas::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_AUDIO:
	{
		curObj = HtmlAudio::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_INPUT:
	{
		curObj = HtmlInput::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_TEXTAREA://text
	{
		assert(0);
		break;
	}
	case GUMBO_TAG_SPAN:
	{
		curObj = HtmlSpan::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_P:
	{
		curObj = HtmlP::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_STYLE:
	{
		curObj = HtmlStyleElement::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_LINK:
	{
		
		curObj = HtmlLink::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_LI:
	{
		curObj = HtmlLI::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_UL:
	{
		curObj = HtmlUL::CreateObject(GetContext());
		break;
	}
	case GUMBO_TAG_META:
	case GUMBO_TAG_TITLE:
	case GUMBO_TAG_BASE:
	case GUMBO_TAG_BR:
		return false;


	default:
		return false;
	}
	//加载属性
	if (!curObj.IsNull())
	{
		if (bNeedAppendChild)
		{
			HtmlElement*pParent = dynamic_cast<HtmlElement*>(CustomClassBase::FindCustomClass(objParent));
			CustomClassAutoThisObj cc(pParent, objParent);
			pParent->AppendChild(GetContext(), curObj);
			HtmlElement*pEle = dynamic_cast<HtmlElement*>(CustomClassBase::FindCustomClass(curObj));
// 			pEle->m_width = -1;//从HTML加载的元素默认跟父元素对齐
// 			pEle->m_height = -1;
		}
		HtmlElement*pEle = dynamic_cast<HtmlElement*>(CustomClassBase::FindCustomClass(curObj));
		CustomClassAutoThisObj cc(pEle, curObj);
		for (int i = 0; i < pNode->v.element.attributes.length; i++)
		{
			
			GumboAttribute*pAttr = (GumboAttribute*)pNode->v.element.attributes.data[i];
			std::string &&szvalue = Utf8ToString(pAttr->value);
			if (!_stricmp(pAttr->name,"style"))
			{
				pEle->SetStyle(GetContext(),pAttr->value);
			}
			else
			{
				curObj.SetProperty(pAttr->name, pAttr->value,true);
				if (!_stricmp(pAttr->name, "id"))//自动添加名称为id的全局变量
				{
					GetGlobal().SetProperty(GetContext(), szvalue.c_str(), curObj);
				}
			}
			//同时加到DOM的attribute里
			pEle->SetAttribute(pAttr->name, szvalue.c_str());
		}
	}

	


	/* 获取该节点的所有子元素节点 */

	children = &pNode->v.element.children;

	/* 递归该节点下的所有子节点 */
	for (i = 0; i < children->length; ++i)
	{
		
		GumboNode*pchild = (GumboNode*)children->data[i];
		
		LoadHtml(curObj, pchild,bFromInnerText);
	}
	return true;
}

bool JSVM::LoadHtml(std::string path)
{
	char*pp = (char*)path.c_str();
	while (*pp)
	{
		if (*pp == '\\')*pp = '/';
		pp++;
	}

	m_szUrl = path;
	JSVMAutoContext ac(this);

	HttpRequestInterface*pReq = GetHttpRequestInterface();
	char*pRet = 0;
	int nLen = 0;
	int ret = pReq->SendGetRequest(path.c_str(), RST_TEXT, "", pRet, nLen);
	if (ret != 200)
	{
        char debugStr[500];
        sprintf(debugStr, "load html failed, ret:%d, file:%s", ret, path.c_str());
        OutputDebugStringA(debugStr);
        //assert(0);
		return false;
	}



	char*p = strdup(path.c_str());
	char*q = strrchr(p, '/');
	if (q)
	{
		*q = 0;
		SetRootPath(p);
	}
	else
	{

		char pa[MAX_PATH];
		getcwd(pa, sizeof(pa));
		SetRootPath(pa);

	}
	free(p);

	if ((uint8_t)pRet[0] == 0xef && (uint8_t)pRet[1] == 0xbb && (uint8_t)pRet[2] == 0xbf)
	{
		p = pRet + 3;
	}
	else
		p = pRet;
#ifdef _WIN32
	GumboOutput* output = gumbo_parse(AnsiToUtf8(p).c_str());
#else
	GumboOutput* output = gumbo_parse(p);
#endif


	

	LocalObject global(GetContext(), GetGlobal());
	LoadHtml(global, output->root,false);






	gumbo_destroy_output(&kGumboDefaultOptions, output);
	delete[]pRet;
	
	return true;
}

void JSVM::OnSize(int width, int height)
{
	if (!m_cx || !m_globalObj)return;
	JSVMAutoContext ac(this);
	LocalObject global(GetContext(), GetGlobal());
	GlobalClass*pWindow = dynamic_cast<GlobalClass*>(CustomClassBase::FindCustomClass(global));
	CustomClassAutoThisObj cc(pWindow, global);
	pWindow->ExecEventListener(GetContext(), "resize");
}

std::string JSVM::GetMD5(const char * str)
{
	MD5DATA md5;
	MD5(str, strlen(str), md5);
	return md5.GetString();
}

std::string& JSVM::GetRootPath()
{
	return m_szRootPath;
}

std::string JSVM::GetFullPath(const std::string &szFile) //返回完整路径
{
	if (szFile.empty())return szFile;
	int i = szFile.find(":");
	if (i >= 0)return szFile;
	if (szFile.find("//") == 0)//有些URL类似//hm.baidu.com/hm.js?b48a4a0959fb8dcf406a3f480a52db53，自动加上http:
	{
		return std::string("http:") + szFile;
	}
	if (szFile.find("/") == 0)return szFile;
	std::string ret= m_szRootPath + "/" + szFile;
	i = ret.rfind('?');
	if (i>=0)
	{
		ret.resize(i);
	}
	return ret;
}

std::string JSVM::GetLocalStoragePath()
{
	if (m_szUrl.empty())return"";
	std::string api;
	int port;
	std::string&&host=GetHostName(m_szUrl.c_str(), api, port);
	std::string szFilename = host + ".dat";
	return m_szExternalPath+"/"+m_szLocalStoragePath + "/" + szFilename;
}
std::string JSVM::GetCookiePath()
{
	if (m_szUrl.empty())return"";
	std::string api;
	int port;
	std::string&&host = GetHostName(m_szUrl.c_str(), api, port);
	std::string szFilename = host + ".cookie";
	return m_szExternalPath + "/" + m_szLocalStoragePath + "/" + szFilename;
}

void JSVM::AddVisibleCanvas(HtmlCanvas*pCanvas)
{
	m_VisibleCanvas[pCanvas] = pCanvas;
}

void JSVM::RemoveVisibleCanvas(HtmlCanvas*pCanvas)
{
	m_VisibleCanvas.erase(pCanvas);
}


std::string JSVM::ByteArrayToString(const char*szText)//任意字符数组转系统匹配的字符串
{
	if ((uint8_t)szText[0] == 0xff && (uint8_t)szText[1] == 0xfe)//UNICODE
	{
		return UnicodeToString((jschar*)(szText + 2));
	}
	
	bool bUtf8 = false;
	if ((uint8_t)szText[0] == 0xef && (uint8_t)szText[1] == 0xbb && (uint8_t)szText[2] == 0xbf)
	{
		szText += 3;
		bUtf8 = true;
	}
	else
	{
		int len = strlen(szText);
		if (IsTextUtf8(szText, len))
		{
			bUtf8 = true;
		}
	}
	if (bUtf8)
	{
#ifdef _WIN32
		return Utf8ToString(szText);
#else
		return szText;
#endif
	}
	else //ASCII
	{
#ifdef _WIN32
		return szText;
#else
		return AnsiToUtf8(szText);
#endif
	}
}


std::string JSVM::AnsiToUtf8(const char*str)
{
#ifndef _WIN32
	int alen = strlen(str);
	int clen = alen * 6 + 3;
	std::string ret;
	ret.resize(clen);
	int len = code_convert("GBK", "UTF-8", (char*)str, alen, (char*)ret.data(), clen);
	ret.resize(len);
	return ret;
#else
	int clen = strlen(str);
	int wlen=MultiByteToWideChar(0, 0, str, clen, 0, 0);
	WCHAR*wbuf = new WCHAR[wlen + 1];
	MultiByteToWideChar(0, 0, str, clen, wbuf, wlen);
	wbuf[wlen] = 0;
	int nlen = WideCharToMultiByte(CP_UTF8, 0, wbuf, wlen, 0, 0, 0, 0);
	std::string ret;
	ret.resize(nlen);
	WideCharToMultiByte(CP_UTF8, 0, wbuf, wlen, (char*)ret.data(), nlen, 0, 0);
	delete[]wbuf;
	return ret;
#endif
}
wstring16 JSVM::StringToUnicode(const char* szText)
{
#ifdef _WIN32
	int clen = strlen(szText);
	int wlen = MultiByteToWideChar(0, 0, szText, clen, 0, 0);
	wstring16 ret;
	ret.resize(wlen);
	MultiByteToWideChar(0, 0, szText, clen, (WCHAR*)ret.data(), wlen);
	return ret;
#else
	int alen = strlen(szText);
	int wlen = alen + 1;
	wstring16 ret;
	ret.resize(wlen);
	int len = code_convert("UTF-8", "UTF-16LE", (char*)szText, alen, (char*)ret.data(), wlen * 2);
	ret.resize(len/2);
	return ret;
#endif
}
std::string JSVM::UnicodeToString(const jschar* str)
{
#ifdef _WIN32
	int wlen = wcslen(str);
	int nLen = WideCharToMultiByte(0, 0, str, wlen, 0, 0, 0, 0);
	std::string ret;
	ret.resize(nLen);
	WideCharToMultiByte(0, 0, str, wlen, (char*)ret.data(), nLen, 0, 0);
	return ret;
#else
	int wlen = jscharlen(str);
	int clen = wlen * 6 + 3;
	std::string ret;
	ret.resize(clen);
	int len = code_convert("UTF-16LE", "UTF-8", (char*)str, wlen*2, (char*)ret.data(), clen);
	ret.resize(len);
	return ret;
#endif
}

std::string JSVM::Utf8ToString(const char* szText)//UTF8转系统匹配的字符串
{
	
#ifdef _WIN32
		//UTF8转ASCII
	int clen = strlen(szText);
	int wlen = MultiByteToWideChar(CP_UTF8, 0, szText, clen, 0, 0);
	WCHAR *pwstr = new WCHAR[wlen + 1];
	MultiByteToWideChar(CP_UTF8, 0, szText, clen, pwstr, wlen);
	pwstr[wlen] = 0;
	clen = WideCharToMultiByte(0, 0, pwstr, wlen, 0, 0, 0, 0);
	std::string ret;
	ret.resize(clen);
	WideCharToMultiByte(0, 0, pwstr, wlen, (char*)ret.data(), clen, 0, 0);
	delete[]pwstr;
	return ret;
#else
		return szText;
#endif

	
}

void JSVM::AddTouchEventObject(const HandleObject&obj)
{
	if (m_bdisposeEnvironment)return;
	LocalObject global(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), global.GetProperty("__TouchEventObject").GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		global.SetProperty("__TouchEventObject", objlist);
	}
	int nCount = objlist.GetArrayLength();
	int iEmpty = -1;
	for (int i = 0; i < nCount; i++)
	{
		LocalObject objitem(GetContext(), objlist.GetElement(i).GetObject());
		if (objitem.IsNull())
		{
			if (iEmpty<0)iEmpty = i;
			continue;
		}
		if (objitem == obj)return;
	}
	if (iEmpty >= 0)
	{
		objlist.SetElement(iEmpty, obj);
	}
	else
	{
		objlist.SetElement(nCount, obj);
	}
}

void JSVM::RemoveTouchEventObject(const HandleObject&obj)
{
	if (m_bdisposeEnvironment)return;
	LocalObject global(GetContext(), GetGlobal());
	LocalObject objlist(GetContext(), global.GetProperty("__TouchEventObject").GetObject());
	if (objlist.IsNull())
	{
		return;
	}
	int nCount = objlist.GetArrayLength();
	for (int i = 0; i < nCount; i++)
	{
		LocalObject objitem(GetContext(), objlist.GetElement(i).GetObject());
		if (objitem == obj)
		{
			objlist.SetElement(i, ValueBase());
			break;
		}
	}
	
}

bool JSVM::IsTextUtf8(const char* str, int length)
{
	
		int i;
		uint32_t nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
		uint8_t chr;
		bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
		for (i = 0; i < length; i++)
		{
			chr = *(str + i);
			if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
				bAllAscii = false;
			if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
			{
				if (chr >= 0x80)
				{
					if (chr >= 0xFC && chr <= 0xFD)
						nBytes = 6;
					else if (chr >= 0xF8)
						nBytes = 5;
					else if (chr >= 0xF0)
						nBytes = 4;
					else if (chr >= 0xE0)
						nBytes = 3;
					else if (chr >= 0xC0)
						nBytes = 2;
					else
					{
						return false;
					}
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx
			{
				if ((chr & 0xC0) != 0x80)
				{
					return false;
				}
				nBytes--;
			}
		}
		if (nBytes > 0) //违返规则
		{
			return false;
		}
		if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
		{
			return false;
		}
		return true;

}

bool JSVM::LoadHtmlFromInnerHtml(LocalObject & objParent, const std::string& szhtml)
{
	GumboOutput* output = gumbo_parse(szhtml.c_str());
	if (!output)return false;
	bool ret=LoadHtml(objParent, output->root,true);
	gumbo_destroy_output(&kGumboDefaultOptions, output);
	return ret;
}

GLuint JSVM::s_framebuffer = 0;
GLuint JSVM::s_colorRenderbuffer = 0;
GLuint JSVM::s_depthRenderbuffer = 0;

void JSVM::initGlobalFBO(int width, int height)
{
#if defined(_IOS)
    glGenFramebuffers(1, &s_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, s_framebuffer);
    
    // color renderbuffer to displayed on CAEGLLayer
    glGenRenderbuffers(1, &s_colorRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, s_colorRenderbuffer);
    JSVM::GetInstance()->GetOpenglInterface()->allocatRenderbuffer(GL_RENDERBUFFER, GL_RGBA, 0, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, s_colorRenderbuffer);
    
    // get width and height set by CAEAGLLayer
    GLint fboWidth;
    GLint fboHeight;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &fboWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &fboHeight);
    
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    // depth buffer
    glGenRenderbuffers(1, &s_depthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, s_depthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, fboWidth, fboHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, s_depthRenderbuffer);
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    assert(status == GL_FRAMEBUFFER_COMPLETE);
    
    GLCHECKERR;
#endif
}

void JSVM::RenderCanvasFBO()
{
#if defined(_IOS)
	glBindFramebuffer(GL_FRAMEBUFFER, s_framebuffer);
#else
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
    
	HtmlCanvas::s_prevCanvas = nullptr;
	glDisable(GL_BLEND);
	glClearColor(0,0,0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (std::unordered_map<HtmlCanvas*, HtmlCanvas*>::iterator it = m_VisibleCanvas.begin(); it != m_VisibleCanvas.end(); ++it)
	{
		HtmlCanvas*pcanvas = it->second;
		if (pcanvas->m_texBackground != 0)//先画背景
		{
			pcanvas->DrawBackGround();



		}
		pcanvas->DrawFBO();
		
	}
	glEnable(GL_BLEND);
    
#if defined(_IOS)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}



void JSVM::ExecAsyncTask()
{

	while (true)
	{
		pthread_mutex_lock(&m_lock);
		int n = m_RunOnUIThreadList.size();
		if (n == 0)
		{
			pthread_mutex_unlock(&m_lock);
			break;
		}
		RunOnUIThread*ptask = m_RunOnUIThreadList.front();
		m_RunOnUIThreadList.pop_front();
		pthread_mutex_unlock(&m_lock);
		ptask->OnPostExecute();
	}
}

void JSVM::MeasureElements()
{
	LocalObject objdoc(GetContext(), GetGlobal().GetProperty(GetContext(),"document").GetObject());
	HtmlDocument*pDoc = dynamic_cast<HtmlDocument*>(CustomClassBase::FindCustomClass(objdoc));
	pDoc->Measure(GetContext());
}

std::string JSVM::GetHostName(const char* szURL, std::string&api, int&port)
{
	std::string ret;
	port = 80;
	char*p = strstr((char*)szURL, "//");
	if (p)
	{
		p += 2;
		char*q = strchr(p + 1, '/');
		if (q)api = q;
		else api = "/";
		char*o = p;
		while (*o)
		{
			if (*o == ':')
				break;
	
			if (*o == '/')
			{
				o = 0;
				break;
			}
			o++;

		}

		if (o)
		{
			if (q)
			{
				char tmp[20];
				memcpy(tmp, o + 1, q - o);
				tmp[q - o] = 0;
				port = atoi(tmp);


			}
			else
			{
				port = atoi(o + 1);

			}
			ret.resize(o - p);
			memcpy((char*)ret.data(), p, o - p);
		}
		else
		{
			ret.resize(q - p);
			memcpy((char*)ret.data(), p, q - p);
		}

		return ret;
	}
	else
	{
		api = szURL;
		return "";
	}
}
void JSVM::StringReplace(std::string &strBase, std::string strSrc, std::string strDes)
{
	std::string::size_type pos = 0;
	std::string::size_type srcLen = strSrc.size();
	std::string::size_type desLen = strDes.size();
	pos = strBase.find(strSrc, pos);
	while ((pos != std::string::npos))
	{
		strBase.replace(pos, srcLen, strDes);
		pos = strBase.find(strSrc, (pos + desLen));
	}
}
std::string JSVM::GetCacheFullPath(const std::string&szUrl)//返回缓存的文件路径
{
	char*p = strstr((char*)szUrl.c_str(), "http://");
	if (p)
	{
		p += 7;
		std::string ret;
		char*i = strrchr(p, '?');
		if (i)
		{
			ret.resize(i - p);
			strncpy((char*)ret.data(), p, i - p);
		}
		else
		{
			ret =p;
		}
		StringReplace(ret, ":", "_");
#ifdef _WIN32
		ret = m_szCachePath + "/" + ret.c_str();
#else
		ret = m_szCachePath + "/" + GetMD5(ret.c_str());
#endif
		if (ret.substr(ret.length() - 1) == "/")
			return ret + "index.html";
		else return ret;
		
	}
	else//非HTTP路径，是本地路径，不需要缓存
	{
		return szUrl;
	}
}










JSVM JSVM::_instance;




std::map<std::string, ObjectProperties::PROPERTY>& ObjectProperties::GetProps()
{
	return m_mapProp;
}

ObjectProperties::~ObjectProperties()
{
// 	for (std::map<std::string, PROPERTY>::iterator it = m_mapProp.begin(); it != m_mapProp.end(); ++it)
// 	{
// 		delete it->second.pValue;
// 	}
	m_mapProp.clear();
}
// 
// PersistentValue::PersistentValue(const HandleContext&context, jsval*pValue /*= nullptr*/) :ValueBase(nullptr)
// {
// 	m_cx = (JSContext*)context.Get();
// 	JS::PersistentRootedValue*pval = new JS::PersistentRootedValue((JSContext*)context.Get());
// 	if (pValue)*pval = *(jsval*)pValue;
// 	m_pVal = pval;
// 	
// 
// 	//PersistentRootedObject似乎不能防止被GC，因此添加到global
// 	if (pValue)
// 		JSVM::GetInstance()->AddValueToGlobal(JS::RootedValue(m_cx, *pValue));
// }
// 
// PersistentValue::PersistentValue(const HandleContext&context, ValueBase&value) :ValueBase(nullptr)
// {
// 	new(this)PersistentValue(context, (jsval*)value.Get());
// }
// 
// PersistentValue::PersistentValue(const HandleContext&context, HandleObject&value) : ValueBase(nullptr)
// {
// 	new(this)PersistentValue(context, ValueBase(value));
// }
// 
// PersistentValue::~PersistentValue()
// {
// 	JS::PersistentRootedValue*pval = (JS::PersistentRootedValue*)m_pVal;
// 
// 	JSVM::GetInstance()->RemoveValueFromGlobal(*pval);
// 	delete pval;
// 	m_pVal = nullptr;
// }
// 
// void* PersistentValue::Get() const
// {
// 	JS::PersistentRootedValue*pval = (JS::PersistentRootedValue*)m_pVal;
// 	return &pval->get();
// }


// ArrayViewItem& ArrayViewItem::SetValue(const ValueBase& src)
// {
// 	Set(*src.Get());
// 	return *this;
// 	
// }

int CustomClassAutoThisObj::s_nCount;




JSVMAutoContext::JSVMAutoContext(JSVM*pVM) :ar(pVM->m_cx), ac(pVM->m_cx, pVM->m_globalObj)
{

}
