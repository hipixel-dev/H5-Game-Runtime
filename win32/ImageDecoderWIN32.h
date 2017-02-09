#pragma once
#include "..\ImageDecoderInterface.h"


class ImageDecoderWIN32 :
	public ImageDecoderInterface
{
public:
	ImageDecoderWIN32();
	virtual ~ImageDecoderWIN32();

	virtual bool DecodeFile(const char*szFilePath, ImageData&Data) ;

	virtual bool DecodeData(void*pData, int nDataLen, ImageData&Data) ;

	virtual bool CreateStringImage(const char*szText, const FontInfo&Fontinfo, ImageData&Data, bool bMeasureOnly, uint32_t nColor, _TEXTBASELINE baseline = TB_TOP);
//	bool DecodeFromByteArray(unsigned char* pBuf, unsigned long nlen, ImageData& Data);
};

