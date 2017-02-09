#pragma once
#include <stdio.h>
#include <string>
#include "Mutexs.h"
//文件操作接口，对应到操作系统接口（可以HOOK）,默认已经用fopen/fread/fwrite实现
class FileHandle
{
	friend class FileIOInterface;
	
public:
	FILE*pFile = NULL;
};
class FileIOInterface
{
public:
	enum SEEKTYPE
	{
		SE_BEGIN,
		SE_CURRENT,
		SE_END,
	};
	enum OPENMODE
	{
		OP_READ,//只读，文件不存在出错
		OP_READWRITE,//读写，文件不存在创建，存在则清空
		OP_READAPPEND,//读写，文件不存在创建，存在不清空
	};

	virtual ~FileIOInterface();
	virtual FileHandle* Open(const char*szFilePath, OPENMODE mode);
	virtual void Close(FileHandle*fHandle) ;
	virtual long long GetLength(FileHandle*fHandle);
	virtual long long Seek(FileHandle*fHandle, SEEKTYPE type,long long nOffset) ;
	virtual long long Read(FileHandle*fHandle,void*pBuffer, int nLen) ;
	virtual long long Write(FileHandle*fHandle,const void*pBuffer, int nLen) ;
	virtual bool MakeDir(const std::string& szPath);
	virtual std::string Getdir(const char* path);
	virtual void RmDir(const std::string&szPath);//删除文件或文件夹
	bool IsDirectory(const char* szPath);
	bool IsFileExist(const char*szPath);
};

