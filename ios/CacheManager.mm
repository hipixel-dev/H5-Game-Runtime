//
//  CacheManager.mm
//  wuwanlib
//
//  Created by 陈JH on 15/8/10.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#import "CacheManager.h"
#import <list>
#import "../JSVMInterface.h"
#import "../ios/FileIOIOS.h"
#import <stdio.h>
#import <fstream>
#import <iostream>

@implementation CacheManager

static CacheManager* _instance = nil;

+ (CacheManager*)getInstance
{
    if (_instance == nil) {
        _instance = [[CacheManager alloc] init];
    }
    return _instance;
}

- (instancetype)init
{
    if (self = [super init]) {
        _downloading = false;
    }
    
    return self;
}

- (void)setGamePath:(std::string&)path
{
    std::string tmpPath = [self formatPath:path];
    _gameDir = [self getDir:tmpPath];
    size_t pos = tmpPath.find_last_of('/');
    if (pos != std::string::npos) {
        _gameFile = tmpPath.substr(pos+1);
    }
    else
    {
        _gameFile = tmpPath;
    }
    
    NSArray* paths = NSSearchPathForDirectoriesInDomains(NSDocumentationDirectory, NSUserDomainMask, YES);
    NSString* rootPath = [paths objectAtIndex:0];
    std::string strRoot = [rootPath UTF8String];
    _cachePath = strRoot + "/" + JSVM::GetInstance()->GetMD5(_gameDir.c_str());
    JSVM::GetInstance()->GetFileIOInterface()->MakeDir(_cachePath);
}

- (const std::string)getGamePath
{
    return _gameDir + "/" + _gameFile;
}

- (const std::string&)getGameDir
{
    return _gameDir;
}

//将带有./../的绝对路径转成不带./../的绝对路径
- (std::string)formatPath:(std::string&)path
{
    std::list<std::string> subPaths;
    size_t start = 0;
    size_t pos = 0;
    while ((pos = path.find('/', start)) != std::string::npos) {
        std::string subStr = path.substr(start, pos - start);
        if (!subStr.compare(".")) {
            // DO NOTHING
        }
        else if (!subStr.compare("..")) {
            if (!subPaths.empty()) {
                subPaths.pop_back();
            }
            else {
                subPaths.push_back("..");
            }
        }
        else {
            if (start == 0) {
                subPaths.push_back(subStr);
            }
            else {
                subPaths.push_back("/" + subStr);
            }
        }
        
        start = pos + 1;
    }
    subPaths.push_back("/" + path.substr(start));

    
    std::string retStr;
    for (auto itor = subPaths.begin(); itor != subPaths.end(); ++itor) {
        retStr += *itor;
    }
    return retStr;
}

- (std::string)getDir:(std::string&)path
{
    int i=path.find_last_of('/');
    if(i == std::string::npos)return "";
    return path.substr(0,i);
}

- (std::string)getFileName:(std::string&)path
{
    int i=path.find_last_of('/');
    if(i<0)return path;
    return path.substr(i+1);
}

- (std::string)getLocalPath : (std::string&)path
{
    std::string relatedPath = [self getRelatePath:path];
    return _cachePath + "/" + JSVM::GetInstance()->GetMD5(relatedPath.c_str());
}


- (std::string)getRelatePath:(std::string&)urlpath//转成相对于gameDir的相对路径
{
    std::string path = urlpath;
    bool bAbsolutePath=false;
    if(!path.substr(0, 1).compare("/") || path.find(':') != std::string::npos) {
        bAbsolutePath=true;
    }
    else
    {
        path = _gameDir + "/" + path;
    }
    
    path = [self formatPath:path];
    std::string file =  [self getFileName:path];
    path = [self getDir:path];
    
    size_t start = 0;
    size_t posPath = 0;
    size_t posGameDir = 0;
    std::string retStr;
    
    while(true)
    {
        posPath = path.find('/', start);
        posGameDir = _gameDir.find('/', start);
        
        std::string subPath, subGameDir;
        if (posPath != std::string::npos && posGameDir != std::string::npos) {
            subPath = path.substr(start, posPath - start);
            subGameDir = _gameDir.substr(start, posGameDir-start);
            
            if (subPath.compare(subGameDir) != 0) {
                size_t temp = start;
                while ((temp = _gameDir.find('/', temp)) != std::string::npos) {
                    retStr += "../";
                    temp += 1;
                }
                retStr += "../" + path.substr(posPath) + "/";
                break;
            }
            else {
                start = posPath + 1;
                continue;
            }
        }
        else if (posGameDir == std::string::npos) {
            subGameDir = _gameDir.substr(start);
            if (posPath == std::string::npos) {
                subPath = path.substr(start);
            }
            else {
                subPath = path.substr(start, posPath - start);
            }
            
            if (subPath.compare(subGameDir) != 0) {
                retStr = "../" + subPath + "/";
            }
            break;
        }
        else {
            size_t temp = start;
            while ((temp = _gameDir.find('/', temp)) != std::string::npos) {
                retStr += "../";
                temp += 1;
            }
            subPath = path.substr(start);
            retStr += "../" + subPath + "/";
            break;
        }
    }
    retStr += file;
    
    return retStr;
}

//下载szUrlPath到缓存文件夹并返回下载的缓存文件，如果缓存存在则直接打开缓存文件返回
- (FILE*)loadFileFromUrl:(std::string&)szUrlPath
{
    //XiaoBaiHelper.ShowToast("加载"+szUrlPath+"...");
    //printf("\n加载 %s", szUrlPath.c_str());
    
    /*
    //TODO(CJH):目前的实现仅参考android工程的缓存机制，该机制存在如下缺陷，待解决：
     1. JS文件没必要缓存
     2. 在ios中有最大打开文件数的限制（模拟器中是250），缓存FILE指针的方式很有可能达到最大限制
    */
    
    // 查找内存
    /*
    auto itor = _fileCache.find(szUrlPath);
    if (itor != _fileCache.end()) {
        printf("找到了cache中的文件:%s", szUrlPath.c_str());
        return itor->second;
    }
     */
    
    // 查找本地缓存文件
    std::string filepath = [self getLocalPath:szUrlPath];
    FILE* file = [self loadFileFromCache:filepath];
    if(file == nil)
    {
        printf("\ncan't find in cache for file:%s, \nstart to download", filepath.c_str());
        //没缓存，下载
        file = [self downloadFile:szUrlPath withFilePath:filepath andModifiedFlag:0];
    }
    
    /*
    if(file!=nil)
    {
        _fileCache[szUrlPath] = file;
    }
     */
    
    //if(file==null)XiaoBaiHelper.ShowToast("加载"+szUrlPath+"失败");
    return file;
}

//下载szUrlPath到filepath，并返回filepath的文件
- (FILE*)downloadFile:(std::string&)szUrlPath withFilePath:(std::string&)filepath andModifiedFlag:(long)lastModified {
    /*
    if (_downloading) {
        return nil;
    }
     */
    
    // 设置请求路径
    NSString* urlStr = [NSString stringWithUTF8String:szUrlPath.c_str()];
    NSURL* url = [NSURL URLWithString:urlStr];
    
    // 创建请求对象，设置请求超时
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:url];
    request.timeoutInterval = 5.0;
    
    /*
    // 发送异步请求
    NSURLConnection* conn = [NSURLConnection connectionWithRequest:request delegate:self];
    [conn start];
    NSLog(@"发送网络请求");
     
     _downloading = true;
     */
    
    // 发送同步请求
    NSError* error = nil;
    NSData* data = [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:&error];
    if (data == nil) {
        NSLog(@"download file failed: %@", error);
        return nil;
    }
    
    // 保存文件
    //NSString* filePathStr = [NSString stringWithUTF8String:filepath.c_str()];
    //[data writeToFile:filePathStr atomically:YES];
    
    Byte* dataBytes = (Byte*)[data bytes];
    std::ofstream fout(filepath, std::ios::binary);
    fout.write((char*)dataBytes, data.length);
    fout.close();
    //[data release]; //TODO(CJH): 需要释放吗？
    
    //TODO(CJH): 待重构。仅仅是为了返回一个file指针。。。
    FileIOInterface* pFileIO = JSVM::GetInstance()->GetFileIOInterface();
    FileHandle* outFile = pFileIO->Open(filepath.c_str(), FileIOInterface::OP_READ);
    _downloadFiles.push_back(outFile);
    
    assert(outFile);
    return outFile->pFile;
}

- (FILE*)loadFileFromCache:(std::string&)szFile//路径为相对路径
{
    FILE* file= std::fopen(szFile.c_str(), "rb");
    if(!file)return nil;
    return file;
}

- (void)cleanup
{
    FileIOInterface* pFileIO = JSVM::GetInstance()->GetFileIOInterface();
    for (auto itor = _downloadFiles.begin(); itor != _downloadFiles.end(); ++itor) {
        pFileIO->Close(*itor);
    }
    
    _downloadFiles.clear();
    _fileCache.clear();
    _downloading = false;
}

#pragma mark- NSURLConnectionDataDelegate代理方法
/*
 *当接收到服务器的响应（连通了服务器）时会调用
 */
-(void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse *)response
{
    NSLog(@"接收到服务器的响应");
    //初始化数据
    self->_responseData=[NSMutableData data];
}

/*
 *当接收到服务器的数据时会调用（可能会被调用多次，每次只传递部分数据）
 */
-(void)connection:(NSURLConnection *)connection didReceiveData:(NSData *)data
{
    NSLog(@"接收到服务器的数据");
    //拼接数据
    [self->_responseData appendData:data];
    NSLog(@"%d---%@--",self->_responseData.length,[NSThread currentThread]);
}

/*
 *当服务器的数据加载完毕时就会调用
 */
-(void)connectionDidFinishLoading:(NSURLConnection *)connection
{
    NSLog(@"服务器的数据加载完毕");
    //隐藏HUD
    //[MBProgressHUD hideHUD];
    
    //处理服务器返回的所有数据
    NSDictionary *dict=[NSJSONSerialization JSONObjectWithData:self->_responseData options:NSJSONReadingMutableLeaves error:nil];
    
    //判断后，在界面提示信息
    /*
    NSString *error=dict[@"error"];
    if (error) {
        [MBProgressHUD showError:error];
    }else
    {
        NSString *success=dict[@"success"];
        [MBProgressHUD showSuccess:success];
    }
     */
    NSLog(@"%d---%@--",self->_responseData.length,[NSThread currentThread]);
    _downloading = false;
}

/*
 *请求错误（失败）的时候调用（请求超时\断网\没有网\，一般指客户端错误）
 */
-(void)connection:(NSURLConnection *)connection didFailWithError:(NSError *)error
{
    NSLog(@"请求错误");
    _downloading = false;
    //隐藏HUD
    //[MBProgressHUD hideHUD];
    //[MBProgressHUD showError:@"网络繁忙，请稍后重试！"];
}

@end