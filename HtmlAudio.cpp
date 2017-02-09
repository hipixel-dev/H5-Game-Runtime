#include "stdafx.h"
#include "HtmlAudio.h"
#include "JSVMInterface.h"
IMPLEMENT_CUSTOM_CLASS(HtmlAudio, HtmlEventTarget)

HtmlAudio::HtmlAudio()
{
	m_pAudio = JSVM::GetInstance()->CreateAudioInterface();
}


HtmlAudio::~HtmlAudio()
{
	m_pAudio->Release();
}

bool HtmlAudio::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	return InitClass_s(context, "Audio", HtmlEventTarget::GetPrototype_s());
}

bool HtmlAudio::canPlayType(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(1, "canPlayType");
	std::string str = args[0].GetString(args.getContext());
	if (str == "audio/mpeg")
	{
		args.setReturnValue(LocalValue(args.getContext()).SetString(args.getContext(), "maybe"));
	}
	else
	{
		args.setReturnValue(LocalValue(args.getContext()).SetString(args.getContext(), "no"));
	}
	return true;
}

bool HtmlAudio::load(IFunctionCallbackArgs&args)
{

	LocalObject objthis(args.getContext(), args.thisObj());
	JSVM::GetInstance()->executeFunctionAsync(args.getContext(), objthis, objthis.GetProperty("__load"));
// 	std::string szPath = objthis.GetProperty("src").GetString(args.getContext());
// 	if (szPath.empty())
// 	{
// 		m_readyState = 1;
// 		ExecEventListenerAsync(args.getContext(), "error");
// 		return true;
// 	}
// 	bool ret=m_pAudio->Open(szPath.c_str());
// 	if (!ret)
// 	{
// 		m_readyState = 1;
// 		ExecEventListenerAsync(args.getContext(), "error");
// 		return true;
// 	}
// 	m_readyState = 5;
// 	ExecEventListenerAsync(args.getContext(),"canplaythrough");
	return true;
}

bool HtmlAudio::__load(IFunctionCallbackArgs&args)
{
	LocalObject objthis(args.getContext(), args.thisObj());
	std::string szPath = objthis.GetProperty("src").GetString(args.getContext());
	if (szPath.empty())
	{
		m_readyState = 0; // HAVE_NOTHING
		ExecEventListenerAsync(args.getContext(), "error");
		return true;
	}
	szPath = JSVM::GetInstance()->GetFullPath(szPath);

	class MyAsyncTask :public AsyncTask < std::string, bool >
	{
	public:
		std::string szCache;
		HtmlAudio*m_pthis;
		MyAsyncTask(HtmlAudio*pthis)
		{
			m_pthis = pthis;
		}

		virtual bool doInBackground(std::string para) override
		{
			szCache = JSVM::GetInstance()->GetHttpRequestInterface()->GetFileToCachePath(para);
			if (szCache.empty())return false;
			return true;
			
		}

		virtual void OnPostExecute(bool Result) override
		{
			LocalObject objthis(JSVM::GetInstance()->GetContext(), m_pthis->GetObjFromNoGC());
			CustomClassAutoThisObj cc(m_pthis, objthis);
			m_pthis->ProtectObjNoGC(false);
			if (!Result)
			{
				m_pthis->m_readyState = 0; // HAVE_NOTHING
				m_pthis->ExecEventListenerAsync(JSVM::GetInstance()->GetContext(), "error");
				
			}
			else
			{
				bool ret = m_pthis->m_pAudio->Open(szCache.c_str());
			}
			m_pthis->m_readyState = 4; // HAVE_ENOUGH_DATA
			m_pthis->ExecEventListenerAsync(JSVM::GetInstance()->GetContext(), "canplaythrough");
			delete this;
		}

	};
	ProtectObjNoGC(true);
	MyAsyncTask*tsk = new MyAsyncTask(this);
// 	bool ret=tsk->doInBackground(szPath);
// 	tsk->OnPostExecute(ret);
	tsk->Start(szPath);


// 	std::string &&szCache = JSVM::GetInstance()->GetHttpRequestInterface()->GetFileToCachePath(szPath);
// 	if (szCache.empty())return false;
// 	bool ret = m_pAudio->Open(szCache.c_str());
// 	if (!ret)
// 	{
// 		m_readyState = 1;
// 		ExecEventListenerAsync(args.getContext(), "error");
// 		return true;
// 	}
// 	m_readyState = 5;
// 	ExecEventListenerAsync(args.getContext(), "canplaythrough");
	return true;
}


bool HtmlAudio::play(IFunctionCallbackArgs&args)
{
	if (m_volume == 0)
	{
		if (!m_pAudio->isOpen())
		{
			LocalObject objthis(args.getContext(), args.thisObj());
			JSVM::GetInstance()->executeFunctionAsync(args.getContext(), objthis, objthis.GetProperty("__load"));
		}
		return true;
	}
    
	LocalObject objthis(args.getContext(), args.thisObj());
	//if (m_readyState == 5)return true;
	if (m_src.empty())return true;
	if (!m_pAudio->isOpen())
	{
		std::string str = JSVM::GetInstance()->GetHttpRequestInterface()->GetFileToCachePath(m_src);
		if (str.empty())return true;
		m_pAudio->Open(str.c_str());
	}
	//	PrintDescribeScriptedCaller(cx, 0);
	m_pAudio->Play(0, m_bloop);
	return true;
}

bool HtmlAudio::pause(IFunctionCallbackArgs&args)
{
	if (m_pAudio->isOpen())
	{
		m_pAudio->Pause();
	}

	return true;
}

bool HtmlAudio::OnGetProperty(GetPropertyArgs&args)
{
	const char*str = args.GetName();
	if (!PROPCMP(str, "autoplay"))
	{
		args.SetRetValue(ValueBase().SetBoolean(m_bAutoPlay));
	}
	else if (!PROPCMP(str, "currentSrc"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_src.c_str()));
	}

	else if (!PROPCMP(str, "defaultMuted"))
	{
		args.SetRetValue(ValueBase().SetBoolean(m_bdefaultMuted));
	}



	else if (!PROPCMP(str, "loop"))
	{
		args.SetRetValue(ValueBase().SetBoolean(m_bloop));
	}
	else if (!PROPCMP(str, "muted"))
	{
		args.SetRetValue(ValueBase().SetBoolean(m_bmuted));
	}
	else if (!PROPCMP(str, "paused"))
	{
		args.SetRetValue(ValueBase().SetBoolean(m_bpaused));
	}
	// 	else if (str == "playbackRate")
	// 	{
	// 
	// 	}
	else if (!PROPCMP(str, "preload"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_preload.c_str()));
	}
	else if (!PROPCMP(str, "readyState"))
	{
		args.SetRetValue(ValueBase().SetInt(m_readyState));
	}
	else if (!PROPCMP(str, "src"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_src.c_str()));

	}
	else if (!PROPCMP(str, "volume"))
	{
		args.SetRetValue(ValueBase().SetNumber(m_volume));
	}
	else if (!PROPCMP(str, "currentTime"))
	{
		unsigned long dwTime = m_pAudio->GetCurrentPosition();
		args.SetRetValue(ValueBase().SetNumber(dwTime / 1000.0));
	}
	else if (!PROPCMP(str, "ended"))
	{
		bool isEnded = m_pAudio->isStop();
		args.SetRetValue(ValueBase().SetBoolean(isEnded));
	}
	else
	{

		return HtmlEventTarget::OnGetProperty(args);
	}
	return true;
}

bool HtmlAudio::OnSetProperty(SetPropertyArgs&args)
{
	const char*str = args.GetName();
	if (!PROPCMP(str, "autoplay"))
	{
		m_bAutoPlay = args.GetValue().GetBoolean();
	}
	else if (!PROPCMP(str, "currentSrc"))
	{
		m_src = args.GetValue().GetString(args.GetContext());
	}

	else if (!PROPCMP(str, "defaultMuted"))
	{
		m_bdefaultMuted = args.GetValue().GetBoolean();
	}


	else if (!PROPCMP(str, "ended"))
	{
		assert(0);
	}
	else if (!PROPCMP(str, "loop"))
	{
		m_bloop = args.GetValue().GetBoolean();
	}
	else if (!PROPCMP(str, "muted"))
	{
		m_bmuted = args.GetValue().GetBoolean();
	}
	else if (!PROPCMP(str, "paused"))
	{
		m_bpaused = args.GetValue().GetBoolean();
	}

	else if (!PROPCMP(str, "preload"))
	{
		m_preload = args.GetValue().GetString(args.GetContext());
	}
	else if (!PROPCMP(str, "readyState"))
	{
		m_readyState = args.GetValue().GetInt();
	}
	else if (!PROPCMP(str, "src"))
	{
		m_src = JSVM::GetInstance()->GetFullPath(args.GetValue().GetString(args.GetContext()));
//		m_szFullPath = JSVM::GetInstance()->GetFullPath(m_src);
	}
	else if (!PROPCMP(str, "volume"))
	{
		m_volume = args.GetValue().GetNumber();
	}
	else if (!PROPCMP(str, "currentTime"))
	{
		if (!m_src.empty())
		{
			double dtime;
			dtime = args.GetValue().GetNumber();
			m_pAudio->Seek(dtime * 1000);
		}
	}
	else
	{

		return HtmlEventTarget::OnSetProperty(args);
	}
	return true;
}

bool HtmlAudio::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret= HtmlEventTarget::OnConstructor(args);
	if (args.length() == 1)
	{
		std::string&&szfile = args[0].GetString(args.getContext());
		m_src = JSVM::GetInstance()->GetFullPath(szfile);
	}


	return ret;
}

