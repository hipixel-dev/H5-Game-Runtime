#include "stdafx.h"
#include "HtmlAudioContext.h"
#include "HtmlGainNode.h"
#include "HtmlAudioDestinationNode.h"
#include "HtmlAudioBuffer.h"
#include "HtmlAudioBufferSourceNode.h"
IMPLEMENT_CUSTOM_CLASS(HtmlAudioContext, CustomClassBase)

HtmlAudioContext::HtmlAudioContext()
{
}


HtmlAudioContext::~HtmlAudioContext()
{
}

bool HtmlAudioContext::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HTMLAudioContext", HandleObject());
}

bool HtmlAudioContext::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "currentTime"))
	{

	}
	else if (!PROPCMP(name, "currentTime"))
	{

	}
	else if (!PROPCMP(name, "destination"))
	{

	}
	else if (!PROPCMP(name, "listener"))
	{

	}
	else if (!PROPCMP(name, "sampleRate"))
	{

	}
	else if (!PROPCMP(name, "state"))
	{

	}
	else if (!PROPCMP(name, "mozAudioChannelType"))
	{

	}
	else if (!PROPCMP(name, "onstatechange"))
	{

	}
	else if (!PROPCMP(name, "currentTime"))
	{

	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlAudioContext::OnSetProperty(SetPropertyArgs&args)
{
	return CustomClassBase::OnSetProperty(args);
}

bool HtmlAudioContext::close(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createBuffer(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createBufferSource(IFunctionCallbackArgs&args)
{
	LocalObject objsrc(args.getContext(), HtmlAudioBufferSourceNode::CreateObject(args.getContext()));
	HtmlAudioBufferSourceNode*pnode = (HtmlAudioBufferSourceNode*)FindCustomClass(objsrc);
	CustomClassAutoThisObj cc(pnode, objsrc);
	pnode->SetContext(args.getContext(), GetJsObject());
	args.setReturnValue(objsrc);
	return true;
}

bool HtmlAudioContext::createMediaElementSource(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createMediaStreamSource(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createMediaStreamDestination(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createScriptProcessor(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createStereoPanner(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createAnalyser(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createBiquadFilter(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createChannelMerger(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createChannelSplitter(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createConvolver(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createDelay(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createDynamicsCompressor(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createGain(IFunctionCallbackArgs&args)
{
	LocalObject objgain(args.getContext(), HtmlGainNode::CreateObject(args.getContext()));
	HtmlGainNode*pnode = (HtmlGainNode*)FindCustomClass(objgain);
	CustomClassAutoThisObj cc(pnode, objgain);
	pnode->SetContext(args.getContext(),GetJsObject());
	args.setReturnValue(objgain);
	return true;
}

bool HtmlAudioContext::createOscillator(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createPanner(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createPeriodicWave(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::createWaveShaper(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::decodeAudioData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "decodeAudioData");
	LocalObject objdata(args.getContext(), args[0].GetObject());
	if (!objdata.IsArray())
	{
		assert(0);
		return false;
	}
	LocalObject objthis(args.getContext(), GetJsObject());

	LocalObject objbuf(args.getContext(), HtmlAudioBuffer::CreateObject(args.getContext()));
	HtmlAudioBuffer*pbuffer = (HtmlAudioBuffer*)FindCustomClass(objbuf);
	bool bOK=pbuffer->SetAudioData(objdata.GetArrayData(args.getContext()), objdata.GetArrayByteLength(args.getContext()));

	if (bOK)
	{
		IFunctionCallJsArgs funargs(args.getContext());
		LocalValue vpara(args.getContext(), objbuf);
		funargs.SetParams(&objthis, 1, &vpara);
		LocalValue vret(args.getContext());
		bOK=JSVM::GetInstance()->executeFunction(args.getContext(), args[1], funargs, vret);
	}
	else
	{
		IFunctionCallJsArgs funargs(args.getContext());
		funargs.SetParams(&objthis, 0);
		LocalValue vret(args.getContext());
		bOK = JSVM::GetInstance()->executeFunction(args.getContext(), args[2], funargs, vret);
	}
	
	return true;
}

bool HtmlAudioContext::resume(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::suspend(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlAudioContext::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = CustomClassBase::OnConstructor(args);
	if (!ret)return false;
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject objdest(args.getContext(),HtmlAudioDestinationNode::CreateObject(args.getContext()));
	m_pDestination = (HtmlAudioDestinationNode*)FindCustomClass(objdest);
	CustomClassAutoThisObj cc2(m_pDestination, objdest);
	m_pDestination->SetContext(args.getContext(), GetJsObject());
	
	GetJsObject().SetProperty(args.getContext(), "destination", objdest);
	return true;
	
}
