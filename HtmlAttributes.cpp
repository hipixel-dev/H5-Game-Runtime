#include "stdafx.h"
#include "HtmlAttributes.h"
#include "HtmlElement.h"
#include "HtmlTextNode.h"

IMPLEMENT_CUSTOM_CLASS(HtmlAttributes, CustomClassBase)
HtmlAttributes::HtmlAttributes()
{
}


HtmlAttributes::~HtmlAttributes()
{
}

bool HtmlAttributes::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "NamedNodeMap", HandleObject(),true);
}

void HtmlAttributes::SetParent(const HandleContext&context, const HandleObject&objparent)
{
	m_pParent = dynamic_cast<HtmlElement*>(FindCustomClass(objparent));
	GetJsObject().SetProperty(context, "__parent", objparent);
}

bool HtmlAttributes::getNamedItem(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "getNamedItem");
	std::string &&key = args[0].GetString(args.getContext());
	std::string &&val=m_pParent->GetAttribute(key);
	LocalObject objret(args.getContext(), HtmlTextNode::CreateObject(args.getContext()));
	HtmlTextNode*pnode = (HtmlTextNode*)FindCustomClass(objret);
	pnode->m_szText = val;
	args.setReturnValue(objret);
	return true;
}

bool HtmlAttributes::OnGetProperty(GetPropertyArgs&args)
{
	std::string name = args.GetName();
	if (m_pParent->HasAttribute(name))
	{
		LocalObject objret(args.GetContext(), HtmlTextNode::CreateObject(args.GetContext()));
		HtmlTextNode*pNode = (HtmlTextNode*)FindCustomClass(objret);
		pNode->m_szText = m_pParent->GetAttribute(name);
		args.SetRetValue(objret);
		return true;
	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlAttributes::OnSetProperty(SetPropertyArgs&args)
{
	const char* name = args.GetName();

	return CustomClassBase::OnSetProperty(args);
}
