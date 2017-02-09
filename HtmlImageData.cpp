#include "stdafx.h"
#include "HtmlImageData.h"
IMPLEMENT_CUSTOM_CLASS(HtmlImageData, CustomClassBase)

HtmlImageData::HtmlImageData()
{
}


HtmlImageData::~HtmlImageData()
{
}

bool HtmlImageData::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "ImageData", HandleObject());
}

bool HtmlImageData::SetData(const HandleContext&context, int width, int height, unsigned char*pData)//pDataµÄ³¤¶È£½width*height*4
{
	LocalObject objthis(context, GetJsObject());
	int nDataLen = width*height * 4;

	LocalObject pDataObj(context);
	pDataObj.CreatePrimitiveArray(context, nDataLen, LocalObject::AT_UINT8, pData);
	objthis.SetProperty(context, "data", pDataObj);
	objthis.SetProperty(context, "width", ValueBase().SetInt(width));
	objthis.SetProperty(context, "height", ValueBase().SetInt(height));
	

	return true;
}
bool HtmlImageData::GetData(const HandleContext&context, unsigned char*pbuf)
{
	LocalObject objthis(context, GetJsObject());
	LocalObject dat(context, objthis.GetProperty(context, "data").GetObject());
	if (dat.IsNull())return false;
	
	int width = GetWidth(context);
	int height = GetHeight(context);
	if (!dat.IsArray(context))
	{
		return false;
	}
	uint32_t datalen = dat.GetArrayByteLength(context);
	LocalObject::ARRAYTYPE typ = dat.GetPrimitiveArrayType(context);


	if (typ != LocalObject::AT_INT8&&typ!=LocalObject::AT_UINT8)
		assert(0);


	if (width*height * 4 != datalen)
	{
		assert(0);
	}
	unsigned char*pdata = (unsigned char*)dat.GetArrayData(context);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int i = (y*width + x) * 4;
			int j = ((height - y - 1)*width + x) * 4;
			pbuf[i] = pdata[j + 3];
			pbuf[i + 1] = pdata[j + 2];
			pbuf[i + 2] = pdata[j + 1];
			pbuf[i + 3] = pdata[j];
		}
	}
	return true;
}

int HtmlImageData::GetWidth(const HandleContext&context)
{
	LocalValue v(context, GetJsObject().GetProperty(context, "width"));
	
	if (v.IsUndefined())return 0;
	return v.GetInt();
}

int HtmlImageData::GetHeight(const HandleContext&context)
{
	LocalValue v(context, GetJsObject().GetProperty(context, "height"));

	if (v.IsUndefined())return 0;
	return v.GetInt();
}