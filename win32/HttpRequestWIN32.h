#pragma once
#include "..\HttpRequestInterface.h"

class HttpRequestWIN32 :
	public HttpRequestInterface
{
public:
	HttpRequestWIN32();
	virtual ~HttpRequestWIN32();

	virtual int SendGetRequest(const char*szURL, RESPONSETYPE respType, const char*szReferer, char*&pDataRet, int &nRetLen);

	virtual int SendPostRequest(const char*szURL, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen);


	int SendPostOrGetRequest(const char*szURL, const char*szType, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen);
};

