#include "stdafx.h"
#include "HtmlCanvasRenderingContext2D.h"
#include "HtmlCanvas.h"
#include "HTML5Color.h"
#include "HtmlImgClass.h"
#include "JSVMInterface.h"
#include "HtmlImageData.h"
#include "HtmlLinearGradient.h"

IMPLEMENT_CUSTOM_CLASS(HtmlCanvasRenderingContext2D, CustomClassBase)







HtmlCanvasRenderingContext2D::HtmlCanvasRenderingContext2D()
{
}


HtmlCanvasRenderingContext2D::~HtmlCanvasRenderingContext2D()
{
}

bool HtmlCanvasRenderingContext2D::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "CanvasRenderingContext2D", HandleObject());

}

bool HtmlCanvasRenderingContext2D::OnGetProperty(GetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "globalAlpha"))
	{
		args.SetRetValue(ValueBase().SetNumber(m_pCanvas->m_curState->m_globalAlpha));
		return true;
	}
	else if (!PROPCMP(name, "globalCompositeOperation"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_pCanvas->m_curState->m_globalCompositeOperation.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "lineCap"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_pCanvas->m_curState->m_lineCap.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "lineJoin"))
	{
		args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), m_pCanvas->m_curState->m_lineJoin.c_str()));
		return true;
	}
	else if (!PROPCMP(name, "lineWidth"))
	{
		args.SetRetValue(ValueBase().SetNumber(m_pCanvas->m_curState->m_lineWidth));
		return true;
	}
	else if (!PROPCMP(name, "miterLimit"))
	{
		args.SetRetValue(ValueBase().SetNumber(m_pCanvas->m_curState->m_miterLimit));
		return true;
	}
	else if (!PROPCMP(name, "textBaseline"))
	{
		switch (m_textbaseline)
		{
		case TB_ALPHABETIC:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "alphabetic")); break;
		case TB_TOP:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "top")); break;
		case TB_MIDDLE:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "middle")); break;
		case TB_BOTTOM:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "bottom")); break;
		case TB_HANGING:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "hanging")); break;
		case TB_ideographic:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "ideographic")); break;

		}
		return true;
	}
	else if (!PROPCMP(name, "textAlign"))
	{
		switch (m_textalign)
		{
		case AL_LEFT:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "left")); break;
		case AL_RIGHT:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "right")); break;
		case AL_CENTER:args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), "center")); break;


		}
		return true;
	}
	 
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlCanvasRenderingContext2D::OnSetProperty(SetPropertyArgs&args)
{
	const char*name = args.GetName();
	if (!PROPCMP(name, "globalAlpha"))
	{
		m_pCanvas->m_curState->m_globalAlpha = args.GetValue().GetNumber();
		return true;
	}
	else if (!PROPCMP(name, "globalCompositeOperation"))
	{
		m_pCanvas->m_curState->m_globalCompositeOperation = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "lineCap"))
	{
		m_pCanvas->m_curState->m_lineCap = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "lineJoin"))
	{
		m_pCanvas->m_curState->m_lineJoin = args.GetValue().GetString(args.GetContext());
		return true;
	}
	else if (!PROPCMP(name, "lineWidth"))
	{
		m_pCanvas->m_curState->m_lineWidth = args.GetValue().GetNumber();
		return true;
	}
	else if (!PROPCMP(name, "miterLimit"))
	{
		m_pCanvas->m_curState->m_miterLimit = args.GetValue().GetNumber();
		return true;
	}
	else if (!PROPCMP(name, "textBaseline"))
	{
		std::string &&val = args.GetValue().GetString(args.GetContext());
		std::transform(val.begin(), val.end(), val.begin(), tolower);  //字母转小写
		if (val == "alphabetic")m_textbaseline = TB_ALPHABETIC;
		else if (val == "top")m_textbaseline = TB_TOP;
		else if (val == "middle")m_textbaseline = TB_MIDDLE;
		else if (val == "bottom")m_textbaseline = TB_BOTTOM;
		else if (val == "hanging")m_textbaseline = TB_HANGING;
		else if (val == "ideographic")m_textbaseline = TB_ideographic;
		
		return true;
	}
	else if (!PROPCMP(name, "textAlign"))
	{
		std::string &&val = args.GetValue().GetString(args.GetContext());
		std::transform(val.begin(), val.end(), val.begin(), tolower);  //字母转小写
		if (val == "left"||val=="start")m_textalign = AL_LEFT;
		else if (val == "right" || val == "end")m_textalign = AL_RIGHT;
		else if (val == "center")m_textalign = AL_CENTER;


		return true;
	}
	else if (!PROPCMP(name, "fillStyle"))
	{
		if (args.GetValue().IsString())
		{
			std::string&&val = args.GetValue().GetString(args.GetContext());
			m_fillStyleColor= CHTML5Color::GetColorValue(val);
			m_fillStyle = FS_STRING;
		}
		else if (args.GetValue().IsObject())
		{
			m_fillStyle = FS_OBJECT;
		}
		else assert(0);
	}
	return CustomClassBase::OnSetProperty(args);
}

void HtmlCanvasRenderingContext2D::OnFinalize(FinalizeArgs&args)
{
	return CustomClassBase::OnFinalize(args);
}

bool HtmlCanvasRenderingContext2D::OnConstructor(IFunctionCallbackArgs&args)
{
	return CustomClassBase::OnConstructor(args);
}

bool HtmlCanvasRenderingContext2D::arc(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(5, "arc");
	double x, y, radius, startAngle, endAngle;
	bool counterclockwise=false;
	x = args[0].GetNumber();
	y = args[1].GetNumber();
	radius = args[2].GetNumber();
	startAngle = args[3].GetNumber();
	endAngle = args[4].GetNumber();
	if (args.length()>5)
		counterclockwise = args[5].GetBoolean();
	CANVASPATH*pointPath = m_pCanvas->m_pointPath.GetCurPath();
	DXPOINT pt;
	double dangle = PI / 180;
	if (!counterclockwise)
	{
		for (double ang = startAngle; ang < endAngle; ang+=dangle)
		{
			pt.x = radius*cosf(ang) + x;
			pt.y = radius*sinf(ang) + y;
			pointPath->AddPoint(pt);
		}
		pt.x = radius*cosf(endAngle) + x;
		pt.y = radius*sinf(endAngle) + y;
		pointPath->AddPoint(pt);
	}
	else
	{
		endAngle -= 2 * PI;
		for (double ang = startAngle; ang > endAngle; ang-=dangle)
		{
			pt.x = radius*cosf(ang) + x;
			pt.y = radius*sinf(ang) + y;
			pointPath->AddPoint(pt);
		}
		pt.x = radius*cosf(endAngle) + x;
		pt.y = radius*sinf(endAngle) + y;
		pointPath->AddPoint(pt);
	}
// 	if (!m_bBeginPath)
// 	{
// 		DrawPathLine(args.getContext());
// 		m_pointPath.clear();
// 	}
	return true;
}

bool HtmlCanvasRenderingContext2D::arcTo(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlCanvasRenderingContext2D::beginPath(IFunctionCallbackArgs&args)
{
//	m_bBeginPath = true;
	m_pCanvas->m_pointPath.Clear();
	return true;
}

bool HtmlCanvasRenderingContext2D::bezierCurveTo(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlCanvasRenderingContext2D::clearRect(IFunctionCallbackArgs&args)
{
//	AutoFbo af(m_pCanvas); 
	CHECK_ARGS_COUNT(4, "HtmlCanvasRenderingContext2D::clearRect");
	int x = args[0].GetInt();
	int y = args[1].GetInt();
	int width = args[2].GetInt();
	int height = args[3].GetInt();

	DXPOINT vPos[] =
	{
		{ -1,1 },
		{-1,-1 },
		{ 1,-1 },
		{ 1,1 },
	};
	glViewport(x, m_pCanvas->GetHeight()-y-height, width, height);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 	DXPOINT vPos[] =
// 	{
// 		{ x, y },
// 		{ x, y + height },
// 		{ x + width, y + height },
// 		{ x + width, y },
// 	};
// 
// 
// 	vPos[0] = m_pCanvas->WindowPointToGlPoint(vPos[0]);
// 	vPos[1] = m_pCanvas->WindowPointToGlPoint(vPos[1]);
// 	vPos[2] = m_pCanvas->WindowPointToGlPoint(vPos[2]);
// 	vPos[3] = m_pCanvas->WindowPointToGlPoint(vPos[3]);
// 
// 
// 
// 
// 	
// 	gl.BindBuffer(GL_ARRAY_BUFFER, 0);//如果画图没用到这个也要设为0，因为cocos2d用完后没有还原，会导致图画不出来
// 	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
// 
// 
// 	// Use the program object
// 	
// 	gl.Disable(GL_BLEND);
// 	gl.UseProgram(OpenglContext::s_SingleColorProgram);
// 	OpenglContext::s_SingleColorProgram.SetColor(0,0,0,0);
// 	// Load the vertex position
// 	OpenglContext::s_TextureProgram.SetVertexPosition(2, GL_FLOAT, GL_FALSE, 0, vPos);
// 
// 
// 
// 	gl.DrawArrays(GL_TRIANGLE_FAN, 0, 4);
// 	gl.Enable(GL_BLEND);


	return true;
}

bool HtmlCanvasRenderingContext2D::clip(IFunctionCallbackArgs&args)
{
	m_pCanvas->Clip();

	return true;
}

bool HtmlCanvasRenderingContext2D::closePath(IFunctionCallbackArgs&args)
{
//	m_bBeginPath = false;
	CANVASPATH*curpath = m_pCanvas->m_pointPath.GetCurPath();
	if (curpath->m_pointPath.size() > 2)
	{
		if (curpath->m_pointPath[0] != curpath->m_pointPath[curpath->m_pointPath.size()-1])
			curpath->AddPoint(curpath->m_pointPath[0]);
	}
	return true;
}

bool HtmlCanvasRenderingContext2D::createLinearGradient(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "createLinearGradient");
	int x0 = args[0].GetInt();
	int y0 = args[1].GetInt();
	int x1 = args[2].GetInt();
	int y1 = args[3].GetInt();
	LocalObject objLG(args.getContext(), HtmlLinearGradient::CreateObject(args.getContext()));
	HtmlLinearGradient*pLG = (HtmlLinearGradient*)FindCustomClass(objLG);
	pLG->m_x0 =x0 ;
	pLG->m_x1 = x1;
	pLG->m_y0 = y0;
	pLG->m_y1 = y1;
	args.setReturnValue(objLG);
	return true;
}

bool HtmlCanvasRenderingContext2D::createPattern(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlCanvasRenderingContext2D::createRadialGradient(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlCanvasRenderingContext2D::drawImage(IFunctionCallbackArgs&args)
{
	//AutoFbo af(m_pCanvas);
	BeginClip(args.getContext());

	int x, y, width, height, sx, sy, swidth, sheight;
	LocalObject objImg(args.getContext(), args[0].GetObject());
	CustomClassBase*pObj = FindCustomClass(objImg);
	do
	{
		if (args.length() == 3)//(img,x,y);
		{

			x = args[1].GetInt();
			y = args[2].GetInt();
			HtmlImgClass*pImg = dynamic_cast<HtmlImgClass*>(pObj);
			if (pImg)
			{
				if (!pImg->IsLoaded())
				{
					ReportError(args.getContext(), "图像未加载");
					break;
				}
				width = pImg->m_imgData.nWidth;
				height = pImg->m_imgData.nHeight;
				sx = 0;
				sy = 0;
				swidth = width;
				sheight = height;
//				m_pCanvas->m_fbo.BeginFBO();
				m_pCanvas->DrawImage(pImg->m_nTexID, pImg->m_imgData.nWidth, pImg->m_imgData.nHeight, sx, sy, swidth, sheight, x, y, width, height, &m_pCanvas->m_curState->m_matrix);
//				m_pCanvas->m_fbo.EndFBO();
				break;
			}
			HtmlCanvas*pCanvas = dynamic_cast<HtmlCanvas*>(pObj);
			if (pCanvas)
			{
				swidth=width = pCanvas->m_fbo.m_width;
				sheight=height = pCanvas->m_fbo.m_height;
				sx = sy = 0;
//				m_pCanvas->m_fbo.BeginFBO();
				m_pCanvas->DrawImage(pCanvas->m_fbo.m_FboTexture, pCanvas->m_fbo.m_width, pCanvas->m_fbo.m_height,
					sx, sy, swidth, sheight, x, y, width, height, &m_pCanvas->m_curState->m_matrix);
//				m_pCanvas->m_fbo.EndFBO();
				break;
			}
			assert(0);
		}
		else if (args.length() == 5)//(img,x,y,width,height
		{

			x = args[1].GetInt();
			y = args[2].GetInt();
			width = args[3].GetInt();
			height = args[4].GetInt();
			HtmlImgClass*pImg = dynamic_cast<HtmlImgClass*>(pObj);
			if (pImg)
			{
				if (!pImg->IsLoaded())
				{
					ReportError(args.getContext(), "图像未加载");
					break;
				}
				swidth = width;
				sheight = height;
				sx = 0;
				sy = 0;
//				m_pCanvas->m_fbo.BeginFBO();
				m_pCanvas->DrawImage(pImg->m_nTexID, pImg->m_imgData.nWidth, pImg->m_imgData.nHeight, sx, sy, swidth, sheight, x, y, width, height, &m_pCanvas->m_curState->m_matrix);

//				m_pCanvas->m_fbo.EndFBO();
				break;
			}
			HtmlCanvas*pCanvas = dynamic_cast<HtmlCanvas*>(pObj);
			if (pCanvas)
			{
				swidth = width;
				sheight = height;
				sx = sy = 0;
//				m_pCanvas->m_fbo.BeginFBO();
				m_pCanvas->DrawImage(pCanvas->m_fbo.m_FboTexture, pCanvas->m_fbo.m_width, pCanvas->m_fbo.m_height,
					sx, sy, swidth, sheight, x, y, width, height, &m_pCanvas->m_curState->m_matrix);
//				m_pCanvas->m_fbo.EndFBO();
				break;
			}
			assert(0);
		}
		else if (args.length() == 9)//(img,sx,sy,swidth,sheight,x,y,width,height);
		{

			sx = args[1].GetInt();
			sy = args[2].GetInt();
			swidth = args[3].GetInt();
			sheight = args[4].GetInt();
			x = args[5].GetInt();
			y = args[6].GetInt();
			width = args[7].GetInt();
			height = args[8].GetInt();
			HtmlImgClass*pImg = dynamic_cast<HtmlImgClass*>(pObj);
			if (pImg)
			{
				if (!pImg->IsLoaded())
				{
					ReportError(args.getContext(), "图像未加载");
					break;
				}
//				m_pCanvas->m_fbo.BeginFBO();
				m_pCanvas->DrawImage(pImg->m_nTexID, pImg->m_imgData.nWidth, pImg->m_imgData.nHeight, sx, sy, swidth, sheight, x, y, width, height, &m_pCanvas->m_curState->m_matrix);

//				m_pCanvas->m_fbo.EndFBO();
				break;
			}
			HtmlCanvas*pCanvas = dynamic_cast<HtmlCanvas*>(pObj);
			if (pCanvas)
			{
// 				uint32_t*da = new uint32_t[pCanvas->m_fbo.m_width*pCanvas->m_fbo.m_height];
// 				pCanvas->m_fbo.BeginFBO();
// 				pCanvas->m_fbo.GetImageData(0, 0, pCanvas->m_fbo.m_width, pCanvas->m_fbo.m_height, (uint8_t*)da);
// 				m_pCanvas->m_fbo.BeginFBO();
// 				delete[]da;
				m_pCanvas->DrawImage(pCanvas->m_fbo.m_FboTexture, pCanvas->m_fbo.m_width, pCanvas->m_fbo.m_height,
					sx, sy, swidth, sheight, x, y, width, height, &m_pCanvas->m_curState->m_matrix);
//				m_pCanvas->m_fbo.EndFBO();
				break;
			}
			assert(0);
		}
		else assert(0);
	} while (false);
	EndClip();

	return true;
}

bool HtmlCanvasRenderingContext2D::fill(IFunctionCallbackArgs&args)
{
	BeginClip(args.getContext());
	FillPointPath(m_pCanvas->m_pointPath,args.getContext());
	
	EndClip();
	return true;
}

bool HtmlCanvasRenderingContext2D::fillRect(IFunctionCallbackArgs&args)
{
	//AutoFbo af(m_pCanvas);
	CHECK_ARGS_COUNT(4, "HtmlCanvasRenderingContext2D::fillRect");
	int x = args[0].GetNumber();
	int y = args[1].GetNumber();
	int width = args[2].GetNumber();
	int height = args[3].GetNumber();

//	m_pCanvas->m_fbo.BeginFBO();
	BeginClip(args.getContext());
	glViewport(0, 0, m_pCanvas->m_fbo.m_width, m_pCanvas->m_fbo.m_height);



	DXPOINT vVertices[] = { 
		{ x, y },
		{ x, y + height },
		{ x + width, y },
		{ x + width, y + height }
	};
	vVertices[0] = m_pCanvas->WindowPointToGlPoint(m_pCanvas->m_curState->m_matrix*vVertices[0]);
	vVertices[1] = m_pCanvas->WindowPointToGlPoint(m_pCanvas->m_curState->m_matrix*vVertices[1]);
	vVertices[2] = m_pCanvas->WindowPointToGlPoint(m_pCanvas->m_curState->m_matrix*vVertices[2]);
	vVertices[3] = m_pCanvas->WindowPointToGlPoint(m_pCanvas->m_curState->m_matrix*vVertices[3]);

	gl.BindBuffer(GL_ARRAY_BUFFER, 0);//如果画图没用到这个也要设为0，因为cocos2d用完后没有还原，会导致图画不出来
	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (!glSetFillStyle(args.getContext(), vVertices))
		return true;

	
// 	gl.VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vVertices);
// 	gl.EnableVertexAttribArray(0);
	gl.DrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	m_pCanvas->m_fbo.EndFBO();
	int err = gl.GetError();
	assert(!err);
	EndClip();

	return true;
}

bool HtmlCanvasRenderingContext2D::lineTo(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "lineTo");
	int x = args[0].GetInt();
	int y = args[1].GetInt();
	m_pCanvas->m_pointPath.GetCurPath()->AddPoint(DXPOINT(x, y));
// 	if (!m_bBeginPath)
// 	{
// 		DrawPathLine(args.getContext());
// 		m_pointPath.clear();
// 		m_pointPath.push_back(DXPOINT(x, y));//lineto后自动设为moveto
// 	}

	return true;
}

bool HtmlCanvasRenderingContext2D::moveTo(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "moveTo");
	int x = args[0].GetInt();
	int y = args[1].GetInt();
	m_pCanvas->m_pointPath.NewPath()->AddPoint(DXPOINT(x, y));
	return true;
}

bool HtmlCanvasRenderingContext2D::quadraticCurveTo(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlCanvasRenderingContext2D::rect(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "HtmlCanvasRenderingContext2D::rect");
	int x = args[0].GetInt();
	int y = args[1].GetInt();
	int width = args[2].GetInt();
	int height = args[3].GetInt();

	DXPOINT lt(x, y);
	DXPOINT lb(x, y + height);
	DXPOINT rb(x + width, y + height);
	DXPOINT rt(x + width, y);
	CANVASPATH*pointPath = m_pCanvas->m_pointPath.NewPath();
	pointPath->AddPoint(lt);
	pointPath->AddPoint(lb);
	pointPath->AddPoint(rb);
	pointPath->AddPoint(rt);
	pointPath->AddPoint(lt);

	return true;
}

bool HtmlCanvasRenderingContext2D::restore(IFunctionCallbackArgs&args)
{
	if (m_pCanvas->m_saveState.size() == 0)return true;
	SAFE_DELETE(m_pCanvas->m_curState);
	m_pCanvas->m_curState = m_pCanvas->m_saveState.front();
	m_pCanvas->m_saveState.pop_front();
	
	return true;
}

bool HtmlCanvasRenderingContext2D::rotate(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlCanvasRenderingContext2D::rotate");
	double angle = args[0].GetNumber();
	m_pCanvas->m_curState->m_fRotate += angle;// += angle;
	UpdateMatrix();
	return true;
}

bool HtmlCanvasRenderingContext2D::save(IFunctionCallbackArgs&args)
{
	HtmlCanvas::CANVASSTATE*state = m_pCanvas->m_curState->Clone();
	m_pCanvas->m_saveState.push_front(m_pCanvas->m_curState);
	m_pCanvas->m_curState = state;
	return true;
}

bool HtmlCanvasRenderingContext2D::scale(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlCanvasRenderingContext2D::scale");
	double sx = args[0].GetNumber();
	double sy = args[1].GetNumber();
	m_pCanvas->m_curState->m_fScaleX *= sx;//*= sx;
	m_pCanvas->m_curState->m_fScaleY *= sy;//*= sy;
	UpdateMatrix();
	return true;
}

bool HtmlCanvasRenderingContext2D::stroke(IFunctionCallbackArgs&args)
{
//	m_bBeginPath = false;
	DrawPathLine(args.getContext());
//	m_pointPath.clear();
	return true;
}

bool HtmlCanvasRenderingContext2D::strokeRect(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlCanvasRenderingContext2D::translate(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlCanvasRenderingContext2D::translate");
	float x = args[0].GetNumber();
	float y = args[1].GetNumber();
	//平移前如果有旋转缩放，则要对translate的参数先做旋转缩放
	DXPOINT vec(x*m_pCanvas->m_curState->m_fScaleX, y*m_pCanvas->m_curState->m_fScaleY);
	vec = vec.Rotate(m_pCanvas->m_curState->m_fRotate);
	m_pCanvas->m_curState->m_ptOrigin.x += vec.x;
	m_pCanvas->m_curState->m_ptOrigin.y += vec.y;


	UpdateMatrix();
	return true;
}

bool HtmlCanvasRenderingContext2D::getImageData(IFunctionCallbackArgs&args)
{
	CustomClassAutoThisObj cc(this, args,false);
	CHECK_ARGS_COUNT(4, "getImageData");
	int x = args[0].GetInt();
	int y = args[1].GetInt();
	int width = args[2].GetInt();
	int height = args[3].GetInt();

	uint8_t*pData = new uint8_t[width*height * 4];
	
	m_pCanvas->m_fbo.GetImageData(x, y, width, height, pData);
	LocalObject objimg(args.getContext(), HtmlImageData::CreateObject(args.getContext()));
	HtmlImageData*pImg = (HtmlImageData*)objimg.GetNativeObj();
	CustomClassAutoThisObj ci(pImg, objimg);
	bool bOK=pImg->SetData(args.getContext(), width, height, pData);
	delete[]pData;
	args.setReturnValue(objimg);
	return true;
}
bool HtmlCanvasRenderingContext2D::transform(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(6, "HtmlCanvasRenderingContext2D::transform");
	float a = args[0].GetNumber();
	float b = args[1].GetNumber();
	float c = args[2].GetNumber();
	float d = args[3].GetNumber();
	float e = args[4].GetNumber();
	float f = args[5].GetNumber();
	MATRIX mat;
	mat._11 = a;
	mat._12 = c;
	mat._13 = e;
	mat._21 = b;
	mat._22 = d;
	mat._23 = f;
	mat._31 = 0;
	mat._32 = 0;
	mat._33 = 1;

	m_pCanvas->m_curState->m_fRotate = 0;
	m_pCanvas->m_curState->m_fScaleX = m_pCanvas->m_curState->m_fScaleY = 1;
	m_pCanvas->m_curState->m_ptOrigin.x = m_pCanvas->m_curState->m_ptOrigin.y = 0;
	m_pCanvas->m_curState->m_matrix = m_pCanvas->m_curState->m_matrix*mat;
	return true;
}

bool HtmlCanvasRenderingContext2D::setTransform(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(6, "HtmlCanvasRenderingContext2D::setTransform");
	float a = args[0].GetNumber();
	float b = args[1].GetNumber();
	float c = args[2].GetNumber();
	float d = args[3].GetNumber();
	float e = args[4].GetNumber();
	float f = args[5].GetNumber();
	m_pCanvas->m_curState->m_matrix._11 = a;
	m_pCanvas->m_curState->m_matrix._12 = c;
	m_pCanvas->m_curState->m_matrix._13 = e;
	m_pCanvas->m_curState->m_matrix._21 = b;
	m_pCanvas->m_curState->m_matrix._22 = d;
	m_pCanvas->m_curState->m_matrix._23 = f;
	m_pCanvas->m_curState->m_matrix._31 = 0;
	m_pCanvas->m_curState->m_matrix._32 = 0;
	m_pCanvas->m_curState->m_matrix._33 = 1;
	m_pCanvas->m_curState->m_fRotate = 0;
	m_pCanvas->m_curState->m_fScaleX = m_pCanvas->m_curState->m_fScaleY = 1;
	m_pCanvas->m_curState->m_ptOrigin.x = m_pCanvas->m_curState->m_ptOrigin.y = 0;
	return true;
}

bool HtmlCanvasRenderingContext2D::fillText(IFunctionCallbackArgs&args)
{
	
	CHECK_ARGS_COUNT_MIN(3, "HtmlCanvasRenderingContext2D::fillText");
	std::string &&text = args[0].GetString(args.getContext());
	int x = args[1].GetInt();
	int y = args[2].GetInt();
	int maxwidth = 0xfffffff;
	if (args.length() == 4)
	{
		maxwidth = args[3].GetInt();
	}
	FontInfo finfo;
	ImageData data;
//	PrintDescribeScriptedCaller(args.getContext());
	//生成文字图片模板
	CSSFONT font;
	std::string &&szfont = args.thisObj().GetProperty(args.getContext(), "font").GetString(args.getContext());
	font.FromString(szfont.c_str());
	finfo.nFontSize = font.fontsize;
	finfo.szFontName = font.fontfamily;
	
	//设置文字填充色
	LocalValue vFillstyle(args.getContext(), GetJsObject().GetProperty(args.getContext(), "fillStyle"));
	if (vFillstyle.IsString())
	{//单色
		
		std::string &&val = vFillstyle.GetString(args.getContext());
		uint32_t nColor = CHTML5Color::GetColorValue(val);
		JSVM::GetInstance()->GetImageDecoder()->CreateStringImage(text.c_str(), finfo, data, false, nColor,m_textbaseline);

	}
	else if (vFillstyle.IsUndefined())
	{
		JSVM::GetInstance()->GetImageDecoder()->CreateStringImage(text.c_str(), finfo, data, false, 0xffffffff,m_textbaseline);
	}
	else
	{//其它渐变暂不支持
		assert(0);
		return false;
	}

	int xoffset = 0;
	switch (m_textalign)
	{
	case AL_LEFT:xoffset = 0; break;
	case AL_RIGHT:xoffset = -data.nWidth; break;
	case AL_CENTER:xoffset = -data.nWidth*0.5f; break;
	}

	
	GLuint nTexID = OpenglContext::CreateGlTexID(data);

	m_pCanvas->DrawImage(nTexID, data.nWidth, data.nHeight, 0, 0, data.nWidth, data.nHeight, x + xoffset, y + data.nYOffset, min(maxwidth, data.nWidth), data.nHeight, &m_pCanvas->m_curState->m_matrix);
	

// 	uint8_t*pData = new uint8_t[data.nWidth * data.nHeight * 4];
// 	m_pCanvas->m_fbo.GetImageData(0, 0, data.nWidth, data.nHeight, pData);
// 
// 	delete[]pData;
	gl.DeleteTextures(1, &nTexID);
	return true;
}

bool HtmlCanvasRenderingContext2D::strokeText(IFunctionCallbackArgs&args)
{
	return fillText(args);//空心字体暂时不会做，先用实心字体代替
	return true;
}

bool HtmlCanvasRenderingContext2D::measureText(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "measureText");
	std::string &&str= args[0].GetString(args.getContext());
	CSSFONT font;
	FontInfo finfo;
	ImageData data;
	std::string &&szfont = args.thisObj().GetProperty(args.getContext(), "font").GetString(args.getContext());
	font.FromString(szfont.c_str());
	finfo.nFontSize = font.fontsize;
	finfo.szFontName = font.fontfamily;
	JSVM::GetInstance()->GetImageDecoder()->CreateStringImage(str.c_str(), finfo, data, true,0,m_textbaseline);
	LocalObject ret(args.getContext());
	ret.CreateJsObject();
	ret.SetProperty("width", ValueBase().SetInt(data.nWidth));
	ret.SetProperty("height", ValueBase().SetInt(data.nHeight));
	args.setReturnValue(ret);



	return true;
}

bool HtmlCanvasRenderingContext2D::glSetStrokeStyle(const HandleContext& context)
{
	LocalValue vStrokestyle(context, GetJsObject().GetProperty(context, "strokeStyle"));
	if (vStrokestyle.IsString())
	{//单色
		std::string &&val = vStrokestyle.GetString(context);
		uint32_t nColor = CHTML5Color::GetColorValue(val);
		gl.UseProgram(OpenglContext::s_SingleColorProgram);

		OpenglContext::s_SingleColorProgram.SetColor(GetRValue(nColor) / 255.0, GetGValue(nColor) / 255.0, GetBValue(nColor) / 255.0, GetAValue(nColor)*m_pCanvas->m_curState->m_globalAlpha / 255.0);


		return true;
	}
	else
	{//默认黑色
		uint32_t nColor = 0xff000000;
		gl.UseProgram(OpenglContext::s_SingleColorProgram);

		OpenglContext::s_SingleColorProgram.SetColor(GetRValue(nColor) / 255.0, GetGValue(nColor) / 255.0, GetBValue(nColor) / 255.0, GetAValue(nColor)*m_pCanvas->m_curState->m_globalAlpha / 255.0);
		return true;
	}
	return false;
}


bool HtmlCanvasRenderingContext2D::glSetFillStyle(const HandleContext&context, DXPOINT*vPos)
{
	if (m_fillStyle==FS_STRING)
	{//单色
// 		std::string &&val = vFillstyle.GetString(context);
		uint32_t nColor = m_fillStyleColor;// CHTML5Color::GetColorValue(val);
		gl.UseProgram(OpenglContext::s_SingleColorProgram);

		OpenglContext::s_SingleColorProgram.SetColor(GetRValue(nColor) / 255.0, GetGValue(nColor) / 255.0, GetBValue(nColor) / 255.0, GetAValue(nColor)*m_pCanvas->m_curState->m_globalAlpha / 255.0);
		OpenglContext::s_SingleColorProgram.SetVertexPosition(2, GL_FLOAT, GL_FALSE, 0, vPos);

		return true;
	}
	else if (m_fillStyle==FS_OBJECT)
	{
		LocalValue vFillstyle(context, GetJsObject().GetProperty(context, "fillStyle"));
		HtmlLinearGradient*pGr = dynamic_cast<HtmlLinearGradient*>(FindCustomClass(vFillstyle.GetObject()));
		if (pGr)
		{//线性渐变，生成长(x0,y0)到(x1,y1)宽1像素纹理

			GLuint nTexID = pGr->GetTexture();
			gl.ActiveTexture(GL_TEXTURE0);
			assert(!glGetError());
			gl.BindTexture(GL_TEXTURE_2D, nTexID);
			assert(!glGetError());
			gl.UseProgram(OpenglContext::s_LinearGradientProgram);
			// Set the sampler texture unit to 0
			OpenglContext::s_LinearGradientProgram.SetTextureUnit(0);
			assert(!gl.GetError());
			DXPOINT pt1(pGr->m_x0, pGr->m_y0);
			DXPOINT pt2(pGr->m_x1, pGr->m_y1);
			pt1 = m_pCanvas->m_curState->m_matrix*pt1;
			pt2 = m_pCanvas->m_curState->m_matrix*pt2;
// 			pt1 = m_pCanvas->WindowPointToGlPoint(pt1);
// 			pt2 = m_pCanvas->WindowPointToGlPoint(pt2);
			OpenglContext::s_LinearGradientProgram.SetPosition(pt1,pt2,m_pCanvas->m_width,m_pCanvas->m_height);
			assert(!gl.GetError());
			OpenglContext::s_LinearGradientProgram.SetAlpha(m_pCanvas->m_curState->m_globalAlpha);
			OpenglContext::s_LinearGradientProgram.SetVertexPosition(2, GL_FLOAT, GL_FALSE, 0, vPos);
			return true;
		}
		else
		{
			assert(0);
			return false;
		}
	}
	else
	{
		assert(0);
		return false;
	}
	
}

void HtmlCanvasRenderingContext2D::UpdateMatrix()
{
	MATRIX matTran;
	matTran.SetTranslate(m_pCanvas->m_curState->m_ptOrigin.x, m_pCanvas->m_curState->m_ptOrigin.y);
	MATRIX matRot;
	matRot.SetRotate(m_pCanvas->m_curState->m_fRotate);
	MATRIX matScale;
	matScale.SetScale(m_pCanvas->m_curState->m_fScaleX, m_pCanvas->m_curState->m_fScaleY);
	m_pCanvas->m_curState->m_matrix = matTran*matRot*matScale;
}

bool HtmlCanvasRenderingContext2D::OnFunctionCall(IFunctionCallbackArgs&args, CUSTOMFUNCTION*pFunc)
{

	gl.SetState(&m_pCanvas->m_glState);
// 	if (m_pCanvas->s_prevCanvas != m_pCanvas)
// 	{
		if (pFunc->bIsRenderFunc)
			m_pCanvas->m_fbo.BeginFBO();
//	}
	bool ret = CustomClassBase::OnFunctionCall(args, pFunc);
	m_pCanvas->s_prevCanvas = m_pCanvas;
// 	if (pFunc->bIsRenderFunc)
// 		m_pCanvas->m_fbo.EndFBO();
//	gl.SetState(nullptr);
	return ret;
}

// 
// bool HtmlCanvasRenderingContext2D::CreatePathMesh(std::vector<_INDEX>&indexList)
// {
// 	//将path生成网格
// 	/*推进波前法：1、按顺序查找锐角，连接锐角两边的顶点（添加索引），从查找列表中去掉顶点，直到找不到锐角
// 	2、查找角度最小的钝角，连接两边顶点，重复1
// 	3、查找列表.size()<3结束
// 
// 	*/
// 	if (m_pointPath.size() < 3)return false;
// 	
// 	RoundList poslist;
// 	int n = m_pointPath.size();
// 	if (n > 2 && m_pointPath[n - 1] == m_pointPath[0])//最后一个点跟第一个点重合，去掉
// 		n--;
// 
// 	for (int i = 0; i <n; i++)
// 	{
// 		DXPOINT&pt = m_pointPath[i];
// 		poslist.Append(pt,i);
// 	}
// 
// 	//判断顺时针还是逆时针:所有向量旋转角度加起来<0顺时针，否则逆时针
// 	bool bClockwise;
// 	float angles = 0;
// 	RoundList::_ROUNDLIST*pitem = poslist.pHead;
// 	for (int i = 0; i < poslist.m_nCount; i++)
// 	{
// 		DXPOINT v1 = pitem->prev->pt - pitem->pt;
// 		DXPOINT v2 = pitem->next->pt - pitem->pt;
// 		assert(v1 != v2);
// 		float d = v1*v2;
// 		float len = v1.GetLength()*v2.GetLength();
// 		float ang = acosf(d / len);
// 		DXPOINT3 vcross = v1.cross(v2);
// 		if (vcross.z < 0)ang = -ang;
// 		angles += ang;
// 		pitem = pitem->next;
// 	}
// 	bClockwise = angles < 0;
// 
// 	pitem = poslist.pHead;
// 	
// 	n = 0;
// 	while (poslist.m_nCount>3)
// 	{
// 		DXPOINT v1 = pitem->prev->pt - pitem->pt;
// 		DXPOINT v2 = pitem->next->pt - pitem->pt;
// 		float ang;
// 		if (pitem->bNeedRecalc)
// 		{
// 			float d = v1*v2;
// 			float len = v1.GetLength()*v2.GetLength();
// 			ang = 1 - (d / len);//相当于1-cosa，0－180度时范围0-2，180－360度时范围2－4
// 			DXPOINT3 vcross = v1.cross(v2);
// 			if ((vcross.z > 0 && bClockwise)||(vcross.z<0&&!bClockwise))ang = 4 - ang;
// 			pitem->ang = ang;
// 			pitem->bNeedRecalc = false;
// 		}
// 		else
// 		{
// 			ang = pitem->ang;
// 		}
// 		if (ang<1)
// 		{
// 			//锐角，连接v1v2组成三角形
// 			_INDEX idx;
// 			idx.a = pitem->index;
// 			if (bClockwise)
// 			{//索引顺序为逆时针
// 				idx.b = pitem->prev->index;
// 				idx.c = pitem->next->index;
// 			}
// 			else
// 			{
// 				idx.c = pitem->prev->index;
// 				idx.b = pitem->next->index;
// 			}
// 			indexList.push_back(idx);
// 			pitem->prev->bNeedRecalc = true;
// 			pitem->next->bNeedRecalc = true;
// 			//将本点从列表中移除
// 			RoundList::_ROUNDLIST* p = pitem->next;
// 			poslist.Remove(pitem);
// 			pitem = p;
// 			n = 0;
// 		}
// 		else
// 		{
// 			pitem = pitem->next;
// 			n++;
// 		}
// 		if (n >= poslist.m_nCount)//循环一圈没有找到锐角，可随机取相邻两个点连接成三角形
// 		{
// 			_INDEX idx;
// 			idx.a = pitem->index;
// 			if (bClockwise)
// 			{
// 				idx.b = pitem->prev->index;
// 				idx.c = pitem->next->index;
// 			}
// 			else
// 			{
// 				idx.c = pitem->prev->index;
// 				idx.b = pitem->next->index;
// 			}
// 			indexList.push_back(idx);
// 			pitem->prev->bNeedRecalc = true;
// 			pitem->next->bNeedRecalc = true;
// 			RoundList::_ROUNDLIST* p = pitem->next;
// 			poslist.Remove(pitem);
// 			pitem = p;
// 			n = 0;
// 			
// 		}
// 	}
// 	//添加最后3个顶点
// 	_INDEX idx;
// 	idx.a = poslist.pHead->index;
// 	if (bClockwise)
// 	{
// 		idx.b = poslist.pHead->prev->index;
// 		idx.c = poslist.pHead->next->index;
// 	}
// 	else
// 	{
// 		idx.c = poslist.pHead->prev->index;
// 		idx.b = poslist.pHead->next->index;
// 	}
// 	indexList.push_back(idx);
// 	return true;
// 
// }

bool HtmlCanvasRenderingContext2D::DrawPathLine(const HandleContext&context)
{
	gl.LineWidth(m_pCanvas->m_curState->m_lineWidth);
	assert(!gl.GetError());
	glViewport(0, 0, m_pCanvas->m_fbo.m_width, m_pCanvas->m_fbo.m_height);

	for (int j = 0; j < m_pCanvas->m_pointPath.m_PathList.size(); j++)
	{
		CANVASPATH*pointPath = m_pCanvas->m_pointPath.m_PathList[j];
		DXPOINT *vVertices = new DXPOINT[pointPath->m_pointPath.size()];
		for (int i = 0; i < pointPath->m_pointPath.size(); i++)
		{
			vVertices[i] = m_pCanvas->WindowPointToGlPoint(m_pCanvas->m_curState->m_matrix*pointPath->m_pointPath[i]);
		}


		gl.BindBuffer(GL_ARRAY_BUFFER, 0);
		gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (!glSetStrokeStyle(context))
		{
			delete[]vVertices;
			return true;
		}
			


		gl.VertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, vVertices);
		gl.EnableVertexAttribArray(0);
		gl.DrawArrays(GL_LINE_STRIP, 0, pointPath->m_pointPath.size());
		//	m_pCanvas->m_fbo.EndFBO();
		assert(!gl.GetError());
		delete[]vVertices;
	}
	return true;
}

void HtmlCanvasRenderingContext2D::BeginClip(const HandleContext&context)
{

	if (!m_pCanvas->m_curState->m_clipPath.IsEmpty())
	{//有剪切，使用模板
		glEnable(GL_STENCIL_TEST);
		glClearStencil(0);
		assert(!gl.GetError());
		glClear(GL_STENCIL_BUFFER_BIT);
		assert(!gl.GetError());
		
		assert(!gl.GetError());
		glStencilFunc(GL_NEVER, 0x0, 0x0);
		assert(!gl.GetError());
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);
		assert(!gl.GetError());

		FillPointPath(m_pCanvas->m_curState->m_clipPath, context);
		glStencilFunc(GL_EQUAL, 0x1, 0x1);
		assert(!gl.GetError());
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		assert(!gl.GetError());
	}
}

void HtmlCanvasRenderingContext2D::EndClip()
{
	if (!m_pCanvas->m_curState->m_clipPath.IsEmpty())
	{
		glDisable(GL_STENCIL_TEST);
		assert(!gl.GetError());
	}
}


void HtmlCanvasRenderingContext2D::FillPointPath(const CANVASPATHS &pointPaths, const HandleContext&context)
{
	glViewport(0, 0, m_pCanvas->m_fbo.m_width, m_pCanvas->m_fbo.m_height);
	for (int j = 0; j < pointPaths.m_PathList.size(); j++)
	{
		CANVASPATH*pointPath = pointPaths.m_PathList[j];
		if (!pointPath->CreatePathMesh())continue;

		DXPOINT *vVertices = new DXPOINT[pointPath->m_pointPath.size()];
		for (int i = 0; i < pointPath->m_pointPath.size(); i++)
		{
			vVertices[i] = m_pCanvas->WindowPointToGlPoint(m_pCanvas->m_curState->m_matrix*pointPath->m_pointPath[i]);
		}


		gl.BindBuffer(GL_ARRAY_BUFFER, 0);
		gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (!glSetFillStyle(context, vVertices))
			continue;

		gl.DrawElements(GL_TRIANGLES, pointPath->m_indexList.size() * 3, GL_UNSIGNED_SHORT, pointPath->m_indexList.data());
		assert(!gl.GetError());

		delete[]vVertices;
	}

}