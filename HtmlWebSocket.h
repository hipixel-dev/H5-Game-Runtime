#pragma once
#include "JSVMInterface.h"
#include "HtmlEventTarget.h"
//#include "../external/websockets/WebSocket.h"
//https://developer.mozilla.org/en-US/docs/Web/API/WebSocket
//using namespace network;



class HtmlWebSocket :
	public HtmlEventTarget, ISocketEvent//WebSocket::Delegate
{
	DEFINE_CUSTOM_CLASS(HtmlWebSocket, HtmlEventTarget)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("close", &HtmlWebSocket::close, 0)
		DEFINE_CUSTOM_FUNCTION("send", &HtmlWebSocket::send, 1)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("binaryType")
		DEFINE_CUSTOM_PROPERTY("bufferedAmount")
		DEFINE_CUSTOM_PROPERTY("extensions")
		DEFINE_CUSTOM_PROPERTY("protocol")
		DEFINE_CUSTOM_PROPERTY("readyState")
		DEFINE_CUSTOM_PROPERTY("url")
	END_CUSTOM_PROPERTY


	enum WebSocketReadyState
	{
		CONNECTING = 0,
		OPEN = 1,
		CLOSING = 2,
		CLOSED = 3,
	};


	 	std::string m_szHostName;
	 	int m_nPort=80;
	 


	 	std::string m_binaryType;
	 	int m_bufferedAmount=0;
		std::string m_extensions;
	 	std::vector<std::string> m_protocol;
//	 	int m_readyState=0;
	 	std::string m_url;//设置url时会解析到m_szHostName和m_nPort
	 	HSOCKET* m_hSocket = nullptr;
	//network::WebSocket* m_hSocket=nullptr;




	HtmlWebSocket();
	virtual ~HtmlWebSocket();
	static bool InitClass(const HandleContext&context);

	virtual bool OnGetProperty(GetPropertyArgs&args) ;

	virtual bool OnSetProperty(SetPropertyArgs&args) ;

	virtual void OnFinalize(FinalizeArgs&args) ;

	virtual bool OnConstructor(IFunctionCallbackArgs&args) ;
	bool close(IFunctionCallbackArgs&args);
	bool send(IFunctionCallbackArgs&args);



	virtual void OnError(HSOCKET *hSocket, int nErrCode);

	virtual void OnClose(HSOCKET *hSocket) ;

	virtual void OnReceive(HSOCKET* hSocket, char* pData, int nLen) ;

	virtual void OnConnect(HSOCKET*hSocket) ;

// 	virtual void onOpen(WebSocket* ws) ;
// 
// 	virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
// 
// 
// 	virtual void onClose(WebSocket* ws) ;
// 
// 	virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
	int getReadyState();
	void Close();

//	virtual void onHaveUiMessage(WebSocket* ws, WsMessage * msg) ;

};

