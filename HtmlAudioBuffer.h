#pragma once
#include "JSVMInterface.h"
//https://developer.mozilla.org/en-US/docs/Web/API/AudioBuffer
class HtmlAudioBuffer :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlAudioBuffer, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("getChannelData", &HtmlAudioBuffer::getChannelData, 1)
		DEFINE_CUSTOM_FUNCTION("copyFromChannel", &HtmlAudioBuffer::copyFromChannel, 3)
		DEFINE_CUSTOM_FUNCTION("copyToChannel", &HtmlAudioBuffer::copyToChannel, 3)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("sampleRate")
		DEFINE_CUSTOM_PROPERTY("length")
		DEFINE_CUSTOM_PROPERTY("duration")
		DEFINE_CUSTOM_PROPERTY("numberOfChannels")

		END_CUSTOM_PROPERTY

	int m_sampleRate = 0;
	int m_length = 0;
	double m_duration = 0;
	int m_numberOfChannels = 0;
	uint8_t* m_pData = nullptr;
	int m_nDataLen = 0;
	HtmlAudioBuffer();
	virtual ~HtmlAudioBuffer();
	static bool InitClass(const HandleContext&context);

	virtual bool OnGetProperty(GetPropertyArgs&args) ;

	virtual bool OnSetProperty(SetPropertyArgs&args) ;
	

	bool getChannelData(IFunctionCallbackArgs&args);
	bool copyFromChannel(IFunctionCallbackArgs&args);
	bool copyToChannel(IFunctionCallbackArgs&args);
	bool SetAudioData(const void* pData, int nLen);
	bool DecodeData();

};

