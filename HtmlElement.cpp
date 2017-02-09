#include "stdafx.h"
#include "HtmlElement.h"
#include "HtmlEventTarget.h"
#include <string>
#include "JSVMInterface.h"
#include "HtmlStyle.h"
#include "OpenglContext.h"
#include "HtmlTextNode.h"
#include "HtmlAttributes.h"
#include "HtmlCanvas.h"
#include "HtmlRect.h"
IMPLEMENT_CUSTOM_CLASS(HtmlElement, CustomClassBase)




HtmlElement::HtmlElement()
{
}


HtmlElement::~HtmlElement()
{
}

bool HtmlElement::OnGetProperty(GetPropertyArgs&args)
{
	//http://www.cnblogs.com/yuteng/articles/1894578.html
	const char* name = args.GetName();
	
	if (!PROPCMP(name, "documentElement"))
	{
		LocalObject obj(args.GetContext(), args.GetThisObject());
		while (true)
		{
			LocalObject parent(args.GetContext(), obj.GetProperty(args.GetContext(), JSNODE_parentNode).GetObject());
			if (parent.IsNull())
			{
				args.SetRetValue(obj);
				return true;
			}
			obj = parent;
		}
	}
	else if (!PROPCMP(name, "left"))
	{
		// set 0 temporary
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(name, "top"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(name, "width"))
	{
		assert(0);
	}
	else if(!PROPCMP(name, "clientWidth") || !PROPCMP(name, "offsetWidth"))
	{
		int width = GetClientWidth();
		args.SetRetValue(ValueBase().SetInt(width));
// 		if (m_width ==-1)
// 		{
// 			CustomClassAutoThisObj cc(this, args);
// 			int width = GetStyledWidth(/*args.GetContext()*/);//GetParentWidth(args.GetContext());
// 			assert(width >= 0);
// 			args.SetRetValue(ValueBase().SetInt(width));
// 		}
// 		else
// 			args.SetRetValue(ValueBase().SetInt(m_width));
// 		return true;

	}
	else if (!PROPCMP(name, "height"))
	{
		assert(0);
	}
	else if(!PROPCMP(name, "clientHeight") || !PROPCMP(name, "offsetHeight"))
	{
		int height = GetClientHeight();
		args.SetRetValue(ValueBase().SetInt(height));
// 		if (m_height == -1)
// 		{
// 			CustomClassAutoThisObj cc(this, args);
// 			int height = GetStyledHeight(/*args.GetContext()*/);//GetParentHeight(args.GetContext());
// 			assert(height >= 0);
// 			args.SetRetValue(ValueBase().SetInt(height));
// 		}
// 		else
// 			args.SetRetValue(ValueBase().SetInt(m_height));
 		return true;

	}
	else if (!PROPCMP(name, "tagName"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szTagName.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "nodeName"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szTagName.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "id"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szID.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "innerHTML"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szText.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "innerText"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szText.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "nodeValue"))
	{

		LocalValue ret(args.GetContext());
		ret.SetString(args.GetContext(), m_szText.c_str());
// 		if (m_szText.find("ip.png") != m_szText.npos)
// 		{
// 			std::string str = ret.GetString(args.GetContext());
// 			int ddd = 3;
// 		}
		args.SetRetValue(ret);
		return true;
	}
	else if (!PROPCMP(name, "boundingClientRect"))
	{
		assert(0);
	}

	//////////////////////////////////////////////////////////////////////////
	else if (!PROPCMP(name, "baseURI"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "localName"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "namespaceURI"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "ownerDocument"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "prefix"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "textContent"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szText.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "text"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szText.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "xml"))
	{
		assert(0);
	}








	return CustomClassBase::OnGetProperty(args);
}

bool HtmlElement::OnSetProperty(SetPropertyArgs&args)
{
	CustomClassAutoThisObj cc(this, args);
	const char*name = args.GetName();
	if (!PROPCMP(name, "left"))
	{
//		assert(0);
//		m_left = args.GetValue().GetInt();
//		return true;
	}
	else if (!PROPCMP(name, "top"))
	{
//		assert(0);
//		m_top = args.GetValue().GetInt();
//		return true;
	}
	else if (!PROPCMP(name, "width"))
	{
		assert(0);
// 		if(args.GetValue().IsNumber())
// 			m_width = args.GetValue().GetInt();
// 		else
// 		{
// 			HTMLCOORD hc;
// 			hc.SetValue(args.GetValue().GetString(args.GetContext()));
// 			if (hc.typ == CT_PX)m_width = hc.value;
// 			else if (hc.typ == CT_PERCENT)
// 			{
// 				m_width = GetParentWidth(/*args.GetContext()*/)*hc.value*0.01;
// 			}
// 		}
// 
// 		return true;
	}
	else if (!PROPCMP(name, "height"))
	{
		assert(0);
// 		if (args.GetValue().IsNumber())
// 			m_height = args.GetValue().GetInt();
// 		else
// 		{
// 			HTMLCOORD hc;
// 			hc.SetValue(args.GetValue().GetString(args.GetContext()));
// 			if (hc.typ == CT_PX)m_height = hc.value;
// 			else if (hc.typ == CT_PERCENT)
// 			{
// 				m_height = GetParentHeight(/*args.GetContext()*/)*hc.value*0.01;
// 			}
// 		}
// 		return true;
	}
	else if (!PROPCMP(name, "tagName")||!PROPCMP(name,"nodeName"))
	{
		m_szTagName = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "id"))
	{
		m_szID = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "innerHTML"))
	{
		m_szText = args.GetValue().GetString(args.GetContext());
		CustomClassAutoThisObj cc(this, args);
		RemoveAllChildren(args.GetContext());
		LocalObject objthis(args.GetContext(), GetJsObject());
		JSVM::GetInstance()->LoadHtmlFromInnerHtml(objthis, m_szText);
		return true;
	}
	else if (!PROPCMP(name, "innerText"))
	{
		m_szText = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "nodeValue"))
	{
		m_szText = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "boundingClientRect"))
	{
		assert(0);
	}

	//////////////////////////////////////////////////////////////////////////
	else if (!PROPCMP(name, "baseURI"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "localName"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "namespaceURI"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "ownerDocument"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "prefix"))
	{
		assert(0);
	}
	else if (!PROPCMP(name, "textContent"))
	{
		m_szText = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "text"))
	{
		m_szText = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "xml"))
	{
		assert(0);
	}
// 	else if (!PROPCMP(name, "style"))
// 	{
// 		HtmlStyle*pstyle = dynamic_cast<HtmlStyle*>(FindCustomClass(args.GetValue().GetObject()));
// 		if (pstyle&&pstyle->m_pParent==this)
// 		{
// 		}
// 		else
// 		{
// 			assert(0);
// 		}
// 	}
	return CustomClassBase::OnSetProperty(args);
}

void HtmlElement::OnFinalize(FinalizeArgs&args)
{
	return CustomClassBase::OnFinalize(args);
}

bool HtmlElement::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = CustomClassBase::OnConstructor(args);
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject objstyle(args.getContext(),HtmlStyle::CreateObject(args.getContext()));
	LocalObject objthis(args.getContext(), args.getReturnValue().GetObject());
	HtmlStyle*pStyle = (HtmlStyle*)FindCustomClass(objstyle);
	CustomClassAutoThisObj cc2(pStyle, objstyle);
	pStyle->SetParent(args.getContext(), this);
//	objthis.SetProperty(args.getContext(), "style", pStyle);




	objthis.SetProperty( "className", "");


	objthis.SetProperty("clientLeft", ValueBase().SetInt(0));
	objthis.SetProperty("clientTop", ValueBase().SetInt(0));
	objthis.SetProperty("clientHeight", ValueBase().SetInt(0));
	objthis.SetProperty("clientWidth", ValueBase().SetInt(0));

	LocalObject obj(args.getContext(),HtmlRect::CreateObject(args.getContext()));
	m_bundingClientRect = (HtmlRect*)FindCustomClass(obj);
	objthis.SetProperty("__boundingClientRect", obj);


	LocalObject objattr(args.getContext(),HtmlAttributes::CreateObject(args.getContext()));
	HtmlAttributes*pattr = (HtmlAttributes*)FindCustomClass(objattr);
	CustomClassAutoThisObj cc3(pattr, objattr);
	pattr->SetParent(args.getContext(), objthis);
	objthis.SetProperty("attributes", objattr);


// 	obj.SetProperty("left", ValueBase().SetInt(0));
// 	obj.SetProperty("top", ValueBase().SetInt(0));
// 	obj.SetProperty("width", ValueBase().SetInt(0));
// 	obj.SetProperty("height", ValueBase().SetInt(0));
	
	

	return ret;
}

bool HtmlElement::InitClass(const HandleContext&context)
{

	bool ret= InitClass_s(context, "HTMLElement", HandleObject());
	return ret;
}



bool HtmlElement::getElementsByName(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlElement::getElementsByTagName(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlElement::getElementsByTagName");
	std::string &&szName = args[0].GetString(args.getContext());
	ObjectList objList(args.getContext());
	int nElement = GetElementsByTagName(args.getContext(),szName, objList);
	LocalObject objret(args.getContext());
	objret.CreateArrayObject(args.getContext(), objList.objList.size());
	for (int i = 0; i < objList.objList.size(); i++)
	{
		objret.SetElement(args.getContext(), i, ValueBase(objList.objList[i]));
	}
	args.setReturnValue(objret);

	return true;
}
bool HtmlElement::RemoveAllChildren(const HandleContext&context)
{
	SetFirstChild(context, ValueBase());
	return GenerateChildNodeArray(context);
}



bool HtmlElement::appendChild(IFunctionCallbackArgs&args)
{

	CHECK_ARGS_COUNT(1, "HtmlElement::appendChild");
	LocalValue val(args.getContext(), args[0]);
	
	bool ret= AppendChild(args.getContext(),val);
	if (ret)args.setReturnValue(val);
	return ret;
}

bool HtmlElement::removeChild(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlElement::removeChild");

	const HandleContext&context = args.getContext();
	LocalValue val(context, args[0]);
	bool ret= RemoveChild(context, val);
	if (ret)args.setReturnValue(val);
	return ret;
}

bool HtmlElement::insertBefore(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlElement::insertBefore");
	LocalValue vNew(args.getContext(), args[0]);
	LocalValue vOld(args.getContext(), args[1]);

	bool ret= InsertBefore(args.getContext(), vOld, vNew);
	if (ret)args.setReturnValue(vNew);
	return ret;

}

bool HtmlElement::replaceChild(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlElement::replaceChild");

	bool ret= ReplaceChild(args.getContext(), args[0], args[1]);
	if (ret)args.setReturnValue(args[0]);
	return ret;
}

bool HtmlElement::hasChildNodes(IFunctionCallbackArgs&args)
{
	if (GetFirstChild(args.getContext()).IsUndefined())
	{
		args.setReturnValue(ValueBase().SetBoolean(true));
	}
	else args.setReturnValue(ValueBase().SetBoolean(false));
	return true;
}

bool HtmlElement::getAttribute(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlElement::getAttribute");
	std::string &&key = args[0].GetString(args.getContext());
// 	if (key == "width")
// 	{
// 		int dd = 3;
// 	}
// 	if (key == "tilewidth")
// 	{
// 		int dd = 3;
// 	}
	if (!HasAttribute(key))
	{
		args.setReturnValue(ValueBase().SetNull());
	}
	else
	{
		LocalValue ret(args.getContext());
		ret.SetString(args.getContext(),GetAttribute(key).c_str());
		args.setReturnValue(ret);
	}
	return true;
}

bool HtmlElement::setAttribute(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlElement::setAttribute");
	std::string &&key = args[0].GetString(args.getContext());
	std::string &&value = args[1].GetString(args.getContext());
	SetAttribute(key, value);
	OnSetAttribute(args.getContext(),key, value);
	return true;
}
void HtmlElement::SetAttribute(const std::string&key, const std::string&value)
{
	m_AttributeMap[key] = value;

}
void HtmlElement::OnSetAttribute(const HandleContext&context,const std::string&key, const std::string&value)
{
	if (key == "style")
	{
		HtmlStyle*pStyle = GetStyle(/*context*/);
		pStyle->SetStyle(context, value.c_str(), this);
	}
	else 
	{
		LocalObject objthis(context, GetJsObject());
		objthis.SetProperty(key.c_str(), value.c_str());
	}
}



bool HtmlElement::removeAttribute(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlElement::removeAttribute");
	std::string &&key = args[0].GetString(args.getContext());
	m_AttributeMap.erase(key);
	return true;
}

bool HtmlElement::hasAttribute(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlElement::hasAttribute");
	std::string &&key = args[0].GetString(args.getContext());
	bool bHas = HasAttribute(key);

	args.setReturnValue(ValueBase().SetBoolean(bHas));
	return true;
}

bool HtmlElement::hasAttributes(IFunctionCallbackArgs&args)
{
	LocalValue ret(args.getContext());
	if (m_AttributeMap.size() > 0)ret.SetBoolean(true);
	else ret.SetBoolean(false);
	args.setReturnValue(ret);
	return true;
}
std::string GetToken(char*&p,std::string&tok)// p points start point of next token, return string before tokchar
{
	tok.empty();
	static const char*tokop = "~!@#$%^&*()_+=,.<>/?;:[]{}\\|";
	char*q = p;
	std::string ret;
	char*t = 0;
	char str[100];
	char*lQuote = 0;
	char*rQuote = 0;
	char quotType = 0;



	while (*p)
	{
		if (!lQuote)
		{
			if (strchr(tokop, *p))
			{
				if (!t)
				{
					if (ret.empty())
					{
						memcpy(str, q, p - q);
						str[p - q] = 0;
						ret = str;
					}
					t = p;// operator start
					if (*p == '['||*p==']')
					{
						p++;
						memcpy(str, t, p - t);
						str[p - t] = 0;
						tok = str;
						return ret;
					}
				}

			}
			else if (*p == '\"'||*p=='\'')
			{// comma start
				if (p != q)
				{
					if (t)
					{
						memcpy(str, t, p - t);
						str[p - t] = 0;
						tok = str;
						return ret;
					}
				}
				lQuote = p;
				quotType = *p;
			}
			else
			{
				if (t)
				{
					memcpy(str, t, p - t);
					str[p - t] = 0;
					tok = str;
					return ret;
				}
			}
		}
		else
		{
			if (*p == quotType)
			{// comma end
				rQuote = p;
				memcpy(str, lQuote+1,rQuote-lQuote-1);
				str[rQuote - lQuote - 1] = 0;
				ret = str;
				lQuote = rQuote = 0;
				q = p + 1;
			}
		}
		p++;
	}
	return q;
}

extern char*TrimLeft(char*p);

int HtmlElement::ParseQuerySelector(const char*szText,QuerySelectorInfos&info)
{
	char*pbuf = _strdup(szText);
	int nRet = 0;
	char*selctx = 0;

	char*pSel = strtok_s(pbuf, ",", &selctx);
	while (pSel)
	{
		QuerySelectorInfo*qinfo = nullptr;
		pSel=TrimLeft(pSel);
		if (pSel[0] == '.')//.class
		{
			qinfo = new QuerySelectorInfo;
			qinfo->cssType = QuerySelectorInfo::CS_CLASS;
			qinfo->szTok1 = pSel + 1;
			info.m_infoList.push_back(qinfo);
			nRet++;;
		}
		else if (pSel[0] == '#')//#id
		{
			qinfo = new QuerySelectorInfo;
			qinfo->cssType = QuerySelectorInfo::CS_ID;
			qinfo->szTok1 = pSel + 1;
			info.m_infoList.push_back(qinfo);
			nRet++;;
		}
		else if (pSel[0] == '*')//all
		{
			qinfo = new QuerySelectorInfo;
			qinfo->cssType = QuerySelectorInfo::CS_ALL;
			info.m_infoList.push_back(qinfo);
			nRet++;;
		}
		else if (pSel[0] == '[')//Êattribute
		{
			QuerySelectorInfo::_ATTR*pattr = new QuerySelectorInfo::_ATTR;
			if (!qinfo)
			{
				qinfo = new QuerySelectorInfo;
			}
			qinfo->attrList.push_back(pattr);
			pSel++;
			pSel = TrimLeft(pSel);
			char*opstart = 0;
			char*opend = 0;
			char szOP[20]="";
			char*p = pSel;
			bool bGetOP = false;

			while (*p)
			{
				switch (*p)
				{
				case ']':
					if (!opend)opend = p;
					*p = 0;
					if (opend)
					{
						pattr->szAttrValue = opend + 1;
					}
					break;
				case '=':
					if (!opstart)opstart = p;
					break;
				case '~':
					if (!opstart)opstart = p;
					break;
				case '|':
					if (!opstart)opstart = p;
					break;
				case '^':
					if (!opstart)opstart = p;
					break;
				case '$':
					if (!opstart)opstart = p;
					break;
				case '*':
					if (!opstart)opstart = p;
					break;
				default:
					if (!opend)opend = p;
					break;
				}
				if (opstart&&opend&&!bGetOP)
				{
					bGetOP = true;
					strncpy(szOP,  opstart, opend - opstart);
					szOP[opend - opstart] = 0;
					opstart = 0;
					pattr->szAttrName = pSel;
					if (szOP[0] == 0)// choose all elements with target attribute
					{
						pattr->AttrType = QuerySelectorInfo::AT_HAVEATTR;
					}
					else if (!strcmp(szOP, "="))// choose all elements use target="-blank"
					{
						pattr->AttrType = QuerySelectorInfo::AT_EQUAL;
					}
					else if (!strcmp(szOP, "~="))//choose all elements whose title attribute including word "flower"
					{
						pattr->AttrType = QuerySelectorInfo::AT_WITHLETTER;
					}
					else if (!strcmp(szOP, "|="))// choose all elements whose lang attribute start with "EN"
					{
						assert(0);
						pattr->AttrType = QuerySelectorInfo::AT_LEQUAL;
					}
					// what's the difference?
					else if (!strcmp(szOP, "^="))// choose all elements whose src attribute start with "https"
					{
						assert(0);
						pattr->AttrType = QuerySelectorInfo::AT_LEQUAL;
					}
					else if (!strcmp(szOP, "$="))
					{
						assert(0);
					}
					else if (!strcmp(szOP, "*="))
					{
						assert(0);
					}	
					else 
						assert(0);
				}
				p++;
			}
		}
		else if (pSel[0] == ':')
		{
		}
		else
		{
			qinfo = new QuerySelectorInfo;
			info.m_infoList.push_back(qinfo);
			nRet++;;
			char*p = pSel;
			while (*p)
			{
				if (*p == ' ')
				{
					if (qinfo->szTok1.length() == 0)
					{
						*p = 0;
						qinfo->szTok1 = pSel;
					}
				}
				else if (*p == '>')
				{
					if (qinfo->szTok1.length() == 0)
					{
						*p = 0;
						qinfo->szTok1 = pSel;
					}
					qinfo->cssType = QuerySelectorInfo::CS_TAGINTAG;
					
				}
				else if(*p=='+')
				{
					if (qinfo->szTok1.length() == 0)
					{
						*p = 0;
						qinfo->szTok1 = pSel;
					}
					
					qinfo->cssType = QuerySelectorInfo::CS_TAGNEXTTAG;
				}
				else if (*p == '~')
				{
					if (qinfo->szTok1.length() == 0)
					{
						*p = 0;
						qinfo->szTok1 = pSel;
					}

					qinfo->cssType = QuerySelectorInfo::CS_TAGNEXTTAGS;
				}
				else
				{
					if (qinfo->szTok1.length() > 0)
					{
						if (qinfo->cssType == QuerySelectorInfo::CS_UNKNOWN)
						{
							qinfo->cssType = QuerySelectorInfo::CS_ALLTAGINTAG;
						}
						else
						{
							qinfo->szTok2 = p;
							break;
						}
					}
				}
				p++;
				
			}
			if (qinfo->cssType == QuerySelectorInfo::CS_UNKNOWN&&qinfo->szTok1.empty())
			{
				qinfo->cssType = QuerySelectorInfo::CS_TAGNAME;
				qinfo->szTok1 = pSel;
			}
		}
		pSel = strtok_s(0, ",", &selctx);
	}




// 	char*pSel = strtok_s(pbuf, ", >+", &selctx);
// 	std::string tok;
// 	while (pSel)
// 	{
// 		char*p = pSel;
// 		int idx = pbuf - pSel;
// 		if (idx>0)
// 			assert(szText[idx] == ',');//TODO:
// 
// 
// 		QuerySelectorInfo*pinfo = new QuerySelectorInfo;
// 		info.m_infoList.push_back(pinfo);
// 		nRet++;
// 		std::string prevtok = "";
// 
// 		QuerySelectorInfo::_ATTR*pAttr = nullptr;
// 		while (true)
// 		{
// 			std::string tokstr = GetToken(p,tok);
// 			if (prevtok == "")
// 			{
// 				pinfo->szTag = tokstr;
// 			}
// 			else if (prevtok == ".")
// 			{
// 				pinfo->szClass = tokstr;
// 			}
// 			else if (prevtok == "#")
// 			{
// 				pinfo->szID = tokstr;
// 			}
// 			else if (prevtok == "[")
// 			{
// 				if (pAttr)
// 				{
// 					pAttr->szAttrName = tokstr;
// 				}
// 				else assert(0);
// 			}
// 			else if (prevtok == "=")
// 			{
// 				if (pAttr)
// 				{
// 					pAttr->szAttrValue = tokstr;
// 					pAttr->AttrType = QuerySelectorInfo::AT_EQUAL;
// 				}
// 				else assert(0);
// 			}
// 			else if (prevtok == "~=")
// 			{
// 				if (pAttr)
// 				{
// 					pAttr->szAttrValue = tokstr;
// 					pAttr->AttrType = QuerySelectorInfo::AT_HAVEATTR;
// 				}
// 				else assert(0);
// 			}
// 			else if (prevtok == "|=")
// 			{
// 				if (pAttr)
// 				{
// 
// 					pAttr->szAttrValue = tokstr;
// 					pAttr->AttrType = QuerySelectorInfo::AT_LEQUAL;
// 
// 				}
// 				else assert(0);
// 			}
// 			
// 			prevtok = tok;
// 
// 			if (prevtok == "[")
// 			{
// 				pAttr = new QuerySelectorInfo::_ATTR;
// 				pinfo->attrList.push_back(pAttr);
// 			}
// 			else if (prevtok == "]")
// 			{
// 				pAttr = nullptr;
// 			}
// 			if (!*p)break;
// 		}
// 	next:
// 		pSel = strtok_s(nullptr, ",", &selctx);
// 	}
// 
// 



	free(pbuf);
	return nRet;
}
bool CheckSelectorAttrib(QuerySelectorInfo*info, LocalObject& objChild)
{
	HtmlElement*pChild = dynamic_cast<HtmlElement*>(CustomClassBase::FindCustomClass(objChild));
	assert(pChild);
	CustomClassAutoThisObj cc(pChild, objChild);
	for (int j = 0; j < info->attrList.size(); j++)
	{
		QuerySelectorInfo::_ATTR*attr = info->attrList[j];

		switch (attr->AttrType)
		{
		case QuerySelectorInfo::AT_EQUAL:
		{
			if (!pChild->HasAttribute(attr->szAttrName))
			{
				return false;
			}
			std::string attrval = pChild->GetAttribute(attr->szAttrName);
			if (attrval != attr->szAttrValue)
			{
				return false;
			}
			break;
		}
		case QuerySelectorInfo::AT_HAVEATTR:
		{
			if (!pChild->HasAttribute(attr->szAttrName))
			{
				return false;
			}
			break;
		}
		default:
			assert(0);
			break;
		}
	}
	return true;
}

int HtmlElement::QuerySelector(const HandleContext&context, LocalObject& objthis,QuerySelectorInfos &infos, LocalObjectArray&objList, bool bQueryOne)
{
	int nRet = 0;
	for (int i = 0; i < infos.m_infoList.size(); i++)
	{
		QuerySelectorInfo*info = infos.m_infoList[i];
		switch (info->cssType)
		{
		case QuerySelectorInfo::CS_TAGNAME:
		{
			LocalObject objchild(context, objthis.GetProperty(JSNODE_firstChild).GetObject());
			while(!objchild.IsNull())
			{
				HtmlElement*pele = dynamic_cast<HtmlElement*>(FindCustomClass(objchild));
				if (!stricmp(pele->GetTagName().c_str(), info->szTok1.c_str()))
				{
					objList.append(objchild);
					nRet++;
					if (bQueryOne)return nRet;
					
				}
				int ret = pele->QuerySelector(context, objchild, infos, objList, bQueryOne);
				nRet += ret;
				if (bQueryOne&&ret)return nRet;
				objchild = objchild.GetProperty(JSNODE_nextSibling).GetObject();
			}
			break;
		}
		case QuerySelectorInfo::CS_ID:
		{
			LocalObject objchild(context, objthis.GetProperty(JSNODE_firstChild).GetObject());
			while (!objchild.IsNull())
			{
				HtmlElement*pele = dynamic_cast<HtmlElement*>(FindCustomClass(objchild));
				if (!stricmp(pele->GetID().c_str(), info->szTok1.c_str()))
				{
					objList.append(objchild);
					nRet++;
					if (bQueryOne)return nRet;
				}
				int ret = pele->QuerySelector(context, objchild, infos, objList, bQueryOne);
				nRet += ret;
				if (bQueryOne&&ret)return nRet;
				objchild = objchild.GetProperty(JSNODE_nextSibling).GetObject();
			}
			break;
		}
		case QuerySelectorInfo::CS_CLASS:
		{
			assert(0);
			break;
		}
		case QuerySelectorInfo::CS_TAGINTAG:
		{
			ObjectList objlist(context);
			int nObj=GetElementsByTagName(context, info->szTok1, objlist);
			for (int i = 0; i < nObj; i++)
			{
				LocalObject objitem(context, objlist.objList[i]);
				HtmlElement*pele = dynamic_cast<HtmlElement*>(FindCustomClass(objitem));
				if (!stricmp(pele->GetTagName().c_str(), info->szTok2.c_str()))
				{
					objList.append(objitem);
					nRet++;
				}
			}
			break;
		}
		case QuerySelectorInfo::CS_ALL:
		{
			LocalObject objchild(context, objthis.GetProperty(JSNODE_firstChild).GetObject());
			while (!objchild.IsNull())
			{
				HtmlElement*pele = dynamic_cast<HtmlElement*>(FindCustomClass(objchild));
				objList.append(objchild);
				nRet++;
				if (bQueryOne)return nRet;
				CustomClassAutoThisObj cc(pele, objchild);
				int ret = pele->QuerySelector(context, objchild, infos, objList, bQueryOne);
				nRet += ret;
				if (bQueryOne&&ret)return nRet;
				objchild = objchild.GetProperty(JSNODE_nextSibling).GetObject();
			}
			break;
		}
		default:
			assert(0);
			break;
		}
	}

// 	// search for tag
// 	LocalObject objChild(context, objthis.GetProperty(JSNODE_firstChild).GetObject());
// 	while (!objChild.IsNull())
// 	{
// 		std::string &&sztag = objChild.GetProperty("tagName").GetString(context);
// 		std::string &&szID = objChild.GetProperty("id").GetString(context);
// 		for (int i = 0; i < infos.m_infoList.size(); i++)
// 		{
// 			QuerySelectorInfo*info = infos.m_infoList[i];
// 		
// 			if (!info->szTag.empty())
// 			{
// 				if (!_stricmp(sztag.c_str(), info->szTag.c_str()))
// 				{
// 					if (info->tag2Type == QuerySelectorInfo::TT_UNKNOWN)
// 					{
// 						//
// 						if (CheckSelectorAttrib(info, objChild))
// 						{
// 							objList.append(objChild);
// 							nRet++;
// 
// 							if (bQueryOne)
// 								return nRet;
// 						}
// 
// 					}
// 					else assert(0);
// 				}
// 			}
// 			else if (!info->szID.empty())
// 			{
// 				if (szID == info->szID)
// 				{
// 					if (CheckSelectorAttrib(info, objChild))
// 					{
// 						objList.append(objChild);
// 						nRet++;
// 
// 						if (bQueryOne)
// 							return nRet;
// 					}
// 
// 				}
// 			}
// 			else
// 			{
// 				assert(0);
// 				return 0;
// 			}
// 		}
// 		HtmlElement*pChild = dynamic_cast<HtmlElement*>(FindCustomClass(objChild));
// 		if (pChild)
// 		{
// 			CustomClassAutoThisObj cc(pChild, objChild);
// 			int n = pChild->QuerySelector(context, objChild, infos, objList, bQueryOne);
// 			nRet += n;
// 			if (bQueryOne&&n > 0)break;
// 		}
// 		objChild = objChild.GetProperty(JSNODE_nextSibling).GetObject();
// 	}
	return nRet;
}

int HtmlElement::QuerySelector(const HandleContext&context,const char*szString,LocalObjectArray&objList, bool bQueryOne)
{
	LocalObject objthis(context, GetJsObject());

	QuerySelectorInfos infos;
	int nSel = ParseQuerySelector(szString, infos);


	if (nSel == 0)
	{
		return 0;
	}
	int nRet = QuerySelector(context, objthis, infos, objList, bQueryOne);

	
	return nRet;
}

bool HtmlElement::querySelector(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "querySelector");

	std::string &&str = args[0].GetString(args.getContext());
	LocalObject objthis(args.getContext(), GetJsObject());

	LocalObject objret(args.getContext());
	objret.CreateArrayObject(0);
	LocalObjectArray objarray(args.getContext());
	int nCount = QuerySelector(args.getContext(), str.c_str(), objarray, true);
	if (nCount == 0)
		args.setReturnValue(ValueBase().SetNull());
	else
		args.setReturnValue(objarray[0]);

	return true;
}

bool HtmlElement::querySelectorAll(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "querySelectorAll");
	std::string &&str = args[0].GetString(args.getContext());
	LocalObject objthis(args.getContext(), GetJsObject());

	LocalObject objret(args.getContext());
	objret.CreateArrayObject(0);
	LocalObjectArray objarray(args.getContext());
	int nCount = QuerySelector(args.getContext(), str.c_str(), objarray, false);
	for (int i = 0; i < nCount; i++)
	{
		objret.SetElement(i, objarray[i]);
	}
	args.setReturnValue(objret);

	return true;
}

int HtmlElement::GetLeft()
{
	return m_left;

}

int HtmlElement::GetTop()
{
	return m_top;
}

int HtmlElement::GetWidth()
{
	return m_width;
}

int HtmlElement::GetHeight()
{
	return m_height;
}

bool HtmlElement::focus(IFunctionCallbackArgs&args)
{
	return true;
}

bool HtmlElement::cloneNode(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlElement::cloneNode");
	LocalValue vRet(args.getContext(), CloneNode(args.getContext(), args[0].GetBoolean()));
	args.setReturnValue(vRet);
	return true;
}

bool HtmlElement::getElementById(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlDocument::getElementById");
	std::string &&szid = args[0].GetString(args.getContext());
	LocalObject obj(args.getContext(), FindChildByPropertyValue(args.getContext(), "id", szid, true));
	if (obj.IsNull())
		args.setReturnValue(ValueBase().SetNull());
	else
		args.setReturnValue(ValueBase(obj));

	return true;
}

ValueBase HtmlElement::GetFirstChild(const HandleContext& context)
{
	LocalObject thisobj(context, GetJsObject());
	return thisobj.GetProperty(context, JSNODE_firstChild);
}

bool HtmlElement::SetFirstChild(const HandleContext&context, const ValueBase&value)
{
	LocalObject thisobj(context, GetJsObject());
	return thisobj.SetProperty(context, JSNODE_firstChild, value);
	
}

bool HtmlElement::AppendChild(const HandleContext& context, const ValueBase&val)
{
	if (!val.IsObject())
	{
		ReportError(context,"AppendChild MUST be object");
		return false;
	}
	

	LocalObject thisobj(context, GetJsObject());
	LocalObject oVal(context, val.GetObject());
	HtmlElement*pval =dynamic_cast<HtmlElement*>( FindCustomClass(oVal));
	assert(pval);
	HtmlTextNode*pValTextNode = dynamic_cast<HtmlTextNode*>(pval);
	if (pValTextNode)//Êis text
	{
		thisobj.SetProperty("innerText", LocalValue(context).SetString(context,pValTextNode->m_szText.c_str()));
		
	}
	


	LocalValue vFirstChild(context,GetFirstChild(context));
	LocalObject oValParent(context, oVal.GetProperty(JSNODE_parentNode).GetObject());
	if (!oValParent.IsNull())// if node to insert has parent, then remove from parent first
	{
		HtmlElement*pParent = dynamic_cast<HtmlElement*>(FindCustomClass(oValParent));
		if (pParent)
		{
			CustomClassAutoThisObj cc(pParent, oValParent);
			pParent->RemoveChild(context, val);
		}
	}
	if (vFirstChild.IsUndefined())
	{
		oVal.SetProperty(context, JSNODE_previousSibling, ValueBase());
		oVal.SetProperty(context, JSNODE_nextSibling, ValueBase());
		oVal.SetProperty(context, JSNODE_parentNode, ValueBase(thisobj));
		SetFirstChild(context, ValueBase(oVal));
		SetLastChild(context, ValueBase(oVal));

		pval->m_pParent = this;
		m_childList.push_back(pval);
		GenerateChildNodeArray(context);
		pval->OnAddToParent(context,this);
		
		
		return true;
	}
	else
	{
		LocalValue vLast(context, GetLastChild(context));
		if (!vLast.IsObject())
		{
			ReportError(context, "nextSibling MUST be object");
			return false;
		}
		LocalObject oLast(context, vLast.GetObject());
		oLast.SetProperty(context, JSNODE_nextSibling, val);
		oVal.SetProperty(context, JSNODE_previousSibling, vLast);
		oVal.SetProperty(context, JSNODE_nextSibling, ValueBase());
		oVal.SetProperty(context, JSNODE_parentNode, ValueBase(thisobj));
		SetLastChild(context, ValueBase(oVal));
		pval->m_pParent = this;
		m_childList.push_back(pval);
		GenerateChildNodeArray(context);
		
		pval->OnAddToParent(context, this);
		
		return true;
	}
}

ValueBase HtmlElement::GetLastChild(const HandleContext& context)
{
	LocalObject thisobj(context, GetJsObject());
	LocalValue vRet(context, thisobj.GetProperty(context, JSNODE_lastChild));
	if (vRet.IsUndefined())
	{
		LocalObject thisobj(context, GetJsObject());
		LocalObject obj(context, thisobj.GetProperty(context, JSNODE_firstChild).GetObject());
		if (obj.IsNull())return ValueBase();
		while (true)
		{
			LocalObject objNext(context, obj.GetProperty(context, JSNODE_nextSibling).GetObject());
			if (objNext.IsNull())return obj;
			obj = objNext;
		}
	}
	return vRet;
}

bool HtmlElement::InsertBefore(const HandleContext&context, ValueBase& vOld, ValueBase& vNew)
{

	if (!vNew.IsObject())
	{
		ReportError(context, "element to insertBefore is not a object");
		return false;
	}
	LocalObject firstobj(context, GetFirstChild(context).GetObject());
	LocalObject oldobj(context, vOld.GetObject());
	LocalObject newobj(context, vNew.GetObject());
	HtmlElement*pvalElementNew = dynamic_cast<HtmlElement*>(this->FindCustomClass(newobj));
	assert(pvalElementNew);


	LocalObject oValParent(context, newobj.GetProperty(JSNODE_parentNode).GetObject());
	if (!oValParent.IsNull())//if node to insert has parent, then remove from parent first
	{
		HtmlElement*pParent = dynamic_cast<HtmlElement*>(FindCustomClass(oValParent));
		if (pParent)
		{
			CustomClassAutoThisObj cc(pParent, oValParent);
			pParent->RemoveChild(context, vNew);
		}
	}



	if (oldobj.IsNull())// insert to the front
	{
		if (!firstobj.IsNull())
		{
			firstobj.SetProperty(JSNODE_previousSibling, vNew);
			newobj.SetProperty(JSNODE_nextSibling, firstobj);
		}
		else
		{
			newobj.SetProperty(JSNODE_nextSibling, ValueBase());
		}
		newobj.SetProperty(JSNODE_previousSibling, ValueBase());
		newobj.SetProperty(JSNODE_parentNode, GetJsObject());
		SetFirstChild(context, newobj);
		pvalElementNew->m_pParent = this;
		m_childList.push_front(pvalElementNew);
		GenerateChildNodeArray(context);
		return true;

	}

	

	LocalObject oldparent(context, oldobj.GetProperty(context, JSNODE_parentNode).GetObject());
	if (oldparent != GetJsObject())
	{
		ReportError(context, "old parent is different with the node");
		return false;
	}
	if (firstobj.IsNull())
	{
		ReportError(context, "no child node, check inputs");
		return false;
	}

	

	if (firstobj == oldobj)
	{
		SetFirstChild(context, vNew);
		newobj.SetProperty(context, JSNODE_previousSibling, ValueBase());
		newobj.SetProperty(context, JSNODE_parentNode, ValueBase(oldparent));
		newobj.SetProperty(context, JSNODE_nextSibling, vOld);
		oldobj.SetProperty(context, JSNODE_previousSibling, vNew);
	}
	else
	{
		LocalObject prevObj(context, oldobj.GetProperty(context, JSNODE_previousSibling).GetObject());
		if (prevObj.IsNull())
		{
			ReportError(context, "attribution of previousSibling is wrong");
			return false;
		}
		prevObj.SetProperty(context, JSNODE_nextSibling, vNew);
		oldobj.SetProperty(context, JSNODE_previousSibling, vNew);
		newobj.SetProperty(context, JSNODE_previousSibling, ValueBase(prevObj));
		newobj.SetProperty(context, JSNODE_nextSibling, ValueBase(oldobj));
		newobj.SetProperty(context, JSNODE_parentNode, ValueBase( oldparent));
	}

	pvalElementNew->m_pParent = this;

	HtmlElement*pvalElementOld = dynamic_cast<HtmlElement*>(FindCustomClass(oldobj));
	
	for (std::list<HtmlElement*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it)
	{
		if (*it == pvalElementOld)
		{
			m_childList.insert(it, pvalElementNew);
			break;
		}
	}


	GenerateChildNodeArray(context);
	

	if (pvalElementNew)
	{
		pvalElementNew->OnAddToParent(context, this);
	}

	return true;
}

bool HtmlElement::RemoveChild(const HandleContext& context, const ValueBase& val)
{
	if (!val.IsObject())
	{
		ReportError(context, "removeChild must be object");
		return false;
	}
	LocalObject obj(context, val.GetObject());
	LocalObject oParent(context, obj.GetProperty(context, JSNODE_parentNode).GetObject());
	if (oParent.IsNull())
	{
		return true;
// 		ReportError(context, "parent of removeChild do not exist");
// 		return false;
	}
	HtmlElement*pvalElement = dynamic_cast<HtmlElement*>(FindCustomClass(obj));
	assert(pvalElement);

	LocalObject objPrev(context, obj.GetProperty(context, JSNODE_previousSibling).GetObject());
	LocalObject objNext(context, obj.GetProperty(context, JSNODE_nextSibling).GetObject());


	LocalObject oParentFirstChild(context, oParent.GetProperty(context, JSNODE_firstChild).GetObject());
	if (oParentFirstChild == obj)// remove the first node
	{
		oParent.SetProperty(context, JSNODE_firstChild, ValueBase( objNext));
	}
	LocalObject oParentLastChild(context, oParent.GetProperty(context, JSNODE_lastChild).GetObject());
	if (oParentLastChild == obj)// remove the last node
	{
		oParent.SetProperty(context, JSNODE_lastChild, ValueBase( objPrev));
		objPrev.SetProperty(context, JSNODE_nextSibling, ValueBase(objNext));
	
	}


	if (!objPrev.IsNull())
	{
		objPrev.SetProperty(context, JSNODE_nextSibling, ValueBase(objNext));
	}
	if (!objNext.IsNull())
	{
		objNext.SetProperty(context, JSNODE_previousSibling, ValueBase(objPrev));
	}

	obj.SetProperty(context, JSNODE_parentNode, ValueBase( ));
	obj.SetProperty(context, JSNODE_previousSibling, ValueBase());
	obj.SetProperty(context, JSNODE_nextSibling, ValueBase());

	pvalElement->m_pParent = nullptr;
	m_childList.remove(pvalElement);
	GenerateChildNodeArray(context);
	pvalElement->OnRemoveFromParent(context, this);
	
	
	return true;
}

bool HtmlElement::ReplaceChild(const HandleContext& context, const ValueBase& vOld, const ValueBase&vNew)
{
	LocalObject objOld(context, vOld.GetObject());
	if (objOld.IsNull())
	{
		ReportError(context, "ReplaceChild: the old node must be object");
		return false;
	}
	LocalObject objNew(context, vNew.GetObject());
	if (objNew.IsNull())
	{
		ReportError(context, "ReplaceChild: the new node must be object");
		return false;
	}

	LocalObject objParentOld(context, objOld.GetProperty(context, JSNODE_parentNode).GetObject());
	if (objParentOld.IsNull())
	{
		ReportError(context, "ReplaceChild: the old node don't have parent");
		return false;
	}
	LocalObject objParentOldFirstChild(context,objParentOld.GetProperty(context, JSNODE_firstChild).GetObject());
	if (objParentOldFirstChild == objOld)// replace the first child node
	{
		objParentOld.SetProperty(context, JSNODE_firstChild, vNew);
	}
	LocalObject objParentOldLastChild(context, objParentOld.GetProperty(context, JSNODE_lastChild).GetObject());
	if (objParentOldLastChild == objOld)// replace the last child node
	{
		objParentOld.SetProperty(context, JSNODE_lastChild, vNew);
	}
	objNew.SetProperty(context, JSNODE_parentNode, ValueBase(objParentOld));
	objNew.SetProperty(context, JSNODE_previousSibling, objOld.GetProperty(context, JSNODE_previousSibling));
	objNew.SetProperty(context, JSNODE_nextSibling, objOld.GetProperty(context, JSNODE_nextSibling));
	objOld.SetProperty(context, JSNODE_parentNode, ValueBase());
	objOld.SetProperty(context, JSNODE_previousSibling, ValueBase());
	objOld.SetProperty(context, JSNODE_nextSibling, ValueBase());

	
	HtmlElement*pvalElementOld = dynamic_cast<HtmlElement*>(FindCustomClass(objOld));
	assert(pvalElementOld);
	HtmlElement*pvalElementNew = dynamic_cast<HtmlElement*>(FindCustomClass(objNew));
	assert(pvalElementNew);
	pvalElementOld->m_pParent = nullptr;
	pvalElementNew->m_pParent = this;
	for (std::list<HtmlElement*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it)
	{
		if (*it == pvalElementOld)
		{
			*it = pvalElementNew;
			break;
		}
	}


	GenerateChildNodeArray(context);
	
	pvalElementOld->OnRemoveFromParent(context, this);

	
	pvalElementNew->OnAddToParent(context, this);
	
	
	return true;
}

HandleObject HtmlElement::CloneNode(const HandleContext& context, bool bCloneChildren)
{
	LocalObject objNew(context, CreateThisObject(context));
	HtmlElement*pNewElement = (HtmlElement*)objNew.GetNativeObj();
	LocalObject objThis(context, GetJsObject());
	CustomClassAutoThisObj cc(pNewElement, objThis);


	if (bCloneChildren)
	{
		LocalObject objChild(context, GetFirstChild(context).GetObject());
		while (!objChild.IsNull())
		{
			
			if (objChild.IsNull())
			{
				break;
			}
			HtmlElement*pChildElement = dynamic_cast<HtmlElement*>(FindCustomClass(objChild));
			assert(pChildElement);
			LocalObject objChildNew(context, pChildElement->CloneNode(context, true));

			pNewElement->AppendChild(context, ValueBase(objChildNew));
			


			objChild=objChild.GetProperty(context, JSNODE_nextSibling).GetObject();
		}
	}

	//copy attributions
	ObjectProperties props(context);
	objThis.GetProperties(context, props);
	
	std::map<std::string, ObjectProperties::PROPERTY>&mapProps = props.GetProps(); 
	for (std::map < std::string, ObjectProperties::PROPERTY >::iterator it = mapProps.begin(); it != mapProps.end(); ++it)
	{
		if (!it->second.isReadOnly)
		{
			if (it->first == JSNODE_parentNode)
			{
			}
			else if (it->first == JSNODE_previousSibling)
			{
			}
			else if (it->first == JSNODE_nextSibling)
			{
			}
			else if (it->first == JSNODE_firstChild)
			{

			}
			else if (it->first == JSNODE_lastChild)
			{

			}
			else if (it->first == "childNodes")
			{

			}
			else if (it->first == "length")
			{

			}
			else if (!strncmp(it->first.c_str(), "__", 2))
			{

			}
			else
			{
				
				objNew.SetProperty(context, it->first.c_str(), it->second.Value());
			}
		}
	}
	
//	pNewElement->GenerateChildNodeArray(context);
	// copy member variables of HtmlElement
	pNewElement->OnCloneNode(this);
	
	return objNew;
}

bool HtmlElement::SetLastChild(const HandleContext&context, const ValueBase&value)
{
	LocalObject thisobj(context, GetJsObject());
	return thisobj.SetProperty(context, JSNODE_lastChild, value);
}

void HtmlElement::OnCloneNode(HtmlElement*pSrc)
{
	m_AttributeMap.clear();
	for (std::map<std::string, std::string>::iterator it = pSrc->m_AttributeMap.begin(); it != pSrc->m_AttributeMap.end(); ++it)
	{
		m_AttributeMap[it->first] = it->second;
	}
}

HandleObject HtmlElement::FindChildByPropertyValue(const HandleContext&context,const std::string&szPropName, const std::string&value, bool bFindChildrens)
{
	LocalObject objChild(context, GetFirstChild(context).GetObject());
	
	while (!objChild.IsNull())
	{
		LocalValue vProp(context,objChild.GetProperty(context, szPropName.c_str()));
		if (vProp.IsObject() || vProp.IsNull() || vProp.IsUndefined())
		{ 
		}
		else
		{
			std::string &&strr = vProp.GetString(context);
			if (strr == value)
			{
				return objChild;
			}
		}
		if (bFindChildrens)
		{
			HtmlElement*pChildrenElement = dynamic_cast<HtmlElement*>(FindCustomClass(objChild));
			if (pChildrenElement)
			{
				CustomClassAutoThisObj cc(pChildrenElement, objChild);
				LocalObject objChildren(context, pChildrenElement->FindChildByPropertyValue(context, szPropName, value, bFindChildrens));
				if (!objChildren.IsNull())return objChildren;
			}
		}

		objChild=objChild.GetProperty(context, JSNODE_nextSibling).GetObject();
	}
	return HandleObject();
}

int HtmlElement::GetElementsByTagName(const HandleContext&context,const std::string &szName, ObjectList&objList)
{
	int n = 0;
	std::string tagname;
	LocalObject objChild (context, GetFirstChild(context).GetObject());

	while (true)
	{
		if (objChild.IsNull())
		{
//			delete objChild;
			break;
		}
		HtmlElement*pChild = dynamic_cast<HtmlElement*>(FindCustomClass(objChild));
		if (!pChild)
		{
			objChild = objChild.GetProperty(context, JSNODE_nextSibling).GetObject();
			continue;
		}
		CustomClassAutoThisObj cchild(pChild, objChild);
		tagname = objChild.GetProperty(context, "tagName").GetString(context);
		if (!_stricmp(tagname.c_str(), szName.c_str()))
		{
			objList.objList.append(objChild);
			n++;
			n+=pChild->GetElementsByTagName(context, szName, objList);
			objChild = objChild.GetProperty(context, JSNODE_nextSibling).GetObject();
		}
		else
		{
			n+=pChild->GetElementsByTagName(context, szName, objList);
			objChild=objChild.GetProperty(context, JSNODE_nextSibling).GetObject();
		}
		
	
	}
	return n;
}

void HtmlElement::OnAddToParent(const HandleContext&context,HtmlElement*pParent)
{
	Measure(pParent->GetLeft(), pParent->GetTop(), GetParentWidth(), GetParentHeight());
}

void HtmlElement::OnRemoveFromParent(const HandleContext&context,HtmlElement*pParent)
{

}

void HtmlElement::SetTagName(const HandleContext& context, const char * name)
{
	LocalObject(context,GetJsObject()).SetProperty(context, "tagName", LocalValue(context).SetString(context, name));
}

void HtmlElement::SetNodeType(const HandleContext& context, int nodetype)
{
	LocalObject(context, GetJsObject()).SetProperty(context, "nodeType", ValueBase().SetInt(nodetype));
}

bool HtmlElement::SetNodeValue(const HandleContext& context, const char * value)
{
	return LocalObject(context, GetJsObject()).SetProperty(context, "nodeValue", LocalValue(context).SetString(context, value));
}

bool HtmlElement::GenerateChildNodeArray(const HandleContext& context)
{
	LocalObject objthis(context, GetJsObject());

	LocalObject objChildNodes(context, objthis.GetProperty(context, "childNodes").GetObject());
	if (objChildNodes.IsNull()||!objChildNodes.IsArray(context))
	{
		objChildNodes.CreateArrayObject(context, 0);
		objthis.SetProperty(context, "childNodes", objChildNodes);
	}
	else
	{
		objChildNodes.SetArrayLength(context, 0);
	}
	LocalObject objChild(context, GetFirstChild(context).GetObject());
	int idx = 0;
	while (!objChild.IsNull())
	{
		objChildNodes.SetElement(context, idx, ValueBase(objChild));

		objChild = objChild.GetProperty(context, JSNODE_nextSibling).GetObject();
		idx++;
	}
	objthis.SetProperty(context, "length", ValueBase().SetInt(idx));
	return true;
}

void HtmlElement::SetStyle(const HandleContext& context,const char* value)
{
	LocalObject objthis(context, GetJsObject());
	HtmlStyle*pStyle = GetStyle(/*context*/);
	
	pStyle->SetStyle(context, value,this);



}

int HtmlElement::GetParentWidth(/*const HandleContext&context*/)
{
	HtmlElement*pParent = GetParent();
	if (!pParent)
		return 0;
	return pParent->GetClientWidth();
// 
// 	double width;
// 	LocalObject objthis(context, GetJsObject());
// 	LocalObject objparent(context, objthis.GetProperty(context, JSNODE_parentNode).GetObject());
// 	if (!objparent.IsNull())
// 	{
// 		width = objparent.GetProperty(context, "clientWidth").GetNumber();
// 		
// 	}
// 	else
// 	{
// 		width = OpenglContext::GetWidth();
// 	}
// 	return width;
}

int HtmlElement::GetParentHeight(/*const HandleContext& context*/)
{
	HtmlElement*pParent = GetParent();
	if (!pParent)
		return 0;
	return pParent->GetClientHeight();
// 	double height;
// 	LocalObject objthis(context, GetJsObject());
// 	LocalObject objparent(context, objthis.GetProperty(context, JSNODE_parentNode).GetObject());
// 	if (!objparent.IsNull())
// 	{
// 		height = objparent.GetProperty(context, "clientHeight").GetNumber();
// 
// 	}
// 	else
// 	{
// 		height = OpenglContext::GetHeight();
// 	}
// 	return height;
}

// getting width by style is only used in measure(), call GetWidth() directly to get width in other situations
int HtmlElement::GetStyledWidth(/*const HandleContext&context*/)
{
	HtmlStyle*pStyle = GetStyle(/*context*/);
	switch (pStyle->m_width.typ)
	{
	case CT_PX:return pStyle->m_width.value;
	case CT_PERCENT:
	{
		int width = GetParentWidth(/*context*/);
		return width*(pStyle->m_width.value*0.01)+0.5;
	}
	default:
// 		if (!m_bSetWidth)
// 			return GetParentWidth();
// 		else
			return m_width;
		//return m_width == -1 ? GetParentWidth(/*context*/):m_width;
	}
}

int HtmlElement::GetStyledHeight(/*const HandleContext&context*/)
{
	HtmlStyle*pStyle = GetStyle(/*context*/);
	switch (pStyle->m_height.typ)
	{
	case CT_PX:return pStyle->m_height.value;
	case CT_PERCENT:
	{
		int height = GetParentHeight(/*context*/);
		return height*(pStyle->m_height.value*0.01)+0.5;
	}
	default:
// 		if (!m_bSetHeight)
// 			return GetParentHeight();
// 		else 
			return m_height;
		//return m_height==-1?GetParentHeight(/*context*/):m_height;
	}
}

int HtmlElement::GetStyledLeft(/*const HandleContext&context*/)
{
	HtmlStyle*pStyle = GetStyle(/*context*/);
	switch (pStyle->m_left.typ)
	{
	case CT_PX:return pStyle->m_left.value+pStyle->m_Transform.xTran;
	case CT_PERCENT:
	{
 		int width = GetParentWidth();
		return width*(pStyle->m_left.value*0.01) + 0.5 + pStyle->m_Transform.xTran;
	}
	default:
		return 0;//return m_left;
	}
}

int HtmlElement::GetStyledTop(/*const HandleContext&context*/)
{
	HtmlStyle*pStyle = GetStyle(/*context*/);
	switch (pStyle->m_top.typ)
	{
	case CT_PX:return pStyle->m_top.value+pStyle->m_Transform.yTran;
	case CT_PERCENT:
	{

		int height = GetParentHeight();
		return height*(pStyle->m_top.value*0.01) + 0.5 + pStyle->m_Transform.yTran;
	}
	default:
		return 0;//return m_top;
	}
}



HtmlStyle* HtmlElement::GetStyle(/*const HandleContext&context*/)
{
	return m_pStyle;
// 	LocalObject objstyle(context, GetJsObject().GetProperty(context, "style").GetObject());
// 	if (objstyle.IsNull())return nullptr;
// 	return dynamic_cast<HtmlStyle*>(FindCustomClass(objstyle));
}

std::string HtmlElement::GetAttribute(const std::string &szAttrName)
{
	std::map<std::string, std::string>::iterator it=this->m_AttributeMap.find(szAttrName);
	if (it == m_AttributeMap.end())return "";
	return it->second;
}

bool HtmlElement::HasAttribute(const std::string &szAttrName)
{
	std::map<std::string, std::string>::iterator it = this->m_AttributeMap.find(szAttrName);
	if (it == m_AttributeMap.end())return false;
	return true;
}

bool HtmlElement::SetBackgroundImage(const std::string &backgroundImage)
{
	return true;
}

bool HtmlElement::createTextNode(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "createTextNode");
	LocalObject obj(args.getContext(), HtmlTextNode::CreateObject(args.getContext()));
	HtmlTextNode*pText = dynamic_cast<HtmlTextNode*>(FindCustomClass(obj));
	pText->m_szText = args[0].GetString(args.getContext());
	args.setReturnValue(obj);
	return true;
}

void HtmlElement::OnSetStyle(const HandleContext&context, const char*szName)
{
	if (!strcmp(szName, "left") || !strcmp(szName, "top") || !strcmp(szName, "width") || !strcmp(szName, "height")
		||!strcmp(szName,"transform"))
	{
		HtmlElement*pParent = GetParent();
		if (pParent)
		{
			Measure(pParent->GetLeft(), pParent->GetTop(), pParent->GetWidth(), pParent->GetHeight());
		}
		else
		{
			Measure(0, 0, 0xfffffff, 0xfffffff);
		}
	}
}

HtmlElement* HtmlElement::GetParent()
{
	return m_pParent;
}

int HtmlElement::GetClientWidth()
{
	return m_width;
//	return GetStyledWidth();
}

int HtmlElement::GetClientHeight()
{
	return m_height;
//	return GetStyledHeight();
}

//calulate coordinate of element by coordinate of parent
void HtmlElement::Measure(int parentX, int parentY, int parentWidth, int parentHeight)
{
	OnMeasure(parentX, parentY, parentWidth, parentHeight);
	
	for (std::list<HtmlElement*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it)
	{
		HtmlElement*pchild = *it;
		pchild->Measure(m_left, m_top, m_width, m_height);
	}
}

void HtmlElement::OnMeasure(int parentX, int parentY, int parentWidth, int parentHeight)
{
	m_left = parentX+GetStyledLeft();
	m_top = parentY+GetStyledTop();
	m_width = GetStyledWidth();
	m_height = GetStyledHeight();
}

HandleObject HtmlElement::GetObjFromDomTree(const HandleContext&context)
{
	if (m_pParent)
	{
		LocalObject objparent(context, m_pParent->GetObjFromDomTree(context));
		LocalObject objchild(context, objparent.GetProperty(JSNODE_firstChild).GetObject());
		while (!objchild.IsNull())
		{
			HtmlElement*pEle = dynamic_cast<HtmlElement*>(FindCustomClass(objchild));
			if (pEle == this)
				return objchild;
			objchild = objchild.GetProperty(JSNODE_nextSibling).GetObject();
		}
		return HandleObject();
	}
	else
	{
		return JSVM::GetInstance()->GetGlobal().GetProperty(context,"document").GetObject();
	}
}

std::string & HtmlElement::GetTagName()
{
	return m_szTagName;
}

std::string &HtmlElement::GetID()
{
	return m_szID;
}

