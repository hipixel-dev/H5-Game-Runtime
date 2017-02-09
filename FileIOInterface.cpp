
#include "stdafx.h"
#include "FileIOInterface.h"

#ifdef _WIN32
#include <io.h>
#else
#include<dirent.h>
#endif


#include <sys/stat.h>



FileIOInterface::~FileIOInterface()
{
}

FileHandle* FileIOInterface::Open(const char*szFilePath, OPENMODE mode)
{
	std::string filepath;
	if (!strstr(szFilePath,":") && szFilePath[0]!='/')
	{//相对路径
		filepath = JSVM::GetInstance()->m_szExternalPath + "/" + szFilePath;
	}
	else filepath = szFilePath;

	FileHandle *f = new FileHandle;
	const char*mod="";
	if (mode == OP_READ)mod = "rb";
	else if (mode == OP_READWRITE)mod = "wb+";
	else if (mode == OP_READAPPEND)mod = "ab+";
	
	if (mode == OP_READWRITE || mode == OP_READAPPEND)
	{
		MakeDir(Getdir(filepath.c_str()));
	}
	f->pFile = fopen(filepath.c_str(), mod);
	if (!f->pFile)
	{
		int err = errno;
		delete f;
		return nullptr;
	}
	return f;
}

void FileIOInterface::Close(FileHandle*fHandle)
{
	fclose(fHandle->pFile);
	delete fHandle;
}

long long FileIOInterface::GetLength(FileHandle*fHandle)
{
	long n=ftell(fHandle->pFile);
	fseek(fHandle->pFile, 0, SEEK_END); //定位到文件末 
	long nFileLen = ftell(fHandle->pFile); //文件长度
	fseek(fHandle->pFile, n,SEEK_SET);
	return nFileLen;
}

long long FileIOInterface::Seek(FileHandle*fHandle, SEEKTYPE type, long long nOffset)
{
	if (type == SE_BEGIN)return fseek(fHandle->pFile, nOffset, SEEK_SET);
	else if (type == SE_CURRENT)return fseek(fHandle->pFile, nOffset, SEEK_CUR);
	else if (type == SE_END)return fseek(fHandle->pFile, nOffset, SEEK_END);
	else return 0;
}

long long FileIOInterface::Read(FileHandle*fHandle, void*pBuffer, int nLen)
{
	return fread(pBuffer, 1, nLen, fHandle->pFile);
}

long long FileIOInterface::Write(FileHandle*fHandle, const void*pBuffer, int nLen)
{
	return fwrite(pBuffer, 1, nLen, fHandle->pFile);
}

std::string FileIOInterface::Getdir(const char* path)
{
	path = _strdup(path);
	char*p = strrchr((char*)path, '/');
	if (p)
	{
		*p = 0;
		std::string ret = path;
		free((void*)path);
		return ret;
	}
	else
	{
		free((void*)path);
		return "";
	}
}
bool FileIOInterface::MakeDir(const std::string& szPath)
{
	int re = 0;
	if (_mkdir(szPath.c_str()))re = errno;
	if (re == EEXIST)
	{
		
		return FALSE;
	}
	else if (re == ENOENT)
	{
		if (!MakeDir(Getdir(szPath.c_str())))return FALSE;
		return _mkdir(szPath.c_str()) == 0;
	}
	else return TRUE;
}

void FileIOInterface::RmDir(const std::string&szPath)
{
	if (!IsDirectory(szPath.c_str()))
	{
		remove(szPath.c_str());
		return;
	}
#ifdef _WIN32
	long handle; // handle for search
	struct _finddata_t fileinfo; // sturct of file info
	handle = _findfirst((std::string(szPath)+"/*").c_str(), &fileinfo); // first search
	if (-1 == handle)return ;
	RmDir(fileinfo.name);
	while (!_findnext(handle, &fileinfo)) // loop search until no more matching files
	{
		if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
		RmDir(fileinfo.name);
	}
	_findclose(handle); // close handle
#else
	DIR*pDir;
	dirent*dmsg;
	if ((pDir = opendir(szPath.c_str())) != NULL)
	{
		// travesal dirs to delete file
		while ((dmsg = readdir(pDir)) != NULL)
		{		
			if (strcmp(dmsg->d_name, ".") != 0 && strcmp(dmsg->d_name, "..") != 0)	
			{		
				RmDir(szPath+"/"+dmsg->d_name);
			}	
		}	
	}	
	if (pDir != NULL)	
	{	
		closedir(pDir);	
		
	}	
	

#endif
	rmdir(szPath.c_str());
}

bool FileIOInterface::IsDirectory(const char* szPath)
{
	struct stat buf;
	int result;
	result = stat(szPath, &buf);
	if (S_IFDIR & buf.st_mode){
		return true;
	}
	else if (S_IFREG & buf.st_mode){
		return false;
	}

	return false;
}

bool FileIOInterface::IsFileExist(const char*szPath)
{
	struct stat buf;
	int result;
	result = stat(szPath, &buf);
	if (result == -1)return false;
	return true;
}

