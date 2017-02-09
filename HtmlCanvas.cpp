#include "stdafx.h"
#include "HtmlCanvas.h"
#include "HtmlCanvasRenderingContext2D.h"
#include "OpenglFBO.h"
#include "HtmlWebGLRenderingContext.h"
#include "HtmlEvent.h"
#include "HtmlRect.h"
IMPLEMENT_CUSTOM_CLASS(HtmlCanvas, HtmlEventTarget)

HtmlCanvas::HtmlCanvas()
{
	m_curState = new CANVASSTATE;
}


HtmlCanvas::~HtmlCanvas()
{
	if (m_texBackground)
		gl.DeleteTextures(1, &m_texBackground);
	m_texBackground = 0;
	SAFE_DELETE(m_curState);
	for (std::list<CANVASSTATE*>::iterator it = m_saveState.begin(); it != m_saveState.end(); ++it)
	{
		delete *it;
	}
	m_saveState.clear();
}

bool HtmlCanvas::OnGetProperty(GetPropertyArgs&args)
{
//	PrintDescribeScriptedCaller(args.GetContext());
	const char* str = args.GetName();
	if (!PROPCMP(str, "width"))
	{
		args.SetRetValue(ValueBase().SetInt(m_fbo.m_width));
		return true;
	}
	else if (!PROPCMP(str, "height"))
	{
		args.SetRetValue(ValueBase().SetInt(m_fbo.m_height));

		return true;
	}
// 	else if (!PROPCMP(str, "clientWidth"))
// 	{
// 		args.SetRetValue(ValueBase().SetInt(GetWidth()/*m_fbo.m_width*/));
// 		return true;
// 	}
// 	else if (!PROPCMP(str, "clientHeight"))
// 	{
// 		args.SetRetValue(ValueBase().SetInt(GetHeight()/*m_fbo.m_height*/));
// 		return true;
// 	}
	 if (!PROPCMP(str, "offsetLeft"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
	else if (!PROPCMP(str, "offsetTop"))
	{
		args.SetRetValue(ValueBase().SetInt(0));
		return true;
	}
// 	else if (str == "boundingClientRect")
// 	{
// 		LocalObject obj(args.GetContext(),args.CallGetPropertyFunc().GetObject());
// 		if (obj.IsNull())
// 		{
// 			obj.CreateJsObject();
// 			args.GetThisObject().SetProperty(args.GetContext(), "boundingClientRect", obj);
// 			obj.SetProperty("left", ValueBase().SetInt(0));
// 			obj.SetProperty("top", ValueBase().SetInt(0));
// 			obj.SetProperty("width", ValueBase().SetInt(m_fbo.m_width));
// 			obj.SetProperty("height", ValueBase().SetInt(m_fbo.m_height));
// 			args.SetRetValue(obj);
// 		}
// 		else
// 		{
// 			obj.SetProperty("left", ValueBase().SetInt(0));
// 			obj.SetProperty("top", ValueBase().SetInt(0));
// 			obj.SetProperty("width", ValueBase().SetInt(m_fbo.m_width));
// 			obj.SetProperty("height", ValueBase().SetInt(m_fbo.m_height));
// 
// 		}
// 		
// 		return true;
// 	}
	return HtmlEventTarget::OnGetProperty(args);
}

bool HtmlCanvas::OnSetProperty(SetPropertyArgs&args)
{
	const char* str = args.GetName();
	if (!PROPCMP(str, "width"))//实际图像大小
	{
		m_bSetWidth = true;
		gl.SetState(&m_glState);

		int width = args.GetValue().GetInt();
		if (width == 0||width>3000)
			PrintDescribeScriptedCaller(args.GetContext(), str);
 		m_fbo.EndFBO();
 		s_prevCanvas = nullptr;
		m_fbo.Init(width, m_fbo.m_height);
		gl.Viewport(0, 0, m_fbo.m_width,m_fbo.m_height);//因为要画到fbo上，所以viewport应该跟fbo一致

		m_width = width;
		

		//尺寸改变后要重置变换矩阵
		for (std::list<CANVASSTATE*>::iterator it = m_saveState.begin(); it != m_saveState.end(); ++it)
		{
			delete *it;
		}
		m_saveState.clear();
		m_curState->Reset();
		HtmlElement*pparent = GetParent();
		if (pparent)
			Measure(pparent->GetLeft(), pparent->GetTop(), GetParentWidth(), GetParentHeight());
		return true;
		

	}
	else if (!PROPCMP(str, "height"))
	{
		m_bSetHeight = true;
		gl.SetState(&m_glState);
	
		int height = args.GetValue().GetInt();
		m_fbo.EndFBO();
 		s_prevCanvas = nullptr;
		m_fbo.Init(m_fbo.m_width, height);
		gl.Viewport(0, 0, m_fbo.m_width, m_fbo.m_height);
		m_height = height;
//		PrintDescribeScriptedCaller(args.GetContext(), str);
		//尺寸改变后要重置变换矩阵
		for (std::list<CANVASSTATE*>::iterator it = m_saveState.begin(); it != m_saveState.end(); ++it)
		{
			delete *it;
		}
		m_saveState.clear();
		m_curState->Reset();
		HtmlElement*pparent = GetParent();
		if (pparent)
			Measure(pparent->GetLeft(), pparent->GetTop(), GetParentWidth(), GetParentHeight());
		return true;
		

	}
	return HtmlEventTarget::OnSetProperty(args);
}

void HtmlCanvas::OnFinalize(FinalizeArgs&args)
{
	JSVM::GetInstance()->RemoveVisibleCanvas(this);
	return HtmlEventTarget::OnFinalize(args);
}

bool HtmlCanvas::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = HtmlEventTarget::OnConstructor(args);
	if (!ret) return false;
	CustomClassAutoThisObj cc(this, args, true);
	LocalObject thisobj(args.getContext(), args.getReturnValue().GetObject());

	
	SetTagName(args.getContext(), "CANVAS");
	//设置2d和webgl接口
	LocalObject obj2D(args.getContext(), HtmlCanvasRenderingContext2D::CreateObject(args.getContext()));
	HtmlCanvasRenderingContext2D *p2D = (HtmlCanvasRenderingContext2D*)obj2D.GetNativeObj();
	thisobj.SetProperty(args.getContext(), "__canvas2d", obj2D);
	obj2D.SetProperty("canvas", thisobj);
	p2D->m_pCanvas = this;
	LocalObject objwebgl(args.getContext(), HtmlWebGLRenderingContext::CreateObject(args.getContext()));
	HtmlWebGLRenderingContext*pwebgl = (HtmlWebGLRenderingContext*)objwebgl.GetNativeObj();
	thisobj.SetProperty(args.getContext(), "__webgl", objwebgl);
	objwebgl.SetProperty("canvas", thisobj);
	pwebgl->m_pCanvas = this;
//	m_fbo.Init(1,1);
	return true;
}


bool HtmlCanvas::InitClass(const HandleContext&context)
{
	if (HtmlEventTarget::GetPrototype_s().IsNull())
	{
		HtmlEventTarget::InitClass(context);
	}


	return InitClass_s(context, "HTMLCanvasElement", HtmlEventTarget::GetPrototype_s());
}

bool HtmlCanvas::getContext(IFunctionCallbackArgs&args)
{
//	PrintDescribeScriptedCaller(args.getContext());
	CHECK_ARGS_COUNT_MIN(1, "HtmlCanvas::getContext");
	std::string name = args[0].GetString(args.getContext());
	if (!_stricmp(name.c_str(), "2d"))
	{
		args.setReturnValue(args.thisObj().GetProperty(args.getContext(), "__canvas2d"));
	}
	else if (!_stricmp(name.c_str(),"webgl") )
	{
		if (JSVM::GetInstance()->m_bEnableWebgl)args.setReturnValue(args.thisObj().GetProperty(args.getContext(), "__webgl"));
	}
	else args.setReturnValue(ValueBase());
	return true;
}

bool HtmlCanvas::getImageData(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlCanvas::getBoundingClientRect(IFunctionCallbackArgs&args)
{
	m_bundingClientRect->m_left = GetLeft();
	m_bundingClientRect->m_top = GetTop();
	m_bundingClientRect->m_width = GetWidth();
	m_bundingClientRect->m_height = GetHeight();
	args.setReturnValue(GetJsObject().GetProperty(args.getContext(), "__boundingClientRect"));
	return true;
}

bool HtmlCanvas::DrawFBO()
{
	if (m_width == 0 || m_height == 0)return true;


	//居中
// 	int x =  (OpenglContext::GetWidth() - m_width)*0.5f;
// 	int y = (OpenglContext::GetHeight() - m_height)*0.5f;
	return m_fbo.DrawFBO(GetLeft(),GetTop());
}

bool HtmlCanvas::Setid(const HandleContext& context, const char*szID)
{
	LocalObject thisobj(context, GetJsObject());
	return thisobj.SetProperty(context, "id", LocalValue(context).SetString(context, szID));

}

void HtmlCanvas::DrawImage(GLuint nTexID, int nTexWidth, int nTexHeight, int sx, int sy,
	int sWidth, int sHeight, int x, int y, int nWidth, int nHeight, MATRIX*pMat)
{
	//AutoFbo af(m_pCanvas);

//	m_glState.SetEnable(GL_TEXTURE_2D);//opengles无需调用
	assert(!glGetError());
	DXPOINT vPos[] =
	{
		{ x, y },
		{ x, y + nHeight },
		{ x + nWidth, y + nHeight },
		{ x + nWidth, y },
	};
	if (pMat)
	{
		vPos[0] = *pMat*vPos[0];
		vPos[1] = *pMat*vPos[1];
		vPos[2] = *pMat*vPos[2];
		vPos[3] = *pMat*vPos[3];
	}
	vPos[0] = WindowPointToGlPoint(vPos[0]);
	vPos[1] = WindowPointToGlPoint(vPos[1]);
	vPos[2] = WindowPointToGlPoint(vPos[2]);
	vPos[3] = WindowPointToGlPoint(vPos[3]);

	float texl = (float)sx / (nTexWidth);
	float text = float(nTexHeight - sy) / (nTexHeight);
	float texb = (float)(nTexHeight - sy - sHeight) / (nTexHeight);
	float texr = float(sx + sWidth) / (nTexWidth);


	//  	float texl = (float)sx / (nTexWidth-1);
	//  	float texb = (float)(nTexHeight-sy-sHeight) / (nTexHeight-1);
	//  	float texr = float(sx + sWidth-1) / (nTexWidth-1);
	// 	float text = float(nTexHeight-sy-1)  / (nTexHeight-1);



	DXPOINT vTex[] =
	{
		{ texl, text },
		{ texl, texb },
		{ texr, texb },
		{ texr, text },
	};




//	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	gl.BindBuffer(GL_ARRAY_BUFFER, 0);//如果画图没用到这个也要设为0，因为cocos2d用完后没有还原，会导致图画不出来
	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



	// Set the viewport
	glViewport(0,0,m_fbo.m_width,m_fbo.m_height);
	//	glViewport(OpenglContext::GetX(), OpenglContext::GetY(), OpenglContext::GetWidth(), OpenglContext::GetHeight());
	assert(!gl.GetError());

	// Use the program object
	gl.UseProgram(OpenglContext::s_TextureProgram);
	assert(!gl.GetError());
	// Load the vertex position
	OpenglContext::s_TextureProgram.SetVertexPosition(2, GL_FLOAT, GL_FALSE, 0, vPos);
	OpenglContext::s_TextureProgram.SetTexturePosition(2, GL_FLOAT, GL_FALSE, 0, vTex);
	OpenglContext::s_TextureProgram.SetAlpha(m_curState->m_globalAlpha);



	// Bind the texture
	gl.ActiveTexture(GL_TEXTURE0);
    
	assert(!gl.GetError());
	gl.BindTexture(GL_TEXTURE_2D, nTexID);
	assert(!gl.GetError());
	// Set the sampler texture unit to 0
	OpenglContext::s_TextureProgram.SetTextureUnit(0);
	assert(!gl.GetError());

	gl.DrawArrays(GL_TRIANGLE_FAN, 0, 4);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	assert(!gl.GetError());
	gl.BindTexture(GL_TEXTURE_2D, 0);
	assert(!gl.GetError());
}


// void HtmlCanvas::SetboundingClientRect(const HandleContext&context, int x, int y, int width, int height)
// {
// 	LocalObject objbc(context,GetJsObject().GetProperty(context, "boundingClientRect").GetObject());
// 	if(objbc.IsNull())
// 	{
// 		objbc.CreateJsObject();
// 		GetJsObject().SetProperty(context, "boundingClientRect", objbc);
// 	}
// 	objbc.SetProperty("left", ValueBase().SetInt(x));
// 	objbc.SetProperty("top", ValueBase().SetInt(y));
// 	objbc.SetProperty("width", ValueBase().SetInt(width));
// 	objbc.SetProperty("height", ValueBase().SetInt(height));
// }

void HtmlCanvas::OnAddToParent(const HandleContext&context, HtmlElement*pParent)
{
	JSVM::GetInstance()->AddVisibleCanvas(this);
}

void HtmlCanvas::OnRemoveFromParent(const HandleContext&context, HtmlElement*pParent)
{
	JSVM::GetInstance()->RemoveVisibleCanvas(this);
}

float HtmlCanvas::WindowPointToGlPointX(float x)
{
	return 2.0f*x / m_fbo.m_width - 1;
//	return 2.0f*x / m_width/*m_glState.m_viewport.width*/ - 1;
}

float HtmlCanvas::WindowPointToGlPointY(float y)
{
	return -2.0f*y / m_fbo.m_height + 1;
//	return -2.0f*y / m_height/*m_glState.m_viewport.height*/ + 1;
}

float HtmlCanvas::GlPointToWindowPointX(float x)
{
	return (x + 1)*m_fbo.m_width*0.5f;
//	return (x + 1)*m_width/*m_glState.m_viewport.width*/*0.5f;
}

float HtmlCanvas::GlPointToWindowPointY(float y)
{
	return m_fbo.m_height- (y + 1)*m_fbo.m_height/*m_glState.m_viewport.height*/*0.5f;
//	return m_height/*m_glState.m_viewport.height*/ - (y + 1)*m_height/*m_glState.m_viewport.height*/*0.5f;
}

DXPOINT HtmlCanvas::WindowPointToGlPoint(const DXPOINT &vPos)
{
	DXPOINT pt;
	pt.x = WindowPointToGlPointX(vPos.x);
	pt.y = WindowPointToGlPointY(vPos.y);
	return pt;
}

// int HtmlCanvas::GetWidth()
// {;
// 	return m_width;
// 	return m_fbo.m_width;
// }
// 
// int HtmlCanvas::GetHeight()
// {
// 	return m_height;
// 	return m_fbo.m_height;
// }

bool HtmlCanvas::SetBackgroundImage(const std::string &backgroundImage)
{
	if (m_texBackground)
		gl.DeleteTextures(1, &m_texBackground);
	m_imgBackground.Cleanup();
	ImageDecoderInterface*pDec=JSVM::GetInstance()->GetImageDecoder();
	std::string szPath = JSVM::GetInstance()->GetFullPath(backgroundImage);
	if (!pDec->DecodeFile(szPath.c_str(), m_imgBackground))return false;

	m_texBackground = OpenglContext::CreateGlTexID(m_imgBackground);
	assert(!glGetError());
	return m_texBackground!=0;
}

void HtmlCanvas::DrawBackGround()
{
	if (m_texBackground)
	{
		DrawImage(m_texBackground, m_imgBackground.nWidth, m_imgBackground.nHeight,
			0, 0, m_imgBackground.nWidth, m_imgBackground.nHeight,
			0, 0, GetWidth(), GetHeight(), nullptr);
	}
	else
	{//默认画白色
		DXPOINT vPos[] =
		{
			{ -1, 1 },
			{ -1, -1 },
			{ 1, -1 },
			{ 1, 1 },
		};


//		GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

		gl.Viewport(0, 0, OpenglContext::GetWidth(), OpenglContext::GetHeight());
		gl.BindBuffer(GL_ARRAY_BUFFER, 0);
		gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		assert(!gl.GetError());

		gl.UseProgram(OpenglContext::s_SingleColorProgram);
		assert(!gl.GetError());
		OpenglContext::s_SingleColorProgram.SetColor(1,1, 1, 1);
		OpenglContext::s_TextureProgram.SetVertexPosition(2, GL_FLOAT, GL_FALSE, 0, vPos);
		assert(!gl.GetError());
		gl.DrawArrays(GL_TRIANGLE_FAN, 0, 4);
//		gl.DrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
		assert(!gl.GetError());

	}
}

void HtmlCanvas::OnMeasure(int parentX, int parentY, int parentWidth, int parentHeight)
{//居中
	HtmlEventTarget::OnMeasure(parentX, parentY, parentWidth, parentHeight);
 	m_left =  (OpenglContext::GetWidth() - m_width)*0.5f;
 	m_top = (OpenglContext::GetHeight() - m_height)*0.5f;
}

void HtmlCanvas::Clip()
{
	m_curState->m_clipPath = m_pointPath;
// 	glViewport(0, 0, m_fbo.m_width, m_fbo.m_height);
// 	OpenglFBO fbo;
// 	fbo.Init(m_fbo.m_width, m_fbo.m_height);
// 	fbo.BeginFBO();
// 	for (int j = 0; j < m_pointPath.m_PathList.size(); j++)
// 	{
// 		CANVASPATH*pointPath = m_pointPath.m_PathList[j];
// 		if (!pointPath->CreatePathMesh())continue;
// 
// 		DXPOINT *vVertices = new DXPOINT[pointPath->m_pointPath.size()];
// 		for (int i = 0; i < pointPath->m_pointPath.size(); i++)
// 		{
// 			vVertices[i] = WindowPointToGlPoint(m_curState->m_matrix*pointPath->m_pointPath[i]);
// 		}
// 
// 
// 		gl.BindBuffer(GL_ARRAY_BUFFER, 0);
// 		gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// 
// 		
// 		gl.UseProgram(OpenglContext::s_SingleColorProgram);
// 
// 		OpenglContext::s_SingleColorProgram.SetColor(1,1,1,1);
// 		OpenglContext::s_SingleColorProgram.SetVertexPosition(2, GL_FLOAT, GL_FALSE, 0, vVertices);
// 
// 		gl.DrawElements(GL_TRIANGLES, pointPath->m_indexList.size() * 3, GL_UNSIGNED_SHORT, pointPath->m_indexList.data());
// 		assert(!gl.GetError());
// 
// 		delete[]vVertices;
// 	}
// 	fbo.EndFBO();
// 	m_curState->m_clipTexture = CTexture(fbo.m_FboTexture);
// 	m_curState->m_clipTexture.m_nWidth = m_fbo.m_width;
// 	m_curState->m_clipTexture.m_nHeight = m_fbo.m_height;
// 	fbo.m_FboTexture = 0;
// 	fbo.Uninit();
}







HtmlCanvas* HtmlCanvas::s_prevCanvas=nullptr;




// 
// HtmlCanvas* AutoFbo::s_PrevCanvas=nullptr;
// 
// AutoFbo::AutoFbo(HtmlCanvas*pCanvas)
// {
// 	new(this)AutoFbo(pCanvas, true);
// 	
// }
// 
// AutoFbo::AutoFbo(HtmlCanvas*pCanvas, bool bSetState)
// {
// //	m_bSetState = bSetState;
// 	m_pCurCanvas = pCanvas;
// 	if (pCanvas != s_PrevCanvas)
// 	{
// 		m_pOldCanvas = s_PrevCanvas;
// //		if (m_bSetState)pCanvas->m_glState.SetStates();
// 
// 		s_PrevCanvas = pCanvas;
// 		m_pCurCanvas->m_fbo.BeginFBO();
// 		m_hasBeginFbo = true;
// 	}
// }
// 
// AutoFbo::~AutoFbo()
// {
// 	if (m_hasBeginFbo)
// 	{
// 		m_pCurCanvas->m_fbo.EndFBO();
// 	}
// 	s_PrevCanvas = m_pOldCanvas;
// 	if (m_pOldCanvas)
// 	{
// //		if (m_bSetState)m_pOldCanvas->m_glState.SetStates();
// 	}
// 	
// }

