#include "stdafx.h"
#include "HtmlInput.h"

IMPLEMENT_CUSTOM_CLASS(HtmlInput, HtmlEventTarget)
HtmlInput::HtmlInput()
{
}


HtmlInput::~HtmlInput()
{
	if (m_ctrl)
	{
		JSVM::GetInstance()->GetSystemInterface()->DeleteCtrl(m_ctrl);
	}
}

bool HtmlInput::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLInputElement", HtmlEventTarget::GetPrototype_s());
}

bool HtmlInput::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "value"))
	{
		SystemInterface *psi=JSVM::GetInstance()->GetSystemInterface();
		
		std::string &&value = psi->GetCtrlText(m_ctrl);
		args.SetRetValue(LocalValue(args.GetContext()).SetString(value.c_str()));
		return true;
	}
	return HtmlEventTarget::OnGetProperty(args);
}

bool HtmlInput::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "value"))
	{
		std::string &&value = args.GetValue().GetString(args.GetContext());
		SystemInterface *psi = JSVM::GetInstance()->GetSystemInterface();
		psi->SetCtrlText(m_ctrl, value.c_str());
		return true;
	}
	return HtmlEventTarget::OnSetProperty(args);
}

bool HtmlInput::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = HtmlEventTarget::OnConstructor(args);
	if (!ret)return false;

	SystemInterface*sysint = JSVM::GetInstance()->GetSystemInterface();
	m_ctrl=sysint->CreateCtrl(CTRL_TEXTBOX,this, 0, 0, 0, 0);

	return true;
}

void HtmlInput::OnAddToParent(const HandleContext&context, HtmlElement*pParent)
{
// 	SystemInterface*sysint = JSVM::GetInstance()->GetSystemInterface();
// 	sysint->ShowCtrl(m_ctrl, true);
}

void HtmlInput::OnRemoveFromParent(const HandleContext&context, HtmlElement*pParent)
{
	SystemInterface*sysint = JSVM::GetInstance()->GetSystemInterface();
	sysint->ShowCtrl(m_ctrl, false);
}

void HtmlInput::OnSetStyle(const HandleContext&context,const char*szName)
{
	HtmlEventTarget::OnSetStyle(context, szName);
	if (!strcmp(szName,"font"))
	{
		SystemInterface*sysint = JSVM::GetInstance()->GetSystemInterface();
		HtmlStyle*pstyle = GetStyle();
		FontInfo&&finfo=pstyle->GetFontInfo(context);
		sysint->SetFont(m_ctrl,finfo);
	}
// 	SystemInterface*sysint = JSVM::GetInstance()->GetSystemInterface();
// 	HtmlStyle*pstyle=this->GetStyle(/*context*/);
// 	if (!strcmp(szName, "width"))
// 	{
// 		int x, y, width, height;
// 		x = GetStyledLeft(/*context*/);
// 		y = GetStyledTop(/*context*/);
// 		width = GetStyledWidth(/*context*/);
// 		height = GetStyledHeight(/*context*/);
// 
// 		sysint->MoveCtrl(m_ctrl,x,y,width,height);
// 
// 	}
}

void HtmlInput::OnMeasure(int parentX, int parentY, int parentWidth, int parentHeight)
{
	HtmlEventTarget::OnMeasure(parentX, parentY, parentWidth, parentHeight);
	SystemInterface*sysint = JSVM::GetInstance()->GetSystemInterface();
	sysint->MoveCtrl(m_ctrl, GetLeft(), GetTop(), GetWidth(), GetHeight());
}

bool HtmlInput::blur(IFunctionCallbackArgs&args)
{
	SystemInterface*pint = JSVM::GetInstance()->GetSystemInterface();
	return pint->FocusCtrl(m_ctrl, false);
}

bool HtmlInput::focus(IFunctionCallbackArgs&args)
{
	SystemInterface*pint=JSVM::GetInstance()->GetSystemInterface();
	return pint->FocusCtrl(m_ctrl, true);
}

bool HtmlInput::select(IFunctionCallbackArgs&args)
{
	SystemInterface*pint = JSVM::GetInstance()->GetSystemInterface();
	return pint->SelectText(m_ctrl, 0,-1);
}
