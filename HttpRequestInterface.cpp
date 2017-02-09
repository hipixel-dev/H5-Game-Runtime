#include "stdafx.h"
#include "HttpRequestInterface.h"
#include "HttpParser.h"


HttpRequestInterface::HttpRequestInterface()
{
}


HttpRequestInterface::~HttpRequestInterface()
{
}

int HttpRequestInterface::SendGetRequest(const char*szURL, RESPONSETYPE respType, const char*szReferer, char*&pDataRet, int &nRetLen)
{
	
	if (!strnicmp(szURL, "http://", 7))
	{
		HttpFileCache* cache = JSVM::GetInstance()->GetHttpFileCache();
		if (!cache->GetCacheFileData(szURL, pDataRet, nRetLen))
		{
			int ret = SendPostOrGetRequest(szURL, "GET", nullptr, 0, nullptr, szReferer, pDataRet, nRetLen);
			if (ret == 200 && pDataRet)
				cache->SetCacheFileData(szURL, pDataRet, nRetLen);
			else
			{
				SAFE_DELETE(pDataRet);
				nRetLen = 0;
				return ret;
			}
		}
	}
	else
	{
		std::string path = JSVM::GetInstance()->GetFullPath(szURL);
		FileIOInterface *pfileio = JSVM::GetInstance()->GetFileIOInterface();
		FileHandle*pfile = pfileio->Open(path.c_str(), FileIOInterface::OP_READ);
		if (!pfile)return 404;
		long nLen = pfileio->GetLength(pfile);
		pDataRet = new char[nLen + 1];
		nRetLen = pfileio->Read(pfile, pDataRet, nLen);
		pDataRet[nRetLen] = 0;
		pfileio->Close(pfile);
	}
	if (respType == RST_TEXT || respType == RST_DOCUMENT || respType == RST_JSON)
	{
// 		if ((uint8_t)pDataRet[0] == 0xff && (uint8_t)pDataRet[1] == 0xfe)//UNICODE
//		{
			std::string&&str = JSVM::ByteArrayToString(pDataRet);
			delete[]pDataRet;
			pDataRet = new char[str.length() + 1];
			memcpy(pDataRet, str.data(), str.length());
			pDataRet[str.length()] = 0;
			nRetLen = str.length();

//		}
	}



	return 200;
}
std::string toString(int num)
{
	char tmp[30];
	sprintf(tmp, "%d", num);
	return tmp;
}
int HttpRequestInterface::SendPostOrGetRequest(const char*szURL, const char*szType, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen)
{
	pDataRet = nullptr;
	nRetLen = 0;
	int nRetCode = 200;


	std::string api;
	int port;
	std::string &&hostname = JSVM::GetHostName(szURL, api, port);

	//初始化socket
	struct hostent* host_addr = gethostbyname(hostname.c_str());
	if (host_addr == NULL)
	{
		return 404;
	}


	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons((unsigned short)port);
	sin.sin_addr.s_addr = *((int*)*host_addr->h_addr_list);


	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		return 404;
	}


	//建立连接
	if (connect(sock, (const struct sockaddr *)&sin, sizeof(sockaddr_in)) == -1)
	{
		shutdown(sock, 2);
		closesocket(sock);
		return 404;
	}


	//初始化发送信息
	char send_str[2048] = { 0 };


	//头信息
	strcat(send_str, szType);
	strcat(send_str, " ");
	strcat(send_str, api.c_str());
	strcat(send_str, " HTTP/1.1\r\n");
	strcat(send_str, "Host: ");
	strcat(send_str, (hostname + ":"+toString(port)).c_str());
	strcat(send_str, "\r\n");
	strcat(send_str, "Connection: keep-alive\r\n");


	char content_header[100];
	if (nDataLen > 0)
	{
		sprintf(content_header, "Content-Length: %d\r\n", nDataLen);
		strcat(send_str, content_header);
	}
	strcat(send_str, "Cache-Control: max-age=0\r\n");
	strcat(send_str, "User-Agent: Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.1 (KHTML, like Gecko) Chrome/15.0.849.0 Safari/535.1\r\n");
	if (szContentType)
	{
		sprintf(content_header, "Content-Type: %s\r\n", szContentType);
		strcat(send_str, content_header);
	}
	strcat(send_str, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
	if (szReferer && szReferer[0])
	{
		sprintf(content_header, "Referer: %s\r\n", szReferer);
		strcat(send_str, content_header);
	}
//	strcat(send_str, "Accept-Encoding: gzip,deflate,sdch\r\n");
	strcat(send_str, "Accept-Language: zh-CN,zh;q=0.8\r\n");


	//内容信息
	strcat(send_str, "\r\n");
	CArrayBuffer ab;
	ab.AddData(send_str, strlen(send_str));
	if (nDataLen > 0)
	{
		ab.AddData(szPostData, nDataLen);
	}

	if (send(sock, (const char*)ab.GetBuffer(), ab.GetLength(), 0) == -1)
	{
		shutdown(sock, 2);
		closesocket(sock);
		return 403;
	}
	CArrayBuffer abRet;

	//获取返回信息

	char recv_str[102400] = { 0 };
	bool bFirst = true;
	HttpParser hp;
	while (true)
	{
		int nRecv = recv(sock, recv_str, sizeof(recv_str), 0);
		if (nRecv <= 0)
		{
			int nErrCode = SocketInterface::GetSockErr();
			nRetCode = -nErrCode;
//			assert(0);
			break;
		}
		abRet.AddData(recv_str, nRecv);
		if (bFirst)
		{
			if (!hp.Parse((char*)abRet.GetBuffer()))continue;
			if (hp.m_nRetCode != 200)
			{
				char tmp[100];
				sprintf(tmp, "HTTP RESP:%d", hp.m_nRetCode);
				OutputDebugStringA(tmp);
				OutputDebugStringA((char*)ab.GetBuffer());
				return hp.m_nRetCode;
			}
			if (hp.m_DataOffset == 0)
			{
				if(hp.m_nContentLength == 0)
					return hp.m_nRetCode;
				else
				{
					bFirst = true;//还没确定m_DataOffset
					continue;
				}
			}
			bFirst = false;
			if (hp.m_DataOffset + hp.m_nContentLength>abRet.GetLength())//没收完
				continue;
			else
				goto line1;

		}
		else
		{
			if (hp.m_DataOffset + hp.m_nContentLength <= abRet.GetLength())
			{
			line1:
				assert(hp.m_DataOffset);
//				assert(hp.m_nContentLength > 0);
				if (hp.m_nContentLength == 0)
				{
//					OutputDebugStringA((char*)ab.GetBuffer());
					break;
				}
				pDataRet = new char[hp.m_nContentLength + 1];
				memcpy(pDataRet, abRet.GetBuffer() + hp.m_DataOffset, hp.m_nContentLength);
				pDataRet[hp.m_nContentLength] = 0;
				nRetLen = hp.m_nContentLength;
				nRetCode = 200;
				break;
			}
		}
	}

	shutdown(sock, 2);
	closesocket(sock);

	return nRetCode;
}


int HttpRequestInterface::SendPostRequest(const char*szURL, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen)
{
	return SendPostOrGetRequest(szURL, "POST", szPostData, nDataLen, szContentType, szReferer, pDataRet, nRetLen);

}

std::string HttpRequestInterface::GetFileToCachePath(const std::string&szURL)//返回缓存路径
{
	if (!strnicmp(szURL.c_str(), "data:", 5))
		return szURL;
	if (!strnicmp(szURL.c_str(), "http://", 7))
	{
		std::string&&cachePath = JSVM::GetInstance()->GetCacheFullPath(szURL);
		FileIOInterface *pfileio = JSVM::GetInstance()->GetFileIOInterface();
		if (pfileio->IsFileExist(cachePath.c_str()))return cachePath;
		HttpFileCache* cache = JSVM::GetInstance()->GetHttpFileCache();
		
		char*pDataRet = nullptr;
		int nRetLen = 0;
		int ret;
		for (int i = 0; i < 3; i++)
		{
			ret = SendPostOrGetRequest(szURL.c_str(), "GET", nullptr, 0, nullptr, nullptr, pDataRet, nRetLen);
			if (ret == 200)
			{
				if (nRetLen>0)
				{
					cache->SetCacheFileData(szURL, pDataRet, nRetLen);
				}
				else 
					assert(0);
				break;
			}
			else if (ret < 0)
			{
				usleep(500000);
				continue;
			}
			else break;
		}
		SAFE_DELETE_ARRAY(pDataRet);
		if (ret==200)
			return cachePath; 
		else return "";
	}
	else
	{
		std::string path = JSVM::GetInstance()->GetFullPath(szURL);
		FileIOInterface *pfileio = JSVM::GetInstance()->GetFileIOInterface();
		FileHandle*pfile = pfileio->Open(path.c_str(), FileIOInterface::OP_READ);
		if (!pfile)return "";
		pfileio->Close(pfile);
		return szURL;
	}
}
