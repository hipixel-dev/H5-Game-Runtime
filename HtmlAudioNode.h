#pragma once
#include "JSVMInterface.h"
class HtmlAudioContext;
class HtmlAudioParam;
class HtmlAudioNode :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlAudioNode, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("connect", &HtmlAudioNode::connect, 1)
		DEFINE_CUSTOM_FUNCTION("disconnect", &HtmlAudioNode::disconnect, 0)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("context")
		DEFINE_CUSTOM_PROPERTY("numberOfInputs")
		DEFINE_CUSTOM_PROPERTY("numberOfOutputs")
		DEFINE_CUSTOM_PROPERTY("channelCount")
		DEFINE_CUSTOM_PROPERTY("channelCountMode")
		DEFINE_CUSTOM_PROPERTY("channelInterpretation")

		END_CUSTOM_PROPERTY

		HtmlAudioContext*m_pContext = nullptr;


	HtmlAudioNode();
	virtual ~HtmlAudioNode();
	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);

	bool connect(IFunctionCallbackArgs&args);
	bool disconnect(IFunctionCallbackArgs&args);
	bool AddConnectedNode(const HandleContext& context, const HandleObject & objnode);
	bool RemoveConnectedNode(const HandleContext& context, const HandleObject&objnode);
	bool SetContext(const HandleContext& context, const HandleObject & objctx);
	virtual void OnValueChanged(HtmlAudioParam* pParam);
};

