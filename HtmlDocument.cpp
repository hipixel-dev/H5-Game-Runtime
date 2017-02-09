#include "stdafx.h"
#include "HtmlDocument.h"
#include "HtmlCanvas.h"
#include "HtmlImgClass.h"
#include "HtmlScript.h"
#include "HtmlDiv.h"
#include "HtmlInput.h"
#include "HtmlAudio.h"
#include "HtmlLink.h"
#include "HtmlSpan.h"
#include "HtmlP.h"
#include "HtmlTextarea.h"
#include "HtmlStyleElement.h"
#include "HtmlUL.h"
#include "HtmlLI.h"
#include "HtmlIFrame.h"
IMPLEMENT_CUSTOM_CLASS(HtmlDocument, HtmlEventTarget)

HtmlDocument::HtmlDocument()
{
}


HtmlDocument::~HtmlDocument()
{
}

bool HtmlDocument::OnGetProperty(GetPropertyArgs&args)
{
	const char* str = args.GetName();
	if (!PROPCMP(str, "clientWidth"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetWidth()));
		return true;
	}
	else if (!PROPCMP(str, "clientHeight"))
	{
		args.SetRetValue(ValueBase().SetInt(OpenglContext::GetHeight()));
		return true;
	}
	else if (!PROPCMP(str, "scrollLeft"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(str, "scrollTop"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(str, "hidden"))
	{
		args.SetRetValue(ValueBase().SetBoolean(false));
		return true;
	}
	else if (!PROPCMP(str, "URL"))
	{
		JSVM::GetInstance()->m_szUrl = "word/Tip.png";
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), JSVM::GetInstance()->m_szUrl.c_str()));
		return true;
	}
	else if (!PROPCMP(str, "cookie"))
	{
		if (!m_bCookieLoaded)
		{
//			LoadCookie();
		}
		std::string &&ret = GetCookieStr();
		
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), ret.c_str()));
		return true;
	}
	return HtmlEventTarget::OnGetProperty(args);
}
void Trim(char*&str)
{
	while ((*str) == ' ' || (*str) == '\t' || (*str) == '\r' || (*str) == '\n')str++;
	int len = strlen(str);
	for (int i = len - 1; i >= 0; i--)
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n')str[i] = 0;
		else break;
	}
}
bool HtmlDocument::OnSetProperty(SetPropertyArgs&args)
{
 	const char* str = args.GetName();
// 	if (!PROPCMP(str, "URL"))
// 	{
// 		std::string szurl = args.GetValue().GetString(args.GetContext());
// 		int dd = 3;
// 	}
	if (!PROPCMP(str, "cookie"))
	{
		if (!m_bCookieLoaded)LoadCookie();
		std::string val = args.GetValue().GetString(args.GetContext());
		SetCookieStr(val);
//		SaveCookie();
		return true;
	}
	return HtmlEventTarget::OnSetProperty(args);
}

void HtmlDocument::OnFinalize(FinalizeArgs&args)
{
	HtmlEventTarget::OnFinalize(args);
}

bool HtmlDocument::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = HtmlEventTarget::OnConstructor(args);
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject thisobj(args.getContext(), args.getReturnValue().GetObject());
	thisobj.SetProperty("tagName", LocalValue(args.getContext()).SetString(args.getContext(), "body"));
	thisobj.SetProperty(args.getContext(), "body", thisobj);
	thisobj.SetProperty(args.getContext(), "hidden", ValueBase().SetBoolean(false));
	thisobj.SetProperty(args.getContext(),"ontouchstart", ValueBase().SetNull());
	thisobj.SetProperty(args.getContext(), "ontouchmove", ValueBase().SetNull());
	thisobj.SetProperty(args.getContext(), "ontouchend", ValueBase().SetNull());


	thisobj.SetProperty("head", HtmlElement::CreateObject(args.getContext()));
	return ret;
}

bool HtmlDocument::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	bool ret = InitClass_s(context, "HtmlDocument", HtmlEventTarget::GetPrototype_s());

	return ret;
}

bool HtmlDocument::createElement(IFunctionCallbackArgs&args)
{


	CHECK_ARGS_COUNT(1, "HtmlDocument::createElement");
	std::string &&name = args[0].GetString(args.getContext());
	if (!_stricmp(name.c_str(), "canvas"))
	{
//		PrintDescribeScriptedCaller(args.getContext());
		args.setReturnValue(HtmlCanvas::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "img"))
	{
		args.setReturnValue(HtmlImgClass::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "script"))
	{
		args.setReturnValue(HtmlScript::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "div"))
	{
		args.setReturnValue(HtmlDiv::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "input"))
	{
		args.setReturnValue(HtmlInput::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "audio"))
	{
		args.setReturnValue(HtmlAudio::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "link"))
	{
		args.setReturnValue(HtmlLink::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "span"))
	{
		args.setReturnValue(HtmlSpan::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "p"))
	{
		args.setReturnValue(HtmlP::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "textarea"))
	{
		args.setReturnValue(HtmlTextarea::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "style"))
	{
		args.setReturnValue(HtmlStyleElement::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "ul"))
	{
		args.setReturnValue(HtmlUL::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "li"))
	{
		args.setReturnValue(HtmlLI::CreateObject(args.getContext()));
		return true;
	}
	else if (!_stricmp(name.c_str(), "iframe"))
	{
		args.setReturnValue(HtmlIFrame::CreateObject(args.getContext()));
		return true;
	}
	ReportError(args.getContext(), "暂不支持%s", name.c_str());
	return false;
}

bool HtmlDocument::write(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlDocument::write");
	std::string &&str = args[0].GetString(args.getContext());
	LocalObject objthis(args.getContext(), GetJsObject());
	JSVM::GetInstance()->LoadHtmlFromInnerHtml(objthis, str);
	return true;
}

bool HtmlDocument::getElementsByTagName(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "getElementByTagName");
	std::string &&str = args[0].GetString(args.getContext());

	if (!_stricmp(str.c_str(), "head"))
	{
		LocalObject objret(args.getContext());
		objret.CreateArrayObject(1);
		objret.SetElement(0, GetJsObject());
		args.setReturnValue(objret);
		return true;
	}
	else if (!_stricmp(str.c_str(), "body"))
	{
		LocalObject objret(args.getContext());
		objret.CreateArrayObject(1);
		objret.SetElement(0, GetJsObject());
		args.setReturnValue(objret);
		return true;
	}
	return HtmlEventTarget::getElementsByTagName(args);
}

//根据style计算m_left/m_top/m_width/m_height
void HtmlDocument::Measure(const HandleContext&context)
{
	m_left = 0;
	m_top = 0;
	m_width = OpenglContext::GetWidth();
	m_height = OpenglContext::GetHeight();
	for (std::list<HtmlElement*>::iterator it = m_childList.begin(); it != m_childList.end(); ++it)
	{
		HtmlElement*pchild = *it;
		pchild->Measure(0, 0, m_width, m_height);
	}
}

int HtmlDocument::GetWidth()
{
	return OpenglContext::GetWidth();
}

int HtmlDocument::GetHeight()
{
	return OpenglContext::GetHeight();
}

int HtmlDocument::GetClientWidth()
{
	return GetWidth();
}

int HtmlDocument::GetClientHeight()
{
	return GetHeight();
}

void HtmlDocument::SaveCookie()
{
	FileIOInterface*pfileio = JSVM::GetInstance()->GetFileIOInterface();
	FileHandle*file=pfileio->Open(JSVM::GetInstance()->GetCookiePath().c_str(),FileIOInterface::OP_READWRITE);
	if (!file)return;
	std::string&&str = GetCookieStr();
	pfileio->Write(file, str.c_str(), str.length());
	pfileio->Close(file);
	m_bCookieLoaded = true;
}

std::string HtmlDocument::GetCookieStr()
{
	std::string ret;
	for (std::map<std::string, std::string>::iterator it = m_cookie.begin(); it != m_cookie.end(); ++it)
	{
		ret.append(it->first + "=" + it->second + "; ");
	}
	return ret;
}

void HtmlDocument::SetCookieStr(std::string val)
{
	char*ctx=nullptr;
	char*p = strtok_s((char*)val.data(), ";", &ctx);
	while (p)
	{
		char*ctx2=nullptr;
		char*q = strtok_s(p, "=", &ctx2);
		if (q&&*q)
		{
			Trim(q);
			Trim(ctx2);
			if (*q)
			{
				m_cookie[q] = ctx2;
			}

		}
		p = strtok_s(nullptr, ";", &ctx);
	}
}

void HtmlDocument::LoadCookie()
{
	FileIOInterface*pfileio = JSVM::GetInstance()->GetFileIOInterface();
	FileHandle*file = pfileio->Open(JSVM::GetInstance()->GetCookiePath().c_str(), FileIOInterface::OP_READ);
	if (!file)return;
	int len=pfileio->GetLength(file);
	char*str = new char[len + 1];
	pfileio->Read(file, str, len);
	str[len] = 0;
	m_cookie.clear();
	SetCookieStr(str);
	delete[]str;
	pfileio->Close(file);
	m_bCookieLoaded = true;
}

