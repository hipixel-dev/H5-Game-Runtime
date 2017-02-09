#include "stdafx.h"
#include "HtmlAudioParam.h"
#include "HtmlAudioNode.h"


IMPLEMENT_CUSTOM_CLASS(HtmlAudioParam, CustomClassBase)

HtmlAudioParam::HtmlAudioParam()
{
}


HtmlAudioParam::~HtmlAudioParam()
{
}

bool HtmlAudioParam::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "AudioParam", HandleObject());
}

bool HtmlAudioParam::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "value"))
	{
		args.SetRetValue(ValueBase().SetNumber(m_value));
		return true;
	}
	else if (!PROPCMP(name, "defaultValue"))
	{
		args.SetRetValue(ValueBase().SetNumber(m_defaultValue));
		return true;
	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlAudioParam::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "value"))
	{
		m_value = args.GetValue().GetNumber();
		if (m_pNode)
		{
			m_pNode->OnValueChanged(this);
		}
		return true;
	}
	else if (!PROPCMP(name, "defaultValue"))
	{
		m_defaultValue = args.GetValue().GetNumber();
		return true;
	}
	return CustomClassBase::OnSetProperty(args);
}
