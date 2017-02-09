#pragma once
#include "HtmlAudioNode.h"
class HtmlAudioDestinationNode :
	public HtmlAudioNode
{
	DEFINE_CUSTOM_CLASS(HtmlAudioDestinationNode, HtmlAudioNode)
public:
	BEGIN_CUSTOM_FUNCTION
	
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		
		END_CUSTOM_PROPERTY


	HtmlAudioDestinationNode();
	virtual ~HtmlAudioDestinationNode();

	static bool InitClass(const HandleContext&context);
};

