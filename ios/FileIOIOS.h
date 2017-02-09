//
//  FileIOIOS.h
//  wuwanlib
//
//  Created by 陈JH on 15/8/7.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#ifndef __wuwanlib__FileIOIOS__
#define __wuwanlib__FileIOIOS__


#import <Foundation/Foundation.h>
#import "../FileIOInterface.h"
#import <string>

class FileIOIOS : public FileIOInterface {
    
public:
    FileIOIOS();
    virtual ~FileIOIOS();
    
    static std::string getPathForFilename(const std::string& filename);
    
    virtual FileHandle* Open(const char*szFilePath, OPENMODE mode);
    
    static bool isFileExist(std::string& fileName);
    //virtual void Close(FileHandle*fHandle);
    //virtual long long GetLength(FileHandle*fHandle);
    //virtual long long Seek(FileHandle*fHandle, SEEKTYPE type, long long nOffset);
    //virtual long long Read(FileHandle*fHandle, void*pBuffer, int nLen);
    //virtual long long Write(FileHandle*fHandle, const void*pBuffer, int nLen);
    //virtual bool MakeDir(const std::string& szPath);
    
private:
    NSBundle* _bundle;
};

#endif /* defined(__wuwanlib__FileIOIOS__) */
