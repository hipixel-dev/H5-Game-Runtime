#include "stdafx.h"
#include "HtmlXMLHttpRequest.h"
#include "HtmlDomParser.h"
#include "JSVMInterface.h"
IMPLEMENT_CUSTOM_CLASS(HtmlXMLHttpRequest, CustomClassBase)

HtmlXMLHttpRequest::HtmlXMLHttpRequest()
{
}


HtmlXMLHttpRequest::~HtmlXMLHttpRequest()
{
//	delete m_pData;
}

bool HtmlXMLHttpRequest::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "XMLHttpRequest", HandleObject());
}

bool HtmlXMLHttpRequest::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "readyState"))
	{
		args.SetRetValue(ValueBase().SetInt(m_readyState));
		return true;
	}
	else if (!PROPCMP(name, "status"))
	{
		args.SetRetValue(ValueBase().SetInt(m_status));
		return true;
	}
	else if (!PROPCMP(name, "responseType"))
	{
		switch (m_ResponseType)
		{
		case RST_ARRAYBUFFER:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(),"arraybuffer")); break;
		case RST_BLOB:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "blob")); break;
		case RST_DOCUMENT:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "document")); break;
		case RST_JSON:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "json")); break;
		default:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "text")); break;
		}
		
		return true;
	}
// 	else if (!strcmp(name , "responseText"))
// 	{
// 		if (m_readyState < 4 || !m_pData)
// 		{
// 			args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), ""));
// 		}
// 		else
// 		{
// 			args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(),(const char*) m_pData));//可能需要转换成UTF8
// 		}
// 		return true;
// 	}
// 	else if (!strcmp(name , "responseXML"))
// 	{
// 		LocalObject objxml(args.GetContext(),HtmlDomParser::CreateObjFromXml(args.GetContext(), (const char*)m_pData));
// 		args.SetRetValue(objxml);
// 		return true;
// 	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlXMLHttpRequest::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "responseType"))
	{
		std::string&&str = args.GetValue().GetString(args.GetContext());
		if (str == "arraybuffer")m_ResponseType = RST_ARRAYBUFFER;
		else if (str == "blob")m_ResponseType = RST_BLOB;
		else if (str == "document")m_ResponseType = RST_DOCUMENT;
		else if (str == "json")m_ResponseType = RST_JSON;
		else m_ResponseType = RST_TEXT;
		return true;
	}
	return CustomClassBase::OnSetProperty(args);
}

void HtmlXMLHttpRequest::OnFinalize(FinalizeArgs&args)
{
	return CustomClassBase::OnFinalize(args);
}

bool HtmlXMLHttpRequest::OnConstructor(IFunctionCallbackArgs&args)
{
	return CustomClassBase::OnConstructor(args);
}

bool HtmlXMLHttpRequest::open(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "open");
	std::string method;
	m_szMethod = args[0].GetString((args.getContext()));
	
	m_szURL = args[1].GetString(args.getContext());
	m_bAsync = args[2].GetBoolean();
	m_readyState = 1;
	return true;
}

bool HtmlXMLHttpRequest::send(IFunctionCallbackArgs&args)
{
	
	if (args.length() > 0)
	{

	}
	m_readyState = 2;
	struct RET
	{
		char*pData;
		int nLen;
	};
	class MyAsyncTask :public AsyncTask<std::string, RET>
	{
	public:
		HtmlXMLHttpRequest*m_pThis;
		HandleContext m_context;
		
		MyAsyncTask(HtmlXMLHttpRequest*pThis,const HandleContext&ctx)
		{
			m_pThis = pThis;
			m_context = ctx;
		}

		virtual RET doInBackground(std::string para) override
		{
			RET ret = {};
			char*m_pData = nullptr;
			int m_nDataLen = 0;
			std::string url = JSVM::GetInstance()->GetFullPath(m_pThis->m_szURL);

			if (!stricmp(m_pThis->m_szMethod.c_str(), "get"))
			{
				m_pThis->m_status = JSVM::GetInstance()->GetHttpRequestInterface()->SendGetRequest(url.c_str(), m_pThis->m_ResponseType, nullptr, m_pData, m_nDataLen);
			}
			else if (!stricmp(m_pThis->m_szMethod.c_str(), "post"))
			{
				
				std::string&szPost = para;
				m_pThis->m_status = JSVM::GetInstance()->GetHttpRequestInterface()->SendPostRequest(url.c_str(), szPost.c_str(), szPost.length(), m_pThis->m_szContentType.c_str(), nullptr, m_pData, m_nDataLen);
			
			}
			else
			{
				assert(0);
				return ret;
			}
			//		if (m_status == 200)
			{
				m_pThis->m_readyState = 4;
			}
			if (m_pThis->m_status != 200)
			{
				//			assert(0);
				return ret;
			}
			if (!m_pData)
			{
				int dd = 3;
			}
			ret.pData = m_pData;
			ret.nLen = m_nDataLen;
			return ret;

			
		}

		virtual void OnPostExecute(RET data) override
		{
			LocalObject objthis(m_context, m_pThis->GetObjFromNoGC());
			CustomClassAutoThisObj cc(m_pThis, objthis);
			m_pThis->ProtectObjNoGC(false);
			if (m_pThis->m_ResponseType == RST_TEXT || m_pThis->m_ResponseType == RST_DOCUMENT || m_pThis->m_ResponseType == RST_JSON)
			{
				if (!data.pData)
				{
					objthis.SetProperty("responseText", ValueBase());
					goto line1;
				}
				objthis.SetProperty("responseText", data.pData);
				if (m_pThis->m_ResponseType == RST_DOCUMENT)
				{
					LocalObject objxml(m_context, HtmlDomParser::CreateObjFromXml(m_context, data.pData));
					if (!objxml.IsNull())
					{
						objthis.SetProperty("responseXML", objxml);
					}
					else
					{
						objthis.SetProperty("responseXML", ValueBase());
					}
				}
			}
			else
			{
				objthis.SetProperty("responseText", ValueBase().SetNull());
				if (m_pThis->m_ResponseType == RST_ARRAYBUFFER)
				{
					LocalObject objdata(m_context);
					objdata.CreatePrimitiveArray(m_context, data.nLen, LocalObject::AT_UINT8, data.pData);
					objthis.SetProperty(m_context, "response", objdata);
				}
				else assert(0);
			}







			//		PrintDescribeScriptedCaller(args.getContext());
			line1:
			m_pThis->onreadystatechange(m_context);
			delete []data.pData;
			delete this;
		}

	};
	MyAsyncTask*tsk = new MyAsyncTask(this,args.getContext());
	ProtectObjNoGC(true);
	if (m_bAsync)
	{

		if (args.length() > 0 && !args[0].IsNull())
		{
			if (args[0].IsString())
			{
				std::string&&para = args[0].GetString(args.getContext());
				tsk->Start(para);
			}
			else
				assert(0);
		}
		else
		{
			tsk->Start("");
		}
	}
	else
	{
		if (args.length() > 0 && !args[0].IsNull())
		{
			if (args[0].IsString())
			{
				std::string&&para = args[0].GetString(args.getContext());
				RET&&re=tsk->doInBackground(para);
				tsk->OnPostExecute(re);
			}
			else
				assert(0);
		}
		else
		{
			RET&&re = tsk->doInBackground("");
			tsk->OnPostExecute(re);
		}
	}
// 	char*m_pData = nullptr;
// 	int m_nDataLen = 0;
// 	std::string url = JSVM::GetInstance()->GetFullPath(m_szURL);
// 
// 	if (!stricmp(m_szMethod.c_str(), "get"))
// 	{
// 		m_status = JSVM::GetInstance()->GetHttpRequestInterface()->SendGetRequest(url.c_str(), m_ResponseType, nullptr, m_pData, m_nDataLen);
// 	}
// 	else if (!stricmp(m_szMethod.c_str(), "post"))
// 	{
// 		CHECK_ARGS_COUNT(1, "HtmlXMLHttpRequest::send(post)");
// 		if (args[0].IsString())
// 		{
// 			std::string&&szPost = args[0].GetString(args.getContext());
// 			m_status = JSVM::GetInstance()->GetHttpRequestInterface()->SendPostRequest(url.c_str(), szPost.c_str(), szPost.length(), m_szContentType.c_str(), nullptr, m_pData, m_nDataLen);
// 		}
// 		else 
// 			assert(0);
// 	}
// 	else
// 	{
// 		assert(0);
// 		return true;
// 	}
// //		if (m_status == 200)
// 	{
// 		m_readyState = 4;
// 	}
// 	if (m_status != 200)
// 	{
// //			assert(0);
// 		return true;
// 	}
// 	if (!m_pData)
// 	{
// 		int dd = 3;
// 	}
// 	LocalObject objthis(args.getContext(), GetJsObject());
// 
// 	if (m_ResponseType == RST_TEXT || m_ResponseType == RST_DOCUMENT || m_ResponseType == RST_JSON )
// 	{
// 		if (!m_pData)
// 		{
// 			objthis.SetProperty("responseText", ValueBase());
// 			return true;
// 		}
// 		objthis.SetProperty("responseText", m_pData);
// 		if (m_ResponseType == RST_DOCUMENT)
// 		{
// 			LocalObject objxml(args.getContext(), HtmlDomParser::CreateObjFromXml(args.getContext(), m_pData));
// 			if (!objxml.IsNull())
// 			{
// 				objthis.SetProperty("responseXML", objxml);
// 			}
// 			else
// 			{
// 				objthis.SetProperty("responseXML", ValueBase());
// 			}
// 		}
// 	}
// 	else
// 	{
// 		objthis.SetProperty("responseText", ValueBase().SetNull());
// 		if (m_ResponseType == RST_ARRAYBUFFER)
// 		{
// 			LocalObject objdata(args.getContext());
// 			objdata.CreatePrimitiveArray(args.getContext(), m_nDataLen, LocalObject::AT_UINT8, m_pData);
// 			GetJsObject().SetProperty(args.getContext(),"response", objdata);
// 		}
// 		else assert(0);
// 	}
// 
// 		
// 		
// 
// 
// 
// 
// //		PrintDescribeScriptedCaller(args.getContext());
// 	onreadystatechange(args.getContext());
// 	delete[]m_pData;
	


	
	return true;
}

bool HtmlXMLHttpRequest::onreadystatechange(const HandleContext&context)
{
	LocalObject objthis(context, GetJsObject());
	LocalValue fun(context, objthis.GetProperty(context, "onreadystatechange"));

	if (!fun.IsUndefined())
	{
		JSVM::GetInstance()->executeFunctionAsync(context, objthis,fun);
		return true;
	}
	else
	{
		fun = LocalValue(context, objthis.GetProperty(context, "onload"));

		if (!fun.IsUndefined())
		{
			JSVM::GetInstance()->executeFunctionAsync(context, objthis,fun);
			return true;
		}
	}
	return false;
}

bool HtmlXMLHttpRequest::setRequestHeader(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "setRequestHeader");
	std::string&&szkey = args[0].GetString(args.getContext());
	std::string&&szval = args[1].GetString(args.getContext());
	if (!_stricmp(szkey.c_str(), "Content-Type"))
	{
		m_szContentType = szval;
	}
	else
		assert(0);
	return true;
}

bool HtmlXMLHttpRequest::abort(IFunctionCallbackArgs&args)
{
	m_readyState = 0;
	return true;
}

bool HtmlXMLHttpRequest::getAllResponseHeaders(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlXMLHttpRequest::getResponseHeader(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}
