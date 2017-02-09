#pragma once

#if !defined(_IOS)
#include "Include\GLES2\gl2.h"
#endif

#include "HtmlEventTarget.h"

class HtmlImgClass :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlImgClass, HtmlEventTarget)

public:
//	PersistentObject*pob = nullptr;
	ImageData m_imgData;
	GLuint m_nTexID=0;//LoadImageData成功后会创建OPENGL纹理
	std::string m_src;
// 	int m_nWidth = 0;
// 	int m_nHeight = 0;

	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("__LoadImage", &HtmlImgClass::__LoadImage, 0)//内部函数，用于异步加载图片

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("src")
		DEFINE_CUSTOM_PROPERTY("width")
		DEFINE_CUSTOM_PROPERTY("height")
		DEFINE_CUSTOM_PROPERTY("complete")
	END_CUSTOM_PROPERTY



	HtmlImgClass();
	virtual ~HtmlImgClass();
	static bool InitClass(const HandleContext&context);

	virtual bool OnGetProperty(GetPropertyArgs&args) ;
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);

	virtual void OnAddToParent(const HandleContext&context, HtmlElement*pParent);//当被加入别的element时调用
	virtual void OnRemoveFromParent(const HandleContext&context, HtmlElement*pParent);//当被移除element时调用
	

	bool LoadImageData(const std::string&szSrc);

	bool IsLoaded();
	void CleanImg();

	bool __LoadImage(IFunctionCallbackArgs&args);
};

