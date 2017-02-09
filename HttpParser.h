#pragma once
class HttpParser
{
public:
	HttpParser();
	~HttpParser();

	int m_nRetCode = 0;//返回200/404等
	int m_nContentLength = 0;//数据长度
	int m_DataOffset = 0;//数据指针
	bool m_bTransferEncodingChunked = false;

	bool m_bHaveContentLength = false;
	bool Parse(char*pData);
};

