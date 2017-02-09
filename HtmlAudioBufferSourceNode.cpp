#include "stdafx.h"
#include "HtmlAudioBufferSourceNode.h"

IMPLEMENT_CUSTOM_CLASS(HtmlAudioBufferSourceNode, HtmlAudioNode)
HtmlAudioBufferSourceNode::HtmlAudioBufferSourceNode()
{
}


HtmlAudioBufferSourceNode::~HtmlAudioBufferSourceNode()
{
}

bool HtmlAudioBufferSourceNode::OnGetProperty(GetPropertyArgs&args)
{
	return HtmlAudioNode::OnGetProperty(args);
}

bool HtmlAudioBufferSourceNode::OnSetProperty(SetPropertyArgs&args)
{
	return HtmlAudioNode::OnSetProperty(args);
}

bool HtmlAudioBufferSourceNode::OnConstructor(IFunctionCallbackArgs&args)
{
	return HtmlAudioNode::OnConstructor(args);
}

bool HtmlAudioBufferSourceNode::InitClass(const HandleContext&context)
{
	if (HtmlAudioNode::GetPrototype_s().IsNull())
	{
		HtmlAudioNode::InitClass(context);
	}
	return InitClass_s(context, "AudioBufferSourceNode", HtmlAudioNode::GetPrototype_s());
}

bool HtmlAudioBufferSourceNode::start(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioBufferSourceNode::stop(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}
