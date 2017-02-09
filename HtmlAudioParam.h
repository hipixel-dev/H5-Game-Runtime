#pragma once
#include "JSVMInterface.h"
class HtmlAudioNode;
class HtmlAudioParam :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlAudioParam, CustomClassBase)
		
public:
	BEGIN_CUSTOM_FUNCTION

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("value")
		DEFINE_CUSTOM_PROPERTY("defaultValue")

		END_CUSTOM_PROPERTY

	HtmlAudioNode* m_pNode = nullptr;
	double m_value=0;
	double m_defaultValue=0;

	HtmlAudioParam();
	virtual ~HtmlAudioParam();

	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
};

