#pragma once
class HttpFileCache
{
public:
	HttpFileCache();
	virtual ~HttpFileCache();
	bool GetCacheFileData(const std::string& szURL, char*& pDataRet, int & nRetLen);
	bool SetCacheFileData(const std::string& szURL, const char* pDataRet, int  nRetLen);
};

