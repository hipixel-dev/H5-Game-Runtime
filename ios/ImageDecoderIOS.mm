//
//  ImageDecoderIOS.cpp
//  wuwanlib
//
//  Created by 陈JH on 15/8/6.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#include "ImageDecoderIOS.h"
#include "../Base64.h"
#include "stdafx.h"
#include "../FileIOInterface.h"
#include "CacheManager.h"
#import <UIKit/UIKit.h>

ImageDecoderIOS::ImageDecoderIOS()
{
    
}

ImageDecoderIOS::~ImageDecoderIOS()
{
    
}

bool ImageDecoderIOS::DecodeFile(const char* szFilePath, ImageData& data)
{
    return ImageDecoderInterface::DecodeFile(szFilePath, data);
    /*
    if (!strncmp(szFilePath, "data:", 5)) // 内嵌式图片数据
    {
        const char* p = strstr(szFilePath, ",");
        if (!p) {
            return  false;
        }
        p++;
        unsigned long len = strlen(p);
        unsigned long lenout = len * 1.5 + 10;
        unsigned char* pOut = new unsigned char[lenout];
        
        bool bOK = CBase64::Decode((unsigned char*)p, len, pOut, &lenout);
        if (!bOK) {
            return false;
        }
        bOK = DecodeData(pOut, lenout, data);
        
        delete [] pOut;
        return bOK;
    }
    
    
    std::string filePath = szFilePath;
    FILE* pFile = [[CacheManager getInstance] loadFileFromUrl:filePath];
    if (!pFile) {
        return false;
    }
    
    // 读取图像文件数据
    // 获取文件大小
    long n = std::ftell(pFile);
    fseek(pFile, 0, SEEK_END);
    long nFileLen = ftell(pFile);
    fseek(pFile, n, SEEK_SET);
    assert(nFileLen > 0);
    
    char* buff = new char[nFileLen];
    std::fread(buff, 1, nFileLen, pFile);
    Byte* buffBytes = (Byte*)buff;
    NSData* nsData = [[NSData alloc] initWithBytes:buffBytes length:nFileLen];
    UIImage* image = [UIImage imageWithData:nsData];
    CGImageRef cgimage = image.CGImage;
    
    size_t width = CGImageGetWidth(cgimage);
    size_t height = CGImageGetHeight(cgimage);
    
    data.nWidth = width;
    data.nHeight = height;
    if (data.pData == nullptr) {
        data.pData = new uint32_t[width * height];
    }
    memset(data.pData, 0, width * height * 4);
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(data.pData, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    assert(context != nullptr);
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgimage);
  
    // cleanup
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
    delete []buff;
    fclose(pFile);
    
    return true;
     */
}

bool ImageDecoderIOS::DecodeData(void* pData, int nDataLen, ImageData& data)
{
    NSData* nsData = [[NSData alloc] initWithBytes:pData length:nDataLen];
    UIImage* img = [UIImage imageWithData:nsData];
    [nsData release];
    
    CGImageRef cgimage = img.CGImage;
    
    size_t width = CGImageGetWidth(cgimage);
    size_t height = CGImageGetHeight(cgimage);
    if (width == 0 && height == 0) {
        data.nHeight = 0;
        data.nWidth = 0;
        data.pData = nullptr;
        return true;
    }
    
    data.nWidth = width;
    data.nHeight = height;
    if (data.pData == nullptr) {
        data.pData = new uint32_t[width * height];
    }
    memset(data.pData, 0, width * height * 4);
    
    //TODO(CJH): 调试
    /*
    CGDataProviderRef provider = CGImageGetDataProvider(cgimage);
    NSData* ddd = (id)CGDataProviderCopyData(provider);
    void* dddd2 = (void*)CFDataGetBytePtr(CGDataProviderCopyData(provider));
    //const uint8_t* bytes = (const uint8_t*)[ddd bytes];
    const uint8_t* bytes = (const uint8_t*)dddd2;
    //const uint8_t* bytes = (const uint8_t*)[nsData bytes];
    //const uint8_t* bytes = (const uint8_t*)pData;
    printf("pixel data:\n%02x %02x %02x %02x %02x\n", bytes[300], bytes[301], bytes[302], bytes[3], bytes[4]);
    for(size_t row = 0; row < height; row++)
    {
        for(size_t col = 0; col < width; col++)
        {
            const uint8_t* pixel =
            &bytes[row * width * 4 + col * 4];
            
            printf("(");
            for(size_t x = 0; x < 4; x++)
            {
                printf("%.2X", pixel[x]);
                if( x < 4 - 1 )
                    printf(",");
            }
            
            printf(")");
            if( col < width - 1 )
                printf(", ");
        }
        
        printf("\n");
    }
     */
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(data.pData, width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
    
    CGContextDrawImage(context, CGRectMake(0, 0, width, height), cgimage);
    
    //TODO(CJH):调试
    /*
    printf("img data: \n%02x %02x %02x %02x %02x\n", data.pData[0], data.pData[1], data.pData[2], data.pData[3], data.pData[4]);
    uint8_t* ddddd3 = (uint8_t*)CGBitmapContextGetData(context);
    printf("img data: \n%02x %02x %02x %02x %02x", ddddd3[0], ddddd3[1], ddddd3[2], ddddd3[3],ddddd3[4]);
    for(size_t row = 0; row < height; row++)
    {
        for(size_t col = 0; col < width; col++)
        {
            const uint8_t* pixel =
            &ddddd3[row * width * 4 + col * 4];
            
            printf("(");
            for(size_t x = 0; x < 4; x++)
            {
                printf("%.2X", pixel[x]);
                if( x < 4 - 1 )
                    printf(",");
            }
            
            printf(")");
            if( col < width - 1 )
                printf(", ");
        }
        
        printf("\n");
    }
     */
    
    // cleanup
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
    
    return true;
}

typedef struct
{
    unsigned int height;
    unsigned int width;
    unsigned int yOffset;
    bool         isPremultipliedAlpha;
    bool         hasShadow;
    CGSize       shadowOffset;
    float        shadowBlur;
    float        shadowOpacity;
    bool         hasStroke;
    float        strokeColorR;
    float        strokeColorG;
    float        strokeColorB;
    float        strokeColorA;
    float        strokeSize;
    float        tintColorR;
    float        tintColorG;
    float        tintColorB;
    float        tintColorA;
    
    unsigned char*  data;
    
} tImageInfo;

static bool s_isIOS7OrHigher = false;

static void _lazyCheckIOS7()
{
    static bool isInited = false;
    if (!isInited)
    {
        s_isIOS7OrHigher = [[[UIDevice currentDevice] systemVersion] compare:@"7.0" options:NSNumericSearch] != NSOrderedAscending;
        isInited = true;
    }
}

static CGSize _calculateStringSize(NSString *str, id font)
{
    CGSize textRect = CGSizeZero;
    textRect.width = 0x7fffffff;
    textRect.height = 0x7fffffff;
    
    CGSize dim;
    if(s_isIOS7OrHigher){
        NSDictionary *attibutes = @{NSFontAttributeName:font};
        dim = [str boundingRectWithSize:textRect options:(NSStringDrawingOptions)(NSStringDrawingUsesLineFragmentOrigin) attributes:attibutes context:nil].size;
    }
    else {
        dim = [str sizeWithFont:font constrainedToSize:textRect];
    }
    
    dim.width = ceilf(dim.width);
    dim.height = ceilf(dim.height);
    
    return dim;
}

static bool _initWithString(const char * text, const char * fontName, int size, tImageInfo* info, _TEXTBASELINE& baseline)
{
    // lazy check whether it is iOS7 device
    _lazyCheckIOS7();
    
    bool bRet = false;
    do
    {
        if (!text || !info) {
            break;
        }
        
        NSString * str          = [NSString stringWithUTF8String:text];
        NSString * fntName      = [NSString stringWithUTF8String:fontName];
        
        CGSize dim;
        
        // create the font
        id font = [UIFont fontWithName:fntName size:size];
        
        if (font)
        {
            dim = _calculateStringSize(str, font);
        }
        else
        {
            if (!font)
            {
                font = [UIFont systemFontOfSize:size];
            }
            
            if (font)
            {
                dim = _calculateStringSize(str, font);
            }
        }
        
        // compute the padding needed by shadow and stroke
        float shadowStrokePaddingX = 0.0f;
        float shadowStrokePaddingY = 0.0f;
        
        if ( info->hasStroke )
        {
            shadowStrokePaddingX = ceilf(info->strokeSize);
            shadowStrokePaddingY = ceilf(info->strokeSize);
        }
        
        // add the padding (this could be 0 if no shadow and no stroke)
        dim.width  += shadowStrokePaddingX*2;
        dim.height += shadowStrokePaddingY*2;
        
        
        unsigned char* data = (unsigned char*)malloc(sizeof(unsigned char) * (int)(dim.width * dim.height * 4));
        memset(data, 0, (int)(dim.width * dim.height * 4));
        
        // draw text
        CGColorSpaceRef colorSpace  = CGColorSpaceCreateDeviceRGB();
        CGContextRef context        = CGBitmapContextCreate(data,
                                                            dim.width,
                                                            dim.height,
                                                            8,
                                                            (int)(dim.width) * 4,
                                                            colorSpace,
                                                            kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
        if (!context)
        {
            CGColorSpaceRelease(colorSpace);
            free(data);
            break;
        }
        
        // text color
        CGContextSetRGBFillColor(context, info->tintColorR, info->tintColorG, info->tintColorB, info->tintColorA);
        // move Y rendering to the top of the image
        CGContextTranslateCTM(context, 0.0f, (dim.height - shadowStrokePaddingY) );
        CGContextScaleCTM(context, 1.0f, -1.0f); //NOTE: NSString draws in UIKit referential i.e. renders upside-down compared to CGBitmapContext referential
        
        // store the current context
        UIGraphicsPushContext(context);
        CGColorSpaceRelease(colorSpace);
        
        // compute the rect used for rendering the text
        // based on wether shadows or stroke are enabled
        
        float textOriginX  = 0;
        float textOrigingY = 0;
        
        float textWidth    = dim.width;
        float textHeight   = dim.height;
        
        CGRect rect = CGRectMake(textOriginX, textOrigingY, textWidth, textHeight);
        
        CGContextSetShouldSubpixelQuantizeFonts(context, false);
        
        CGContextBeginTransparencyLayerWithRect(context, rect, NULL);
        
        if ( info->hasStroke )
        {
            CGContextSetTextDrawingMode(context, kCGTextStroke);
            
            if(s_isIOS7OrHigher)
            {
                NSMutableParagraphStyle *paragraphStyle = [[NSMutableParagraphStyle alloc] init];
                paragraphStyle.alignment = NSTextAlignmentLeft;
                paragraphStyle.lineBreakMode = NSLineBreakByWordWrapping;
                [str drawInRect:rect withAttributes:@{
                                                      NSFontAttributeName: font,
                                                      NSStrokeWidthAttributeName: [NSNumber numberWithFloat: info->strokeSize / size * 100 ],
                                                      NSForegroundColorAttributeName:[UIColor colorWithRed:info->tintColorR
                                                                                                     green:info->tintColorG
                                                                                                      blue:info->tintColorB
                                                                                                     alpha:info->tintColorA],
                                                      NSParagraphStyleAttributeName:paragraphStyle,
                                                      NSStrokeColorAttributeName: [UIColor colorWithRed:info->strokeColorR
                                                                                                  green:info->strokeColorG
                                                                                                   blue:info->strokeColorB
                                                                                                  alpha:info->strokeColorA]
                                                      }
                 ];
                
                [paragraphStyle release];
            }
            else
            {
                CGContextSetRGBStrokeColor(context, info->strokeColorR, info->strokeColorG, info->strokeColorB, info->strokeColorA);
                CGContextSetLineWidth(context, info->strokeSize);
                
                //original code that was not working in iOS 7
                [str drawInRect: rect withFont:font lineBreakMode:NSLineBreakByWordWrapping alignment:NSTextAlignmentLeft];
            }
        }
        
        CGContextSetTextDrawingMode(context, kCGTextFill);
        
        // actually draw the text in the context
        [str drawInRect: rect withFont:font lineBreakMode:NSLineBreakByWordWrapping alignment:NSTextAlignmentLeft];
        
        CGContextEndTransparencyLayer(context);
        
        // pop the context
        UIGraphicsPopContext();
        
        // release the context
        CGContextRelease(context);
        
        // output params
        info->data                 = data;
        info->isPremultipliedAlpha = true;
        info->width                = dim.width;
        info->height               = dim.height;
        
        // set yOffset for baseline
        switch (baseline) {
            case TB_ALPHABETIC:
                info->yOffset = -[font ascender];
                break;
            case TB_TOP:
                info->yOffset = 0;
                break;
            case TB_BOTTOM:
                info->yOffset = -dim.height;
                break;
            case TB_MIDDLE:
                info->yOffset = -dim.height * 0.5f;
                break;
            case TB_HANGING:
                info->yOffset = 0;
                break;
            case TB_ideographic:
                info->yOffset = -[font ascender];
                break;
            default:
                info->yOffset = 0;
                break;
        }
        
        bRet = true;
    } while (0);
    
    return bRet;
}


UIImage* imageFromText(std::string text, std::string fontName, float fontSize)
{
    NSString* nsText = [NSString stringWithUTF8String:text.c_str()];
    NSString* nsFontName = [NSString stringWithUTF8String:fontName.c_str()];
    
    // set the font type and size
    UIFont *font = [UIFont fontWithName:nsFontName size:fontSize];
    CGSize size  = [nsText sizeWithFont:font];
    CGRect rect = CGRectMake(0, 0, size.width, size.height);
    
    UIGraphicsBeginImageContextWithOptions(size, NO, 0.0);
    
    // optional: add a shadow, to avoid clipping the shadow you should make the context size bigger
    //
    // CGContextRef ctx = UIGraphicsGetCurrentContext();
    // CGContextSetShadowWithColor(ctx, CGSizeMake(1.0, 1.0), 5.0, [[UIColor grayColor] CGColor]);
    
    if ([nsText respondsToSelector:@selector(drawInRect:withAttributes:)])
    {
        // iOS 7+
        NSDictionary* attr = @{NSFontAttributeName:font};
        [nsText drawInRect:rect withAttributes:attr];
    }
    else
    {
        [nsText drawInRect:CGRectIntegral(rect) withFont:font];
    }
    
    // transfer image
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return image;
}

bool ImageDecoderIOS::CreateStringImage(const char* szText, const FontInfo& fontInfo, ImageData& data, bool bMeasureOnly, uint32_t nColor, _TEXTBASELINE baseline)
{
    data.Cleanup();

    tImageInfo info = {0};
    // runtime核心目前仅提供字体／大小／颜色这些接口，其它如阴影／下划线等样式未实现，这里为未来需要保留接口
    info.width                  = 0;
    info.height                 = 0;
    info.yOffset                = 0;
    info.hasShadow              = false;
    info.shadowOffset.width     = 0;
    info.shadowOffset.height    = 0;
    info.shadowBlur             = false;
    info.shadowOpacity          = false;
    info.hasStroke              = false;
    info.strokeColorR           = 0;//textDefinition._stroke._strokeColor.r / 255.0f;
    info.strokeColorG           = 0;//textDefinition._stroke._strokeColor.g / 255.0f;
    info.strokeColorB           = 0;//textDefinition._stroke._strokeColor.b / 255.0f;
    info.strokeColorA           = 0;//textDefinition._stroke._strokeAlpha / 255.0f;
    info.strokeSize             = 0;//textDefinition._stroke._strokeSize;
    info.tintColorR             = (nColor >> 24) / 255.0f;
    info.tintColorG             = (nColor & 0x00ff0000 >> 16) / 255.0f;
    info.tintColorB             = (nColor & 0x0000ff00 >> 8) / 255.0f;
    info.tintColorA             = (nColor & 0x000000ff) / 255.0f;
    
    if (! _initWithString(szText, fontInfo.szFontName.c_str(), fontInfo.nFontSize, &info, baseline))
    {
        assert(0);
        return false;
    }
    data.nHeight = info.height;
    data.nWidth = info.width;
    data.pData = (uint32_t*)info.data;
    data.nYOffset = info.yOffset;
    
    return true;
}
