#include "stdafx.h"
#include "HtmlImgClass.h"
#include "OpenglContext.h"


IMPLEMENT_CUSTOM_CLASS(HtmlImgClass, HtmlEventTarget)

HtmlImgClass::HtmlImgClass()
{
}


HtmlImgClass::~HtmlImgClass()
{
	CleanImg();
}

bool HtmlImgClass::OnGetProperty(GetPropertyArgs&args)
{
	const char*str= args.GetName();
	if (!PROPCMP(str, "width"))
	{
		
		if (m_width == 0)
		{
			int dd = 3;
		}
		args.SetRetValue(ValueBase().SetInt(m_width));
		return true;
	}
	else if (!PROPCMP(str, "height"))
	{
		args.SetRetValue(ValueBase().SetInt(m_height));
		return true;
	}
	else if (!PROPCMP(str, "complete"))
	{
		args.SetRetValue(ValueBase().SetBoolean(IsLoaded()));
		return true;
	}
	else if (!PROPCMP(str, "src"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_src.c_str()));
	}
	return HtmlEventTarget::OnGetProperty(args);
}

bool HtmlImgClass::OnSetProperty(SetPropertyArgs&args)
{
	CustomClassAutoThisObj cc(this, args);
	LocalObject objthis(args.GetContext(), GetJsObject());
	const char*szName= args.GetName();
	if (!PROPCMP(szName, "src"))
	{
		std::string&&src = args.GetValue().GetString(args.GetContext());
// 		if (src.find("data:") == 0)
// 		{
// 			bool bLoaded = LoadImageData(src);
// 			if (bLoaded)
// 			{
// 				m_nTexID = OpenglContext::CreateGlTexID(m_imgData);
// 			}
// 			return true;
// 		}
		m_src = JSVM::GetInstance()->GetFullPath(src);
	
		JSVM::GetInstance()->executeFunctionAsync(args.GetContext(),objthis, objthis.GetProperty("__LoadImage"));



		return true;
	}
	else if (!PROPCMP(szName, "width"))
	{
		m_width = args.GetValue().GetInt();
		return true;
	}
	else if (!PROPCMP(szName, "height"))
	{
		m_height = args.GetValue().GetInt();
		return true;
	}
	else if (!PROPCMP(szName, "complete"))
	{
		return true;
	}
	
	return HtmlEventTarget::OnSetProperty(args);
}

void HtmlImgClass::OnFinalize(FinalizeArgs&args)
{

	return HtmlEventTarget::OnFinalize(args);
}

bool HtmlImgClass::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = HtmlEventTarget::OnConstructor(args);
	CustomClassAutoThisObj cc(this, args, true);
	SetTagName(args.getContext(), "img");
//	pob = new PersistentObject(args.getContext(), GetJsObject());
	return ret;
}

bool HtmlImgClass::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "HTMLImageElement", HtmlEventTarget::GetPrototype_s());
}

void HtmlImgClass::OnAddToParent(const HandleContext&context,HtmlElement*pParent)
{
	
}

void HtmlImgClass::OnRemoveFromParent(const HandleContext&context, HtmlElement*pParent)
{

}

bool HtmlImgClass::LoadImageData(const std::string&szSrc)
{
	CleanImg();
	if (m_imgData.IsInit())return true;
	if (szSrc.empty())return false;
	std::string&&szPath = JSVM::GetInstance()->GetFullPath(szSrc);
	std::string&&szCache = JSVM::GetInstance()->GetHttpRequestInterface()->GetFileToCachePath(szPath);
	bool ret = JSVM::GetInstance()->GetImageDecoder()->DecodeFile(szCache.c_str(), m_imgData);
	if (!ret)return false;
	
	if (m_width <= 0)m_width = m_imgData.nWidth;
	if (m_height <= 0)m_height = m_imgData.nHeight;
	// Use tightly packed data
	
	// Generate a texture object
//	m_nTexID=OpenglContext::CreateGlTexID(m_imgData);

	return true;
}

bool HtmlImgClass::IsLoaded()
{
	if (m_imgData.IsInit() && m_nTexID != 0)return true;
	return false;
}

void HtmlImgClass::CleanImg()
{
	if (m_nTexID)glDeleteTextures(1, &m_nTexID);
	m_nTexID = 0;
	m_imgData.Cleanup();
}

bool HtmlImgClass::__LoadImage(IFunctionCallbackArgs&args)
{
	LocalObject objthis(args.getContext(), GetJsObject());

	class myTask :public AsyncTask<std::string,bool>
	{
	public:
		HtmlImgClass*m_pThis;
		myTask(HtmlImgClass*pThis) :AsyncTask()
		{
			m_pThis = pThis;
		}
		virtual bool doInBackground(std::string para) override
		{
			bool bLoaded = m_pThis->LoadImageData(para);
			return bLoaded;
		}

		virtual void OnPostExecute(bool Result) override
		{
			LocalObject objthis(JSVM::GetInstance()->GetContext(), m_pThis->GetObjFromNoGC());
			CustomClassAutoThisObj cc(m_pThis, objthis);
			m_pThis->ProtectObjNoGC(false);
			if (Result)
			{
				m_pThis->m_nTexID = OpenglContext::CreateGlTexID(m_pThis->m_imgData);
				m_pThis->ExecEventListenerAsync(JSVM::GetInstance()->GetContext(), "load");
			}
			else
			{
				m_pThis->ExecEventListenerAsync(JSVM::GetInstance()->GetContext(), "error");
			}
			delete this;
		}

	};
	ProtectObjNoGC(true);
	myTask *tsk = new myTask(this);
	tsk->Start(m_src);


// 	bool bLoaded = LoadImageData(m_src);
// 	if (bLoaded)
// 	{
// 		ExecEventListenerAsync(args.getContext(), "load");
// 	}
// 	else
// 	{
// 		ExecEventListenerAsync(args.getContext(), "error");
// 	}
	return true;
}


