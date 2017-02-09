#pragma once
#include "Macros.h"
#include <string>

//图像文件解码器接口，调用不同平台对应的解码方式，最终返回RGBA32位数据(规定原点在左上角）
class ImageData
{
public:
	int nWidth;
	int nHeight;
	int nYOffset = 0;//对于文字，如果textbaseline!=top，则nYOffset会设置为-baselineheight
	uint32_t*pData=nullptr;
	~ImageData()
	{
		Cleanup();
	}
	void Cleanup()
	{
		delete[]pData;
		pData = nullptr;
		nYOffset = 0;
	}
	bool IsInit()
	{
		return pData != nullptr;
	}
	void SwapY(uint32_t*pOutData) const
	{
		for (int y = 0; y <nHeight; y++)
		{
			for (int x = 0; x < nWidth; x++)
			{
				pOutData[(nHeight - y - 1)*nWidth + x] = pData[y*nWidth + x];
			}
		}
	}
};

class FontInfo
{
public:
	std::string szFontName;
	int nFontSize=0;
};

enum _TEXTBASELINE
{//http://www.w3school.com.cn/tags/canvas_textbaseline.asp
	TB_ALPHABETIC,
	TB_TOP,
	TB_BOTTOM,
	TB_MIDDLE,
	TB_HANGING,
	TB_ideographic,
};
enum _TEXTALIGN
{//http://www.w3school.com.cn/tags/canvas_textalign.asp
	AL_LEFT,
	AL_RIGHT,
	AL_CENTER,
};


class ImageDecoderInterface
{
public:
	virtual ~ImageDecoderInterface(){};
	virtual bool DecodeFile(const char*szFilePath, ImageData&Data);//解码文件到Data
	virtual bool DecodeData(void*pData, int nDataLen, ImageData&Data) = 0;//解码文件流到Data
//	virtual bool DecodeFromByteArray(unsigned char* pBuf, unsigned long nlen, ImageData& Data) = 0;
	virtual bool CreateStringImage(const char*szText, const FontInfo&Fontinfo, ImageData&Data, bool bMeasureOnly, uint32_t nColor, _TEXTBASELINE baseline = TB_TOP) = 0;//字符串转位图，只生成黑白位图，填充色需要后续处理。bMeasureOnly：是否只获得位图大小而不创建位图
};

