#include "stdafx.h"
#include "HtmlWebSocket.h"
#include "HtmlEvent.h"
IMPLEMENT_CUSTOM_CLASS(HtmlWebSocket, HtmlEventTarget)

HtmlWebSocket::HtmlWebSocket()
{
}


HtmlWebSocket::~HtmlWebSocket()
{
	Close();
//	SAFE_DELETE(m_hSocket);
// 	if (m_hSocket)
// 	{
// 		m_hSocket->close();
// 		SAFE_DELETE(m_hSocket);
// //		JSVM::GetInstance()->GetSocketInterface()->Close(m_hSocket);
// 		m_hSocket = nullptr;
// 	}
}

bool HtmlWebSocket::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "binaryType"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_binaryType.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "bufferedAmount"))
	{
		args.SetRetValue(ValueBase().SetInt(m_bufferedAmount));
		return true;
	}
	else if (!PROPCMP(name, "extensions"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_extensions.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "protocol"))
	{
		assert(0);
// 		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_protocol.c_str()));
// 		return true;
	}
	else if (!PROPCMP(name, "readyState"))
	{
		int state = 3;
		state = getReadyState();
		args.SetRetValue(ValueBase().SetInt(state));
		return true;
	}
	else if (!PROPCMP(name, "url"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_url.c_str()));
		return true;
	}
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlWebSocket::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "binaryType"))
	{
		m_binaryType = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "bufferedAmount"))
	{
		m_bufferedAmount = args.GetValue().GetInt();
		return true;
	}
	else if (!PROPCMP(name, "extensions"))
	{
		m_extensions = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "protocol"))
	{
		assert(0);
// 		m_protocol = args.GetValue().GetString(args.GetContext());
// 		return true;
	}

	return CustomClassBase::OnSetProperty(args);
}

void HtmlWebSocket::OnFinalize(FinalizeArgs&args)
{
	Close();
//	SAFE_DELETE(m_hSocket);
// 	if (m_hSocket)
// 	{
// 		m_hSocket->close();
// 		SAFE_DELETE(m_hSocket);
// //		JSVM::GetInstance()->GetSocketInterface()->Close(m_hSocket);
// 		m_hSocket = nullptr;
// 	}
}

bool HtmlWebSocket::OnConstructor(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(1, "HtmlWebSocket::OnConstructor");

	bool ret = HtmlEventTarget::OnConstructor(args);
	if (!ret)return false;

	CustomClassAutoThisObj cc(this, args, true);
	LocalObject objthis(args.getContext(), GetJsObject());


	m_url = args[0].GetString(args.getContext());
	if (args.length() > 1)
	{
		for (int i = 1; i < args.length();i++)
		{
			m_protocol.push_back(args[i].GetString(args.getContext()));
		}
		
	}

	ProtectObjNoGC(true);

    assert(!m_hSocket);
	
// 	m_hSocket = new network::WebSocket;
// 	if(m_protocol.size()>0)m_hSocket->init(*this, m_url, &m_protocol);
// 	else m_hSocket->init(*this, m_url);
// 
	SocketInterface*psi = JSVM::GetInstance()->GetSocketInterface();
	m_hSocket=psi->CreateSocket(this);
	std::string api;
	int port;
	std::string&&szIP=JSVM::GetHostName(m_url.c_str(), api, port);
	psi->Connect(m_hSocket, szIP.c_str(), port,api.c_str());

	return true;
}

bool HtmlWebSocket::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}
	bool ret= InitClass_s(context, "WebSocket", HtmlEventTarget::GetPrototype_s());
	JSVM::GetInstance()->RunScriptText("WebSocket.CONNECTING=0;WebSocket.OPEN=1;WebSocket.CLOSING=2;WebSocket.CLOSED=3;", nullptr);
	return ret;
}

bool HtmlWebSocket::close(IFunctionCallbackArgs&args)//因为是JS调用，所以在主线程
{
	m_hSocket->bClosing = true;
	ExecEventListener(JSVM::GetInstance()->GetContext(), "close");
	JSVM::GetInstance()->GetSocketInterface()->Close(m_hSocket);
//	this->m_hSocket->close();
	m_hSocket = nullptr;
	return true;
}

bool HtmlWebSocket::send(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlWebSocket::send");
	if (!m_hSocket)return true;

// 	SocketInterface*psi = JSVM::GetInstance()->GetSocketInterface();
// 	
	if (args[0].IsString())
	{
 		std::string&&str = args[0].GetString(args.getContext());
		JSVM::GetInstance()->GetSocketInterface()->Send(m_hSocket, str.c_str(), str.length());
//		m_hSocket->send(str);
// 		psi->Send(m_hSocket, str.c_str(), str.length());
// 		return true;
 	}
	else
	{
		LocalObject objarg (args.getContext(), args[0].GetObject());
		if (objarg.IsArray())
		{
			void*pdata = objarg.GetArrayData(args.getContext());
			int nlen = objarg.GetArrayByteLength(args.getContext());
			JSVM::GetInstance()->GetSocketInterface()->Send(m_hSocket, pdata,nlen);
		}
		else
		{
			assert(0);
		}
	}
// 	assert(0);
	return true;

}


// 
// void HtmlWebSocket::onOpen(WebSocket* ws)
// {
// 	class ASYNCTASK1 :public RunOnUIThread
// 	{
// 	public:
// 		HtmlWebSocket *pThis;
// 		ASYNCTASK1(HtmlWebSocket*pthis)
// 		{
// 			pThis = pthis;
// 		}
// 		virtual void OnPostExecute()
// 		{
// 			LocalObject objthis(JSVM::GetInstance()->GetContext(), pThis->GetObjFromNoGC());
// 			CustomClassAutoThisObj cc(pThis, objthis);
// 			pThis->ExecEventListener(JSVM::GetInstance()->GetContext(), "open");
// 		}
// 
// 	};
// 	JSVM::GetInstance()->AddRunOnUIThread(new ASYNCTASK1(this));
// }
// 
// void HtmlWebSocket::onMessage(WebSocket* ws, const WebSocket::Data& data)
// {
// 	
// 	HandleContext context = JSVM::GetInstance()->GetContext();
// 	LocalObject objthis(context, GetObjFromNoGC());
// 	CustomClassAutoThisObj cc(this, objthis);
// 	LocalObject objevent(context, HtmlEvent::CreateObject(context));
// 	objevent.SetProperty("type", "message");
// 	if (this->m_binaryType == "arraybuffer")
// 	{
// 		LocalObject objdata(context);
// 		objdata.CreatePrimitiveArray(context, data.len, LocalObject::AT_UINT8, data.bytes);
// 		objevent.SetProperty("data", objdata);
// 	}
// 	else if (this->m_binaryType == "blob")
// 	{
// 		assert(0);
// 	}
// 	else
// 	{
// 		objevent.SetProperty("data", (char*)data.bytes);
// 	}
// 	LocalValue vevent(context, objevent);
// 	ExecEventListener(JSVM::GetInstance()->GetContext(), "message", &vevent);
// 	
// 
// }
// 
// 
// void HtmlWebSocket::onClose(WebSocket* ws)
// {
// 	LocalObject objthis(JSVM::GetInstance()->GetContext(), GetObjFromNoGC());
// 	CustomClassAutoThisObj cc(this, objthis);
// 	ExecEventListener(JSVM::GetInstance()->GetContext(), "close");
// 	ProtectObjNoGC(false);
// 	SAFE_DELETE(m_hSocket);
// }
// 
// void HtmlWebSocket::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
// {
// 	LocalObject objthis(JSVM::GetInstance()->GetContext(), GetObjFromNoGC());
// 	CustomClassAutoThisObj cc(this, objthis);
// 	ExecEventListener(JSVM::GetInstance()->GetContext(), "error");
// //	ProtectObjNoGC(false);
// }


// 
// void HtmlWebSocket::onHaveUiMessage(WebSocket* ws, WsMessage * msg)
// {
// 	assert(ws == m_hSocket);
// 	class ASYNCTASK1 :public ASYNCTASK
// 	{
// 	public:
// 		HtmlWebSocket *pThis;
// 		ASYNCTASK1(HtmlWebSocket*pthis)
// 		{
// 			pThis = pthis;
// 		}
// 		virtual void OnPostExecute()
// 		{
// 			pThis->m_hSocket->DoUiThreadMessage();
// //			pThis->ProtectObjNoGC(false);
// 		}
// 
// 	};
// 	JSVM::GetInstance()->AddAsyncTask(new ASYNCTASK1(this));
// 
// 	
// }

void HtmlWebSocket::OnError(HSOCKET *hSocket, int nErrCode)
{
	class ASYNCTASK1 :public RunOnUIThread
	{
	public:
		HtmlWebSocket *pThis;
		ASYNCTASK1(HtmlWebSocket*pthis)
		{
			pThis = pthis;
		}
		virtual void OnPostExecute()
		{
			LocalObject objthis(JSVM::GetInstance()->GetContext(), pThis->GetObjFromNoGC());
			CustomClassAutoThisObj cc(pThis, objthis);
			pThis->ExecEventListener(JSVM::GetInstance()->GetContext(), "error");
			JSVM::GetInstance()->GetSocketInterface()->Close(pThis->m_hSocket);
			pThis->m_hSocket = nullptr;
		}

	};
	JSVM::GetInstance()->AddRunOnUIThread(new ASYNCTASK1(this));
}

void HtmlWebSocket::OnClose(HSOCKET *hSocket)//SOCKET线程调用，在子线程
{
	m_hSocket->bClosing = true;
	class ASYNCTASK1 :public RunOnUIThread
	{
	public:
		HtmlWebSocket *pThis;
		ASYNCTASK1(HtmlWebSocket*pthis)
		{
			pThis = pthis;
		}
		virtual void OnPostExecute()
		{
			LocalObject objthis(JSVM::GetInstance()->GetContext(), pThis->GetObjFromNoGC());
			CustomClassAutoThisObj cc(pThis, objthis);
			pThis->ExecEventListener(JSVM::GetInstance()->GetContext(), "close");
//			JSVM::GetInstance()->GetSocketInterface()->Close(pThis->m_hSocket);
			SAFE_DELETE(pThis->m_hSocket);

		}

	};
	JSVM::GetInstance()->AddRunOnUIThread(new ASYNCTASK1(this));
}

void HtmlWebSocket::OnReceive(HSOCKET* hSocket, char* pData, int nLen)
{
	class ASYNCTASK1 :public RunOnUIThread
	{
	public:
		char*pData;
		int nLen;
		HtmlWebSocket *pThis;
		ASYNCTASK1(HtmlWebSocket*pthis,char*pData,int nLen)
		{
			pThis = pthis;
			this->pData = new char[nLen];
			memcpy(this->pData, pData, nLen);
			this->nLen = nLen;
		}
		virtual void OnPostExecute()
		{
			HandleContext context = JSVM::GetInstance()->GetContext();
			LocalObject objthis(context, pThis->GetObjFromNoGC());
			CustomClassAutoThisObj cc(pThis, objthis);
			LocalObject objevent(context, HtmlEvent::CreateObject(context));
			objevent.SetProperty("type", "message");
			if (pThis->m_binaryType == "arraybuffer")
			{
				LocalObject objdata(context);
				objdata.CreatePrimitiveArray(context, nLen, LocalObject::AT_UINT8,pData);
				objevent.SetProperty("data", objdata);
			}
			else if (pThis->m_binaryType == "blob")
			{
				assert(0);
			}
			else
			{
				objevent.SetProperty("data", pData);
			}
			LocalValue vevent(context, objevent);
			pThis->ExecEventListener(JSVM::GetInstance()->GetContext(), "message", &vevent);
		}

	};
	JSVM::GetInstance()->AddRunOnUIThread(new ASYNCTASK1(this,pData,nLen));
}

void HtmlWebSocket::OnConnect(HSOCKET*hSocket)
{
	class ASYNCTASK1 :public RunOnUIThread
	{
	public:
		HtmlWebSocket *pThis;
		ASYNCTASK1(HtmlWebSocket*pthis)
		{
			pThis = pthis;
		}
		virtual void OnPostExecute()
		{
			LocalObject objthis(JSVM::GetInstance()->GetContext(), pThis->GetObjFromNoGC());
			CustomClassAutoThisObj cc(pThis, objthis);
			pThis->ExecEventListener(JSVM::GetInstance()->GetContext(), "open");
		}

	};
	JSVM::GetInstance()->AddRunOnUIThread(new ASYNCTASK1(this));
}

int HtmlWebSocket::getReadyState()
{
	if (!m_hSocket)return CLOSED;
	if (m_hSocket->bHandsake)
	{
		if (!m_hSocket->bClosing)
			return OPEN;
		else return CLOSING;
	}
	else return CONNECTING;
}

void HtmlWebSocket::Close()
{
	if (m_hSocket)
	{
		m_hSocket->pInterface = nullptr;
		JSVM::GetInstance()->GetSocketInterface()->Close(m_hSocket);
		SAFE_DELETE(m_hSocket);
	}
}

