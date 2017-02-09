#include "stdafx.h"
#include "HtmlDomParser.h"
#include "Markup.h"
#include "HtmlElement.h"
#include "HtmlTextNode.h"
IMPLEMENT_CUSTOM_CLASS(HtmlDomParser, CustomClassBase)

HtmlDomParser::HtmlDomParser()
{
}


HtmlDomParser::~HtmlDomParser()
{
}

bool HtmlDomParser::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "DOMParser", HandleObject());
}

bool HtmlDomParser::OnGetProperty(GetPropertyArgs&args)
{
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlDomParser::OnSetProperty(SetPropertyArgs&args)
{
	return CustomClassBase::OnSetProperty(args);
}

void HtmlDomParser::OnFinalize(FinalizeArgs&args)
{
	return CustomClassBase::OnFinalize(args);
}

bool HtmlDomParser::OnConstructor(IFunctionCallbackArgs&args)
{
	return CustomClassBase::OnConstructor(args);
}
extern char*TrimLeft(char*p);
bool HtmlDomParser::parseFromString(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(1, "parseFromString");

	std::string &&szXml = args[0].GetString(args.getContext());


	CMarkup xml;
#ifdef _WIN32//WIN32需转为UTF8
	bool bTransfer = false;
	size_t pos=szXml.find('\n');
	if(pos!=szXml.npos)
	{
		std::string head = szXml.substr(0, pos);
		char*p = (char*)head.c_str();
		p=TrimLeft(p);
		if (!strncmp(p, "<?xml", 5))
		{
			transform(head.begin(), head.end(), head.begin(), ::tolower);
			if (head.find("utf-8") != head.npos)
			{
				bTransfer = true;
			}
		}
	}

	bool ok;
	if (bTransfer)
	{
//		szXml = JSVM::StringToUtf8(szXml);
	}
	ok = xml.SetDoc(szXml.c_str());

#else
	bool ok = xml.SetDoc(szXml.c_str());
#endif
	xml.ResetMainPos();

	LocalObject vRet(args.getContext(), HtmlElement::CreateObject(args.getContext()));
	HtmlElement*pRootObj = (HtmlElement*)vRet.GetNativeObj();
	CustomClassAutoThisObj cc(pRootObj, vRet);
	int n = CreateObjFromXml(pRootObj, args.getContext(), xml);
	LocalValue vRoot(args.getContext(),pRootObj->GetFirstChild(args.getContext()));
	LocalObject objRoot(args.getContext(), vRoot.GetObject());
	if (!objRoot.IsNull())
	{
		objRoot.SetProperty(JSNODE_parentNode, ValueBase());
	}
	args.setReturnValue(vRoot);
	return true;
}

HandleObject HtmlDomParser::CreateObjFromXml(const HandleContext&context, const char*szXml)
{
	CMarkup xml;
	bool ok = xml.SetDoc(szXml);
	if (!ok)return HandleObject();
	xml.ResetMainPos();

	LocalObject objRoot(context, HtmlElement::CreateObject(context));
	HtmlElement*pRootObj = (HtmlElement*)objRoot.GetNativeObj();
	CustomClassAutoThisObj cc(pRootObj, objRoot);
	LocalObject vRet(context, objRoot);
	int n = CreateObjFromXml(pRootObj, context, xml);
	return pRootObj->GetFirstChild(context).GetObject();

}


//从XML创建元素，创建的元素添加到pParent子节点

int HtmlDomParser::CreateObjFromXml(HtmlElement*pParent, const HandleContext&context, CMarkup&xml)
{
	int nCount = 0;
	std::string tagname, attrname, attrvalue, nodevalue;
	do
	{
		int nodetype = xml.GetNodeType();


		if (nodetype != 1)
			continue;
		LocalObject objnode(context, HtmlElement::CreateObject(context));
		HtmlElement *pnode = (HtmlElement*)objnode.GetNativeObj();
		CustomClassAutoThisObj cc(pnode, objnode);
		pParent->AppendChild(context, objnode);
		nCount++;

		tagname = xml.GetTagName();
		std::string text = xml.GetData();
		pnode->SetTagName(context, tagname.c_str());
		pnode->SetNodeType(context, nodetype);

		if (nodetype == 1)//ELEMENT_NODE
		{
			for (int iAttr = 0; xml.GetNthAttrib(iAttr, attrname, attrvalue); iAttr++)
			{
				pnode->SetAttribute(attrname, attrvalue);
			}
			if (!text.empty())
			{
				LocalObject objtext(context, HtmlTextNode::CreateObject(context));
				HtmlTextNode *ptext = (HtmlTextNode*)objtext.GetNativeObj();
				CustomClassAutoThisObj cc2(ptext, objtext);
				ptext->m_szText = text;
				pnode->AppendChild(context,objtext);
				ptext->SetNodeType(context, 3);
				
//				ptext->SetNodeValue(context, text.c_str());
				pnode->GenerateChildNodeArray(context);
			}
			else if (xml.IntoElem())
			{
				nCount += CreateObjFromXml(pnode, context, xml);
				xml.OutOfElem();
			}
		}
		// 		else if (nodetype == 3)
		// 		{
		// 			nodevalue=xml.GetData();
		// 			pnode->SetNodeValue(cx, nodevalue.c_str());
		// 		}

	} while (xml.FindElem());
	pParent->GenerateChildNodeArray(context);
	return nCount;
}

