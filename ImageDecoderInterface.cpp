#include "stdafx.h"
#include "ImageDecoderInterface.h"
#include "Base64.h"


bool ImageDecoderInterface::DecodeFile(const char*szFilePath, ImageData&Data)
{

	if (!strncmp(szFilePath, "data:", 5))//内嵌式图片数据
	{
		const char*p = strstr(szFilePath, ",");
		if (!p)return false;
		p++;
		unsigned long len = strlen(p);
		unsigned long lenout = len*1.5 + 10;
		unsigned char*pOut = new unsigned char[lenout];

		bool bOK = CBase64::Decode((unsigned char*)p, len, pOut, &lenout);
		if (bOK)
		{
			bOK = DecodeData(pOut, lenout, Data);


		}
		SAFE_DELETE_ARRAY(pOut);
		return bOK;
	}
	else if (!strnicmp(szFilePath, "http://", 7))
	{
		HttpRequestInterface*phttp = JSVM::GetInstance()->GetHttpRequestInterface();
		char*pbuf = nullptr;
		int nLen = 0;
		if (phttp->SendGetRequest(szFilePath, RESPONSETYPE::RST_BLOB, nullptr, pbuf, nLen) != 200)
		{
			SAFE_DELETE(pbuf);
			return false;
		}
		bool ret = DecodeData((unsigned char*)pbuf, nLen, Data);
		SAFE_DELETE_ARRAY(pbuf);
		return ret;
	}
	else
	{
		FileIOInterface*fio = JSVM::GetInstance()->GetFileIOInterface();
		FileHandle*file = fio->Open(szFilePath, FileIOInterface::OP_READ);
		if (!file)return false;
		int len = fio->GetLength(file);
		char*pbuf = new char[len];
		fio->Read(file, pbuf, len);
		fio->Close(file);
		bool ret = DecodeData((unsigned char*)pbuf, len, Data);
		SAFE_DELETE_ARRAY(pbuf);
		return ret;

	}
}
