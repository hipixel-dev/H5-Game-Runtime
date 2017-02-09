#pragma once
#include "JSVMInterface.h"
#include <vector>
#include "HtmlStyle.h"

#define JSNODE_firstChild "firstChild"
#define JSNODE_previousSibling "previousSibling"
#define JSNODE_nextSibling "nextSibling"
#define JSNODE_lastChild "lastChild"
#define JSNODE_parentNode "parentNode"

//http://www.runoob.com/cssref/css-selectors.html
class QuerySelectorInfo
{
public:
	~QuerySelectorInfo()
	{
		for (int i = 0; i < attrList.size(); i++)
		{
			delete attrList[i];
		}
		attrList.clear();
	}

	std::string szTok1;
	std::string szTok2;

	enum _CSSTYPE
	{
		CS_UNKNOWN,
		CS_CLASS,
		CS_ID,
		CS_ALL,//选择所有元素
		CS_TAGNAME,//选择所有<p>元素
		CS_ALLTAGINTAG,//选择<div>元素内的所有<p>元素
		CS_TAGINTAG,//选择所有父级是 <div> 元素的 <p> 元素
		CS_TAGNEXTTAG,//选择所有紧接着<div>元素之后的<p>元素
		CS_TAGNEXTTAGS,//选择p元素之后的每一个ul元素
	};
	_CSSTYPE cssType = CS_UNKNOWN;
	enum _ATTRTYPE
	{
		AT_HAVEATTR,//有定义这个属性（忽略值）
		AT_EQUAL,//[attribute=value]
		AT_WITHLETTER,//~包含单词
		AT_LEQUAL,//^左匹配
		AT_REQUAL,//$右匹配
		AT_LIKE,//*包含子字符串

	};
	struct _ATTR
	{
		std::string szAttrName;
		std::string szAttrValue;
		_ATTRTYPE AttrType = AT_HAVEATTR;
	};
	std::vector<_ATTR*> attrList;
};
class QuerySelectorInfos
{
public:
	std::vector<QuerySelectorInfo*>m_infoList;
	~QuerySelectorInfos()
	{
		for (int i = 0; i < m_infoList.size(); i++)
		{
			delete m_infoList[i];
		}
		m_infoList.clear();
	}
};


class HtmlStyle;
class HtmlRect;
class HtmlElement :public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlElement, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("getElementById", &HtmlElement::getElementById, 1)
		DEFINE_CUSTOM_FUNCTION("getElementsByName", &HtmlElement::getElementsByName, 1)
		DEFINE_CUSTOM_FUNCTION("getElementsByTagName", &HtmlElement::getElementsByTagName, 1)
		DEFINE_CUSTOM_FUNCTION("appendChild", &HtmlElement::appendChild, 1)
		DEFINE_CUSTOM_FUNCTION("removeChild", &HtmlElement::removeChild, 1)
		DEFINE_CUSTOM_FUNCTION("insertBefore", &HtmlElement::insertBefore, 2)
		DEFINE_CUSTOM_FUNCTION("replaceChild", &HtmlElement::replaceChild, 2)
		DEFINE_CUSTOM_FUNCTION("hasChildNodes", &HtmlElement::hasChildNodes, 0)
		DEFINE_CUSTOM_FUNCTION("getAttribute", &HtmlElement::getAttribute, 1)
		DEFINE_CUSTOM_FUNCTION("setAttribute", &HtmlElement::setAttribute, 2)
		DEFINE_CUSTOM_FUNCTION("removeAttribute", &HtmlElement::removeAttribute, 1)
		DEFINE_CUSTOM_FUNCTION("hasAttribute", &HtmlElement::hasAttribute, 1)
		DEFINE_CUSTOM_FUNCTION("hasAttributes", &HtmlElement::hasAttributes, 0)
		DEFINE_CUSTOM_FUNCTION("querySelector", &HtmlElement::querySelector, 1)
		DEFINE_CUSTOM_FUNCTION("querySelectorAll", &HtmlElement::querySelectorAll,1)
		DEFINE_CUSTOM_FUNCTION("focus", &HtmlElement::focus, 0)
		DEFINE_CUSTOM_FUNCTION("cloneNode", &HtmlElement::cloneNode, 1)
		DEFINE_CUSTOM_FUNCTION("createTextNode", &HtmlElement::createTextNode, 1)

	END_CUSTOM_FUNCTION

	BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("documentElement")
		DEFINE_CUSTOM_PROPERTY("width")
		DEFINE_CUSTOM_PROPERTY("height")
		DEFINE_CUSTOM_PROPERTY("clientHeight")
		DEFINE_CUSTOM_PROPERTY("clientWidth")
		DEFINE_CUSTOM_PROPERTY("offsetWidth")
		DEFINE_CUSTOM_PROPERTY("offsetHeight")
		DEFINE_CUSTOM_PROPERTY("style")
		DEFINE_CUSTOM_PROPERTY("nodeName")
		DEFINE_CUSTOM_PROPERTY("tagName")
		DEFINE_CUSTOM_PROPERTY("id")
		DEFINE_CUSTOM_PROPERTY("innerHTML")
		DEFINE_CUSTOM_PROPERTY("innerText")
		DEFINE_CUSTOM_PROPERTY("nodeValue")
		DEFINE_CUSTOM_PROPERTY("baseURI")
		DEFINE_CUSTOM_PROPERTY("localName")
		DEFINE_CUSTOM_PROPERTY("namespaceURI")
		DEFINE_CUSTOM_PROPERTY("ownerDocument")
		DEFINE_CUSTOM_PROPERTY("prefix")
		DEFINE_CUSTOM_PROPERTY("textContent")
		DEFINE_CUSTOM_PROPERTY("text")
		DEFINE_CUSTOM_PROPERTY("xml")

		DEFINE_CUSTOM_PROPERTY("boundingClientRect")
	END_CUSTOM_PROPERTY


	std::map<std::string, std::string>m_AttributeMap;


	std::list<HtmlElement*>m_childList;//除了在JS中保存DOM树外，在C++对象中也保存，便于布局
	HtmlElement*m_pParent = nullptr;
	
	HtmlRect*m_bundingClientRect = nullptr;

	std::string m_szTagName;
	std::string m_szID;
	std::string m_szText;//innerText

	bool m_bNeedMeasure = true;//是否需要重新计算布局
	//单位为px的实际尺寸，坐标为屏幕坐标
	int m_left=0;
	int m_top=0;
	int m_width=0;
	int m_height=0;

	bool m_bSetWidth = false;
	bool m_bSetHeight = false;


	HtmlStyle* m_pStyle = nullptr;

	HtmlElement();
	virtual ~HtmlElement();


	virtual int GetLeft();
	virtual int GetTop();
	virtual int GetWidth();
	virtual int GetHeight();

	HtmlElement*GetParent();

	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args) ;
	virtual bool OnSetProperty(SetPropertyArgs&args) ;
	virtual void OnFinalize(FinalizeArgs&args) ;
	virtual bool OnConstructor(IFunctionCallbackArgs&args) ;
	virtual void OnAddToParent(const HandleContext&context,HtmlElement*pParent);//当被加入别的element时调用
	virtual void OnRemoveFromParent(const HandleContext&context,HtmlElement*pParent);//当被移除element时调用




	virtual void OnCloneNode(HtmlElement*pSrc);//克隆完JS对象后回调，在这里对C++对象的成员变量等数据进行克隆

	bool getElementById(IFunctionCallbackArgs&args);
	bool getElementsByName(IFunctionCallbackArgs&args);
	bool getElementsByTagName(IFunctionCallbackArgs&args);
	std::string &GetTagName();
	std::string &GetID();

	bool appendChild(IFunctionCallbackArgs&args);
	bool removeChild(IFunctionCallbackArgs&args);
	bool insertBefore(IFunctionCallbackArgs&args);
	bool replaceChild(IFunctionCallbackArgs&args);
	bool hasChildNodes(IFunctionCallbackArgs&args);
	bool getAttribute(IFunctionCallbackArgs&args);
	bool setAttribute(IFunctionCallbackArgs&args);
	bool removeAttribute(IFunctionCallbackArgs&args);
	bool hasAttribute(IFunctionCallbackArgs&args);
	bool hasAttributes(IFunctionCallbackArgs&args);
	bool querySelector(IFunctionCallbackArgs&args);
	bool querySelectorAll(IFunctionCallbackArgs&args);

	bool focus(IFunctionCallbackArgs&args);
	bool cloneNode(IFunctionCallbackArgs&args);
	bool createTextNode(IFunctionCallbackArgs&args);

	ValueBase GetFirstChild(const HandleContext& context);
	bool SetFirstChild(const HandleContext&context, const ValueBase&value);
	bool AppendChild(const HandleContext& context, const ValueBase&val);
	ValueBase GetLastChild(const HandleContext& context);
	bool InsertBefore(const HandleContext&context, ValueBase& vOld, ValueBase& vNew);
	bool RemoveChild(const HandleContext& context, const ValueBase& val);
	bool ReplaceChild(const HandleContext& context, const ValueBase& vOld, const ValueBase&vNew);
	HandleObject CloneNode(const HandleContext& context, bool bCloneChildren);
	bool SetLastChild(const HandleContext&context, const ValueBase&value);
	

	HandleObject FindChildByPropertyValue(const HandleContext&context, const std::string&szPropName, const std::string&value, bool bFindChildrens);//从子节点中查找指定属性值的第一个对象（前序遍历）
	
	int GetElementsByTagName(const HandleContext&context, const std::string &szName, ObjectList&objList);//查找所有对象
	void SetTagName(const HandleContext& context, const char * name);
	void SetNodeType(const HandleContext& context, int nodetype);
	void SetAttribute(const std::string&key, const std::string&value);
	virtual void OnSetAttribute(const HandleContext&context, const std::string&key, const std::string&value);//JS调用了SetAttribute后会触发OnSetAttribute，因为某引擎会使用setAttribute("style","position:absolute;top:0;left:0")方式设置style对象
	bool SetNodeValue(const HandleContext& context, const char * value);
	bool GenerateChildNodeArray(const HandleContext& context);//生成childNodes属性
	void SetStyle(const HandleContext& context,const char* value);
	int GetParentWidth(/*const HandleContext&context*/);
	int GetParentHeight(/*const HandleContext& context*/);


	HtmlStyle* GetStyle(/*const HandleContext&context*/);
	int GetStyledWidth(/*const HandleContext&context*/);//根据样式取得实际屏幕上显示的宽度，如style.width=100%,this.width=500px,parent.clientWidth=1000px，则返回1000*100%，this.width是元素的原始宽度，被拉伸成1000px
	int GetStyledHeight(/*const HandleContext&context*/);
	int GetStyledLeft(/*const HandleContext&context*/);
	int GetStyledTop(/*const HandleContext&context*/);
	int ParseQuerySelector(const char*szText, QuerySelectorInfos&info);
	std::string GetAttribute(const std::string &szAttrName);
	bool HasAttribute(const std::string &szAttrName);
	int QuerySelector(const HandleContext&context, const char*szString, LocalObjectArray&objList, bool bQueryOne);
	int QuerySelector(const HandleContext&context, LocalObject& objthis, QuerySelectorInfos &infos, LocalObjectArray&objList, bool bQueryOne);
	virtual bool SetBackgroundImage(const std::string &backgroundImage);
	virtual void OnSetStyle(const HandleContext&context, const char*szName);
	virtual int GetClientWidth();
	virtual int GetClientHeight();
	virtual void Measure(int parentX, int parentY, int parentWidth, int parentHeight);
	virtual void OnMeasure(int parentX, int parentY, int parentWidth, int parentHeight);


	HandleObject GetObjFromDomTree(const HandleContext&context);//从global开始通过dom树找到本JS对象
	bool RemoveAllChildren(const HandleContext&context);
};

