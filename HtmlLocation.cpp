#include "stdafx.h"
#include "HtmlLocation.h"
#include "JSVMInterface.h"
IMPLEMENT_CUSTOM_CLASS(HtmlLocation, CustomClassBase)

HtmlLocation::HtmlLocation()
{
}


HtmlLocation::~HtmlLocation()
{
}

bool HtmlLocation::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "Location", HandleObject());
}

bool HtmlLocation::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "href"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(),JSVM::GetInstance()->m_szUrl.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "search"))
	{
		std::string str = JSVM::GetInstance()->m_szUrl;
		int i = str.find('?');
		if (i != str.npos)
		{
			str = str.substr(i);
		}
		else str = "";
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), str.c_str()));
		return true;
	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlLocation::OnSetProperty(SetPropertyArgs&args)
{
	CustomClassAutoThisObj cc(this, args);
	const char*name = args.GetName();
	if (!PROPCMP(name, "href"))
	{
		JSVM::GetInstance()->GetSystemInterface()->OpenUrl(args.GetValue().GetString(args.GetContext()).c_str());
	}
	return CustomClassBase::OnSetProperty(args);
}

void HtmlLocation::OnFinalize(FinalizeArgs&args)
{
	return CustomClassBase::OnFinalize(args);
}

bool HtmlLocation::OnConstructor(IFunctionCallbackArgs&args)
{
	return CustomClassBase::OnConstructor(args);
}

bool HtmlLocation::reload(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlLocation::assign(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlLocation::replace(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}
