#include "stdafx.h"
#include "HtmlGainNode.h"
#include "HtmlAudioParam.h"
IMPLEMENT_CUSTOM_CLASS(HtmlGainNode, HtmlAudioNode)

HtmlGainNode::HtmlGainNode()
{
}


HtmlGainNode::~HtmlGainNode()
{
}

bool HtmlGainNode::InitClass(const HandleContext&context)
{
	if (!HtmlAudioNode::GetPrototype_s().IsNull())
	{
		HtmlAudioNode::InitClass(context);
	}
	return InitClass_s(context, "AudioGainNode", HtmlAudioNode::GetPrototype_s());
}

bool HtmlGainNode::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = HtmlAudioNode::OnConstructor(args);
	if (!ret)return false;
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject objgain(args.getContext(), HtmlAudioParam::CreateObject(args.getContext()));
	m_pGain = (HtmlAudioParam*)FindCustomClass(objgain);
	m_pGain->m_pNode = this;
	GetJsObject().SetProperty(args.getContext(),"gain", objgain);
	return true;
}

