#include "stdafx.h"
#include "HtmlNavigator.h"

IMPLEMENT_CUSTOM_CLASS(HtmlNavigator, CustomClassBase)

HtmlNavigator::HtmlNavigator()
{
}


HtmlNavigator::~HtmlNavigator()
{
}

bool HtmlNavigator::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "Navigator", HandleObject());
}

bool HtmlNavigator::OnGetProperty(GetPropertyArgs&args)
{
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlNavigator::OnSetProperty(SetPropertyArgs&args)
{
	return CustomClassBase::OnSetProperty(args);
}

void HtmlNavigator::OnFinalize(FinalizeArgs&args)
{
	return CustomClassBase::OnFinalize(args);
}

bool HtmlNavigator::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = CustomClassBase::OnConstructor(args);
    
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject objthis(args.getContext(), GetJsObject());
	objthis.SetProperty("appCodeName", "Mozilla");
	objthis.SetProperty("appName", "IE");
    objthis.SetProperty("appMinorVersion", "0");
	objthis.SetProperty("buildID", "1");
	objthis.SetProperty("cookieEnabled",ValueBase().SetBoolean(true));
	objthis.SetProperty("cpuClass", "x86");
	objthis.SetProperty("language", "chs");
	objthis.SetProperty("browserLanguage", "chs");
	objthis.SetProperty("systemLanguage", "chs");
	objthis.SetProperty("userLanguage", "chs");
	objthis.SetProperty("onLine", ValueBase().SetBoolean(true));
    
#if defined(_WIN32)
    objthis.SetProperty("appVersion", "Win");
    objthis.SetProperty("platform", "Windows");
    objthis.SetProperty("userAgent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/14.0.835.163 Safari/535.1 iphone (iPhone; CPU iPhone OS 8_0 like Mac OS X)");
#elif defined(_IOS)
    objthis.SetProperty("appVersion", "iPhone");
    objthis.SetProperty("platform", "iPhone");
    //objthis.SetProperty("userAgent", "Mozilla/5.0 (iPhone; CPU OS 6_0 like Mac OS X) AppleWebKit/536.26 (KHTML, like Gecko) Version/6.0 Mobile/10A403 Safari/8536.25");
    objthis.SetProperty("userAgent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/14.0.835.163 Safari/535.1 iphone (iPhone; CPU iPhone OS 8_0 like Mac OS X)");
#else
    objthis.SetProperty("appVersion", "Android");
    objthis.SetProperty("platform", "Android");
    objthis.SetProperty("userAgent", "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/14.0.835.163 Safari/535.1 iphone (iPhone; CPU iPhone OS 8_0 like Mac OS X)");
#endif

	return ret;
}

bool HtmlNavigator::javaEnabled(IFunctionCallbackArgs&args)
{
	args.setReturnValue(ValueBase().SetBoolean(true));
	return true;
}

bool HtmlNavigator::taintEnabled(IFunctionCallbackArgs&args)
{
	args.setReturnValue(ValueBase().SetBoolean(false));
	return true;
}
