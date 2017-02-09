#include "stdafx.h"
#include "HtmlAudioNode.h"
#include "HtmlAudioContext.h"
#include "HtmlAudioParam.h"
IMPLEMENT_CUSTOM_CLASS(HtmlAudioNode, CustomClassBase)

HtmlAudioNode::HtmlAudioNode()
{
}


HtmlAudioNode::~HtmlAudioNode()
{
}

bool HtmlAudioNode::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HTMLAudioNode", HandleObject());
}

bool HtmlAudioNode::OnGetProperty(GetPropertyArgs&args)
{
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlAudioNode::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "context"))
	{
		m_pContext = dynamic_cast<HtmlAudioContext*>(FindCustomClass(args.GetValue().GetObject()));
	}
	return CustomClassBase::OnSetProperty(args);
}

bool HtmlAudioNode::connect(IFunctionCallbackArgs&args)//连接到另一个node的输出端，数据会从另一个node推送过来
{
	CHECK_ARGS_COUNT_MIN(1, "HtmlAudioNode::connect");
	LocalObject objnode(args.getContext(), args[0].GetObject());
	HtmlAudioNode*pnode = dynamic_cast<HtmlAudioNode*>(FindCustomClass(objnode));
	if (!pnode)
	{
		assert(0);
		return false;
	}
	LocalObject objthis(args.getContext(), GetJsObject());
	LocalObject objOldConnect(args.getContext(),objthis.GetProperty("__connectNode").GetObject());
	if (!objOldConnect.IsNull())
	{//解除旧的连接
		HtmlAudioNode*pnodeOld = dynamic_cast<HtmlAudioNode*>(FindCustomClass(objOldConnect));
		assert(pnodeOld);
		CustomClassAutoThisObj cc(pnodeOld, objOldConnect);
		pnodeOld->RemoveConnectedNode(args.getContext(), objthis);
	}
	objthis.SetProperty("__connectNode", args[0]);
	CustomClassAutoThisObj cc(pnode, objnode);
	pnode->AddConnectedNode(args.getContext(), objthis);
	return true;
}

bool HtmlAudioNode::disconnect(IFunctionCallbackArgs&args)
{
	
	LocalObject objthis(args.getContext(), GetJsObject());
	LocalObject objOldConnect(args.getContext(), objthis.GetProperty("__connectNode").GetObject());
	if (!objOldConnect.IsNull())
	{//解除旧的连接
		HtmlAudioNode*pnodeOld = dynamic_cast<HtmlAudioNode*>(FindCustomClass(objOldConnect));
		assert(pnodeOld);
		CustomClassAutoThisObj cc(pnodeOld, objOldConnect);
		pnodeOld->RemoveConnectedNode(args.getContext(), objthis);
		objthis.SetProperty("__connectNode", ValueBase());
	}
	
	return true;
}

bool HtmlAudioNode::AddConnectedNode(const HandleContext& context, const HandleObject & objnode)//调用了objnode的connect()后在被连接的node中添加objnode作为输出node
{
	LocalObject objthis(context, GetJsObject());
	LocalObject objlist(context,objthis.GetProperty("__destNodeList").GetObject());
	if (objlist.IsNull())
	{
		objlist.CreateArrayObject(0);
		objthis.SetProperty("__destNodeList", objlist);
	}

	int nCount = objlist.GetArrayLength();
	int iEmpty = -1;
	for (int i = 0; i < nCount; i++)
	{
		LocalObject objitem(context, objlist.GetElement(i).GetObject());
		if (objitem == objnode)return true;
		if (iEmpty == -1 && objitem.IsNull())
		{
			iEmpty = i;
		}
	}
	if (iEmpty >= 0)
	{
		objlist.SetElement(iEmpty, objnode);
	}
	else
	{
		objlist.SetElement(nCount, objnode);
	}
	return true;
}

bool HtmlAudioNode::RemoveConnectedNode(const HandleContext& context, const HandleObject&objnode)
{
	LocalObject objthis(context, GetJsObject());
	LocalObject objlist(context, objthis.GetProperty("__destNodeList").GetObject());
	if (objthis.IsNull())
	{
		return true;
	}

	int nCount = objlist.GetArrayLength();
	for (int i = 0; i < nCount; i++)
	{
		LocalObject objitem(context, objlist.GetElement(i).GetObject());
		if (objitem == objnode)
		{
			objlist.SetElement(i, ValueBase());
			return true;
		}
		
	}
	
	return true;
}

bool HtmlAudioNode::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = CustomClassBase::OnConstructor(args);
	return ret;
}

bool HtmlAudioNode::SetContext(const HandleContext& context, const HandleObject & objctx)
{
	return GetJsObject().SetProperty(context, "context", objctx);
}

void HtmlAudioNode::OnValueChanged(HtmlAudioParam* pParam)
{
	
}
