#pragma once
#include <string>

//HTTP请求接口，也可以支持本地文件读取
enum RESPONSETYPE
{
	RST_TEXT,
	RST_ARRAYBUFFER,
	RST_BLOB,
	RST_DOCUMENT,
	RST_JSON,
};

class HttpRequestInterface
{
public:
	HttpRequestInterface();
	virtual ~HttpRequestInterface();
	//发送HTTP GET请求并取得返回内容(ANSI或UTF-8),返回的pDataRet需调用者释放,返回值：成功200，未找到404
	virtual int SendGetRequest(const char*szURL, RESPONSETYPE respType, const char*szReferer, char*&pDataRet, int &nRetLen);
	virtual int SendPostRequest(const char*szURL, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen);
	virtual int SendPostOrGetRequest(const char*szURL, const char*szType, const char*szPostData, int nDataLen, const char*szContentType, const char*szReferer, char*&pDataRet, int &nRetLen);
	std::string GetFileToCachePath(const std::string&szURL);
};

