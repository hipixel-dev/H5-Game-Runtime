#pragma once
#include "JSVMInterface.h"
class HtmlAudioNode;
class HtmlAudioContext :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlAudioContext, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("close", &HtmlAudioContext::close, 0)
		DEFINE_CUSTOM_FUNCTION("createBuffer", &HtmlAudioContext::createBuffer, 3)
		DEFINE_CUSTOM_FUNCTION("createBufferSource", &HtmlAudioContext::createBufferSource, 0)
		DEFINE_CUSTOM_FUNCTION("createMediaElementSource", &HtmlAudioContext::createMediaElementSource, 1)
		DEFINE_CUSTOM_FUNCTION("createMediaStreamSource", &HtmlAudioContext::createMediaStreamSource, 1)
		DEFINE_CUSTOM_FUNCTION("createMediaStreamDestination", &HtmlAudioContext::createMediaStreamDestination, 0)
		DEFINE_CUSTOM_FUNCTION("createScriptProcessor", &HtmlAudioContext::createScriptProcessor, 3)
		DEFINE_CUSTOM_FUNCTION("createStereoPanner", &HtmlAudioContext::createStereoPanner, 0)
		DEFINE_CUSTOM_FUNCTION("createAnalyser", &HtmlAudioContext::createAnalyser, 0)
		DEFINE_CUSTOM_FUNCTION("createBiquadFilter", &HtmlAudioContext::createBiquadFilter, 0)
		DEFINE_CUSTOM_FUNCTION("createChannelMerger", &HtmlAudioContext::createChannelMerger, 1)
		DEFINE_CUSTOM_FUNCTION("createChannelSplitter", &HtmlAudioContext::createChannelSplitter, 1)
		DEFINE_CUSTOM_FUNCTION("createConvolver", &HtmlAudioContext::createConvolver, 0)
		DEFINE_CUSTOM_FUNCTION("createDelay", &HtmlAudioContext::createDelay, 1)
		DEFINE_CUSTOM_FUNCTION("createDynamicsCompressor", &HtmlAudioContext::createDynamicsCompressor, 0)
		DEFINE_CUSTOM_FUNCTION("createGain", &HtmlAudioContext::createGain, 0)
		DEFINE_CUSTOM_FUNCTION("createOscillator", &HtmlAudioContext::createOscillator, 0)
		DEFINE_CUSTOM_FUNCTION("createPanner", &HtmlAudioContext::createPanner, 0)
		DEFINE_CUSTOM_FUNCTION("createPeriodicWave", &HtmlAudioContext::createPeriodicWave, 2)
		DEFINE_CUSTOM_FUNCTION("createWaveShaper", &HtmlAudioContext::createWaveShaper, 0)
		DEFINE_CUSTOM_FUNCTION("decodeAudioData", &HtmlAudioContext::decodeAudioData, 2)
		DEFINE_CUSTOM_FUNCTION("resume", &HtmlAudioContext::resume, 0)
		DEFINE_CUSTOM_FUNCTION("suspend", &HtmlAudioContext::suspend, 0)


		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("currentTime")
		DEFINE_CUSTOM_PROPERTY("destination")
		DEFINE_CUSTOM_PROPERTY("listener")
		DEFINE_CUSTOM_PROPERTY("sampleRate")
		DEFINE_CUSTOM_PROPERTY("state")
		DEFINE_CUSTOM_PROPERTY("mozAudioChannelType")
		DEFINE_CUSTOM_PROPERTY("onstatechange")
		DEFINE_CUSTOM_PROPERTY("currentTime")
		END_CUSTOM_PROPERTY



		HtmlAudioNode*m_pDestination = nullptr;



	HtmlAudioContext();
	virtual ~HtmlAudioContext();

	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);

	bool close(IFunctionCallbackArgs&args);
	bool createBuffer(IFunctionCallbackArgs&args);
	bool createBufferSource(IFunctionCallbackArgs&args);
	bool createMediaElementSource(IFunctionCallbackArgs&args);
	bool createMediaStreamSource(IFunctionCallbackArgs&args);
	bool createMediaStreamDestination(IFunctionCallbackArgs&args);
	bool createScriptProcessor(IFunctionCallbackArgs&args);
	bool createStereoPanner(IFunctionCallbackArgs&args);
	bool createAnalyser(IFunctionCallbackArgs&args);
	bool createBiquadFilter(IFunctionCallbackArgs&args);
	bool createChannelMerger(IFunctionCallbackArgs&args);
	bool createChannelSplitter(IFunctionCallbackArgs&args);
	bool createConvolver(IFunctionCallbackArgs&args);
	bool createDelay(IFunctionCallbackArgs&args);
	bool createDynamicsCompressor(IFunctionCallbackArgs&args);
	bool createGain(IFunctionCallbackArgs&args);
	bool createOscillator(IFunctionCallbackArgs&args);
	bool createPanner(IFunctionCallbackArgs&args);
	bool createPeriodicWave(IFunctionCallbackArgs&args);
	bool createWaveShaper(IFunctionCallbackArgs&args);
	bool decodeAudioData(IFunctionCallbackArgs&args);
	bool resume(IFunctionCallbackArgs&args);
	bool suspend(IFunctionCallbackArgs&args);


};

