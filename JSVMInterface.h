#pragma once
#include "stdafx.h"
#include <stdint.h>
#include "jsapi.h"
#include <string>
#include "js/CallArgs.h"
#include <vector>
#include <list>
#include <unordered_map>
#include <map>
#include "Macros.h"
#include "ImageDecoderInterface.h"
#include "jsfriendapi.h"
#include "SystemInterface.h"
#include "HttpRequestInterface.h"
#include "AudioInterface.h"
#include "FileIOInterface.h"
#include "google-gumbo-parser/src/gumbo.h"
#include "md5.h"
#include "SocketInterface.h"
#include "HttpFileCache.h"
#include "OpenglInterface.h"

#if defined(_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif


//因GC时采取两块内存对拷的方式释放未使用对象，因此会导致JSObject*指针变化，Rooted/Heap/PersistentRooted可以在指针变化后自动更新指针
//因cocos2d中的阉割版spidermonkey仅支持Rooted，不支持Heap和PersistentRooted（有定义类但没有实际功能），所以现在只能从JS对象->C++对象，无法从C++对象->JS对象
//根据spidermonkey的规则，Rooted的对象必须先分配的后释放，因此函数返回值不能返回含有Rooted对象,会导致返回的Rooted先释放导致JS_GC()死锁

class HtmlCanvas;

class IFunctionCallbackArgs;
// 普通回调函数类型
typedef bool(*FuncCallback)(IFunctionCallbackArgs& args);
// 属性的存取回调函数类型
typedef bool(*AccessorCallback)(std::string property, IFunctionCallbackArgs& args);

class HandleContext;
void PrintDescribeScriptedCaller(const HandleContext&context, const char*str = nullptr);

std::string trim(const std::string& str);

class Noncopyable
{
	Noncopyable& operator=(const Noncopyable &rhs);
	Noncopyable(const Noncopyable &rhs);

protected:

	Noncopyable(){}
	virtual ~Noncopyable(){}
};


class HandleContext :public Noncopyable//封装JSContext*
{
protected:
	JSContext*m_pCtx;
public:
	HandleContext()
	{
		m_pCtx = nullptr;
	}
	HandleContext(JSContext*pContext);
	HandleContext(const HandleContext&context)
	{
		m_pCtx = context.Get();
	}
	virtual~HandleContext();
	JSContext*Get() const;
	HandleContext&operator=(const HandleContext&src)
	{
		m_pCtx = src.m_pCtx;
		return *this;
	}
};

class ValueBase;
class LocalValue;
class HandleObject;

class ValueBase;
class ObjectProperties;
class HandleObject :public Noncopyable//只封装JSObject*
{
private:
	JSObject*m_pObject = nullptr;
public:
	explicit HandleObject();
	explicit HandleObject(JSObject*pObject);
	HandleObject(const HandleObject&obj);
	virtual ~HandleObject();
	virtual JSObject*Get() const;//返回JS引擎对应的类型，如JSObject*
	virtual void Set(JSObject*pObject);//pObject:JS引擎对应的类型，如JSObject*
	virtual void SetObj(const HandleObject&obj);
	
	bool SetProperty(const HandleContext&context, const char*name, const ValueBase&value);
	ValueBase GetProperty(const HandleContext&context, const char*name);
	bool IsNull()const;

	bool operator==(const HandleObject&src);
	bool operator!=(const HandleObject&src);
	int GetProperties(const HandleContext&context, ObjectProperties&props);//取得所有属性
	virtual HandleObject&operator=(const HandleObject&src);

	

//////////////////////////////////////////////////////////////////////////

};
class CustomClassBase;
class ArrayViewItem;
class LocalObject :public HandleObject//作为局部变量使用，相当于spidermonkey的RootedObject或V8的Local<Object>
{
	friend class GetPropertyArgs;
	JS::RootedObject m_pObject;// = nullptr;
	HandleContext m_cx;
public:
	explicit LocalObject(const HandleContext&context, JSObject*pObj = nullptr);
	explicit LocalObject(const HandleContext&context, const HandleObject&obj);
	explicit LocalObject(const LocalObject&obj);
	virtual~LocalObject();
	virtual JSObject*Get() const;//返回JS引擎对应的类型，如JSObject*
	virtual void Set(JSObject*pObject);//pObject:JS引擎对应的类型，如JSObject*

	bool CreateJsObject(const HandleContext&context);//创建一个JS对象并赋值到this
	bool CreateJsObject();//创建一个JS对象并赋值到this
	bool CreateArrayObject(const HandleContext&context,int nLength);//创建数组对象
	bool CreateArrayObject( int nLength);//创建数组对象
	enum ARRAYTYPE
	{
		AT_NOARRAY,
		AT_INT8,
		AT_UINT8,
		AT_INT16,
		AT_UINT16,
		AT_INT32,
		AT_UINT32,
		AT_FLOAT32,
		AT_FLOAT64,

		AT_BOOLEAN8,
	};
	bool CreatePrimitiveArray(const HandleContext&context, int nLength, ARRAYTYPE type, void*pData = nullptr);
	ARRAYTYPE GetPrimitiveArrayType(const HandleContext&context);
	
	virtual LocalObject&operator=(const HandleObject&src)
	{
		m_pObject = src.Get();
		return *this;
	}
	virtual LocalObject&operator=(const LocalObject&src)
	{
		m_pObject = src.m_pObject;
		return *this;
	}
	bool SetProperty(const HandleContext&context, const char*name, const ValueBase&value){ return HandleObject::SetProperty(context, name, value); }
	bool SetProperty(const char*name, const ValueBase&value);
	bool SetProperty(const char*name, const char*str,bool bAutoToNumber=false);//bAutoToNumber:是否将str转成数字（如果能转）
	ValueBase GetProperty(const HandleContext&context, const char*name);
	ValueBase GetProperty(const char*name);
	//数组操作
	bool IsArray(const HandleContext&context);
	bool IsArray(){ return IsArray(m_cx); }
	int GetArrayLength(const HandleContext&context);
	int GetArrayLength(){ return GetArrayLength(m_cx); }
	ValueBase GetElement(const HandleContext&context, int index);
	ValueBase GetElement(int index);
	bool SetElement(const HandleContext&context, int index, const ValueBase&val);
	bool SetElement( int index, const ValueBase&val);
	void*GetArrayData(const HandleContext&context);
	int GetArrayByteLength(const HandleContext&context);
	bool SetArrayLength(const HandleContext&context, int nLen);
	bool SetArrayLength(int nLen);

	CustomClassBase*GetNativeObj();

//	ArrayViewItem operator[](int iElement);//数组下标访问
	ValueBase operator[](const char*name);//属性访问
	bool RemoveElement(int iElement);//删除数组元素并移动
};
// cocos2d中的阉割版spidermonkey只支持JS::Rooted,其他均不支持，因此不支持
// // 在程序运行时永久存在的全局JS对象
// class PersistentObject :public HandleObject
// {
// 	JSContext*m_cx = nullptr;
// 	JS::PersistentRootedObject*m_pObject = nullptr;//JS::PersistentRootedObject似乎仍然会被GC，所以现在在global中再增加一个隐藏对象来保存
// public:
// 	explicit PersistentObject(const HandleContext&context, JSObject*pObj = nullptr);
// 	explicit PersistentObject(const HandleContext&context, const HandleObject&obj);
// 	virtual~PersistentObject();
// 	virtual void* Get() const;
// 	virtual void Set(JSObject*pObject);
// 	
// };




class ValueBase :public Noncopyable//JS对象和数据的封装
{

protected:
	jsval m_pVal;// = nullptr;
public:
	
	explicit ValueBase(const jsval&val=JS::UndefinedValue());
	ValueBase(const HandleObject&obj);
//	explicit ValueBase(bool b);
	ValueBase(const ValueBase&src);


	virtual~ValueBase();

	virtual jsval* Get() const ;//返回JS引擎类型，如jsval
	virtual void Set(const jsval&pVal)
	{
		m_pVal = pVal;
		OnValueSet();
	}
	virtual void OnValueSet()//值被设置时调用
	{

	}
	bool operator==(const ValueBase&src);
	bool operator!=(const ValueBase&src);
	ValueBase&operator=(ValueBase&src)
	{
		Set(*src.Get());
		return *this;
	}

	virtual bool IsNull() const;
	virtual bool IsUndefined() const;
	virtual bool IsInt32() const;
	virtual bool IsString() const;
	virtual bool IsNumber() const;
	virtual bool IsObject() const;
	virtual int32_t GetInt() const;
	virtual double GetNumber() const;
	virtual bool GetBoolean() const;
	virtual HandleObject GetObject() const;
	virtual ValueBase& SetInt(int32_t val);
	virtual ValueBase& SetNumber(double val);
	virtual ValueBase& SetBoolean(bool val);
	virtual ValueBase& SetObject(const HandleObject&obj);
	virtual ValueBase& SetString(const HandleContext &cx, const char*str);
	virtual ValueBase&SetNull();
	virtual std::string GetString(const HandleContext &context) const;
	virtual int GetStringLength(const HandleContext &context) const;
	virtual ValueBase& SetUndefined();

	virtual size_t GetID(const HandleContext&context);


};



class LocalValue :public ValueBase
{
protected:
	JS::RootedValue m_pVal;// = nullptr;
	HandleContext m_cx;
public:
	explicit LocalValue(const HandleContext&context, const jsval&pValue=JS::UndefinedValue());
	LocalValue(const HandleContext&context,const ValueBase&value);
	LocalValue(const HandleContext&context,const HandleObject&value);
	LocalValue(const LocalValue&src);
	virtual~LocalValue();

	virtual jsval* Get() const;
	virtual void Set(const jsval&pVal)
	{
		m_pVal = pVal;
		OnValueSet();
	}
	LocalValue&operator=(const ValueBase&src)
	{
		Set(*src.Get());
		return *this;
	}
	bool GetIDValue(size_t id);
	// 	LocalValue&operator=(const LocalValue&src)
// 	{
// 		Set(src.Get());
// 		return *this;
// 	}


	virtual int32_t GetInt() const;
	virtual double GetNumber() const;
	virtual ValueBase& SetString(const HandleContext &cx, const char*str){ return ValueBase::SetString(cx, str); }
	virtual LocalValue& SetString(const char*str) ;
};
// 
// class ArrayViewItem :public LocalValue//通过LocalObject::operator[]返回的数组元素，增加了对它赋值重载为同时更新到对应的数组
// {
// protected:
// 	JS::RootedObject m_pArrayObj;// = nullptr;
// 	int m_iIndex = 0;
// public:
// 	explicit ArrayViewItem(const HandleContext&context) :LocalValue(context), m_pArrayObj(context.Get())
// 	{
// 
// 	}
// 	ArrayViewItem(const HandleContext&context, ValueBase&value, HandleObject&arrayObj, int index) :LocalValue(context, value), m_pArrayObj(context.Get(),arrayObj.Get())
// 	{
// 		m_iIndex = index;
// 	}
// 	ArrayViewItem(ArrayViewItem&src) :LocalValue(src) , m_pArrayObj(src.m_cx.Get(), src.m_pArrayObj)
// 	{
// 		m_iIndex = src.m_iIndex;
// 	}
// 	~ArrayViewItem()
// 	{
// 
// 	}
// 	operator ValueBase()
// 	{
// 		return ValueBase(*Get());
// 	}
// 	virtual void OnValueSet()//值被设置时调用
// 	{
// 		JSContext*cx = (JSContext*)m_cx.Get();
// 		bool bOK = JS_SetElement(cx, m_pArrayObj, m_iIndex,m_pVal);
// 	}
// 
// 	bool success()
// 	{
// 		return m_pArrayObj != nullptr;
// 	}
// 	ArrayViewItem& SetValue(const ValueBase& src);
// };

// 
// class PersistentValue :public ValueBase
// {
// private:
// 	JS::PersistentRootedValue*m_pVal = nullptr;
// 	JSContext*m_cx = nullptr;
// public:
// 	explicit PersistentValue(const HandleContext&context, jsval*pValue = nullptr);
// 	PersistentValue(const HandleContext&context, ValueBase&value);
// 	PersistentValue(const HandleContext&context, HandleObject&value);
// 	virtual~PersistentValue();
// 
// 	virtual void*Get() const;
// 	virtual void Set(void*pVal)
// 	{
// 		JS::PersistentRootedValue*pval = (JS::PersistentRootedValue*)m_pVal;
// 		*pval = *(jsval *)pVal;
// 	}
// // 	PersistentValue&operator=(ValueBase&src)
// // 	{
// // 		ValueBase::operator=(src);
// // 		return *this;
// // 	}
// };









/**
* 回调函数所需的参数信息
*/
class IFunctionCallbackArgs {
public:
	IFunctionCallbackArgs(const HandleContext& cx,  int argc, void* args);
	IFunctionCallbackArgs();
	~IFunctionCallbackArgs();

	// 上下文
	const HandleContext& getContext() const;



	// 参数个数
	int length() const;

	// 通过下标读取参数
	ValueBase operator[](int i) const;
	
	// 被调用的函数
	HandleObject callee() const;



	// 调用者
	HandleObject thisObj() ;

	

	// 设置返回值
	void setReturnValue(const ValueBase&val);
	ValueBase getReturnValue();

	const bool hasReturn();


private:
	HandleContext m_cx;
	JS::CallArgs args;
	bool m_return=false;
};

//////////////////////////////////////////////////////////////////////////	


// 回调函数类型枚举
// enum ICallbackType{
// 	UNKNOWN,
// 	CONSTRUCTOR,   // 构造函数
// 	GETTER,        // 获取属性的函数
// 	SETTER,        // 设置属性的函数
// 	COMMON_FUNC    // 普通回调函数
// };
//
// 
// class HandleFunction :public Noncopyable//对JSFunction*封装
// {
// protected:
// 	void*m_pFunc = nullptr;
// 
// // 	static bool functionCallback(JSContext *cx, uint32_t argc, jsval *vp)
// // 	{
// // 
// // 	}
// public:
// 	HandleFunction(JSFunction*pFunc=nullptr)
// 	{
// 		m_pFunc = pFunc;
// 	}
// 	virtual~HandleFunction()
// 	{
// 		m_pFunc = nullptr;
// 	}
// 	virtual void*Get()
// 	{
// 		return m_pFunc;
// 	}
// 	virtual void Set(void*pFunc)
// 	{
// 		m_pFunc = pFunc;
// 	}
// 
// 
// // 	static HandleFunction CreateFunction(const HandleContext&context, FuncCallback callback, int nargs, const char*name)//定义全局函数，window对象的成员函数也用这定义
// // 	{
// // 		JSContext*cx = (JSContext*)context.Get();
// // 		JS::RootedFunction fun(cx, JS_NewFunction(cx, functionCallback, nargs, 0, JS::RootedObject(cx), name));
// // 		return fun;
// // 	}
// };
// class LocalFunction :public HandleFunction
// {
// public:
// 	LocalFunction(const HandleContext&context, JSFunction*pFunc = nullptr)
// 	{
// 		JS::RootedFunction*pfunc=new JS::RootedFunction((JSContext*)context.Get());
// 		if (pFunc)*pfunc = (JSFunction*)pFunc;
// 	}
// 	virtual~LocalFunction()
// 	{
// 		JS::RootedFunction*pfunc = (JS::RootedFunction*)m_pFunc;
// 		delete pfunc;
// 		m_pFunc = nullptr;
// 	}
// 	virtual void*Get()
// 	{
// 		JS::RootedFunction*pfunc = (JS::RootedFunction*)m_pFunc;
// 		return pfunc->get();
// 	}
// 	virtual void Set(void*pFunc)
// 	{
// 		JS::RootedFunction*pfunc = (JS::RootedFunction*)m_pFunc;
// 		*pfunc = (JSFunction*)pFunc;
// 	}
// };
// class HeapFunction :public HandleFunction
// {
// public:
// 	HeapFunction(JSFunction*pFunc = nullptr)
// 	{
// 		JS::Heap<JSFunction*>*pfunc = new JS::Heap < JSFunction* > ;
// 		if (pFunc)*pfunc = (JSFunction*)pFunc;
// 	}
// 	virtual~HeapFunction()
// 	{
// 		JS::Heap<JSFunction*>*pfunc = (JS::Heap<JSFunction*>*)m_pFunc;
// 		delete pfunc;
// 		m_pFunc = nullptr;
// 	}
// 	virtual void*Get()
// 	{
// 		JS::Heap<JSFunction*>*pfunc = (JS::Heap<JSFunction*>*)m_pFunc;
// 		return pfunc->get();
// 	}
// 	virtual void Set(void*pFunc)
// 	{
// 		JS::Heap<JSFunction*>*pfunc = (JS::Heap<JSFunction*>*)m_pFunc;
// 		*pfunc = (JSFunction*)pFunc;
// 	}
// };

//指向 JS::HandleObject*，提高性能
class JSHandleObject :public HandleObject
{
	friend class GetPropertyArgs;
	JS::HandleObject*m_pObject ;
	explicit JSHandleObject(){ assert(0); }
	explicit JSHandleObject(JSObject*pObject){ assert(0); }
	JSHandleObject(HandleObject&obj){ assert(0); }
public:
	explicit JSHandleObject(JS::HandleObject*pObj)
	{
		m_pObject = pObj;
	}

	virtual JSObject*Get() const//返回JS引擎对应的类型，如JSObject*
	{
		if (!m_pObject)return nullptr;
		return *m_pObject;
	}
	virtual void Set(JSObject*pObject)//pObject:JS引擎对应的类型，如JSObject*
	{
		assert(0);
	}
	void SetObj(const HandleObject&obj)
	{
		assert(0);
	}
};

//对MutableHandleValue的封装，析构时不释放m_pVal
class MyMutableHandleValue :public ValueBase
{
private:
	JS::MutableHandleValue*m_pVal = nullptr;
	virtual jsval* Get() const
	{
		return m_pVal->address();

	}

public:
	explicit MyMutableHandleValue(const HandleContext&context, JS::MutableHandleValue* pValue) :ValueBase()
	{
		m_pVal = pValue;
	}
	virtual~MyMutableHandleValue()
	{
		m_pVal = nullptr;
	}
	virtual void Set(const jsval&pVal)
	{

	}

	
	

	// 	MyMutableHandleValue&operator=(ValueBase&src)
	// 	{
	// 		ValueBase::operator=(src);
	// 		return *this;
	// 	}
};


class LocalObjectArray
{
	JS::AutoObjectVector m_jsvector;
public:
	explicit LocalObjectArray(const HandleContext&context) :m_jsvector(context.Get())
	{
	}
	bool append(const HandleObject&obj)
	{
		return m_jsvector.append(obj.Get());
	}
	HandleObject operator [](size_t index)
	{
		HandleObject o(m_jsvector[index].get());
		return o;
	}
	void clear()
	{
		m_jsvector.clear();
	}
	size_t size()
	{
		return m_jsvector.length();
	}

};
class LocalValueArray
{
	friend class JSVM;
	JS::AutoValueVector m_jsvector;
public:
	explicit LocalValueArray(const HandleContext&context) :m_jsvector(context.Get())
	{
	}
	bool append(const ValueBase&val)
	{
		return m_jsvector.append(*val.Get());
	}
	ValueBase operator [](size_t index)
	{
		return ValueBase(m_jsvector[index].get());
	}
	void clear()
	{
		m_jsvector.clear();
	}
	size_t size()
	{
		return m_jsvector.length();
	}

};



class ObjectProperties//GetProperties返回属性信息
{
public:
	struct PROPERTY
	{
		ObjectProperties*pParent;
		int index;
		bool isEnumerate;
		bool isReadOnly;
		bool isPremanent;
		ValueBase Value()
		{
			return pParent->m_values[index];
		}
	};
	LocalValueArray m_values;
	std::map<std::string, PROPERTY>m_mapProp;
public:
	explicit ObjectProperties(const HandleContext&context) :m_values(context)
	{
	}
	std::map<std::string, PROPERTY>&GetProps();
	~ObjectProperties();
};


class GetPropertyArgs
{
	friend class CustomClassBase;
private:
	JS::MutableHandleValue*pRetValue;
	HandleContext*pContext;
	JSHandleObject*pObj;
//	std::string szName;
	const char*szName;
	JS::HandleId* pid;
public:
	

	HandleContext&GetContext()
	{
		return *pContext;
	}
	HandleObject &GetThisObject()
	{
		return *pObj;
	}
	const char*GetName()
	{
		return szName;
	}
// 	const std::string&GetName()
// 	{
// 		return szName;
// 	}
	void SetRetValue(const ValueBase&value)
	{
		pRetValue->set(*value.Get());
	}
	ValueBase GetRetValue()
	{
		jsval v=pRetValue->get();
		return ValueBase(v);
	}
	ValueBase CallGetPropertyFunc()//调用JS_PropertyStub()读取属性，防止重复进入OnGetProperty()
	{
		if(!JS_PropertyStub(pContext->Get(), *pObj->m_pObject, *pid, *pRetValue))
			return ValueBase();
		return ValueBase(*pRetValue);
	}

	bool bCallDefaultProperty = false;//如果要调用引擎默认的处理方式，设置为true
};
class SetPropertyArgs
{
	friend class CustomClassBase;
private:
	HandleContext*pContext;
	JSHandleObject*pObj;
	const char* szName;
	ValueBase*pValue;
public:
	HandleContext&GetContext()
	{
		return *pContext;
	}
	HandleObject&GetThisObject()
	{
		return *pObj;
	}
	const char*GetName()
	{
		return szName;
	}
	ValueBase&GetValue()
	{
		return *pValue;
	}
	bool bCallDefaultProperty = false;//如果要调用引擎默认的处理方式，设置为true
};
struct FinalizeArgs
{
	friend class CustomClassBase;
private:
	HandleObject*pObj;
public:
	HandleObject&GetThisObject()
	{
		return *pObj;
	}
};



class ObjectList
{
public:
	 LocalObjectArray objList;
	 ObjectList(const HandleContext&context) :objList(context)
	 {

	 }
	~ObjectList()
	{
// 		for (int i = objList.size()-1; i >=0; i--)
// 		{
// 			delete objList[i];
// 		}
// 		objList.clear();
	}
};

struct CUSTOMFUNCTION;
struct CustomClassInfo
{
	bool bHookAllProperties=false;
	const char*pClassName = nullptr;
	CustomClassInfo*pParentClassInfo = nullptr;


	typedef std::unordered_map<size_t, CUSTOMFUNCTION*>::iterator MapFunToNativeIt;
	std::unordered_map<size_t, CUSTOMFUNCTION*>s_mapFuncToNative;


	CustomClassInfo(const char*szClassName,CustomClassInfo*pParent)
	{
		pParentClassInfo = pParent;
		pClassName = szClassName;
	}
};

#define DEFINE_CUSTOM_CLASS(NAME,PARENT) \
protected:\
static JSClass s_##NAME##_class; \
static JS::Heap<JSObject*>s_##NAME##_pPrototype; \
static CustomClassInfo s_##NAME##_ClassInfo;\
virtual CustomClassInfo*GetCustomClassInfo(){return &s_##NAME##_ClassInfo;}\
static CustomClassBase*NAME##_New()\
{\
	return new NAME();\
}\
virtual CustomClassBase*New()\
{\
	return NAME##_New();\
}\
virtual JSClass*GetJsClass()\
{\
	return &s_##NAME##_class;\
}\
static JSClass*GetJsClass_s()\
{\
	return &s_##NAME##_class;\
}\
virtual void SetPrototype(JSObject*proto)\
{\
s_##NAME##_pPrototype = proto;\
}\
public:\
virtual HandleObject GetPrototype()\
{\
return HandleObject(s_##NAME##_pPrototype.get());\
}\
static HandleObject GetPrototype_s()\
{\
	return HandleObject(s_##NAME##_pPrototype.get());\
}\
static bool InitClass_s(const HandleContext&context,const char*szName, const HandleObject&ParentProto,bool bHookAllProperties=false) \
{\
	CustomClassBase*pThis = NAME##_New();\
	bool ret=pThis->_InitClass(context,szName, ParentProto,bHookAllProperties);\
	delete pThis;\
	return ret;\
}\
static void DisposeClass()\
{\
	s_##NAME##_pPrototype = nullptr;\
}\
static bool js_##NAME##_constructor(JSContext *cx, uint32_t argc, jsval *vp)\
{\
CustomClassBase*pThis = NAME##_New();\
IFunctionCallbackArgs args(cx,argc,vp);\
bool ret = pThis->OnConstructor(args); \
if (!ret)\
{\
	delete pThis;\
}\
else\
{\
	JSObject*pOBj=(JSObject*)args.getReturnValue().GetObject().Get();\
/*s_mapCustomObj[JS::Heap<JSObject*>(pOBj)] = pThis;*/ \
	JS_SetPrivate(pOBj, pThis);\
}\
return ret;\
}\
virtual JSNative GetConstructor()\
{\
	return js_##NAME##_constructor;\
}\
static HandleObject CreateJsObject_s(const HandleContext&context)\
{\
JSContext*cx = (JSContext*)context.Get(); \
JS::RootedObject obj(cx, JS_NewObject(cx, &s_##NAME##_class, JS::RootedObject(cx, s_##NAME##_pPrototype), JS::RootedObject(cx))); \
return HandleObject(obj.get()); \
}\
static HandleObject CreateObject(const HandleContext&context)\
{\
	CustomClassBase*pThis = NAME##_New();\
	jsval v[10];\
	IFunctionCallbackArgs args(context, 0, v + 5);\
	bool ret=pThis->OnConstructor(args);\
	if(!ret)\
	{\
		delete pThis;\
		return HandleObject();\
	}\
	else\
	{\
		delete pThis->m_pObj;\
		pThis->m_pObj = nullptr;\
		return args.getReturnValue().GetObject(); \
	}\
}

// static CustomClassBase*CreateObject(const HandleContext&context)\
// {\
// 	CustomClassBase*pThis = NAME_New();\
// 	jsval v[10];\
// 	IFunctionCallbackArgs args(context, 0, v + 5);\
// 	bool ret=pThis->OnConstructor(args);\
// 	if(!ret)\
// 	{\
// 		delete pThis;\
// 		return nullptr;\
// 	}\
// 	else\
// 	{\
// 		return pThis; \
// 	}\
// }



#define  IMPLEMENT_CUSTOM_CLASS(NAME,PARENT) \
JSClass NAME::s_##NAME##_class;\
JS::Heap<JSObject*>NAME::s_##NAME##_pPrototype;\
CustomClassInfo NAME::s_##NAME##_ClassInfo(#NAME,&PARENT::s_##PARENT##_ClassInfo);\
static int __FAKE_##NAME##_INIT=CustomClassBase::AddInitList(NAME::InitClass,NAME::DisposeClass);


#define PROPCMP(A,B) (A!=B)/*strcmp(A,B)*/
//#define PROPCMP(A,B) strcmp(A,B)

class CustomClassBase;
typedef bool(CustomClassBase::*CustomFuncCallback)(IFunctionCallbackArgs& args);
//
struct CUSTOMFUNCTION
{
	const char*szName;
	CustomFuncCallback pFunc;
	int nArgs;
	jsid id;
	bool bIsRenderFunc;//是渲染函数还是一般函数
};


#define BEGIN_CUSTOM_FUNCTION \
	virtual int CustomFunction(CUSTOMFUNCTION*&pFuncs)\
	{\
	static CUSTOMFUNCTION s_funcs[]={

	



#define DEFINE_CUSTOM_FUNCTION(NAME,FUNC,NARGS) {NAME,(CustomFuncCallback)FUNC,NARGS,JSID_VOID,false},
#define DEFINE_CUSTOM_RENDER_FUNCTION(NAME,FUNC,NARGS) {NAME,(CustomFuncCallback)FUNC,NARGS,JSID_VOID,true},



#define END_CUSTOM_FUNCTION \
	DEFINE_CUSTOM_FUNCTION(0,0,0) \
};\
pFuncs=s_funcs;\
return sizeof(s_funcs)/sizeof(CUSTOMFUNCTION)-1;}

struct CUSTOMPROPERTY
{
	const char*szName;
	jsid id;
};

#define BEGIN_CUSTOM_PROPERTY \
virtual int CustomProperty(CUSTOMPROPERTY*&pProps)\
{\
	static CUSTOMPROPERTY s_props[] = {




#define DEFINE_CUSTOM_PROPERTY(NAME) {NAME,JSID_VOID},


#define END_CUSTOM_PROPERTY \
	DEFINE_CUSTOM_PROPERTY(nullptr) \
	};\
	pProps=s_props;\
	return sizeof(s_props)/sizeof(CUSTOMPROPERTY)-1;}


//自定义JS类的基类
typedef bool(*INITCLASS)(const HandleContext&context);//自定义类的初始化函数，名称为static bool InitClass(const HandleContext&context);在JS引擎初始化后回调
typedef void(*DISPOSECLASS)();//自定义类的清除函数，名称为static void DisposeClass();在JS引擎销毁时回调
struct CLASSFUNC
{
	INITCLASS pInitClass;
	DISPOSECLASS pDisposeClass;
};


class CustomClassBase :public Noncopyable
{
	friend class JSVM;
protected:
	static CustomClassInfo s_CustomClassBase_ClassInfo;
	virtual const char*GetThisClassName()\
	{
		return GetCustomClassInfo()->pClassName; 
	}
//	DEFINE_CUSTOM_CLASS(CustomClassBase);
	
//	static std::map<JS::Heap<JSObject*>, CustomClassBase*>s_mapCustomObj;//JS里new出来的对象会保存在这里
public:
//	JS::RootedObject*m_pObj=nullptr;//引擎回调上来时会赋值，MainLoop结束后会释放
	LocalObject*m_pObj=nullptr;
//	JS::Heap<JSObject*>m_pObj;
	static bool js_GetProperty(JSContext *cx, JS::Handle<JSObject*> obj, JS::Handle<jsid> id, JS::MutableHandle<JS::Value> vp);
 	static bool js_SetProperty(JSContext *cx, JS::Handle<JSObject*> obj, JS::Handle<jsid> id, bool strict, JS::MutableHandle<JS::Value> vp);
// 	static bool js_GetProperty2(JSContext *cx, uint32_t argc, jsval *vp);
// 	static bool js_SetProperty2(JSContext *cx, uint32_t argc, jsval *vp);
protected:
	static void js_finalize(JSFreeOp *fop, JSObject *obj);
	

	static bool js_function(JSContext *cx, uint32_t argc, jsval *vp);
	
	static CustomClassBase* FindCustomClass(JSObject* Obj);
	
	

	virtual JSClass*GetJsClass() = 0;
	virtual JSNative GetConstructor() = 0;
	virtual void SetPrototype(JSObject* proto) = 0;
	virtual HandleObject GetPrototype() = 0;
	HandleObject CreateThisObject(const HandleContext&context)//创建一个跟this类型相同的JS对象
	{
		CustomClassBase*pThis = New();
		jsval v[10];
		IFunctionCallbackArgs args(context, 0, v + 5);
		bool ret = pThis->OnConstructor(args);
		if (!ret)
		{
			delete pThis;
			HandleObject ob;
			return ob;
//			return HandleObject();
		}
		else
		{
			delete pThis->m_pObj;
			pThis->m_pObj = nullptr;
			return args.getReturnValue().GetObject();
		}
	}
	HandleObject CreateJsObject(const HandleContext&context)//根据本类型的JSClass/prototype创建一个JS对象
	{

		JSContext*cx = (JSContext*)context.Get();
		JS::RootedObject obj(cx);
		obj = JS_NewObject(cx, GetJsClass(), JS::RootedObject(cx, (JSObject*)GetPrototype().Get()), JS::RootedObject(cx));
		return HandleObject(obj.get());
	}
//	typedef std::unordered_map<size_t, CUSTOMFUNCTION*>::iterator MapFunToNativeIt;
//	static std::unordered_map<size_t, CUSTOMFUNCTION*>s_mapFuncToNative;
//	static std::map<JS::Heap<JSFunction*>, CustomFuncCallback>s_mapFuncToNative;
	typedef std::unordered_map<size_t, CUSTOMPROPERTY*>::iterator MapPropToNativeIt;
	static std::unordered_map<size_t, CUSTOMPROPERTY*>s_mapPropToNative;

public:
	
	HandleObject &GetJsObject()
	{
		assert(m_pObj);
		return *m_pObj;
	}
	static CustomClassBase*FindCustomClass(const HandleObject&obj)
	{
		return FindCustomClass((JSObject*)obj.Get());
	}
	static std::list<CLASSFUNC>*s_InitList;
	static int AddInitList(INITCLASS pInitFunc, DISPOSECLASS pDisposeFunc)
	{
		if (!s_InitList)s_InitList = new std::list < CLASSFUNC > ;
		CLASSFUNC cf{ pInitFunc, pDisposeFunc };
		s_InitList->push_back(cf);
		return 0;
	}
	static std::list<CLASSFUNC>*GetInitList()
	{
		return s_InitList;
	}

	bool m_bProtectObjNoGC = false;//是否调用了ProtectObjNoGC()
	static bool js_EnumerateStub(JSContext *cx, JS::HandleObject obj);
	bool ProtectObjNoGC(bool bProtect);//防止本对象被GC回收（对于完整版支持PersistRooted则不需要这个函数）
	HandleObject GetObjFromNoGC();

	BEGIN_CUSTOM_FUNCTION

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY


	virtual CustomClassBase*New() = 0;
	

	CustomClassBase();
	virtual~CustomClassBase();

	virtual CustomClassInfo*GetCustomClassInfo(){return &s_CustomClassBase_ClassInfo; }

	virtual bool _InitClass(const HandleContext&context, const char*szName, const HandleObject&ParentProto, bool bHookAllProperties);//初始化类，跟JS绑定，实际调用应该调用派生类::InitClass(),bHookAllProperties:TRUE则所有的属性操作都会进入OnGetProperty/OnSetProperty，否则只有用户定义的属性才会进入

	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args) ;
	virtual void OnFinalize(FinalizeArgs&args) ;
	virtual bool OnConstructor(IFunctionCallbackArgs&args) ;//构造函数在JS中调用new时调用，创建的JS对象放在args的返回值里

	virtual bool OnFunctionCall(IFunctionCallbackArgs&args, CUSTOMFUNCTION*pFunc);

	static void CleanUp();



};

class CustomClassAutoThisObj//自动生成CustomClassBase的m_pObj，否则GetJsObject()出错
{
	CustomClassBase*m_pThis = nullptr;
	LocalObject m_thisobj;
 	static int s_nCount;
public:
	CustomClassAutoThisObj(CustomClassBase*pThis, LocalObject&objThis):m_thisobj(objThis)
	{
		if (pThis->m_pObj)
			return;
		m_pThis = pThis;
		pThis->m_pObj = &m_thisobj;
//		s_nCount++;
	}
	CustomClassAutoThisObj(CustomClassBase*pThis, IFunctionCallbackArgs&args, bool bInOnConstructor) :m_thisobj(args.getContext())
	{
		if (pThis->m_pObj)
			return;
		m_pThis = pThis;
		if (bInOnConstructor)
		{
			m_thisobj =args.getReturnValue().GetObject();
		}
		else
		{
			m_thisobj =  args.thisObj();
		}
		pThis->m_pObj = &m_thisobj;
// 		s_nCount++;
	}
	CustomClassAutoThisObj(CustomClassBase*pThis, SetPropertyArgs&args) :m_thisobj(args.GetContext(), args.GetThisObject())
	{
		if (pThis->m_pObj)
			return;
		m_pThis = pThis;
		pThis->m_pObj = &m_thisobj;
// 		s_nCount++;
	}
	CustomClassAutoThisObj(CustomClassBase*pThis, GetPropertyArgs&args) :m_thisobj(args.GetContext(), args.GetThisObject())
	{
		if (pThis->m_pObj)
			return;
		m_pThis = pThis;
		pThis->m_pObj = &m_thisobj;
		// 		s_nCount++;
	}
	~CustomClassAutoThisObj()
	{
		if (m_pThis)
		{
			m_pThis->m_pObj = nullptr;
// 			s_nCount--;
		}
	}
};

class IFunctionCallJsArgs
{
public:
	HandleObject*pObjThis = nullptr;
//	std::vector<ValueBase*>m_ParamList;
	LocalValueArray m_ParamList;
	IFunctionCallJsArgs(const HandleContext&context) :m_ParamList(context.Get())
	{

	}
	int SetParams(HandleObject*objThis, int nargs...)//省略的参数类型为ValueBase*，
	{
		pObjThis = objThis;
		va_list ap;
		va_start(ap, nargs);
		for (int i = 0; i < nargs; i++)
		{
			ValueBase*pParam = (ValueBase*)va_arg(ap, ValueBase*);
			m_ParamList.append(*pParam);
//			m_ParamList.push_back(pParam);
		}
		va_end(ap);
		return m_ParamList.size();
	}
	int AddParams(const ValueBase&para)
	{
		m_ParamList.append(para);
		return m_ParamList.size();
	}
};


class RunOnUIThread//从子线程调用主线程代码时使用该类
{
public:
	virtual ~RunOnUIThread()
	{

	}
	virtual void OnPostExecute()//会在下一次JSVM::MainLoop()主线程中执行，执行会后会被自动释放
	{

	}
};
class JSVM;
class JSVMAutoContext
{
	JSAutoRequest ar;
	JSAutoCompartment ac;
public:
	JSVMAutoContext(JSVM*pVM);
};

// 底层JS虚拟机类
class JSVM {
	friend class JSVMAutoContext;
private:
	bool m_bdisposeEnvironment = false;
	bool m_bLoaded = false;//是否调用过onload

	JSRuntime*m_rt=nullptr;
	JSContext*m_cx=nullptr;
	JS::Heap<JSObject*>m_globalObj;
	ImageDecoderInterface*m_pImgDecoder = nullptr;
	SystemInterface*m_pSysInterface = nullptr;
	HttpRequestInterface*m_pHttpReq = nullptr;
	FileIOInterface*m_pFileIO = nullptr;
	AudioInterface*m_pAuido = nullptr;

	SocketInterface*m_pSocket = nullptr;
	HttpFileCache m_FileCache;
    OpenglInterface* m_pGLInterface = nullptr;
	
	
	std::string m_szLocalStoragePath = "localstorage";//LocalStorage保存的文件夹，文件名为域名.dat
	
	
	static JSVM _instance;

	static JSClass global_class;
	

public:
	pthread_mutex_t m_lock;
	std::string m_szCachePath;//缓存路径，安卓为Android/包名/file/cache
	std::string m_szExternalPath = ".";//安卓为Android/包名/file


	std::string m_szUrl;
	std::unordered_map<HtmlCanvas*, HtmlCanvas*>m_VisibleCanvas;//添加到DOM的canvas会在这里记录，移除或GC时会同时从这里删除

	
	std::list<RunOnUIThread*>m_RunOnUIThreadList;//异步调用函数，或者在另一个线程中切换到主线程调用函数
	void AddRunOnUIThread(RunOnUIThread*pTask)
	{
		pthread_mutex_lock(&m_lock);
		m_RunOnUIThreadList.push_back(pTask);
		pthread_mutex_unlock(&m_lock);
	}
	


// 	struct ASYNCFUN
// 	{
// 		PersistentValue*pFun = nullptr;
// 		~ASYNCFUN()
// 		{
// 			delete pFun;
// 		}
// 	};
// 	std::list<ASYNCFUN*>m_asyncFunList;//准备在下一次mainloop调用的函数
//	std::vector<PersistentValue*>m_aniframeList;//requestAnimationFrame指定的事件
	std::string m_szRootPath;//LoadHtml()打开的页面所在路径

// 	struct ASYNCLOADJS
// 	{
// 		PersistentObject*pScriptObj = nullptr;
// 		std::string jsFile;
// 	};
// 
// 	std::list<ASYNCLOADJS>m_asyncScriptList;//要加载的JS文件



//	std::vector<size_t>m_aniframeList;//保存的是requestAnimationFrame的函数jsid

	//////////////////////////////////////////////////////////////////////////
public:
	bool m_bEnableWebgl = true;


	// multi-touch
	struct _TOUCHSTATE
	{
		long clientx = 0;
		long clienty = 0;
		long screenx = 0;
		long screeny = 0;
		bool bEnable = false;
		bool bStateChange = false;//蔭dd to changedTouches if state changes
	};
    
    static const int MAX_TOUCHES = 20;  // support 20 multitouches
    std::map<long, _TOUCHSTATE> m_TouchStateList; // touch state map
    long m_touchIDtoErase;
    
	bool m_balt = false;
	bool m_bctrl = false;
	bool m_bshift = false;

	struct _Touch
	{
		long        identifier = -1;
		JSObject*	 target = NULL;
		long        screenX;
		long        screenY;
		long        clientX;
		long        clientY;
		long        pageX;
		long        pageY;
	};

	enum _EVENTTYPE
	{
		ET_NULL,
		ET_MOUSEDOWN,
		ET_MOUSEUP,
		ET_MOUSEMOVE,
		ET_KEYDOWN,
		ET_KEYUP,

		ET_LOAD,


		ET_TOUCH,
	};
	struct _EVENTQUEUE
	{
		_EVENTTYPE eventType;
		_EVENTQUEUE()
		{
			eventType = ET_NULL;
		}
		virtual ~_EVENTQUEUE()
		{

		}
	};

	struct _TouchEvent :_EVENTQUEUE
	{
		_TouchEvent()
		{
			eventType = ET_TOUCH;
		}
		std::vector<_Touch> touches;
		//	std::vector<_Touch> targetTouches;
		std::vector<_Touch> changedTouches;
		bool   altKey = false;
		bool   metaKey = false;
		bool   ctrlKey = false;
		bool   shiftKey = false;
		enum TOUCHTYPE
		{
			TT_START,
			TT_MOVE,
			TT_END,
		};
		TOUCHTYPE nTouchType = TT_START;
	};
	std::list<_EVENTQUEUE*>m_eventQueue;//消息队列，系统消息先放入队列等下一次mainLoop()时执行
	
public:

	void SetRootPath(const std::string&path);
	std::string&GetRootPath();
	std::string GetFullPath(const std::string &szFile);//返回完整路径
	std::string GetLocalStoragePath();
	std::string GetCacheFullPath(const std::string&szUrl);

	static JSVM*GetInstance()
	{
		return &_instance;
	}


	JSVM();
	~JSVM();
	// 初始化JS运行环境
	bool initEnvironment();

	// 销毁JS运行环境
	void disposeEnvironment();
	bool LoadHtml(std::string path);//加载HTML页，建立DOM结构并加载JS文件
	bool LoadHtml(LocalObject&objParent, GumboNode*pNode, bool bFromInnerText);

	bool runScript(const char* path);
	bool evalString(const char* string);
	virtual void OnInitFinish();//初始化完成后回调
	HandleContext GetContext()
	{
		return HandleContext(m_cx);
	}
	HandleObject GetGlobal()
	{
		return HandleObject(m_globalObj.get());
	}
	void ExecEvents();
	void MainLoop();

	JSScript* compileScript(const char *path);
	bool RunScriptText(const char*szText, const char*szFilename);
	void OnLoad();//JS文件加载完毕后调用
	static bool executeFunction(const HandleContext&context, const ValueBase&vFunc, IFunctionCallJsArgs&args, LocalValue&vRet);
	static bool executeFunction(const HandleContext&context,const std::string szName, IFunctionCallJsArgs&args, LocalValue&vRet);
	void executeFunctionAsync(const HandleContext& context, const HandleObject&objthis,const ValueBase& fun);//异步执行一个函数
//	int GetVisibleCanvas(std::vector<HtmlCanvas*>&canvasList);
	void SetImageDecoder(ImageDecoderInterface*pDec)
	{
		m_pImgDecoder = pDec;
	}
	ImageDecoderInterface* GetImageDecoder()
	{
		return m_pImgDecoder;
	}
	void SetSystemInterface(SystemInterface*p)
	{
		m_pSysInterface = p;
	}
	SystemInterface*GetSystemInterface()
	{
		return m_pSysInterface;
	}
	void SetHttpRequestInterface(HttpRequestInterface* httpreq)
	{
		m_pHttpReq = httpreq;
	}
	HttpRequestInterface*GetHttpRequestInterface()
	{
		return m_pHttpReq;
	}
	void SetAudioInterface(AudioInterface*p)
	{
		m_pAuido = p;
	}
	AudioInterface* GetAudioInterface()
	{
		return m_pAuido;
	}
	AudioInterface*CreateAudioInterface();
	void SetFileIOInterface(FileIOInterface*p)
	{
		m_pFileIO = p;
	}
	FileIOInterface*GetFileIOInterface()
	{
		return m_pFileIO;
	}
	void SetSocketInterface(SocketInterface*p)
	{
		m_pSocket = p;
	}
	SocketInterface*GetSocketInterface()
	{
		return m_pSocket;
	}
	HttpFileCache*GetHttpFileCache()
	{
		return &m_FileCache;
	}
    void SetOpenglInterface(OpenglInterface* p)
    {
        m_pGLInterface = p;
    }
    OpenglInterface* GetOpenglInterface()
    {
        return m_pGLInterface;
    }

	long requestAnimationFrame(const ValueBase&vFun);
	bool cancelAnimationFrame(long id);
	void RunScriptTextAsync(const char*szText, const char*szFilename ,const HandleObject&jsobj);
	void runScriptAsync(const char * szFilename, const HandleObject&jsobj);

	void AddValueToGlobal( JS::HandleValue val);
	void RemoveValueFromGlobal( JS::HandleValue val);
	int ExecRunScript();
	int ExecAsyncFun();
	int ExecEventAsync();
	void ExecAnimationFrame();

	//////////////////////////////////////////////////////////////////////////
	void OnTouchStart(long touchID,int x, int y, int screenX, int screenY, bool bAlt, bool bShift, bool bCtrl);
	void OnTouchMove(long touchID, int x, int y, int screenX, int screenY, bool bAlt, bool bShift, bool bCtrl);
	void OnTouchEnd(long touchID, int x, int y, int screenX, int screenY, bool bAlt, bool bShift, bool bCtrl);
	void SendOnTouchEvent(_TouchEvent::TOUCHTYPE touchtype);
	void ExecEventQueue();
	void CallGC();
	static void OnGCCallBack(JSRuntime *rt, JSGCStatus status, void *data);
	JSObject* sm_newGlobalObject(JSContext* jsCx);
	void OnSize(int width, int height);
	std::string GetMD5(const char * str);

	void AddVisibleCanvas(HtmlCanvas*pCanvas);
	void RemoveVisibleCanvas(HtmlCanvas*pCanvas);

	static std::string Utf8ToString(const char* szText);
	static std::string AnsiToUtf8(const char*str);
	static std::string UnicodeToString(const jschar* str);

	static std::string ByteArrayToString(const char*szText);
	static void StringReplace(std::string &strBase, std::string strSrc, std::string strDes);
	static wstring16 StringToUnicode(const char* szText);

	static bool IsTextUtf8(const char* str, int length);

	bool LoadHtmlFromInnerHtml(LocalObject & objParent, const std::string& szhtml);
	void AddTouchEventObject(const HandleObject&obj);
	void RemoveTouchEventObject(const HandleObject&obj);


	//对所有可见的canvas的fbo进行渲染
    static void initGlobalFBO(int width, int height);
	void RenderCanvasFBO();






	static std::string GetHostName(const char* szURL, std::string&api, int&port);
	void ExecAsyncTask();
	void MeasureElements();
	std::string GetCookiePath();
	
    static GLuint s_framebuffer;
    static GLuint s_colorRenderbuffer;
    static GLuint s_depthRenderbuffer;

};


template<class TPARAM, class TRET>
class AsyncTask//主线程启动子线程执行任务，完成后回到主线程
{
	TPARAM m_param;
public:
	AsyncTask()
	{
	}
	virtual TRET doInBackground(TPARAM para) = 0;
	virtual void OnPostExecute(TRET Result) = 0;
#ifdef _WIN32
	static  DWORD __stdcall AsyncTaskThread(LPVOID lpThreadParameter)
#else 
	static void* AsyncTaskThread(void* lpThreadParameter)
#endif
	{
		AsyncTask*pthis = (AsyncTask*)lpThreadParameter;
		TRET re = pthis->doInBackground(pthis->m_param);
		class UIThread :public RunOnUIThread
		{
		public:
			AsyncTask*m_pTask;
			TRET m_ret;
			UIThread(AsyncTask*pTask, TRET ret)
			{
				m_pTask = pTask;
				m_ret = ret;
			}
			virtual void OnPostExecute()
			{
				m_pTask->OnPostExecute(m_ret);
			}
		};
		JSVM::GetInstance()->AddRunOnUIThread(new UIThread(pthis, re));
		return 0;
	}
	void Start(TPARAM para)
	{
		m_param = para;
#ifdef _WIN32
		CloseHandle(CreateThread(0, 0, AsyncTaskThread, this, 0, nullptr));
#else
		pthread_t ntid;
		pthread_create(&ntid, 0, AsyncTaskThread, this);
#endif
	}
};
