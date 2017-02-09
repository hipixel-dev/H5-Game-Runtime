//
//  FileIOIOS.cpp
//  wuwanlib
//
//  Created by 陈JH on 15/8/7.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#import "FileIOIOS.h"
#import "CacheManager.h"
#import "../JSVMInterface.h"

static NSFileManager* s_fileManager = [NSFileManager defaultManager];

FileIOIOS::FileIOIOS() {
    _bundle = [NSBundle mainBundle];
}

FileIOIOS::~FileIOIOS() {
    // TODO Auto-generated destructor stub
}

std::string FileIOIOS::getPathForFilename(const std::string& filename)
{
    if (!filename.empty() && filename[0] != '/')
    {
        /*  APP目录
        NSString* fullpath = [_bundle pathForResource:[NSString stringWithUTF8String:file.c_str()]
                                               ofType:nil
                                          inDirectory:[NSString stringWithUTF8String:file_path.c_str()]];
        if (fullpath != nil) {
            return [fullpath UTF8String];
        }
         */
        
        // document目录
        NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentationDirectory, NSUserDomainMask, YES);
        NSString* docDir = [paths objectAtIndex:0];
        std::string strDocDir = [docDir UTF8String];
        strDocDir.append("/");
        return strDocDir + filename;
    }
    else
    {
        // Search path is an absolute path.
        if ([s_fileManager fileExistsAtPath:[NSString stringWithUTF8String:filename.c_str()]]) {
            return filename;
        }
    }
    return "";
}

FileHandle* FileIOIOS::Open(const char*szFilePath, OPENMODE mode)
{
    std::string fullPath = getPathForFilename(szFilePath);
    if (fullPath.empty()) {
        return nil;
    }
    
    return FileIOInterface::Open(fullPath.c_str(), mode);
}

bool FileIOIOS::isFileExist(std::string& fileName)
{
    std::string filePath = [[CacheManager getInstance] getLocalPath:fileName];
    FILE* file = [[CacheManager getInstance] loadFileFromCache:filePath];
    if (!file) {
        return false;
    }
    
    fclose(file);
    return true;
}
/*
void FileIOIOS::Close(FileHandle*fHandle)
{
    JNIEnv* jniEnv = JniHelper::getEnv();
    if (!jniEnv)return ;
    JniMethodInfo info;
    bool bOK = JniHelper::getStaticMethodInfo(info, "com/exampleobaiengine/XiaoBaiHelper",
                                              "FileClose", "(I)V");
    if (!bOK)
        return ;
    
    jniEnv->CallStaticVoidMethod(info.classID, info.methodID, (jint)fHandle->pFile);
    
    delete fHandle;
}

long long FileIOIOS::GetLength(FileHandle*fHandle)
{
    JNIEnv* jniEnv = JniHelper::getEnv();
    if (!jniEnv)return false;
    JniMethodInfo info;
    bool bOK = JniHelper::getStaticMethodInfo(info, "com/exampleobaiengine/XiaoBaiHelper",
                                              "FileGetLength", "(I)J");
    if (!bOK)
        return 0;
    
    long long ret=jniEnv->CallStaticLongMethod(info.classID, info.methodID, (jint)fHandle->pFile);
    
    return ret;
}

long long FileIOIOS::Seek(FileHandle*fHandle, SEEKTYPE type, long long nOffset)
{
    JNIEnv* jniEnv = JniHelper::getEnv();
    if (!jniEnv)return false;
    JniMethodInfo info;
    bool bOK = JniHelper::getStaticMethodInfo(info, "com/exampleobaiengine/XiaoBaiHelper",
                                              "FileSeek", "(IIJ)J");
    if (!bOK)
        return 0;
    
    long long ret = jniEnv->CallStaticLongMethod(info.classID, info.methodID, (jint)fHandle->pFile, (jint)type,(jlong)nOffset);
    
    return ret;
}

long long FileIOIOS::Read(FileHandle*fHandle, void*pBuffer, int nLen)
{
    JNIEnv* jniEnv = JniHelper::getEnv();
    if (!jniEnv)return false;
    JniMethodInfo info;
    bool bOK = JniHelper::getStaticMethodInfo(info, "com/exampleobaiengine/XiaoBaiHelper",
                                              "FileRead", "(I[BI)J");
    if (!bOK)
        return 0;
    
    jbyteArray jbuffer = jniEnv->NewByteArray( nLen);
    
    long long ret = jniEnv->CallStaticLongMethod(info.classID, info.methodID, (jint)fHandle->pFile, jbuffer,(jint)nLen);
    jniEnv->GetByteArrayRegion( jbuffer, 0, nLen, (jbyte*)pBuffer);
    
    jniEnv->DeleteLocalRef( jbuffer);
    
    return ret;
}
 
long long FileIOIOS::Write(FileHandle*fHandle, const void*pBuffer, int nLen)
{
    JNIEnv* jniEnv = JniHelper::getEnv();
    if (!jniEnv)return false;
    JniMethodInfo info;
    bool bOK = JniHelper::getStaticMethodInfo(info, "com/exampleobaiengine/XiaoBaiHelper",
                                              "FileWrite", "(I[BI)J");
    if (!bOK)
        return 0;
    
    jbyteArray jbuffer = jniEnv->NewByteArray( nLen);
    jniEnv->SetByteArrayRegion( jbuffer, 0, nLen, (jbyte*)pBuffer);
    
    long long ret = jniEnv->CallStaticLongMethod(info.classID, info.methodID, (jint)fHandle->pFile, jbuffer, (jint)nLen);
    
    jniEnv->DeleteLocalRef( jbuffer);
    
    return ret;
}
 

bool FileIOIOS::MakeDir(const std::string& szPath)
{
    JNIEnv* jniEnv = JniHelper::getEnv();
    if (!jniEnv)return false;
    JniMethodInfo info;
    bool bOK = JniHelper::getStaticMethodInfo(info, "com/exampleobaiengine/XiaoBaiHelper",
                                              "FileMakeDir", "(Ljava/lang/String;)Z");
    if (!bOK)
        return 0;
    
    jstring jstr = jniEnv->NewStringUTF( szPath.c_str());
    bool ret = jniEnv->CallStaticBooleanMethod(info.classID, info.methodID, jstr);
    
    jniEnv->DeleteLocalRef( jstr);
    
    return ret;
}

 
 */