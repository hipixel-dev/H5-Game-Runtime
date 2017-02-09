#pragma once
#ifdef _WIN32
#include <winsock.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SOCKET int
#endif
#include <list>
#include "Mutexs.h"
#include "Events.h"
#include "ArrayBuffer.h"


class SocketInterface;

class HSOCKET;
class ISocketEvent
{
public:
	virtual void OnConnect(HSOCKET*hSocket) = 0;
	virtual void OnReceive(HSOCKET* hSocket, char* pData, int nLen) = 0;
	virtual void OnError(HSOCKET* hSocket, int nErrCode) = 0;
	virtual void OnClose(HSOCKET* hSocket) = 0;//当连接被对方关闭时调用（不会自动关闭socket）
};



//websocket使用,使用非阻塞
struct HSOCKET
{
	SocketInterface*pInterface = nullptr;
	SOCKET sockid=0;
	bool bHandsake = false;//是否已经握手
	bool bClosing = false;//调用close或onclose时为TRUE
	std::string m_szapi = "/";
	enum MSGTYPE
	{
		MT_CONNECT,
		MT_CLOSE,
		MT_SEND,
	};
	


	struct SOCKETMSG
	{
		int msgtype;
		int nParam=0;
		void*pParam=nullptr;
		~SOCKETMSG()
		{
			if (pParam)
			{
				delete[](uint8_t*)pParam;
			}
		}
	};

	CArrayBuffer m_recvBuffer;

	CMutexs msgmutex;
	//用于发送
	std::list<SOCKETMSG*>sendqueue;//线程消息队列
	CEvents sendevent;//通知有新消息
	ISocketEvent* pSocketEvent = nullptr;
	std::string m_szHost;
	int m_nPort;
	HSOCKET();
	~HSOCKET();

};
//WEBSOCKET接口
class SocketInterface
{
protected:
	ISocketEvent*m_pEvent = nullptr;
public:
	struct _HEADER
	{

		uint8_t opcode : 4;
		uint8_t RSV3 : 1;
		uint8_t RSV2 : 1;
		uint8_t RSV1 : 1;
		uint8_t FIN : 1;

		uint8_t payload_len : 7;
		uint8_t MASK : 1;
		union 
		{
			uint16_t payload_len16;
			uint64_t payload_len64;
		};
	};

	struct FRAMEINFO
	{
		uint64_t nLen;
		char*pData;
		uint8_t *mask;
		int nHeaderLen;
	};

	


	
	virtual HSOCKET* CreateSocket(ISocketEvent*pEvent) ;//创建一个SOCKET
	virtual bool Connect(HSOCKET* hSocket, const char*szHostAddr, int nPort, const char*szApi);
	virtual bool Send(HSOCKET* hSocket, const void*pData, unsigned long long nLen);
	virtual bool Close(HSOCKET*& hSocket) ;
#ifdef _WIN32
	static DWORD __stdcall SocketSendThread(LPVOID lpThreadParameter);
#else
	static void* SocketSendThread(void* lpThreadParameter);
#endif
	void PostConnectError(HSOCKET* pSocket,int err);
	void PostConnectOK(HSOCKET* pSocket);
#ifdef _WIN32
	static DWORD __stdcall SocketRecvThread(LPVOID lpThreadParameter);
#else 
	static void* SocketRecvThread(void* lpThreadParameter);
#endif
	void PostOnReceive(HSOCKET* pSocket, char* pData, int nLen);
	void PostSendError(HSOCKET* pSocket, int err, void* pData, int nLen);
	static int GetSockErr();
	FRAMEINFO GetFrameInfo(char* pData);
};

