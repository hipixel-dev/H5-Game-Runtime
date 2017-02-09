#include "stdafx.h"
#include "HtmlTextNode.h"


IMPLEMENT_CUSTOM_CLASS(HtmlTextNode, HtmlElement)
HtmlTextNode::HtmlTextNode()
{
}


HtmlTextNode::~HtmlTextNode()
{
}

bool HtmlTextNode::InitClass(const HandleContext&context)
{
	if (HtmlElement::GetPrototype_s().IsNull())
	{
		HtmlElement::InitClass(context);
	}
	return InitClass_s(context, "HTMLText", HtmlElement::GetPrototype_s());
}

bool HtmlTextNode::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "data"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_szText.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "length"))
	{
		args.SetRetValue(ValueBase().SetInt(m_szText.length()));
		return true;
	}
// 	else if (!PROPCMP(name, "nodeValue"))
// 	{
// 		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_data.c_str()));
// 	}
	return HtmlElement::OnGetProperty(args);
}

bool HtmlTextNode::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "data"))
	{
		m_szText = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "length"))
	{
		
		return true;
	}
// 	if (!PROPCMP(name, "nodeValue"))
// 	{
// 		m_szText = args.GetValue().GetString(args.GetContext());
// 		return true;
// 	}
	return HtmlElement::OnSetProperty(args);
}

bool HtmlTextNode::appendData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlTextNode::appendData");
	std::string &&str = args[0].GetString(args.getContext());
	m_szText += str;
	return true;
}

bool HtmlTextNode::deleteData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlTextNode::deleteData");
	int start = args[0].GetInt();
	int length = args[1].GetInt();
	
	m_szText = m_szText.substr(0, start) + m_szText.substr(start + length);
	return true;
}

bool HtmlTextNode::insertData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, " HtmlTextNode::insertData");
	int start = args[0].GetInt();
	std::string &&str = args[1].GetString(args.getContext());
	m_szText.insert(start, str);
	return true;
}

bool HtmlTextNode::replaceData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "HtmlTextNode::replaceData");
	int start = args[0].GetInt();
	int length = args[1].GetInt();
	std::string &&str = args[2].GetString(args.getContext());
	m_szText = m_szText.substr(0, start) + str + m_szText.substr(start + length);
	return true;
}

bool HtmlTextNode::splitText(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlTextNode::substringData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlTextNode::substringData");
	int start = args[0].GetInt();
	int length = args[1].GetInt();
	std::string str = m_szText.substr(start, length);
	args.setReturnValue(LocalValue(args.getContext()).SetString(args.getContext(), str.c_str()));
	return true;
}

bool HtmlTextNode::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret=HtmlElement::OnConstructor(args);
	if (!ret)return false;
//	args.thisObj().SetProperty(args.getContext(),"nodeType", ValueBase().SetInt(3));
	return true;
}
