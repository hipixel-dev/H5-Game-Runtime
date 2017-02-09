#pragma once
#include "HtmlAudioNode.h"
//https://developer.mozilla.org/en-US/docs/Web/API/AudioBufferSourceNode
class HtmlAudioBufferSourceNode :
	public HtmlAudioNode
{
	DEFINE_CUSTOM_CLASS(HtmlAudioBufferSourceNode, HtmlAudioNode)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("start",&HtmlAudioBufferSourceNode::start,3)
		DEFINE_CUSTOM_FUNCTION("stop", &HtmlAudioBufferSourceNode::stop, 1)
		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("buffer")
		DEFINE_CUSTOM_PROPERTY("detune")
		DEFINE_CUSTOM_PROPERTY("loop")
		DEFINE_CUSTOM_PROPERTY("loopStart")
		DEFINE_CUSTOM_PROPERTY("loopEnd")
		DEFINE_CUSTOM_PROPERTY("playbackRate")


		END_CUSTOM_PROPERTY


	HtmlAudioBufferSourceNode();
	virtual ~HtmlAudioBufferSourceNode();
	static bool InitClass(const HandleContext&context);

	virtual bool OnGetProperty(GetPropertyArgs&args) ;

	virtual bool OnSetProperty(SetPropertyArgs&args) ;

	virtual bool OnConstructor(IFunctionCallbackArgs&args) ;

	bool start(IFunctionCallbackArgs&args);
	bool stop(IFunctionCallbackArgs&args);
};

