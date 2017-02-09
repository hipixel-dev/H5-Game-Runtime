#include "stdafx.h"
#include "HtmlWebGLRenderingContext.h"
#include "HtmlImgClass.h"
#include "HtmlCanvas.h"
#include "HtmlImageData.h"
IMPLEMENT_CUSTOM_CLASS(HtmlWebGLRenderingContext, CustomClassBase)


HtmlWebGLRenderingContext::HtmlWebGLRenderingContext()
{
}


HtmlWebGLRenderingContext::~HtmlWebGLRenderingContext()
{
}

bool HtmlWebGLRenderingContext::InitClass(const HandleContext&context)
{
	bool ret= InitClass_s(context, "WebGLRenderingContext", HandleObject(nullptr));

	AddConstants(context);
	return ret;
}

bool HtmlWebGLRenderingContext::OnGetProperty(GetPropertyArgs&args)
{
	return CustomClassBase::OnGetProperty(args);
}

bool HtmlWebGLRenderingContext::OnSetProperty(SetPropertyArgs&args)
{
	return CustomClassBase::OnSetProperty(args);
}

void HtmlWebGLRenderingContext::OnFinalize(FinalizeArgs&args)
{
	return CustomClassBase::OnFinalize(args);
}

bool HtmlWebGLRenderingContext::OnConstructor(IFunctionCallbackArgs&args)
{
	bool ret = CustomClassBase::OnConstructor(args);
// 	CustomClassAutoThisObj cc(this, args, true);
// 	AddConstants(args.getContext());
	return ret;
}

bool HtmlWebGLRenderingContext::activeTexture(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "activeTexture");
	long texture = args[0].GetInt();
	gl.ActiveTexture(texture);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::attachShader(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "attachShader");
	GLuint program = args[0].GetInt();
	GLuint shader = args[1].GetInt();
	gl.AttachShader(program, shader);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::bindAttribLocation(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "bindAttribLocation");
	GLuint program = args[0].GetInt();
	GLuint index = args[1].GetInt();
	std::string &&name = args[2].GetString(args.getContext());

	gl.BindAttribLocation(program, index, name.c_str());
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::bindBuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "bindBuffer");
	long target = args[0].GetInt();
	GLuint buffer = args[1].GetInt();

	gl.BindBuffer(target, buffer);
	GLCHECKERR;
	return true;

}

bool HtmlWebGLRenderingContext::bindFramebuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "bindFramebuffer");
	long target = args[0].GetInt();
	GLuint framebuffer = args[1].GetInt();
	assert(0);
	gl.BindFramebuffer(target, framebuffer);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::bindRenderbuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "bindRenderbuffer");
	long target = args[0].GetInt();
	GLuint renderbuffer = args[1].GetInt();
	assert(0);
	gl.BindRenderbuffer(target, renderbuffer);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::bindTexture(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "bindTexture");
	long target = args[0].GetInt();
	GLuint texture = args[1].GetInt();

	gl.BindTexture(target, texture);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::blendColor(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "blendColor");
	float red = args[0].GetNumber();
	float green = args[1].GetNumber();
	float blue = args[2].GetNumber();
	float alpha = args[3].GetNumber();


	assert(0);
	gl.BlendColor(red, green, blue, alpha);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::blendEquation(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "blendEquation");
	long mode = args[0].GetInt();
	
	assert(0);
	gl.BlendEquation(mode);
	GLCHECKERR;
	return true;

}

bool HtmlWebGLRenderingContext::blendEquationSeparate(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "blendEquationSeparate");
	long modeRGB = args[0].GetInt();
	long modeAlpha = args[1].GetInt();

	assert(0);
	gl.BlendEquationSeparate(modeRGB, modeAlpha);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::blendFunc(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "blendFunc");
	long sfactor = args[0].GetInt();
	long dfactor=args[1].GetInt();
	gl.BlendFunc(sfactor, dfactor);

	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::blendFuncSeparate(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "blendFuncSeparate");
	long srcRGB = args[0].GetInt();
	long dstRGB = args[1].GetInt();
	long srcAlpha = args[2].GetInt();
	long dstAlpha=args[3].GetInt();

	assert(0);
	gl.BlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::bufferData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "bufferData");
	long target, usage;
	target = args[0].GetInt();
	usage = args[2].GetInt();
	LocalObject data(args.getContext(), args[1].GetObject());

	uint32_t len = 0;
	void*pdata = 0;
	if (!data.IsArray(args.getContext()))
	{
		ReportError(args.getContext(), "bufferData: input argument is NOT array!");
		return true;
	}
	pdata = data.GetArrayData(args.getContext());
	len = data.GetArrayByteLength(args.getContext());
	
	gl.BufferData(target, len, pdata, usage);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::bufferSubData(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "bufferSubData");
	GLint target = args[0].GetInt();
	assert(0);
	GLint ret = gl.CheckFramebufferStatus(target);
	int err = gl.GetError();
	assert(!err);
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}

bool HtmlWebGLRenderingContext::checkFramebufferStatus(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "checkFramebufferStatus");
	GLint target;
	target = args[0].GetInt();
	assert(0);
	GLint ret = gl.CheckFramebufferStatus(target);
	int err = gl.GetError();
	assert(!err);
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}

bool HtmlWebGLRenderingContext::clear(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "clear");
	GLint mask;
	mask = args[0].GetInt();
//	AutoFbo af(m_pCanvas, false);
	gl.Clear(mask);

	GLCHECKERR;

	return true;
}

bool HtmlWebGLRenderingContext::clearColor(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "clearColor");
	GLfloat red, green, blue, alpha;
	red = args[0].GetNumber();
	green = args[1].GetNumber();
	blue = args[2].GetNumber();
	alpha = args[3].GetNumber();
//	AutoFbo af(m_pCanvas, false);
	gl.ClearColor(red, green, blue, alpha);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::clearDepth(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "clearDepth");
	GLfloat depth;
	depth = args[0].GetNumber();
	assert(0);
//	AutoFbo af(m_pCanvas, false);
	gl.ClearDepthf(depth);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::clearStencil(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "clearStencil");
	GLint s;
	s = args[0].GetInt();
	assert(0);
//	AutoFbo af(m_pCanvas, false);
	gl.ClearStencil(s);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::colorMask(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "colorMask");
	bool red, green, blue, alpha;
	red = args[0].GetBoolean();
	green = args[1].GetBoolean();
	blue = args[2].GetBoolean();
	alpha = args[3].GetBoolean();
//	AutoFbo af(m_pCanvas, false);
	gl.ColorMask(red, green, blue, alpha);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::compileShader(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "compileShader");
	GLuint shader;
	shader = args[0].GetInt();

	gl.CompileShader(shader);
	GLCHECKERR;

	return true;

}

bool HtmlWebGLRenderingContext::compressedTexImage2D(IFunctionCallbackArgs&args)
{
	assert(0);

	GLint target, level, internalformat, width, height, border;
	LocalObject data(args.getContext());
	target = args[0].GetInt();
	level = args[1].GetInt();
	internalformat = args[2].GetInt();
	width = args[3].GetInt();
	height = args[4].GetInt();
	border = args[5].GetInt();
	data = args[6].GetObject();
	uint32_t len = 0;
	void*pdata = 0;

	if (!data.IsArray(args.getContext()))
	{
		ReportError(args.getContext(), "compressedTexImage2D: input argument is NOT array!");
		return true;
	}
	pdata = data.GetArrayData(args.getContext());
	len = data.GetArrayByteLength(args.getContext());


	gl.CompressedTexImage2D(target, level, internalformat, width, height, border, len, pdata);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::compressedTexSubImage2D(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(8, "compressedTexSubImage2D");
	assert(0);

	GLint target, level, xoffset, yoffset, width, height, format;

	target = args[0].GetInt();
	level = args[1].GetInt();
	xoffset = args[2].GetInt();
	yoffset = args[3].GetInt();
	width = args[4].GetInt();
	height = args[5].GetInt();
	format = args[6].GetInt();
	LocalObject data(args.getContext(), args[7].GetObject());
	uint32_t len = 0;
	void*pdata = 0;
	if (!data.IsArray(args.getContext()))
	{
		ReportError(args.getContext(), "compressedTexSubImage2D: input argument is NOT array!");
		return true;
	}
	pdata = data.GetArrayData(args.getContext());
	len = data.GetArrayByteLength(args.getContext());



	gl.CompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, len, pdata);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::copyTexImage2D(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(8, "copyTexImage2D");
	GLint target, level, internalformat, border, x, y, width, height;
	target = args[0].GetInt();
	level = args[1].GetInt();
	internalformat = args[2].GetInt();
	x = args[3].GetInt();
	y = args[4].GetInt();
	width = args[5].GetInt();
	height = args[6].GetInt();
	border = args[7].GetInt();
	assert(0);
	gl.CopyTexImage2D(target, level, internalformat, x, y, width, height, border);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::copyTexSubImage2D(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(8, "copyTexSubImage2D");
	GLint target, level, xoffset, yoffset, x, y, width, height;
	target = args[0].GetInt();
	level = args[1].GetInt();
	xoffset = args[2].GetInt();
	yoffset = args[3].GetInt();
	x = args[4].GetInt();
	y = args[5].GetInt();
	width = args[6].GetInt();
	height = args[7].GetInt();
	assert(0);
	gl.CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::createBuffer(IFunctionCallbackArgs&args)
{
	GLuint ret;
	gl.GenBuffers(1, &ret);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(ret));

	return true;
}

bool HtmlWebGLRenderingContext::createFramebuffer(IFunctionCallbackArgs&args)
{
	assert(0);

	GLuint ret;
	gl.GenFramebuffers(1, &ret);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(ret));

	return true;
}

bool HtmlWebGLRenderingContext::createProgram(IFunctionCallbackArgs&args)
{
	GLuint ret = gl.CreateProgram();
	
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}
bool HtmlWebGLRenderingContext::createRenderbuffer(IFunctionCallbackArgs&args)
{
	GLuint buf = 0;

	gl.GenRenderbuffers(1, &buf);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(buf));
	return true;
}

bool HtmlWebGLRenderingContext::createShader(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "createShader");
	int typ;
	typ = args[0].GetInt();

	int ret = gl.CreateShader(typ);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}

bool HtmlWebGLRenderingContext::createTexture(IFunctionCallbackArgs&args)
{
	GLuint tex = 0;
	gl.GenTextures(1, &tex);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(tex));
	return true;
}

bool HtmlWebGLRenderingContext::cullFace(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "cullFace");
	GLint mode;
	mode = args[0].GetInt();

	gl.CullFace(mode);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::deleteBuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "deleteBuffer");
	GLuint buffer;
	buffer = args[0].GetInt();

	gl.DeleteBuffers(1, &buffer);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::deleteFramebuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "deleteFramebuffer");
	GLuint framebuffer;
	framebuffer = args[0].GetInt();

	gl.DeleteFramebuffers(1, &framebuffer);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::deleteRenderbuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "deleteRenderbuffer");
	GLuint renderbuffer;
	renderbuffer = args[0].GetInt();

	gl.DeleteRenderbuffers(1, &renderbuffer);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::deleteShader(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "deleteShader");
	GLuint shader;
	shader = args[0].GetInt();

	gl.DeleteShader(shader);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::depthFunc(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "depthFunc");
	long func;
	func = args[0].GetInt();


	gl.DepthFunc(func);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::depthMask(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "depthMask");
	bool flag;
	flag = args[0].GetBoolean();


	gl.DepthMask(flag);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::depthRange(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "depthRange");
	float zNear, zFar;
	zNear = args[0].GetNumber();
	zFar = args[1].GetNumber();

	
	gl.DepthRangef(zNear, zFar);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::detachShader(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "detachShader");
	GLuint program, shader;
	program = args[0].GetInt();
	shader = args[1].GetInt();


	gl.DetachShader(program, shader);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::disable(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "disable");
	long cap;
	cap = args[0].GetInt();

	gl.Disable(cap);
	
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::disableVertexAttribArray(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "disableVertexAttribArray");
	GLuint index;
	index = args[0].GetInt();


	gl.DisableVertexAttribArray(index);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::drawArrays(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "drawArrays");
	long mode;
	int first, count;
	mode = args[0].GetInt();
	first = args[1].GetInt();
	count = args[2].GetInt();

	//	PrintDescribeScriptedCaller(cx, 0);
//	AutoFbo af(m_pCanvas,false);

	gl.DrawArrays(mode, first, count);
	GLCHECKERR;

	return true;
}

bool HtmlWebGLRenderingContext::drawArraysInstancedANGLE(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlWebGLRenderingContext::drawElements(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "drawElements");
	long mode, count, type;
	GLintptr offset;
	mode = args[0].GetInt();
	count = args[1].GetInt();
	type = args[2].GetInt();
	offset = args[3].GetInt();

	
	assert(offset == 0);
//	AutoFbo af(m_pCanvas, false);

	gl.DrawElements(mode, count, type, (void*)offset);
	GLCHECKERR;

	return true;
}

bool HtmlWebGLRenderingContext::drawElementsInstancedANGLE(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlWebGLRenderingContext::enable(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "enable");
	long cap;
	cap = args[0].GetInt();


	gl.Enable(cap);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::enableVertexAttribArray(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "enableVertexAttribArray");
	GLuint index;
	index = args[0].GetInt();


	gl.EnableVertexAttribArray(index);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::framebufferRenderbuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "framebufferRenderbuffer");
	long target, attachment, renderbuffertarget;
	GLuint renderbuffer;
	target = args[0].GetInt();
	attachment = args[1].GetInt();
	renderbuffertarget = args[2].GetInt();
	renderbuffer = args[3].GetInt();
	assert(0);
	gl.FramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::framebufferTexture2D(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(5, "framebufferTexture2D");
	long target;
	long attachment;
	long textarget;
	GLuint texture;
	int level;
	target = args[0].GetInt();
	attachment = args[1].GetInt();
	textarget = args[2].GetInt();
	texture = args[3].GetInt();
	level = args[4].GetInt();
	assert(0);
	gl.FramebufferTexture2D(target, attachment, textarget, texture, level);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::frontFace(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "frontFace");
	long mode;
	mode = args[0].GetInt();
	assert(0);
	gl.FrontFace(mode);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::generateMipmap(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "generateMipmap");
	long target;
	target = args[0].GetInt();
	assert(0);
	gl.GenerateMipmap(target);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::getActiveAttrib(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getActiveAttrib");
	GLuint program;
	GLuint index;
	program = args[0].GetInt();
	index = args[1].GetInt();

	assert(0);
	char buf[8001];
	GLsizei len = sizeof(buf) - 1;
	GLint siz;
	GLuint typ;
	gl.GetActiveAttrib(program, index, len, &len, &siz, &typ, buf);
	buf[len] = 0;
	LocalObject ret(args.getContext());
	ret.CreateJsObject(args.getContext());
	ret.SetProperty(args.getContext(), "name", LocalValue(args.getContext()).SetString(args.getContext(), buf));
	ret.SetProperty(args.getContext(), "size", ValueBase().SetInt(siz));
	ret.SetProperty(args.getContext(), "type", ValueBase().SetInt(typ));

	args.setReturnValue(ret);
	return true;
}

bool HtmlWebGLRenderingContext::getActiveUniform(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getActiveUniform");
	GLuint program;
	GLuint index;
	program = args[0].GetInt();
	index = args[1].GetInt();

	assert(0);
	char buf[8001];
	GLsizei len = sizeof(buf) - 1;
	GLint siz;
	GLuint typ;
	gl.GetActiveUniform(program, index, len, &len, &siz, &typ, buf);
	GLCHECKERR;
	buf[len] = 0;

	LocalObject ret(args.getContext());
	ret.CreateJsObject(args.getContext());
	ret.SetProperty(args.getContext(), "name", LocalValue(args.getContext()).SetString(args.getContext(), buf));
	ret.SetProperty(args.getContext(), "size", ValueBase().SetInt(siz));
	ret.SetProperty(args.getContext(), "type", ValueBase().SetInt(typ));
	args.setReturnValue(ret);

	return true;
}

bool HtmlWebGLRenderingContext::getAttachedShaders(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "getAttachedShaders");
	GLuint program;
	program = args[0].GetInt();

	assert(0);
	GLuint buf[1000];
	GLsizei count = sizeof(buf);
	gl.GetAttachedShaders(program, sizeof(buf), &count, buf);
	GLCHECKERR;
	LocalObject pret(args.getContext());
	pret.CreatePrimitiveArray(args.getContext(), count, LocalObject::AT_UINT32, buf);
	args.setReturnValue(pret);

	return true;
}

bool HtmlWebGLRenderingContext::getAttribLocation(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getAttribLocation");
	GLuint program;
	std::string name;
	program = args[0].GetInt();
	name = args[1].GetString(args.getContext());


	int ret = gl.GetAttribLocation(program, name.c_str());
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}

bool HtmlWebGLRenderingContext::getBufferParameter(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlWebGLRenderingContext::getContextAttributes(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlWebGLRenderingContext::getError(IFunctionCallbackArgs&args)
{
	assert(0);
	long ret = glGetError();
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}

bool HtmlWebGLRenderingContext::getExtension(IFunctionCallbackArgs&args)
{
	assert(0);
	args.setReturnValue(ValueBase().SetBoolean(true));
	return true;
}

bool HtmlWebGLRenderingContext::getFramebufferAttachmentParameter(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "getFramebufferAttachmentParameter");
	long target;
	long attachment;
	long pname;
	target = args[0].GetInt();
	attachment = args[1].GetInt();
	pname = args[2].GetInt();
	assert(0);
	int ret;
	gl.GetFramebufferAttachmentParameteriv(target, attachment, pname, &ret);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}

ValueBase HtmlWebGLRenderingContext::glGet(const HandleContext&context, GLenum pname, _GLGETTYPE paraType, int nPara)//paraType:0:GLint,1:GLFloat
{
	bool ok;
	switch (paraType)
	{
	case GT_INT:
	{
		GLint *val = new GLint[nPara];
		gl.GetIntegerv(pname, val);
		if (nPara == 1)
		{
			ValueBase ret;
			ret.SetInt(val[0]);

			delete[]val;
			return ret;
		}
		else
		{
			LocalObject objarray(context);
			objarray.CreatePrimitiveArray(context, nPara, LocalObject::AT_INT32, val);
		//	JSObject*objarray = JS_NewArrayObject(cx, nPara, NULL);
// 			for (int i = 0; i < nPara; i++)
// 			{
// 				ok = JS_SetElement(cx, objarray, i, &JS::RootedValue(cx, INT_TO_JSVAL(val[i])));
// 			}
			delete[]val;
			return objarray;
//			return OBJECT_TO_JSVAL(objarray);
		}
		break;
	}
	case GT_FLOAT:
	{
		GLfloat *val = new GLfloat[nPara];
		gl.GetFloatv(pname, val);
		if (nPara == 1)
		{
			ValueBase ret;
			ret.SetNumber(val[0]);
			//jsval ret = DOUBLE_TO_JSVAL(val[0]);
			delete[]val;
			return ret;
		}
		else
		{
			LocalObject objarray(context);
			objarray.CreatePrimitiveArray(context, nPara, LocalObject::AT_FLOAT32, val);
			//JSObject*objarray = JS_NewArrayObject(cx, nPara, NULL);
// 			for (int i = 0; i < nPara; i++)
// 			{
// 				ok = JS_SetElement(cx, objarray, i, &JS::RootedValue(cx, DOUBLE_TO_JSVAL(val[i])));
// 			}
			delete[]val;
			return objarray;
//			return OBJECT_TO_JSVAL(objarray);
		}
		break;
	}
	case GT_BOOLEAN:
	{
		GLboolean *val = new GLboolean[nPara];
		gl.GetBooleanv(pname, val);
		if (nPara == 1)
		{
			ValueBase ret;
			ret.SetBoolean(val[0]);
//			jsval ret = BOOLEAN_TO_JSVAL(val[0]);
			delete[]val;
			return ret;
		}
		else
		{
			LocalObject objarray(context);
			assert(sizeof(GLboolean) == 1);
			objarray.CreatePrimitiveArray(context, nPara, LocalObject::AT_BOOLEAN8, val);
//			JSObject*objarray = JS_NewArrayObject(cx, nPara, NULL);
// 			for (int i = 0; i < nPara; i++)
// 			{
// 				ok = JS_SetElement(cx, objarray, i, &JS::RootedValue(cx, BOOLEAN_TO_JSVAL(val[i])));
// 			}
			delete[]val;
			return objarray;
//			return OBJECT_TO_JSVAL(objarray);
		}
		break;
	}
	case GT_STRING:
	{
		return ValueBase().SetString(context, (const char*)gl.GetString(pname));
//		return c_string_to_jsval(cx, (const char*)glGetString(pname));

	}
	}
	return ValueBase();
}

bool HtmlWebGLRenderingContext::getParameter(IFunctionCallbackArgs&args)
{//https://msdn.microsoft.com/zh-cn/library/dn302414(v=vs.85).aspx
	CHECK_ARGS_COUNT(1, "getParameter");
	GLint pname;
	pname = args[0].GetInt();

#define GLGET(ID,TY,N) case ID:args.setReturnValue(glGet(args.getContext(), ID, TY, N)); break;
	switch (pname)
	{
		GLGET(GL_ACTIVE_TEXTURE, GT_INT, 1);
		GLGET(GL_ALIASED_LINE_WIDTH_RANGE, GT_FLOAT, 2);
		GLGET(GL_ALIASED_POINT_SIZE_RANGE, GT_FLOAT, 2);
		GLGET(GL_ALPHA_BITS, GT_INT, 1);
		GLGET(GL_BLEND, GT_BOOLEAN, 1);
		GLGET(GL_BLEND_COLOR, GT_FLOAT, 4);
		GLGET(GL_BLEND_DST_ALPHA, GT_INT, 1);
		GLGET(GL_BLEND_DST_RGB, GT_INT, 1);
		GLGET(GL_BLEND_EQUATION_ALPHA, GT_INT, 1);
		GLGET(GL_BLEND_EQUATION_RGB, GT_INT, 1);
		GLGET(GL_BLEND_SRC_ALPHA, GT_INT, 1);
		GLGET(GL_BLUE_BITS, GT_INT, 1);
		GLGET(GL_COLOR_CLEAR_VALUE, GT_FLOAT, 4);
		GLGET(GL_COLOR_WRITEMASK, GT_BOOLEAN, 4);
		GLGET(GL_COMPRESSED_TEXTURE_FORMATS, GT_INT, 10);
		GLGET(GL_CULL_FACE, GT_BOOLEAN, 1);
		GLGET(GL_CULL_FACE_MODE, GT_INT, 1);
		GLGET(GL_CURRENT_PROGRAM, GT_INT, 1);
		GLGET(GL_DEPTH_BITS, GT_INT, 1);
		GLGET(GL_DEPTH_CLEAR_VALUE, GT_INT, 1);
		GLGET(GL_DEPTH_FUNC, GT_INT, 1);
		GLGET(GL_DEPTH_RANGE, GT_FLOAT, 2);
		GLGET(GL_DEPTH_TEST, GT_BOOLEAN, 1);
		GLGET(GL_DEPTH_WRITEMASK, GT_BOOLEAN, 1);
		GLGET(GL_DITHER, GT_BOOLEAN, 1);
		GLGET(GL_ELEMENT_ARRAY_BUFFER_BINDING, GT_INT, 1);
		GLGET(GL_FRAMEBUFFER_BINDING, GT_INT, 1);
		GLGET(GL_FRONT_FACE, GT_INT, 1);
		GLGET(GL_GENERATE_MIPMAP_HINT, GT_INT, 1);
		GLGET(GL_GREEN_BITS, GT_INT, 1);
		GLGET(GL_LINE_WIDTH, GT_INT, 1);
//		GLGET(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, GT_INT, 1);
		GLGET(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, GT_INT, 1);
		GLGET(GL_MAX_CUBE_MAP_TEXTURE_SIZE, GT_INT, 1);
		GLGET(GL_MAX_FRAGMENT_UNIFORM_VECTORS, GT_INT, 1);
		GLGET(GL_MAX_RENDERBUFFER_SIZE, GT_INT, 1);
		GLGET(GL_MAX_TEXTURE_IMAGE_UNITS, GT_INT, 1);
		GLGET(GL_MAX_TEXTURE_SIZE, GT_INT, 1);
		GLGET(GL_MAX_VARYING_VECTORS, GT_INT, 1);
		GLGET(GL_MAX_VERTEX_ATTRIBS, GT_INT, 1);
		GLGET(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, GT_INT, 1);
		GLGET(GL_MAX_VERTEX_UNIFORM_VECTORS, GT_INT, 1);
		GLGET(GL_MAX_VIEWPORT_DIMS, GT_INT, 2);
		GLGET(GL_PACK_ALIGNMENT, GT_INT, 1);
		GLGET(GL_POLYGON_OFFSET_FACTOR, GT_INT, 1);
		GLGET(GL_POLYGON_OFFSET_FILL, GT_BOOLEAN, 1);
		GLGET(GL_POLYGON_OFFSET_UNITS, GT_INT, 1);
		GLGET(GL_RED_BITS, GT_INT, 1);
		GLGET(GL_RENDERBUFFER_BINDING, GT_INT, 1);
		GLGET(GL_RENDERER, GT_STRING, 1);
		GLGET(GL_SAMPLE_BUFFERS, GT_INT, 1);
		GLGET(GL_SAMPLE_COVERAGE_INVERT, GT_BOOLEAN, 1);
		GLGET(GL_SAMPLE_COVERAGE_VALUE, GT_INT, 1);
		GLGET(GL_SAMPLES, GT_INT, 1);
		GLGET(GL_SCISSOR_BOX, GT_INT, 4);
		GLGET(GL_SCISSOR_TEST, GT_BOOLEAN, 1);
		GLGET(GL_SHADING_LANGUAGE_VERSION, GT_STRING, 1);
		GLGET(GL_STENCIL_BACK_FAIL, GT_INT, 1);
		GLGET(GL_STENCIL_BACK_FUNC, GT_INT, 1);
		GLGET(GL_STENCIL_BACK_PASS_DEPTH_FAIL, GT_INT, 1);
		GLGET(GL_STENCIL_BACK_PASS_DEPTH_PASS, GT_INT, 1);
		GLGET(GL_STENCIL_BACK_REF, GT_INT, 1);
		GLGET(GL_STENCIL_BACK_VALUE_MASK, GT_INT, 1);
		GLGET(GL_STENCIL_BACK_WRITEMASK, GT_INT, 1);
		GLGET(GL_STENCIL_BITS, GT_INT, 1);
		GLGET(GL_STENCIL_CLEAR_VALUE, GT_INT, 1);
		GLGET(GL_STENCIL_FAIL, GT_INT, 1);
		GLGET(GL_STENCIL_FUNC, GT_INT, 1);
		GLGET(GL_STENCIL_PASS_DEPTH_FAIL, GT_INT, 1);
		GLGET(GL_STENCIL_PASS_DEPTH_PASS, GT_INT, 1);
		GLGET(GL_STENCIL_REF, GT_INT, 1);
		GLGET(GL_STENCIL_TEST, GT_BOOLEAN, 1);
		GLGET(GL_STENCIL_VALUE_MASK, GT_INT, 1);
		GLGET(GL_STENCIL_WRITEMASK, GT_INT, 1);
		GLGET(GL_SUBPIXEL_BITS, GT_INT, 1);
		GLGET(GL_TEXTURE_BINDING_2D, GT_INT, 1);
		GLGET(GL_TEXTURE_BINDING_CUBE_MAP, GT_INT, 1);
		GLGET(GL_UNPACK_ALIGNMENT, GT_INT, 1);
// 		GLGET(GL_UNPACK_COLORSPACE_CONVERSION_WEBGL, GT_INT, 1);
// 		GLGET(GL_UNPACK_FLIP_Y_WEBGL, GT_BOOLEAN, 1);
// 		GLGET(GL_UNPACK_PREMULTIPLY_ALPHA_WEBGL, GT_BOOLEAN, 1);
		GLGET(GL_VENDOR, GT_STRING, 1);
		GLGET(GL_VERSION, GT_STRING, 1);
		GLGET(GL_VIEWPORT, GT_INT, 4);
	


	default:

		assert(0);
		ReportError(args.getContext(), "getParameter");
//		PrintDescribeScriptedCaller(cx, pJsObj);
		break;

	}

	return true;
}

bool HtmlWebGLRenderingContext::getProgramInfoLog(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "getProgramInfoLog");
	GLuint program;
	program = args[0].GetInt();
	assert(0);
	char str[8001];
	GLsizei len = sizeof(str) - 1;
	gl.GetProgramInfoLog(program, len, &len, str);
	GLCHECKERR;
	str[len] = 0;
	args.setReturnValue(ValueBase().SetString(args.getContext(), str));

	return true;
}

bool HtmlWebGLRenderingContext::getProgramParameter(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getProgramParameter");
	GLuint program;
	program = args[0].GetInt();
	GLint pname;
	pname = args[1].GetInt();
	GLint para = 0;
	gl.GetProgramiv(program, pname, &para);
	args.setReturnValue(ValueBase().SetInt(para));

	return true;
}

bool HtmlWebGLRenderingContext::getRenderbufferParameter(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlWebGLRenderingContext::getShaderInfoLog(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "getShaderInfoLog");
	GLuint shader;
	shader = args[0].GetInt();

	char str[8001];
	GLsizei len = sizeof(str) - 1;
	gl.GetShaderInfoLog(shader, len, &len, str);
	GLCHECKERR;
	str[len] = 0;
	args.setReturnValue(ValueBase().SetString(args.getContext(), str));

	return true;
}

bool HtmlWebGLRenderingContext::getShaderParameter(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getShaderParameter");
	GLuint shader;
	shader = args[0].GetInt();
	GLint pname;
	pname = args[1].GetInt();

	GLint para;
	gl.GetShaderiv(shader, pname, &para);//////////////////////////////////////////////////////////////////////////不确定
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(para));
	return true;
}

bool HtmlWebGLRenderingContext::getShaderPrecisionFormat(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getShaderPrecisionFormat");
	int shaderType = args[0].GetInt();
	int precisionType = args[1].GetInt();
	GLint range[2] = {0};
	GLint precision;
	gl.GetShaderPrecisionFormat(shaderType, precisionType, range, &precision);
	GLCHECKERR;
	LocalObject obj(args.getContext());
	obj.CreateJsObject();
	obj.SetProperty("rangeMax", ValueBase().SetInt(range[1]));
	obj.SetProperty("rangeMin", ValueBase().SetInt(range[0]));
	obj.SetProperty("precision", ValueBase().SetInt(precision));
	args.setReturnValue(obj);
	
	return true;
}

bool HtmlWebGLRenderingContext::getShaderSource(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "getShaderSource");
	GLuint shader;
	shader = args[0].GetInt();
	GLint len;
	gl.GetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &len);
	GLCHECKERR;
	char*str = new char[len + 1];
	glGetShaderSource(shader, len, NULL, str);

	GLCHECKERR;
	args.setReturnValue(ValueBase().SetString(args.getContext(), str));
	delete[]str;
	return true;
}

bool HtmlWebGLRenderingContext::getSupportedExtensions(IFunctionCallbackArgs&args)
{
	LocalObject obj(args.getContext());
	obj.CreateArrayObject(args.getContext(), 0);
	args.setReturnValue(obj);
	return true;
}

bool HtmlWebGLRenderingContext::getTexParameter(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getTexParameter");
	GLuint target;
	target = args[0].GetInt();
	GLuint pname;
	pname = args[1].GetInt();
	//////////////////////////////////////////////////////////////////////////类型不清楚
	assert(0);
	GLint pa = 0;
	gl.GetTexParameteriv(target, pname, &pa);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(pa));
	return true;
}

bool HtmlWebGLRenderingContext::getUniform(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getUniform");
	GLuint program;
	program = args[0].GetInt();
	GLint location;
	location = args[1].GetInt();
	//////////////////////////////////////////////////////////////////////////类型不清楚
	assert(0);
	GLint pa = 0;
	gl.GetUniformiv(program, location, &pa);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(pa));
	return true;
}

bool HtmlWebGLRenderingContext::getUniformLocation(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getUniformLocation");
	GLuint program;
	program = args[0].GetInt();
	std::string name;
	name = args[1].GetString(args.getContext());

	GLint ret = gl.GetUniformLocation(program, name.c_str());
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetInt(ret));
	return true;
}

bool HtmlWebGLRenderingContext::getVertexAttrib(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlWebGLRenderingContext::getVertexAttribOffset(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "getVertexAttribOffset");
	GLuint index;
	index = args[0].GetInt();
	GLint pname;
	pname = args[1].GetInt();
	assert(0);
	//////////////////////////////////////////////////////////////////////////没有对应的函数
	// 	GLboolean ret = glvertexattrib(buffer);
	// 	JS_SET_RVAL(cx, vp, BOOLEAN_TO_JSVAL(ret));
	return true;
}

bool HtmlWebGLRenderingContext::isBuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "isBuffer");
	GLuint buffer;
	buffer = args[0].GetInt();
	assert(0);
	GLboolean ret = gl.IsBuffer(buffer);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetBoolean(ret));
	return true;
}

bool HtmlWebGLRenderingContext::isContextLost(IFunctionCallbackArgs&args)
{
	
	assert(0);
	args.setReturnValue(ValueBase().SetBoolean(false)); //////////////////////////////////////////////////////////////////////////没有对应的OGL函数
	return true;
}

bool HtmlWebGLRenderingContext::isEnabled(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "isEnabled");
	GLint cap;
	cap = args[0].GetInt();

	GLboolean ret = gl.IsEnabled(cap);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetBoolean(ret));
	return true;
}

bool HtmlWebGLRenderingContext::isFramebuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "isFramebuffer");
	GLuint framebuffer;
	framebuffer = args[0].GetInt();
	assert(0);
	GLboolean ret = gl.IsFramebuffer(framebuffer);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetBoolean(ret));
	return true;
}

bool HtmlWebGLRenderingContext::isProgram(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "isProgram"); 
	GLuint program;
	program = args[0].GetInt();
	assert(0);
	GLboolean ret = gl.IsProgram(program);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetBoolean(ret));
	return true;
}

bool HtmlWebGLRenderingContext::isRenderbuffer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "isRenderbuffer");
	GLuint renderbuffer;
	renderbuffer = args[0].GetInt();
	assert(0);
	GLboolean ret = gl.IsRenderbuffer(renderbuffer);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetBoolean(ret));
	return true;
}

bool HtmlWebGLRenderingContext::isShader(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "isShader");
	GLuint shader;
	shader = args[0].GetInt();
	assert(0);
	GLboolean ret = gl.IsShader(shader);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetBoolean(ret));
	return true;
}

bool HtmlWebGLRenderingContext::isTexture(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "isTexture");
	GLuint texture;
	texture = args[0].GetInt();
	assert(0);
	GLboolean ret = gl.IsTexture(texture);
	GLCHECKERR;
	args.setReturnValue(ValueBase().SetBoolean(ret));
	return true;
}

bool HtmlWebGLRenderingContext::lineWidth(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "lineWidth");
	GLfloat width;
	width = args[0].GetInt();
	assert(0);
	gl.LineWidth(width);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::linkProgram(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "linkProgram");
	GLuint program;
	program = args[0].GetInt();
	gl.LinkProgram(program);
	GLCHECKERR;

	return true;
}

bool HtmlWebGLRenderingContext::pixelStorei(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "pixelStorei");
	long pname;
	int param;
	pname = args[0].GetInt();
	param = args[1].GetInt();

	gl.PixelStorei(pname, param);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::polygonOffset(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "polygonOffset");
	float factor, units;
	factor = args[0].GetNumber();
	units = args[1].GetNumber();
	assert(0);
	gl.PolygonOffset(factor, units);
	GLCHECKERR;

	return true;
}

bool HtmlWebGLRenderingContext::readPixels(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(7, "readPixels");
	int x;
	int y;
	int width;
	int height;
	long format;
	long type;
	void*pixels;
	x = args[0].GetInt();
	y = args[1].GetInt();
	width = args[2].GetInt();
	height = args[3].GetInt();
	format = args[4].GetInt();
	type = args[5].GetInt();
	assert(0);
	LocalObject pix(args.getContext(), args[6].GetObject());
	if (pix.IsArray(args.getContext()))
	{
		pixels = pix.GetArrayData(args.getContext());
	}
	else
	{
		ReportError(args.getContext(), "readPixels: input argument is NOT array!");
		return true;
	}




	gl.ReadPixels(x, y, width, height, format, type, pixels);
	GLCHECKERR;

	return true;
}

bool HtmlWebGLRenderingContext::renderbufferStorage(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "renderbufferStorage");
	long target;
	long internalformat;
	int width;
	int height;
	target = args[0].GetInt();
	internalformat = args[1].GetInt();
	width = args[2].GetInt();
	height = args[3].GetInt();
	assert(0);
	gl.RenderbufferStorage(target, internalformat, width, height);
	GLCHECKERR;

	return true;
}

bool HtmlWebGLRenderingContext::sampleCoverage(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "sampleCoverage");
	float value;
	bool invert;
	value = args[0].GetNumber();
	invert = args[1].GetBoolean();
	assert(0);
	gl.SampleCoverage(value, invert);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::scissor(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "scissor");
	int x, y, width, height;
	x = args[0].GetInt();
	y = args[1].GetInt();
	width = args[2].GetInt();
	height = args[3].GetInt();

	gl.Scissor(x, y, width, height);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::shaderSource(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "shaderSource");
	GLuint shader;
	shader = args[0].GetInt();
	std::string source;
	source = args[1].GetString(args.getContext());
	
	const char*src = source.c_str();
	int len = source.length();
	gl.ShaderSource(shader, 1, &src, 0);
// 	char str[8001];
// 	GLsizei errlen = sizeof(str) - 1;
// 	glGetShaderInfoLog(shader, errlen, &errlen, str);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::stencilFunc(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "stencilFunc");
	gl.StencilFunc(args[0].GetInt(), args[1].GetInt(), args[2].GetInt());
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::stencilFuncSeparate(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "stencilFuncSeparate");
	gl.StencilFuncSeparate(args[0].GetInt(), args[1].GetInt(), args[2].GetInt(), args[3].GetInt());
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::stencilMask(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "stencilMask");
	int mask = args[0].GetInt();
	gl.StencilMask(mask);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::stencilMaskSeparate(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "stencilMaskSeparate");
	gl.StencilMaskSeparate(args[0].GetInt(), args[1].GetInt());
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::stencilOp(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "stencilOp");
	gl.StencilOp(args[0].GetInt(), args[1].GetInt(),args[2].GetInt());
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::stencilOpSeparate(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "stencilOpSeparate");
	gl.StencilOpSeparate(args[0].GetInt(), args[1].GetInt(), args[2].GetInt(),args[3].GetInt());
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::texImage2D(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT_MIN(6, "texImage2D");

	if (args.length() == 6)
	{
		long target;
		int level;
		long internalformat;
		long format;
		long type;

		int width = 0;
		int height = 0;
		target = args[0].GetInt();
		level = args[1].GetInt();
		internalformat = args[2].GetInt();
		format = args[3].GetInt();
		type = args[4].GetInt();
		LocalObject pix(args.getContext(), args[5].GetObject());

		uint32_t len = 0;
		uint8_t*pdata = 0;
		bool bNeedDel = false;
		if (pix.IsArray(args.getContext()))
		{
			pdata =(uint8_t*) pix.GetArrayData(args.getContext());
			len = pix.GetArrayByteLength(args.getContext());
		}
		else
		{
			do
			{
				CustomClassBase*pObj = FindCustomClass(pix);
				if (!pObj)break;
				CustomClassAutoThisObj cc(pObj, pix);
				HtmlImgClass*pImg = dynamic_cast<HtmlImgClass*>(pObj);
				if (pImg)
				{
				
					width = pImg->m_imgData.nWidth;
					height = pImg->m_imgData.nHeight;
					pdata =(uint8_t*) pImg->m_imgData.pData;
					break;
				}
				HtmlCanvas*pCanvas = dynamic_cast<HtmlCanvas*>(pObj);
				if (pCanvas)
				{
					width = pCanvas->m_fbo.m_width;
					height = pCanvas->m_fbo.m_height;

					//copy texture from pCanvas->m_fbo
// 					glBindFramebuffer(GL_FRAMEBUFFER, m_pCanvas->m_fbo.m_FrameBuffer);
// 					glCopyTexImage2D(target, level, internalformat, 0, 0, width, height, 0);
// 
// 					glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 					return true;

					pdata = new uint8_t[width*height*4];
					m_pCanvas->m_fbo.EndFBO();
					pCanvas->m_fbo.BeginFBO();
					pCanvas->m_fbo.GetImageData(0, 0, width, height, (uint8_t*)pdata);
					pCanvas->m_fbo.EndFBO();
					m_pCanvas->m_fbo.BeginFBO();
					bNeedDel = true;
					break;
				}
				HtmlImageData*pImgData = dynamic_cast<HtmlImageData*>(pObj);
				if (pImgData)
				{
					
					width = pImgData->GetWidth(args.getContext());
					height = pImgData->GetHeight(args.getContext());
					pdata =(uint8_t*) new uint32_t[width*height * 4];
					pImgData->GetData(args.getContext(), (uint8_t*)pdata);
					bNeedDel = true;
					break;
				}
				assert(0);
			} while (false);
		}
		assert(pdata);
		gl.TexImage2D(target, level, internalformat, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pdata);
		GLCHECKERR;
		if (bNeedDel)
		{
			delete []pdata;
		}
		return true;
	}
	else if (args.length() == 9)
	{
		long target;
		int level;
		long internalformat;
		int width;
		int height;
		int border;
		long format;
		long type;
		
		target = args[0].GetInt();
		level = args[1].GetInt();
		internalformat = args[2].GetInt();
		width = args[3].GetInt();
		height = args[4].GetInt();
		border = args[5].GetInt();
		format = args[6].GetInt();
		type = args[7].GetInt();
		LocalObject pixels(args.getContext(), args[8].GetObject());
		void*pdata = NULL;
		if (pixels.IsArray())
		{
			pdata = pixels.GetArrayData(args.getContext());
		}
		else
		{
			CustomClassBase*pObj = FindCustomClass(pixels);
			assert(0);
		}

		assert(pdata);
		gl.TexImage2D(target, level, internalformat, width, height, border, format, type, pdata);
		GLCHECKERR;
		return true;
	}
	assert(0);
	return true;
}

bool HtmlWebGLRenderingContext::texParameterf(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "texParameterf");
	long target;
	long pname;
	float param;
	target = args[0].GetInt();
	pname = args[1].GetInt();
	param = args[2].GetNumber();

	assert(0);
	gl.TexParameterf(target, pname, param);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::texParameteri(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "texParameteri");
	long target;
	long pname;
	int param;
	target = args[0].GetInt();
	pname = args[1].GetInt();
	param = args[2].GetInt();


	gl.TexParameteri(target, pname, param);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::texSubImage2D(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(8, "texSubImage2D");
	long target;
	int level, xoffset, yoffset, width, height;
	long format;
	
	target = args[0].GetInt();
	level = args[1].GetInt();
	xoffset = args[2].GetInt();
	yoffset = args[3].GetInt();
	width = args[4].GetInt();
	height = args[5].GetInt();
	LocalObject data(args.getContext(), args[6].GetObject());
	uint32_t len = 0;
	void*pdata = 0;
	if (data.IsArray())
	{
		pdata = data.GetArrayData(args.getContext());
		len = data.GetArrayByteLength(args.getContext());
	}
	else
	{
		assert(0);
	}

	assert(0);
	gl.TexSubImage2D(target, level, xoffset, yoffset, width, height, format, GL_UNSIGNED_BYTE, pdata);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform1f(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform1f");
	GLuint location;
	float x;

	location = args[0].GetInt();
	x = args[1].GetNumber();

	assert(0);
	gl.Uniform1f(location, x);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform1fv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform1fv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	
	float*pvalue = (float*)value.GetArrayData(args.getContext());
	assert(0);
	gl.Uniform1fv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform1i(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform1i");
	GLuint location;
	location = args[0].GetInt();
	GLint v;
	v = args[1].GetInt();
	gl.Uniform1i(location, v);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform1iv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform1iv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	int*pvalue = (int*)value.GetArrayData(args.getContext());
	assert(0);
	gl.Uniform1iv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform2f(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform2f");
	GLuint location;
	float x, y;

	location = args[0].GetInt();
	x = args[1].GetNumber();
	y = args[2].GetNumber();

	assert(0);
	gl.Uniform2f(location, x, y);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform2fv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform2fv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	float*pvalue = (float*)value.GetArrayData(args.getContext());
	assert(0);
	gl.Uniform2fv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform2i(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "uniform2i");
	GLuint location;
	int x, y;

	location = args[0].GetInt();
	x = args[1].GetInt();
	y = args[2].GetInt();

	assert(0);
	gl.Uniform2i(location, x, y);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform2iv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform2iv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	int*pvalue = (int*)value.GetArrayData(args.getContext());
	assert(0);
	gl.Uniform2iv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform3f(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "uniform3f");
	GLuint location;
	float x, y, z;

	location = args[0].GetInt();
	x = args[1].GetNumber();
	y = args[2].GetNumber();
	z = args[3].GetNumber();

	assert(0);
	gl.Uniform3f(location, x, y, z);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform3fv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform3fv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	float*pvalue = (float*)value.GetArrayData(args.getContext());
	assert(0);
	gl.Uniform3fv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform3i(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "uniform3i");
	GLuint location;
	int x, y, z;

	location = args[0].GetInt();
	x = args[1].GetInt();
	y = args[2].GetInt();
	z = args[3].GetInt();

	assert(0);
	gl.Uniform3i(location, x, y, z);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform3iv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform3iv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	int*pvalue = (int*)value.GetArrayData(args.getContext());
	assert(0);
	gl.Uniform3iv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform4f(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(5, "uniform4f");
	GLuint location;
	float x, y, z, w;

	location = args[0].GetInt();
	x = args[1].GetNumber();
	y = args[2].GetNumber();
	z = args[3].GetNumber();
	w = args[4].GetNumber();

	assert(0);
	gl.Uniform4f(location, x, y, z, w);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform4fv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform4fv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	float*pvalue = (float*)value.GetArrayData(args.getContext());

	gl.Uniform4fv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform4i(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(5, "uniform4i");
	GLuint location;
	int x, y, z, w;

	location = args[0].GetInt();
	x = args[1].GetInt();
	y = args[2].GetInt();
	z = args[3].GetInt();
	w = args[4].GetInt();

	assert(0);
	gl.Uniform4i(location, x, y, z, w);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniform4iv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "uniform4iv");
	GLuint location;

	location = args[0].GetInt();
	LocalObject value(args.getContext(), args[1].GetObject());
	int*pvalue = (int*)value.GetArrayData(args.getContext());
	assert(0);
	gl.Uniform4iv(location, 1, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniformMatrix3fv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "uniformMatrix3fv");
	GLuint location;
	bool transpose;

	location = args[0].GetInt();
	transpose = args[1].GetBoolean();
	LocalObject value(args.getContext(), args[2].GetObject());
	float*pvalue = (float*)value.GetArrayData(args.getContext());
	assert(0);
	gl.UniformMatrix3fv(location, 1, transpose, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::uniformMatrix4fv(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(3, "uniformMatrix4fv");
	GLuint location;
	bool transpose;

	location = args[0].GetInt();
	transpose = args[1].GetBoolean();
	LocalObject value(args.getContext(), args[2].GetObject());
	float*pvalue = (float*)value.GetArrayData(args.getContext());

	gl.UniformMatrix4fv(location, 1, transpose, pvalue);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::useProgram(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "useProgram");
	GLuint program;
	program = args[0].GetInt();
	gl.UseProgram(program);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::validateProgram(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "validateProgram");
	GLuint program;
	program = args[0].GetInt();
	assert(0);
	gl.ValidateProgram(program);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::vertexAttribPointer(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(6, "vertexAttribPointer");
	GLuint indx;
	int size;
	long type;
	bool normalized;
	int stride;
	GLintptr offset;
	indx = args[0].GetInt();
	size = args[1].GetInt();
	type = args[2].GetInt();
	normalized = args[3].GetBoolean();
	stride = args[4].GetInt();
	offset = args[5].GetInt();


	gl.VertexAttribPointer(indx, size, type, normalized, stride, (void*)offset);
	GLCHECKERR;
	return true;
}

bool HtmlWebGLRenderingContext::viewport(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(4, "viewport");
	int x, y, width, height;
	x = args[0].GetInt();
	y = args[1].GetInt();
	width = args[2].GetInt();
	height = args[3].GetInt();
	gl.Viewport(x, y, width, height);
//	glViewport(x, y, width, height);
	GLCHECKERR;
	return true;
}

void HtmlWebGLRenderingContext::AddConstants(const HandleContext&context)
{
	struct WGLCONSTANT
	{
		const char*name;
		int value;
	};
	WGLCONSTANT cons[] = {
		{ "ACTIVE_ATTRIBUTES", 35721 },
		{ "ACTIVE_TEXTURE", 34016 },
		{ "ACTIVE_UNIFORMS", 35718 },
		{ "ALIASED_LINE_WIDTH_RANGE", 33902 },
		{ "ALIASED_POINT_SIZE_RANGE", 33901 },
		{ "ALPHA", 6406 },
		{ "ALPHA_BITS", 3413 },
		{ "ALWAYS", 519 },
		{ "ARRAY_BUFFER", 34962 },
		{ "ARRAY_BUFFER_BINDING", 34964 },
		{ "ATTACHED_SHADERS", 35717 },
		{ "BACK", 1029 },
		{ "BLEND", 3042 },
		{ "BLEND_COLOR", 32773 },
		{ "BLEND_DST_ALPHA", 32970 },
		{ "BLEND_DST_RGB", 32968 },
		{ "BLEND_EQUATION", 32777 },
		{ "BLEND_EQUATION_ALPHA", 34877 },
		{ "BLEND_EQUATION_RGB", 32777 },
		{ "BLEND_SRC_ALPHA", 32971 },
		{ "BLEND_SRC_RGB", 32969 },
		{ "BLUE_BITS", 3412 },
		{ "BOOL", 35670 },
		{ "BOOL_VEC2", 35671 },
		{ "BOOL_VEC3", 35672 },
		{ "BOOL_VEC4", 35673 },
		{ "BROWSER_DEFAULT_WEBGL", 37444 },
		{ "BUFFER_SIZE", 34660 },
		{ "BUFFER_USAGE", 34661 },
		{ "BYTE", 5120 },
		{ "CCW", 2305 },
		{ "CLAMP_TO_EDGE", 33071 },
		{ "COLOR_ATTACHMENT0", 36064 },
		{ "COLOR_BUFFER_BIT", 16384 },
		{ "COLOR_CLEAR_VALUE", 3106 },
		{ "COLOR_WRITEMASK", 3107 },
		{ "COMPILE_STATUS", 35713 },
		{ "COMPRESSED_TEXTURE_FORMATS", 34467 },
		{ "CONSTANT_ALPHA", 32771 },
		{ "CONSTANT_COLOR", 32769 },
		{ "CONTEXT_LOST_WEBGL", 37442 },
		{ "CULL_FACE", 2884 },
		{ "CULL_FACE_MODE", 2885 },
		{ "CURRENT_PROGRAM", 35725 },
		{ "CURRENT_VERTEX_ATTRIB", 34342 },
		{ "CW", 2304 },
		{ "DECR", 7683 },
		{ "DECR_WRAP", 34056 },
		{ "DELETE_STATUS", 35712 },
		{ "DEPTH_ATTACHMENT", 36096 },
		{ "DEPTH_BITS", 3414 },
		{ "DEPTH_BUFFER_BIT", 256 },
		{ "DEPTH_CLEAR_VALUE", 2931 },
		{ "DEPTH_COMPONENT", 6402 },
		{ "DEPTH_COMPONENT16", 33189 },
		{ "DEPTH_FUNC", 2932 },
		{ "DEPTH_RANGE", 2928 },
		{ "DEPTH_STENCIL", 34041 },
		{ "DEPTH_STENCIL_ATTACHMENT", 33306 },
		{ "DEPTH_TEST", 2929 },
		{ "DEPTH_WRITEMASK", 2930 },
		{ "DITHER", 3024 },
		{ "DONT_CARE", 4352 },
		{ "drawingBufferHeight", 450 },
		{ "drawingBufferWidth", 800 },
		{ "DST_ALPHA", 772 },
		{ "DST_COLOR", 774 },
		{ "DYNAMIC_DRAW", 35048 },
		{ "ELEMENT_ARRAY_BUFFER", 34963 },
		{ "ELEMENT_ARRAY_BUFFER_BINDING", 34965 },
		{ "EQUAL", 514 },
		{ "FASTEST", 4353 },
		{ "FLOAT", 5126 },
		{ "FLOAT_MAT2", 35674 },
		{ "FLOAT_MAT3", 35675 },
		{ "FLOAT_MAT4", 35676 },
		{ "FLOAT_VEC2", 35664 },
		{ "FLOAT_VEC3", 35665 },
		{ "FLOAT_VEC4", 35666 },
		{ "FRAGMENT_SHADER", 35632 },
		{ "FRAMEBUFFER", 36160 },
		{ "FRAMEBUFFER_ATTACHMENT_OBJECT_NAME", 36049 },
		{ "FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE", 36048 },
		{ "FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE", 36051 },
		{ "FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL", 36050 },
		{ "FRAMEBUFFER_BINDING", 36006 },
		{ "FRAMEBUFFER_COMPLETE", 36053 },
		{ "FRAMEBUFFER_INCOMPLETE_ATTACHMENT", 36054 },
		{ "FRAMEBUFFER_INCOMPLETE_DIMENSIONS", 36057 },
		{ "FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT", 36055 },
		{ "FRAMEBUFFER_UNSUPPORTED", 36061 },
		{ "FRONT", 1028 },
		{ "FRONT_AND_BACK", 1032 },
		{ "FRONT_FACE", 2886 },
		{ "FUNC_ADD", 32774 },
		{ "FUNC_REVERSE_SUBTRACT", 32779 },
		{ "FUNC_SUBTRACT", 32778 },
		{ "GENERATE_MIPMAP_HINT", 33170 },
		{ "GEQUAL", 518 },
		{ "GREATER", 516 },
		{ "GREEN_BITS", 3411 },
		{ "HIGH_FLOAT", 36338 },
		{ "HIGH_INT", 36341 },
		{ "INCR", 7682 },
		{ "INCR_WRAP", 34055 },
		{ "INT", 5124 },
		{ "INT_VEC2", 35667 },
		{ "INT_VEC3", 35668 },
		{ "INT_VEC4", 35669 },
		{ "INVALID_ENUM", 1280 },
		{ "INVALID_FRAMEBUFFER_OPERATION", 1286 },
		{ "INVALID_OPERATION", 1282 },
		{ "INVALID_VALUE", 1281 },
		{ "INVERT", 5386 },
		{ "KEEP", 7680 },
		{ "LEQUAL", 515 },
		{ "LESS", 513 },
		{ "LINE_LOOP", 2 },
		{ "LINE_STRIP", 3 },
		{ "LINE_WIDTH", 2849 },
		{ "LINEAR", 9729 },
		{ "LINEAR_MIPMAP_LINEAR", 9987 },
		{ "LINEAR_MIPMAP_NEAREST", 9985 },
		{ "LINES", 1 },
		{ "LINK_STATUS", 35714 },
		{ "LOW_FLOAT", 36336 },
		{ "LOW_INT", 36339 },
		{ "LUMINANCE", 6409 },
		{ "LUMINANCE_ALPHA", 6410 },
		{ "MAX_COMBINED_TEXTURE_IMAGE_UNITS", 35661 },
		{ "MAX_CUBE_MAP_TEXTURE_SIZE", 34076 },
		{ "MAX_FRAGMENT_UNIFORM_VECTORS", 36349 },
		{ "MAX_RENDERBUFFER_SIZE", 34024 },
		{ "MAX_TEXTURE_IMAGE_UNITS", 34930 },
		{ "MAX_TEXTURE_SIZE", 3379 },
		{ "MAX_VARYING_VECTORS", 36348 },
		{ "MAX_VERTEX_ATTRIBS", 34921 },
		{ "MAX_VERTEX_TEXTURE_IMAGE_UNITS", 35660 },
		{ "MAX_VERTEX_UNIFORM_VECTORS", 36347 },
		{ "MAX_VIEWPORT_DIMS", 3386 },
		{ "MEDIUM_FLOAT", 36337 },
		{ "MEDIUM_INT", 36340 },
		{ "MIRRORED_REPEAT", 33648 },
		{ "NEAREST", 9728 },
		{ "NEAREST_MIPMAP_LINEAR", 9986 },
		{ "NEAREST_MIPMAP_NEAREST", 9984 },
		{ "NEVER", 512 },
		{ "NICEST", 4354 },
		{ "NO_ERROR", 0 },
		{ "NONE", 0 },
		{ "NOTEQUAL", 517 },
		{ "ONE", 1 },
		{ "ONE_MINUS_CONSTANT_ALPHA", 32772 },
		{ "ONE_MINUS_CONSTANT_COLOR", 32770 },
		{ "ONE_MINUS_DST_ALPHA", 773 },
		{ "ONE_MINUS_DST_COLOR", 775 },
		{ "ONE_MINUS_SRC_ALPHA", 771 },
		{ "ONE_MINUS_SRC_COLOR", 769 },
		{ "OUT_OF_MEMORY", 1285 },
		{ "PACK_ALIGNMENT", 3333 },
		{ "POINTS", 0 },
		{ "POLYGON_OFFSET_FACTOR", 32824 },
		{ "POLYGON_OFFSET_FILL", 32823 },
		{ "POLYGON_OFFSET_UNITS", 10752 },
		{ "RED_BITS", 3410 },
		{ "RENDERBUFFER", 36161 },
		{ "RENDERBUFFER_ALPHA_SIZE", 36179 },
		{ "RENDERBUFFER_BINDING", 36007 },
		{ "RENDERBUFFER_BLUE_SIZE", 36178 },
		{ "RENDERBUFFER_DEPTH_SIZE", 36180 },
		{ "RENDERBUFFER_GREEN_SIZE", 36177 },
		{ "RENDERBUFFER_HEIGHT", 36163 },
		{ "RENDERBUFFER_INTERNAL_FORMAT", 36164 },
		{ "RENDERBUFFER_RED_SIZE", 36176 },
		{ "RENDERBUFFER_STENCIL_SIZE", 36181 },
		{ "RENDERBUFFER_WIDTH", 36162 },
		{ "RENDERER", 7937 },
		{ "REPEAT", 10497 },
		{ "REPLACE", 7681 },
		{ "RGB", 6407 },
		{ "RGB5_A1", 32855 },
		{ "RGB565", 36194 },
		{ "RGBA", 6408 },
		{ "RGBA4", 32854 },
		{ "SAMPLE_ALPHA_TO_COVERAGE", 32926 },
		{ "SAMPLE_BUFFERS", 32936 },
		{ "SAMPLE_COVERAGE", 32928 },
		{ "SAMPLE_COVERAGE_INVERT", 32939 },
		{ "SAMPLE_COVERAGE_VALUE", 32938 },
		{ "SAMPLER_2D", 35678 },
		{ "SAMPLER_CUBE", 35680 },
		{ "SAMPLES", 32937 },
		{ "SCISSOR_BOX", 3088 },
		{ "SCISSOR_TEST", 3089 },
		{ "SHADER_TYPE", 35663 },
		{ "SHADING_LANGUAGE_VERSION", 35724 },
		{ "SHORT", 5122 },
		{ "SRC_ALPHA", 770 },
		{ "SRC_ALPHA_SATURATE", 776 },
		{ "SRC_COLOR", 768 },
		{ "STATIC_DRAW", 35044 },
		{ "STENCIL_ATTACHMENT", 36128 },
		{ "STENCIL_BACK_FAIL", 34817 },
		{ "STENCIL_BACK_FUNC", 34816 },
		{ "STENCIL_BACK_PASS_DEPTH_FAIL", 34818 },
		{ "STENCIL_BACK_PASS_DEPTH_PASS", 34819 },
		{ "STENCIL_BACK_REF", 36003 },
		{ "STENCIL_BACK_VALUE_MASK", 36004 },
		{ "STENCIL_BACK_WRITEMASK", 36005 },
		{ "STENCIL_BITS", 3415 },
		{ "STENCIL_BUFFER_BIT", 1024 },
		{ "STENCIL_CLEAR_VALUE", 2961 },
		{ "STENCIL_FAIL", 2964 },
		{ "STENCIL_FUNC", 2962 },
		{ "STENCIL_INDEX", 6401 },
		{ "STENCIL_INDEX8", 36168 },
		{ "STENCIL_PASS_DEPTH_FAIL", 2965 },
		{ "STENCIL_PASS_DEPTH_PASS", 2966 },
		{ "STENCIL_REF", 2967 },
		{ "STENCIL_TEST", 2960 },
		{ "STENCIL_VALUE_MASK", 2963 },
		{ "STENCIL_WRITEMASK", 2968 },
		{ "STREAM_DRAW", 35040 },
		{ "SUBPIXEL_BITS", 3408 },
		{ "TEXTURE", 5890 },
		{ "TEXTURE_2D", 3553 },
		{ "TEXTURE_BINDING_2D", 32873 },
		{ "TEXTURE_BINDING_CUBE_MAP", 34068 },
		{ "TEXTURE_CUBE_MAP", 34067 },
		{ "TEXTURE_CUBE_MAP_NEGATIVE_X", 34070 },
		{ "TEXTURE_CUBE_MAP_NEGATIVE_Y", 34072 },
		{ "TEXTURE_CUBE_MAP_NEGATIVE_Z", 34074 },
		{ "TEXTURE_CUBE_MAP_POSITIVE_X", 34069 },
		{ "TEXTURE_CUBE_MAP_POSITIVE_Y", 34071 },
		{ "TEXTURE_CUBE_MAP_POSITIVE_Z", 34073 },
		{ "TEXTURE_MAG_FILTER", 10240 },
		{ "TEXTURE_MIN_FILTER", 10241 },
		{ "TEXTURE_WRAP_S", 10242 },
		{ "TEXTURE_WRAP_T", 10243 },
		{ "TEXTURE0", 33984 },
		{ "TEXTURE1", 33985 },
		{ "TEXTURE2", 33986 },
		{ "TEXTURE3", 33987 },
		{ "TEXTURE4", 33988 },
		{ "TEXTURE5", 33989 },
		{ "TEXTURE6", 33990 },
		{ "TEXTURE7", 33991 },
		{ "TEXTURE8", 33992 },
		{ "TEXTURE9", 33993 },
		{ "TEXTURE10", 33994 },
		{ "TEXTURE11", 33995 },
		{ "TEXTURE12", 33996 },
		{ "TEXTURE13", 33997 },
		{ "TEXTURE14", 33998 },
		{ "TEXTURE15", 33999 },
		{ "TEXTURE16", 34000 },
		{ "TEXTURE17", 34001 },
		{ "TEXTURE18", 34002 },
		{ "TEXTURE19", 34003 },
		{ "TEXTURE20", 34004 },
		{ "TEXTURE21", 34005 },
		{ "TEXTURE22", 34006 },
		{ "TEXTURE23", 34007 },
		{ "TEXTURE24", 34008 },
		{ "TEXTURE25", 34009 },
		{ "TEXTURE26", 34010 },
		{ "TEXTURE27", 34011 },
		{ "TEXTURE28", 34012 },
		{ "TEXTURE29", 34013 },
		{ "TEXTURE30", 34014 },
		{ "TEXTURE31", 34015 },
		{ "TRIANGLE_FAN", 6 },
		{ "TRIANGLE_STRIP", 5 },
		{ "TRIANGLES", 4 },
		{ "UNPACK_ALIGNMENT", 3317 },
		{ "UNPACK_COLORSPACE_CONVERSION_WEBGL", 37443 },
		{ "UNPACK_FLIP_Y_WEBGL", 37440 },
		{ "UNPACK_PREMULTIPLY_ALPHA_WEBGL", 37441 },
		{ "UNSIGNED_BYTE", 5121 },
		{ "UNSIGNED_INT", 5125 },
		{ "UNSIGNED_SHORT", 5123 },
		{ "UNSIGNED_SHORT_4_4_4_4", 32819 },
		{ "UNSIGNED_SHORT_5_5_5_1", 32820 },
		{ "UNSIGNED_SHORT_5_6_5", 33635 },
		{ "VALIDATE_STATUS", 35715 },
		{ "VENDOR", 7936 },
		{ "VERSION", 7938 },
		{ "VERTEX_ATTRIB_ARRAY_BUFFER_BINDING", 34975 },
		{ "VERTEX_ATTRIB_ARRAY_ENABLED", 34338 },
		{ "VERTEX_ATTRIB_ARRAY_NORMALIZED", 34922 },
		{ "VERTEX_ATTRIB_ARRAY_POINTER", 34373 },
		{ "VERTEX_ATTRIB_ARRAY_SIZE", 34339 },
		{ "VERTEX_ATTRIB_ARRAY_STRIDE", 34340 },
		{ "VERTEX_ATTRIB_ARRAY_TYPE", 34341 },
		{ "VERTEX_SHADER", 35633 },
		{ "VIEWPORT", 2978 },
		{ "ZERO", 0 },


	};
	HandleObject proto(GetPrototype_s());
	for (int i = 0; i < sizeof(cons) / sizeof(WGLCONSTANT); i++)
	{
		proto.SetProperty(context,cons[i].name, ValueBase().SetInt(cons[i].value));
	}
}

bool HtmlWebGLRenderingContext::OnFunctionCall(IFunctionCallbackArgs&args, CUSTOMFUNCTION*pFunc)
{

	gl.SetState(&m_pCanvas->m_glState);

	//安卓模拟器似乎对FBO支持不好，会卡，真机不会
//	if (m_pCanvas->s_prevCanvas != m_pCanvas)
//	{
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
