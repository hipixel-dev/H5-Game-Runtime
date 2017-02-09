#include "stdafx.h"
#include "ImageDecoderWIN32.h"
#include <atlconv.h>
#include <gdiplus.h>
#include "..\Base64.h"
#include <stdint.h>
#include "..\JSVMInterface.h"


ImageDecoderWIN32::ImageDecoderWIN32()
{
}


ImageDecoderWIN32::~ImageDecoderWIN32()
{
}

static bool Decode(Gdiplus::Bitmap&bitmap, ImageData&Data)
{
	Data.nWidth = bitmap.GetWidth();
	Data.nHeight = bitmap.GetHeight();
	delete[]Data.pData;

	Gdiplus::Rect rc(0, 0, Data.nWidth, Data.nHeight);
	Gdiplus::BitmapData bmpdata;
	Gdiplus::Status st = bitmap.LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &bmpdata);
	assert(st == 0);



	unsigned char* pbuf = new unsigned char[Data.nWidth*Data.nHeight * 4];
	unsigned char*pData = (unsigned char*)bmpdata.Scan0;
	//将BGRA转RGBA
//	int h = Data.nHeight / 2;
	for (int y = 0; y < Data.nHeight; y++)
	{
		for (int x = 0; x < Data.nWidth; x++)
		{
			int i = y*Data.nWidth + x;
//			int j = (Data.nHeight - y - 1)*Data.nWidth + x;

			pbuf[i * 4] = pData[i * 4 + 2];
			pbuf[i * 4 + 1] = pData[i * 4 + 1];
			pbuf[i * 4 + 2] = pData[i * 4 + 0];
			pbuf[i * 4 + 3] = pData[i * 4 + 3];

// 			pbuf[j * 4] = pData[j * 4 + 2];
// 			pbuf[j * 4 + 1] = pData[j * 4 + 1];
// 			pbuf[j * 4 + 2] = pData[j * 4 + 0];
// 			pbuf[j * 4 + 3] = pData[j * 4 + 3];

			//上下翻转
// 			uint32_t tmp = ((uint32_t*)pbuf)[i];
// 			((uint32_t*)pbuf)[i] = ((uint32_t*)pbuf)[j];
// 			((uint32_t*)pbuf)[j] = tmp;
		}

	}
	Data.pData = (uint32_t*)pbuf;
	bitmap.UnlockBits(&bmpdata);
	return true;
}

bool ImageDecoderWIN32::DecodeFile(const char*szFilePath, ImageData&Data)
{
	return ImageDecoderInterface::DecodeFile(szFilePath, Data);

}
// bool ImageDecoderWIN32::DecodeFromByteArray(unsigned char* pBuf, unsigned long nlen, ImageData& Data)
// {
// 	bool bOK;
// 	HRESULT hr;
// 	Gdiplus::Status sr;
// 
// 	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nlen);
// 	LPVOID pvData = NULL;
// 	pvData = GlobalLock(hGlobal);
// 	memcpy(pvData, pBuf, nlen);
// 	GlobalUnlock(hGlobal);
// 
// 	IStream *pStream = NULL;
// 	hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
// 
// 	Gdiplus::Bitmap bmp(pStream);
// 	sr = bmp.GetLastStatus();  // sr = Ok，表示Bitmap创建成功。
// 	bOK = Decode(bmp, Data);
// 
// 
// 	GlobalFree(hGlobal); // 使用Bitmap完后，需要释放资源，以免造成内存泄漏。
// 	return bOK;
// }

bool ImageDecoderWIN32::DecodeData(void*pData, int nDataLen, ImageData&Data)
{
	HRESULT hr;
	Gdiplus::Status sr;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nDataLen);
	LPVOID pvData = NULL;
	pvData = GlobalLock(hGlobal);
	memcpy(pvData, pData, nDataLen);
	GlobalUnlock(hGlobal);

	IStream *pStream = NULL;
	hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);

	Gdiplus::Bitmap bmp(pStream);
	sr = bmp.GetLastStatus();  // sr = Ok，表示Bitmap创建成功。
	assert(sr == Gdiplus::Status::Ok);
	if (sr != Gdiplus::Status::Ok)
	{
		GlobalFree(hGlobal);
		return false;
	}
	bool ret=Decode(bmp, Data);
	

	GlobalFree(hGlobal); // 使用Bitmap完后，需要释放资源，以免造成内存泄漏。
	return ret;
}

bool ImageDecoderWIN32::CreateStringImage(const char*szText, const FontInfo&Fontinfo, ImageData&Data, bool bMeasureOnly, uint32_t nColor, _TEXTBASELINE baseline )
{
	Data.Cleanup();
	if (!szText || !*szText)
	{
		Data.nWidth = Data.nHeight = 0;
		return true;
	}
	HDC dc = GetDC(0);
	HDC dcmem = CreateCompatibleDC(dc);
	
	HFONT font = CreateFontA(Fontinfo.nFontSize, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, Fontinfo.szFontName.c_str());
	HGDIOBJ oldfont = SelectObject(dcmem, font);
	

	TEXTMETRIC textmet;
	GetTextMetricsA(dcmem, &textmet);
	Data.nHeight = textmet.tmHeight;

	switch (baseline)
	{
	case TB_ALPHABETIC:
//		SetTextAlign(dcmem, TA_BASELINE);
		Data.nYOffset = -textmet.tmAscent;
		break;
	case TB_TOP:
//		SetTextAlign(dcmem, TA_TOP);
		Data.nYOffset = 0;
		break;
	case TB_BOTTOM:
//		SetTextAlign(dcmem, TA_BOTTOM);
		Data.nYOffset = -Data.nHeight;
		break;
	case TB_MIDDLE:
//		SetTextAlign(dcmem, VTA_CENTER);
		Data.nYOffset = -Data.nHeight *0.5f;
		break;
	case TB_HANGING:
//		SetTextAlign(dcmem, TA_TOP);
		Data.nYOffset = 0;
		break;
	case TB_ideographic:
//		SetTextAlign(dcmem, TA_BASELINE);
		Data.nYOffset = -textmet.tmAscent;
		break;
	default:
		Data.nYOffset = 0;
		break;
	}
	SIZE size;
	GetTextExtentPoint32A(dcmem, szText, strlen(szText), &size);
	Data.nWidth = size.cx;
	Data.nHeight = size.cy;
	if (!bMeasureOnly)
	{
		RECT rc;
		rc.left = rc.top = 0;
		rc.right = size.cx;
		rc.bottom = size.cy;

		HBITMAP bmp = CreateCompatibleBitmap(dc, size.cx, size.cy);
		HGDIOBJ oldbmp = SelectObject(dcmem, bmp);
		//取得透明区域
		SetBkColor(dcmem, 0);
		SetTextColor(dcmem, 0xffffff);
		ExtTextOutA(dcmem, 0, 0/*-Data.nYOffset*/, ETO_OPAQUE, &rc, szText, strlen(szText), NULL);
		

		BITMAP bitmap = {};
		GetObject(bmp, sizeof(BITMAP), &bitmap);

		uint32_t*pAlphaBit = new uint32_t[bitmap.bmWidth*bitmap.bmHeight];
		long ret=GetBitmapBits(bmp, bitmap.bmWidthBytes*bitmap.bmHeight, pAlphaBit);
		uint32_t nColorRGB = nColor & 0xffffff;
//		int h = bitmap.bmHeight / 2;
		for (int y = 0; y < bitmap.bmHeight; y++)
		{
			for (int x = 0; x < bitmap.bmWidth; x++)
			{
				int i = y*bitmap.bmWidth + x;

				pAlphaBit[i] = nColorRGB | (pAlphaBit[i] << 24);
				//pAlphaBit[i] = nColorRGB | (pAlphaBit[i]?0xff000000:0);
				
			}
		}
		
		Data.pData = pAlphaBit;
		SelectObject(dcmem, oldbmp);
		DeleteObject(bmp);
	}


	ReleaseDC(0, dc);
	SelectObject(dcmem, oldfont);
	DeleteObject(font);
	DeleteDC(dcmem);
	return true;
}

