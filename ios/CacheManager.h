//
//  CacheManager.h
//  wuwanlib
//
//  Created by 陈JH on 15/8/10.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#ifndef __wuwanlib__CacheManager__
#define __wuwanlib__CacheManager__

#import <string>
#import <unordered_map>
#import <list>
#import <Foundation/Foundation.h>
#import "FileIOIOS.h"


@interface CacheManager : NSObject<NSURLConnectionDataDelegate>
{
    std::unordered_map<std::string, FILE*> _fileCache;
    std::string _cachePath;        // 缓存路径，根据gameDir生成一个名为md5的缓存文件夹
    std::string _gameDir;          // http:// 游戏目录
    std::string _gameFile;         // 游戏入口文件名称
    
    NSMutableData* _responseData;  // 服务器回应的数据
    bool _downloading;
    std::list<FileHandle*> _downloadFiles;
}

+ (CacheManager*)getInstance;

- (void)setGamePath:(std::string&)path;
- (const std::string)getGamePath;
- (const std::string&)getGameDir;


- (std::string)formatPath:(std::string&)path;
- (std::string)getDir : (std::string&)path;
// 获取文件的本地存放地址
- (std::string)getLocalPath : (std::string&)path;
// 获取文件相对于本地游戏根目录的相对地址
- (std::string)getRelatePath : (std::string&)path;
    //下载szUrlPath到缓存文件夹并返回下载的缓存文件，如果缓存存在则直接打开缓存文件返回
- (FILE*)loadFileFromUrl : (std::string&)szUrlPath;
- (FILE*)loadFileFromCache : (std::string&)szFile;
- (FILE*)downloadFile:(std::string&)szUrlPath withFilePath:(std::string&)filepath andModifiedFlag:(long)lastModified;
- (void)cleanup;

// NSURLConnectionDataDelegate代理方法
- (void)connection:(NSURLConnection*)connection didReceiveResponse:(NSURLResponse *)response;
- (void)connection:(NSURLConnection*)connection didReceiveData:(NSData *)data;
- (void)connectionDidFinishLoading:(NSURLConnection *)connection;
- (void)connection:(NSURLConnection*)connection didFailWithError:(NSError *)error;




@end


#endif /* defined(__wuwanlib__CacheManager__) */
