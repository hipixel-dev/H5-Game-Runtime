#pragma once
#include "JSVMInterface.h"
#include "OpenglFBO.h"




static bool ReverseCompare(const char*src, const char*str2)
{
	int len = strlen(src);
	int len2 = strlen(str2);
	if (len < len2)return false;
	if (!_stricmp(src + len - len2, str2))return true;
	return false;
}
static bool IsNumber(const char*str)
{
	int len = strlen(str);
	for (int i = 0; i < len; i++)
	{
		if (str[i]<'0' || str[i]>'9')return false;
	}
	return true;
}
struct CSSFONT
{
	std::string fontstyle = "normal";
	std::string fontvariant = "normal";
	std::string fontweight = "normal";
	int fontsize = 10;
	std::string lineheight = "10px";//暂时不用
	std::string fontfamily = "sans-serif";
	char*ctx;
	void FromString(const char* str)
	{
		char*buf = _strdup(str);
		do
		{
			char*p = strtok_s(buf, " ",&ctx);
			if (!p)break;
			if (!strcmp(p, "normal") || !strcmp(p, "italic") || !strcmp(p, "oblique"))
			{
				fontstyle = p;
				p = strtok_s(NULL, " ", &ctx);
				if (!p)break;
			}
			if (!strcmp(p, "normal") || !strcmp(p, "small-caps"))
			{
				fontvariant = p;
				p = strtok_s(NULL, " ", &ctx);
				if (!p)break;
			}
			if (!strcmp(p, "normal") || !strcmp(p, "bold") || !strcmp(p, "bolder") || !strcmp(p, "lighter"))
			{
				fontweight = p;
				p = strtok_s(NULL, " ", &ctx);
				if (!p)break;
			}
			else if (IsNumber(p))
			{
				fontweight = p;
				p = strtok_s(NULL, " ", &ctx);
				if (!p)break;
			}
			if (ReverseCompare(p, "px"))
			{
				fontsize = strtol(p, 0, 10);
				p = strtok_s(NULL, " ", &ctx);
				if (!p)break;
			}
			if (*p == '\'')p++;
			int l = strlen(p);
			if (p[l - 1] == '\'')p[l - 1] = 0;
			fontfamily = p;


		} while (false);




		delete[]buf;
	}
	std::string ToString()
	{
		char tmp[100];
		std::string str;
		if (fontstyle.length())str.append(fontstyle).append(" ");
		if (fontvariant.length())str.append(fontvariant).append(" ");
		if (fontweight.length())str.append(fontweight).append(" ");
		_itoa(fontsize, tmp,10);
		str.append(tmp).append(" ");
		if (fontfamily.length())str.append("'").append(fontfamily).append("'");
		return str;
	}
};















class CANVASPATHS;
class HtmlCanvas;
class HtmlCanvasRenderingContext2D :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlCanvasRenderingContext2D, CustomClassBase)
	HtmlCanvas* m_pCanvas=nullptr;


//	std::vector<DXPOINT>m_pointPath;
//	bool m_bBeginPath = false;

	
	_TEXTBASELINE m_textbaseline = TB_ALPHABETIC;
	_TEXTALIGN m_textalign = AL_LEFT;

	enum _FILLSTYLE
	{
		FS_STRING,
		FS_OBJECT,
	};
	_FILLSTYLE m_fillStyle = FS_STRING;
	uint32_t m_fillStyleColor = 0xff000000;//当m_fillStyle＝＝FS_STRING时有效
public:
	

	BEGIN_CUSTOM_FUNCTION



		DEFINE_CUSTOM_RENDER_FUNCTION("arc", &HtmlCanvasRenderingContext2D::arc, 6)
		DEFINE_CUSTOM_RENDER_FUNCTION("arcTo", &HtmlCanvasRenderingContext2D::arcTo, 5)
		DEFINE_CUSTOM_FUNCTION("beginPath", &HtmlCanvasRenderingContext2D::beginPath, 0)
		DEFINE_CUSTOM_RENDER_FUNCTION("bezierCurveTo", &HtmlCanvasRenderingContext2D::bezierCurveTo, 6)
		DEFINE_CUSTOM_RENDER_FUNCTION("clearRect", &HtmlCanvasRenderingContext2D::clearRect, 4)
		DEFINE_CUSTOM_FUNCTION("clip", &HtmlCanvasRenderingContext2D::clip, 0)
		DEFINE_CUSTOM_FUNCTION("closePath", &HtmlCanvasRenderingContext2D::closePath, 0)
		DEFINE_CUSTOM_FUNCTION("createLinearGradient", &HtmlCanvasRenderingContext2D::createLinearGradient, 4)
		DEFINE_CUSTOM_FUNCTION("createPattern", &HtmlCanvasRenderingContext2D::createPattern, 2)
		DEFINE_CUSTOM_FUNCTION("createRadialGradient", &HtmlCanvasRenderingContext2D::createRadialGradient, 6)
		DEFINE_CUSTOM_RENDER_FUNCTION("drawImage", &HtmlCanvasRenderingContext2D::drawImage, 9)
		DEFINE_CUSTOM_RENDER_FUNCTION("fill", &HtmlCanvasRenderingContext2D::fill, 0)
		DEFINE_CUSTOM_RENDER_FUNCTION("fillRect", &HtmlCanvasRenderingContext2D::fillRect, 4)
		DEFINE_CUSTOM_RENDER_FUNCTION("lineTo", &HtmlCanvasRenderingContext2D::lineTo, 2)
		DEFINE_CUSTOM_RENDER_FUNCTION("moveTo", &HtmlCanvasRenderingContext2D::moveTo, 2)
		DEFINE_CUSTOM_RENDER_FUNCTION("quadraticCurveTo", &HtmlCanvasRenderingContext2D::quadraticCurveTo, 4)
		DEFINE_CUSTOM_RENDER_FUNCTION("rect", &HtmlCanvasRenderingContext2D::rect, 4)
		DEFINE_CUSTOM_FUNCTION("restore", &HtmlCanvasRenderingContext2D::restore, 0)
		DEFINE_CUSTOM_FUNCTION("rotate", &HtmlCanvasRenderingContext2D::rotate,1)
		DEFINE_CUSTOM_FUNCTION("save", &HtmlCanvasRenderingContext2D::save, 0)
		DEFINE_CUSTOM_FUNCTION("scale", &HtmlCanvasRenderingContext2D::scale, 2)
		DEFINE_CUSTOM_RENDER_FUNCTION("stroke", &HtmlCanvasRenderingContext2D::stroke, 2)
		DEFINE_CUSTOM_RENDER_FUNCTION("strokeRect", &HtmlCanvasRenderingContext2D::strokeRect, 4)
		DEFINE_CUSTOM_FUNCTION("translate", &HtmlCanvasRenderingContext2D::translate, 2)
		DEFINE_CUSTOM_RENDER_FUNCTION("getImageData", &HtmlCanvasRenderingContext2D::getImageData, 4)
		DEFINE_CUSTOM_FUNCTION("setTransform", &HtmlCanvasRenderingContext2D::setTransform, 6)
		DEFINE_CUSTOM_FUNCTION("transform", &HtmlCanvasRenderingContext2D::transform, 6)
		DEFINE_CUSTOM_RENDER_FUNCTION("fillText", &HtmlCanvasRenderingContext2D::fillText, 4)
		DEFINE_CUSTOM_RENDER_FUNCTION("strokeText", &HtmlCanvasRenderingContext2D::strokeText, 4)
		DEFINE_CUSTOM_FUNCTION("measureText", &HtmlCanvasRenderingContext2D::measureText,1)
	

	END_CUSTOM_FUNCTION
	
	BEGIN_CUSTOM_PROPERTY
	DEFINE_CUSTOM_PROPERTY("globalAlpha")
	DEFINE_CUSTOM_PROPERTY("globalCompositeOperation")
	DEFINE_CUSTOM_PROPERTY("lineCap")
	DEFINE_CUSTOM_PROPERTY("lineJoin")
	DEFINE_CUSTOM_PROPERTY("lineWidth")
	DEFINE_CUSTOM_PROPERTY("miterLimit")
	DEFINE_CUSTOM_PROPERTY("textAlign")
	DEFINE_CUSTOM_PROPERTY("textBaseline")

	DEFINE_CUSTOM_PROPERTY("fillStyle")
	END_CUSTOM_PROPERTY



	HtmlCanvasRenderingContext2D();
	virtual ~HtmlCanvasRenderingContext2D();


	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	virtual bool OnFunctionCall(IFunctionCallbackArgs&args, CUSTOMFUNCTION*pFunc);


	bool arc(IFunctionCallbackArgs&args);
	bool arcTo(IFunctionCallbackArgs&args);
	bool beginPath(IFunctionCallbackArgs&args);
	bool bezierCurveTo(IFunctionCallbackArgs&args);
	bool clearRect(IFunctionCallbackArgs&args);
	bool clip(IFunctionCallbackArgs&args);
	bool closePath(IFunctionCallbackArgs&args);
	bool createLinearGradient(IFunctionCallbackArgs&args);
	bool createPattern(IFunctionCallbackArgs&args);
	bool createRadialGradient(IFunctionCallbackArgs&args);
	bool drawImage(IFunctionCallbackArgs&args);
	bool fill(IFunctionCallbackArgs&args);
	bool fillRect(IFunctionCallbackArgs&args);
	bool lineTo(IFunctionCallbackArgs&args);
	bool moveTo(IFunctionCallbackArgs&args);
	bool quadraticCurveTo(IFunctionCallbackArgs&args);
	bool rect(IFunctionCallbackArgs&args);
	bool restore(IFunctionCallbackArgs&args);
	bool rotate(IFunctionCallbackArgs&args);
	bool save(IFunctionCallbackArgs&args);
	bool scale(IFunctionCallbackArgs&args);
	bool stroke(IFunctionCallbackArgs&args);
	bool strokeRect(IFunctionCallbackArgs&args);
	bool translate(IFunctionCallbackArgs&args);
	bool getImageData(IFunctionCallbackArgs&args);
	bool setTransform(IFunctionCallbackArgs&args);
	bool transform(IFunctionCallbackArgs&args);
	bool fillText(IFunctionCallbackArgs&args);
	bool strokeText(IFunctionCallbackArgs&args);
	bool measureText(IFunctionCallbackArgs&args);
	bool glSetFillStyle(const HandleContext&context, DXPOINT*vPos);
	void UpdateMatrix();






//	bool CreatePathMesh(std::vector<_INDEX>&indexList);
	bool DrawPathLine(const HandleContext&context);
	bool glSetStrokeStyle(const HandleContext& context);
	void BeginClip(const HandleContext&context);
	void EndClip();
	void FillPointPath(const CANVASPATHS &pointPaths, const HandleContext&context);
};

