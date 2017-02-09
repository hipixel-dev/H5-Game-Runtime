#include "stdafx.h"
#include "SocketInterface.h"
#include "ArrayBuffer.h"
#include "HttpParser.h"





HSOCKET::HSOCKET()
{
	
}

HSOCKET::~HSOCKET()
{

}


//使用多线程阻塞方式实现非阻塞
HSOCKET* SocketInterface::CreateSocket(ISocketEvent*pEvent)
{

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		return nullptr;
	}

	HSOCKET *ret = new HSOCKET;
	ret->sockid = sock;
	ret->pInterface = this;
	ret->pSocketEvent = pEvent;
#ifdef _WIN32
	CloseHandle(CreateThread(0, 0, SocketSendThread, ret, 0, nullptr));
#else
	pthread_t ntid;
	pthread_create(&ntid, 0, SocketSendThread, ret);
#endif
	return ret;
}

#ifdef _WIN32
DWORD SocketInterface::SocketRecvThread(LPVOID lpThreadParameter)
#else 
void* SocketInterface::SocketRecvThread(void* lpThreadParameter)
#endif
{
	HSOCKET*pSocket = (HSOCKET*)lpThreadParameter;
	char pbuf[10001];
	while (true)
	{
		int nRecv=recv(pSocket->sockid, pbuf, 10000, 0);
		if (nRecv < 0)
		{
			int err=GetSockErr();
			break;
		}
		if (nRecv == 0)
		{
			if (pSocket->pSocketEvent)
				pSocket->pSocketEvent->OnClose(pSocket);
			return 0;
		}
		pbuf[nRecv] = 0;
		if (!pSocket->bHandsake)
		{
			HttpParser hp;
			hp.Parse(pbuf);
			if (hp.m_nRetCode == 101)
			{
				pSocket->pInterface->PostOnReceive(pSocket, pbuf, nRecv);
			}
			else
			{
				//验证错误
				pSocket->pInterface->PostConnectError(pSocket, 1);
			}
		}
		else
			pSocket->pInterface->PostOnReceive(pSocket, pbuf, nRecv);
	}
	int err = GetSockErr();
#ifdef _WIN32
	if (err == WSAECONNRESET)
#else
	if(err == 104)
#endif
	{
		if (pSocket->pSocketEvent)
			pSocket->pSocketEvent->OnClose(pSocket);//OnClose里要手动删除
		else delete pSocket;
	}
#ifdef _WIN32
	else if (err == WSAENOTCONN || err==WSAECONNABORTED)
#else
	else if (err == 9)
#endif
	{
//		delete pSocket;
	}
	else
	{
		if (pSocket->pSocketEvent)
			pSocket->pSocketEvent->OnError(pSocket,err);
	}
	
	return 0;
}
#ifdef _WIN32
DWORD SocketInterface::SocketSendThread(LPVOID lpThreadParameter)
#else
void* SocketInterface::SocketSendThread(void* lpThreadParameter)
#endif
{
	HSOCKET*pSocket = (HSOCKET*)lpThreadParameter;
	while (true)
	{
		pSocket->sendevent.Wait();
		pSocket->sendevent.ResetEvent();
		while (true)
		{
			pSocket->msgmutex.lock();
			if (pSocket->sendqueue.size() == 0)
			{ 
				pSocket->msgmutex.unlock();
				break;
			}
			HSOCKET::SOCKETMSG *msg = pSocket->sendqueue.front();
			pSocket->sendqueue.pop_front();

			pSocket->msgmutex.unlock();

			switch (msg->msgtype)
			{
			case HSOCKET::MT_CONNECT:
			{
				sockaddr_in sin;
				sin.sin_family = AF_INET;
				sin.sin_port = htons((unsigned short)msg->nParam);

				struct hostent* host_addr = gethostbyname((char*)msg->pParam);
				if (host_addr == NULL)
				{
					pSocket->pInterface->PostConnectError(pSocket,GetSockErr());
					break;
				}
				sin.sin_addr.s_addr = *((int*)*host_addr->h_addr_list);


				//建立连接
				if (connect(pSocket->sockid, (const struct sockaddr *)&sin, sizeof(sockaddr_in)) == -1)
				{
					pSocket->pInterface->PostConnectError(pSocket,GetSockErr());
					break;
				}
				pSocket->pInterface->PostConnectOK(pSocket);

				break;
			}
			case HSOCKET::MT_CLOSE:
			{
				
				delete msg;
				return 0;
			}
			case HSOCKET::MT_SEND:
			{
				int nSend = send(pSocket->sockid,(char*) msg->pParam, msg->nParam, 0);
				if (nSend < 0)
				{
					int err = GetSockErr();
					pSocket->pInterface->PostSendError(pSocket, err, msg->pParam, msg->nParam);
				}
				else if (nSend < msg->nParam)
				{
					assert(0);
				}
			}
			}

			delete msg;

		}
	}

	return 0;
}
bool SocketInterface::Connect(HSOCKET *hSocket, const char*szHostAddr, int nPort,const char*szApi)
{

	HSOCKET::SOCKETMSG*msg = new HSOCKET::SOCKETMSG;
	msg->msgtype = HSOCKET::MT_CONNECT;
	int len = strlen(szHostAddr);
	msg->pParam = new char[len + 1];
	strcpy((char*)msg->pParam, szHostAddr);
	msg->nParam = nPort;
	hSocket->m_szapi = szApi;
	hSocket->m_szHost = szHostAddr;
	hSocket->m_nPort = nPort;
	hSocket->msgmutex.lock();
	hSocket->sendqueue.push_back(msg);
	hSocket->msgmutex.unlock();
	hSocket->sendevent.SetEvent();
	return true;
}

bool SocketInterface::Send(HSOCKET* hSocket,const void*pData, unsigned long long nLen)
{
	HSOCKET::SOCKETMSG*msg = new HSOCKET::SOCKETMSG;
	msg->msgtype = HSOCKET::MT_SEND;
	if (hSocket->bHandsake)
	{
		bool bMask = true;
		uint8_t nMask[4] = { 0x0a, 0x5e, 0xae, 0x38 };
		int nExtPayloadLen = 0;//ext payload len字节数

		unsigned long long nTotalLen = 2 + nLen;
		if (nLen <= 125)
		{
		}
		else if (nLen <= 0xffff)
		{
			nTotalLen += 2;
		}
		else
			nTotalLen += 8;
		if (bMask)
		{
			nTotalLen += 4;
		}


		msg->nParam = nTotalLen;
		msg->pParam = new uint8_t[nTotalLen];
		uint8_t*p = (uint8_t*)msg->pParam;
		_HEADER*phead = (_HEADER*)msg->pParam;
		phead->FIN = 1;
		phead->RSV1 = phead->RSV2 = phead->RSV3 = 0;
		phead->opcode = 2;
		phead->MASK = bMask ? 1 : 0;
		if (nLen <= 125)
		{
			phead->payload_len = nLen;
			p += 2;
		}
		else if (nLen <= 0xffff)
		{
			phead->payload_len = 126;
			phead->payload_len16 = htons(nLen);
			p += 4;
		}
		else
		{
			phead->payload_len = 127;
			phead->payload_len64 = htonll(nLen);
			p += 10;
		}
		if (bMask)
		{
			memcpy(p, nMask, 4);
			p += 4;
			for (unsigned long long i = 0; i < nLen; i++)
			{
				p[i] = ((uint8_t*)pData)[i] ^ nMask[i % 4];
			}
		}
		else
		{
			memcpy(p, pData, nLen);
		}
	}
	else
	{
		msg->pParam = new uint8_t[nLen];
		memcpy(msg->pParam, pData, nLen);
		msg->nParam = nLen;

		
	}
	
	hSocket->msgmutex.lock();
	hSocket->sendqueue.push_back(msg);
	hSocket->msgmutex.unlock();
	hSocket->sendevent.SetEvent();
	return true;
}

bool SocketInterface::Close(HSOCKET*& hSocket)
{
	hSocket->pSocketEvent = nullptr;
	shutdown(hSocket->sockid, 2);

	closesocket(hSocket->sockid);
	HSOCKET::SOCKETMSG*msg = new HSOCKET::SOCKETMSG;
	msg->msgtype = HSOCKET::MT_CLOSE;
	hSocket->msgmutex.lock();
	hSocket->sendqueue.push_back(msg);
	hSocket->msgmutex.unlock();
	return true;
}


void SocketInterface::PostConnectError(HSOCKET* pSocket, int err)
{
	if (pSocket->pSocketEvent)
		pSocket->pSocketEvent->OnError(pSocket, err);
}
std::string ToString(int n)
{
	char str[20];
	sprintf(str, "%d", n);
	return str;
}
void SocketInterface::PostConnectOK(HSOCKET* pSocket)
{
#ifdef _WIN32
	CloseHandle(CreateThread(0, 0, SocketRecvThread, pSocket, 0, nullptr));
#else
	pthread_t ntid;
	pthread_create(&ntid, 0, SocketRecvThread, pSocket);
#endif
	
	if (!pSocket->bHandsake)
	{
		CArrayBuffer ab;
		ab.AddString((std::string)"GET " + pSocket->m_szapi + " HTTP/1.1\r\n");
		ab.AddString((std::string)"Host: " + pSocket->m_szHost + ":" + ToString(pSocket->m_nPort)+"\r\n");
		ab.AddString("Connection: Upgrade\r\n"
			"Pragma: no-cache\r\n"
			"Cache-Control: no-cache\r\n"
			"Upgrade: websocket\r\n"
			"Origin: null\r\n"
			"Sec-WebSocket-Version: 13\r\n"
			"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2288.6 Safari/537.36\r\n"
			"Accept-Encoding: gzip, deflate, sdch\r\n"
			"Accept-Language: zh-CN,zh;q=0.8,en;q=0.6\r\n"
			"Sec-WebSocket-Key: urRHns4czuiwlnOpIkTslA==\r\n"
			"Sec-WebSocket-Extensions: permessage-deflate; client_max_window_bits\r\n\r\n");
		Send(pSocket, ab.GetBuffer(), ab.GetLength());
	}

}

void SocketInterface::PostOnReceive(HSOCKET* pSocket, char* pData, int nLen)
{
	if (!pSocket->bHandsake)
	{
		pSocket->bHandsake = true;
		if (pSocket->pSocketEvent)
			pSocket->pSocketEvent->OnConnect(pSocket);
	}
	else
	{
		pSocket->m_recvBuffer.AddData(pData, nLen);
		while (pSocket->m_recvBuffer.GetLength() > 0)
		{
			pData = (char*)pSocket->m_recvBuffer.GetBuffer();
			//WEBSOCKET解码
			_HEADER*phead = (_HEADER*)pData;
			assert(phead->FIN);
			FRAMEINFO finfo = GetFrameInfo(pData);
			if (finfo.pData - pData + finfo.nLen <= pSocket->m_recvBuffer.GetLength())//收够数据
			{
				if (phead->MASK == 0)
				{
					if (pSocket->pSocketEvent)
						pSocket->pSocketEvent->OnReceive(pSocket, finfo.pData, finfo.nLen);
				}
				else
				{
					
					char*pdat = new char[finfo.nLen];
					for (uint64_t i = 0; i < finfo.nLen; i++)
					{
						pdat[i] = finfo.pData[i] ^ finfo.mask[i % 4];
					}
					if (pSocket->pSocketEvent)
						pSocket->pSocketEvent->OnReceive(pSocket, pdat, finfo.nLen);
					delete[]pdat;
				}
				pSocket->m_recvBuffer.RemoveData(finfo.nHeaderLen+finfo.nLen);
			}
			else
				break;
		}
	}
}

void SocketInterface::PostSendError(HSOCKET* pSocket, int err, void* pData, int nLen)
{
	if (pSocket->pSocketEvent)
		pSocket->pSocketEvent->OnError(pSocket, err);
}

int SocketInterface::GetSockErr()
{
#ifdef _WIN32 
	return WSAGetLastError();
#else
	return errno;
#endif
}

SocketInterface::FRAMEINFO SocketInterface::GetFrameInfo(char* pData)//收到数据后取得数据长度
{
	FRAMEINFO finfo;

	_HEADER*phead = (_HEADER*)pData;
	assert(phead->FIN);
	char*p = pData;
	uint64_t datalen = 0;
	p += 2;
	if (phead->payload_len < 126)
	{
		datalen = phead->payload_len;
	}
	else if (phead->payload_len == 126)
	{
		datalen = *(uint16_t*)p;
		p += 2;
	}
	else
	{
		datalen = *(uint64_t*)p;
		p += 8;
	}
	if (phead->MASK == 0)
	{
	}
	else
	{
		finfo.mask = (uint8_t*)p;
		p += 4;
	}
	finfo.nHeaderLen = p - pData;
	finfo.nLen = datalen;
	finfo.pData = p;
	return finfo;
}

