#pragma once
#include "JSVMInterface.h"
#include "HtmlAudioNode.h"
// AudioContext.CreateGain返回,用于设置音量
class HtmlGainNode :
	public HtmlAudioNode
{
	DEFINE_CUSTOM_CLASS(HtmlGainNode, HtmlAudioNode)
		
public:
	BEGIN_CUSTOM_FUNCTION
		
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY

		END_CUSTOM_PROPERTY



		HtmlAudioParam*m_pGain = nullptr;


	HtmlGainNode();
	virtual ~HtmlGainNode();
	static bool InitClass(const HandleContext&context);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
};

