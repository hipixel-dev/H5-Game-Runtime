#include "stdafx.h"
#include "HtmlAudioBuffer.h"

IMPLEMENT_CUSTOM_CLASS(HtmlAudioBuffer, CustomClassBase)
HtmlAudioBuffer::HtmlAudioBuffer()
{
}


HtmlAudioBuffer::~HtmlAudioBuffer()
{
	SAFE_DELETE_ARRAY(m_pData);
}

bool HtmlAudioBuffer::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "sampleRate"))
	{
		args.SetRetValue(ValueBase().SetInt(m_sampleRate));
		return true;
	}
	else if (!PROPCMP(name, "length"))
	{
		args.SetRetValue(ValueBase().SetInt(m_length));
		return true;
	}
	else if (!PROPCMP(name, "duration"))
	{
		args.SetRetValue(ValueBase().SetNumber(m_duration));
		return true;
	}
	else if (!PROPCMP(name, "numberOfChannels"))
	{
		args.SetRetValue(ValueBase().SetInt(m_numberOfChannels));
		return true;
	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlAudioBuffer::OnSetProperty(SetPropertyArgs&args)
{
	
	return CustomClassBase::OnSetProperty(args);
}

bool HtmlAudioBuffer::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "AudioBuffer", HandleObject());
}

bool HtmlAudioBuffer::getChannelData(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioBuffer::copyFromChannel(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioBuffer::copyToChannel(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

//设置音频数据（未解码）
bool HtmlAudioBuffer::SetAudioData(const void* pData, int nLen)
{
	SAFE_DELETE_ARRAY(m_pData);
	m_pData = new uint8_t[nLen];
	memcpy(m_pData, pData, nLen);
	m_nDataLen = nLen;

	return DecodeData();
}

bool HtmlAudioBuffer::DecodeData()
{
	return true;
}
