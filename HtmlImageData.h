#pragma once
#include "JSVMInterface.h"
class HtmlImageData :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlImageData, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION


	END_CUSTOM_FUNCTION
	BEGIN_CUSTOM_PROPERTY

	END_CUSTOM_PROPERTY


	HtmlImageData();
	virtual ~HtmlImageData();

	static bool InitClass(const HandleContext&context);
	bool GetData(const HandleContext&context, unsigned char*pbuf);
	int GetWidth(const HandleContext&context);
	int GetHeight(const HandleContext&context);
	bool SetData(const HandleContext&context, int width, int height, unsigned char*pData);//pDataµÄ³¤¶È£½width*height*4;


};

