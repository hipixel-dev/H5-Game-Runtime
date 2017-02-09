#include "stdafx.h"
#include "HtmlAudioDestinationNode.h"
IMPLEMENT_CUSTOM_CLASS(HtmlAudioDestinationNode, HtmlAudioNode)

HtmlAudioDestinationNode::HtmlAudioDestinationNode()
{
}


HtmlAudioDestinationNode::~HtmlAudioDestinationNode()
{
}

bool HtmlAudioDestinationNode::InitClass(const HandleContext&context)
{
	if (HtmlAudioNode::GetPrototype_s().IsNull())
	{
		HtmlAudioNode::InitClass(context);
	}
	return InitClass_s(context, "AudioDestinationNode", HtmlAudioNode::GetPrototype_s());
}
