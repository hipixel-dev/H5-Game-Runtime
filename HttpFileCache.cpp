#include "stdafx.h"
#include "HttpFileCache.h"


HttpFileCache::HttpFileCache()
{
}


HttpFileCache::~HttpFileCache()
{
}

bool HttpFileCache::GetCacheFileData(const std::string& szURL,  char*& pDataRet, int & nRetLen)//从缓存读取数据
{
	std::string&&szCachePath = JSVM::GetInstance()->GetCacheFullPath(szURL);

	FileIOInterface*pfileio=JSVM::GetInstance()->GetFileIOInterface();
	if (!pfileio->IsFileExist(szCachePath.c_str()))return false;

	pthread_mutex_lock(&JSVM::GetInstance()->m_lock);
	FileHandle *file = pfileio->Open(szCachePath.c_str(), FileIOInterface::OP_READ);
	if (!file)
	{
		pthread_mutex_unlock(&JSVM::GetInstance()->m_lock);
		return false;
	}
	nRetLen = pfileio->GetLength(file);
	pDataRet = new char[nRetLen + 1];
	pfileio->Read(file, pDataRet, nRetLen);
	pDataRet[nRetLen] = 0;
	pfileio->Close(file);
	pthread_mutex_unlock(&JSVM::GetInstance()->m_lock);
	return true;
}

bool HttpFileCache::SetCacheFileData(const std::string& szURL, const char* pDataRet, int nRetLen)
{
	pthread_mutex_lock(&JSVM::GetInstance()->m_lock);
	if (nRetLen == 0)
	{
		assert(0);
	}
	std::string&&szCachePath = JSVM::GetInstance()->GetCacheFullPath(szURL);

	FileIOInterface*pfileio = JSVM::GetInstance()->GetFileIOInterface();
	FileHandle*file = pfileio->Open(szCachePath.c_str(), FileIOInterface::OP_READWRITE);
	if (!file)
	{
		pthread_mutex_unlock(&JSVM::GetInstance()->m_lock);
		return false;
	}
	pfileio->Write(file, pDataRet, nRetLen);
	pfileio->Close(file);
	pthread_mutex_unlock(&JSVM::GetInstance()->m_lock);
	return true;
}

