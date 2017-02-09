#include "stdafx.h"
#include "HtmlScript.h"
IMPLEMENT_CUSTOM_CLASS(HtmlScript, HtmlEventTarget)

HtmlScript::HtmlScript()
{
}


HtmlScript::~HtmlScript()
{
}

bool HtmlScript::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HtmlScriptElement", HtmlEventTarget::GetPrototype_s());
}

bool HtmlScript::OnConstructor(IFunctionCallbackArgs&args)
{
	bool re = HtmlEventTarget::OnConstructor(args);
	CustomClassAutoThisObj cc(this, args, true);
	SetTagName(args.getContext(), "script");
	return re;
}

bool HtmlScript::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "src"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_src.c_str()));
		return true;
	}
	return HtmlEventTarget::OnGetProperty(args);
}

bool HtmlScript::OnSetProperty(SetPropertyArgs&args)
{
	CustomClassAutoThisObj cc(this, args);
	const char*name = args.GetName();

	if (!PROPCMP(name, "src"))
	{
		m_src = args.GetValue().GetString(args.GetContext());
		m_src = JSVM::GetInstance()->GetFullPath(m_src);
		std::string &&szAsync = args.GetThisObject().GetProperty(args.GetContext(), "async").GetString(args.GetContext());
		LoadScript(args.GetContext(), m_src, true, false);
		return true;
	}
	else if (!PROPCMP(name, "innerText"))
	{
		std::string &&text = args.GetValue().GetString(args.GetContext());
		LoadScript(args.GetContext(), text, true, true);
	}
	return HtmlEventTarget::OnSetProperty(args);
}

bool HtmlScript::LoadScript(const HandleContext&context, const std::string& name, bool bAsync,bool bIsText)
{
	m_src = name;
// 	if (name == "./frameworks/cocos2d-html5/cocos2d/labels/CCLabelBMFont.js")
// 	{
// 		int ddd = 3;
// 	}
	if (!bAsync)
	{
		if (!bIsText)
		{
			JSVM::GetInstance()->runScript(name.c_str());
		}
		else
		{
			JSVM::GetInstance()->RunScriptText(name.c_str(), name.c_str());
		}
		ExecEventListener(context, "load");
	}
	else
	{
		if (!bIsText)
		{
			JSVM::GetInstance()->runScriptAsync(name.c_str(), GetJsObject());
		}
		else
		{
			JSVM::GetInstance()->RunScriptTextAsync(name.c_str(), name.c_str(), GetJsObject());
		}
	}
	return true;
// 	HttpRequestInterface*pHttp = JSVM::GetInstance()->GetHttpRequestInterface();
// 	int8_t *pData = nullptr;
// 	int nLen = 0;
// 	int nRet = pHttp->SendGetRequest(name.c_str(), nullptr, pData, nLen);
// 	if (nRet != 200)return false;
// 	if (!bAsync)
// 	{
// 		JSVM::GetInstance()->RunScriptText((char*)pData, name.c_str());
// 	}
// 	else
// 	{
// 		JSVM::GetInstance()->RunScriptTextAsync((char*)pData, name.c_str());
// 	}
	return true;
}

void HtmlScript::OnFinalize(FinalizeArgs&args)
{
// 	OutputDebugStringA("OnFinalize:");
// 	OutputDebugStringA(m_src.c_str());
// 	OutputDebugStringA("\n");
	HtmlEventTarget::OnFinalize(args);
}
