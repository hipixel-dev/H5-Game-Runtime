#include "stdafx.h"
#include "HtmlDiv.h"
#include "HtmlStyle.h"

IMPLEMENT_CUSTOM_CLASS(HtmlDiv, HtmlEventTarget)
HtmlDiv::HtmlDiv()
{
}


HtmlDiv::~HtmlDiv()
{
}

bool HtmlDiv::InitClass(const HandleContext&context)
{
	if (!HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLDiv", HtmlEventTarget::GetPrototype_s());
}

bool HtmlDiv::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = HtmlEventTarget::OnConstructor(args);
	CustomClassAutoThisObj cc(this, args, true);
	this->SetTagName(args.getContext(), "DIV");
	return ret;
}
bool HtmlDiv::OnSetProperty(SetPropertyArgs&args)
{
	const char*str = args.GetName();

	return HtmlEventTarget::OnSetProperty(args);
}



bool HtmlDiv::OnGetProperty(GetPropertyArgs&args)
{
	const char*str = args.GetName();
	if (!PROPCMP(str, "clientHeight") || !PROPCMP(str, "offsetHeight"))
	{
		CustomClassAutoThisObj cc(this, args);
		//计算innerHTML字体高度（cocos2d）
// 		LocalObject obstyle(args.GetContext(), args.GetThisObject().GetProperty(args.GetContext(), "style").GetObject());
// 		if (obstyle.IsNull())
// 		{
// 			args.SetRetValue(ValueBase().SetInt(0));
// 			return true;
// 		}
		HtmlEventTarget::OnGetProperty(args);
		int clientheight = args.GetRetValue().GetInt();

		HtmlStyle*pStyle = GetStyle(/*args.GetContext()*/);// dynamic_cast<HtmlStyle*>(FindCustomClass(obstyle));
		assert(pStyle);
		
//		std::string innerHTML = args.GetThisObject().GetProperty(args.GetContext(), "innerHTML").GetString(args.GetContext());
		FontInfo finfo = pStyle->GetFontInfo(args.GetContext());
		ImageData data;
		JSVM::GetInstance()->GetImageDecoder()->CreateStringImage(m_szText.c_str(), finfo, data, true,0);
		args.SetRetValue(ValueBase().SetInt(max(data.nHeight, clientheight)));
		return true;

	}
	else if (!PROPCMP(str, "clientWidth"))
	{//cocos2d没用到这个属性，暂时先直接返回宽度

	}
	return HtmlEventTarget::OnGetProperty(args);
}

