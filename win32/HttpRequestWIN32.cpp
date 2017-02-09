#include "stdafx.h"
#include "HttpRequestWIN32.h"
#include <wininet.h>
#include "..\JSVMInterface.h"
#include <winsock.h>
#include "..\ArrayBuffer.h"
#include "..\HttpParser.h"
#pragma comment(lib,"Ws2_32.lib")

HttpRequestWIN32::HttpRequestWIN32()
{
}


HttpRequestWIN32::~HttpRequestWIN32()
{
}

int HttpRequestWIN32::SendGetRequest(const char*szURL, RESPONSETYPE respType, const char*szReferer, char*&pDataRet, int &nRetLen)
{
	return __super::SendGetRequest(szURL, respType, szReferer, pDataRet, nRetLen);

}
int HttpRequestWIN32::SendPostOrGetRequest(const char*szURL, const char*szType, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen)
{
	return __super::SendPostOrGetRequest(szURL, szType, szPostData, nDataLen, szContentType, szReferer, pDataRet, nRetLen);

}

int HttpRequestWIN32::SendPostRequest(const char*szURL, const char*szPostData, int nDataLen, const char*szContentType,const char*szReferer, char*&pDataRet, int &nRetLen)
{
	return __super::SendPostRequest(szURL, szPostData, nDataLen, szContentType, szReferer, pDataRet, nRetLen);

}

