//
//  ImageDecoderIOS.h
//  wuwanlib
//
//  Created by 陈JH on 15/8/6.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#ifndef _IMAGEDECOERIOS_H_
#define _IMAGEDECOERIOS_H_

#include "../ImageDecoderInterface.h"
#include "../Macros.h"

class ImageDecoderIOS : public ImageDecoderInterface
{
public:
    ImageDecoderIOS();
    virtual ~ImageDecoderIOS();
    
    virtual bool DecodeFile(const char* szFilePath, ImageData& data);
    virtual bool DecodeData(void* pData, int nDataLen, ImageData& data);
    virtual bool CreateStringImage(const char* szText, const FontInfo& fontInfo, ImageData& data, bool bMeasureOnly, uint32_t nColor, _TEXTBASELINE baseline = TB_TOP);
};


#endif  // _IMAGEDECOERIOS_H_
