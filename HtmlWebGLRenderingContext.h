#pragma once
#include "JSVMInterface.h"
#if defined(_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

class HtmlWebGLRenderingContext :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlWebGLRenderingContext, CustomClassBase)
		HtmlCanvas* m_pCanvas=nullptr;
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("activeTexture", &HtmlWebGLRenderingContext::activeTexture,1)
		DEFINE_CUSTOM_FUNCTION("attachShader", &HtmlWebGLRenderingContext::attachShader, 2)
		DEFINE_CUSTOM_FUNCTION("bindAttribLocation", &HtmlWebGLRenderingContext::bindAttribLocation, 3)
		DEFINE_CUSTOM_FUNCTION("bindBuffer", &HtmlWebGLRenderingContext::bindBuffer, 2)
		DEFINE_CUSTOM_FUNCTION("bindFramebuffer", &HtmlWebGLRenderingContext::bindFramebuffer, 2)
		DEFINE_CUSTOM_FUNCTION("bindRenderbuffer", &HtmlWebGLRenderingContext::bindRenderbuffer, 2)
		DEFINE_CUSTOM_FUNCTION("bindTexture", &HtmlWebGLRenderingContext::bindTexture, 2)
		DEFINE_CUSTOM_FUNCTION("blendColor", &HtmlWebGLRenderingContext::blendColor, 4)
		DEFINE_CUSTOM_FUNCTION("blendEquation", &HtmlWebGLRenderingContext::blendEquation, 1)
		DEFINE_CUSTOM_FUNCTION("blendEquationSeparate", &HtmlWebGLRenderingContext::blendEquationSeparate, 2)
		DEFINE_CUSTOM_FUNCTION("blendFunc", &HtmlWebGLRenderingContext::blendFunc, 2)
		DEFINE_CUSTOM_FUNCTION("blendFuncSeparate", &HtmlWebGLRenderingContext::blendFuncSeparate, 4)
		DEFINE_CUSTOM_FUNCTION("bufferData", &HtmlWebGLRenderingContext::bufferData, 3)
		DEFINE_CUSTOM_FUNCTION("bufferSubData", &HtmlWebGLRenderingContext::bufferSubData, 3)
		DEFINE_CUSTOM_FUNCTION("checkFramebufferStatus", &HtmlWebGLRenderingContext::checkFramebufferStatus, 2)
		DEFINE_CUSTOM_RENDER_FUNCTION("clear", &HtmlWebGLRenderingContext::clear, 1)
		DEFINE_CUSTOM_FUNCTION("clearColor", &HtmlWebGLRenderingContext::clearColor, 4)
		DEFINE_CUSTOM_FUNCTION("clearDepth", &HtmlWebGLRenderingContext::clearDepth, 1)
		DEFINE_CUSTOM_FUNCTION("clearStencil", &HtmlWebGLRenderingContext::clearStencil, 1)
		DEFINE_CUSTOM_FUNCTION("colorMask", &HtmlWebGLRenderingContext::colorMask, 4)
		DEFINE_CUSTOM_FUNCTION("compileShader", &HtmlWebGLRenderingContext::compileShader, 1)
		DEFINE_CUSTOM_FUNCTION("compressedTexImage2D", &HtmlWebGLRenderingContext::compressedTexImage2D, 7)
		DEFINE_CUSTOM_FUNCTION("compressedTexSubImage2D", &HtmlWebGLRenderingContext::compressedTexSubImage2D, 8)
		DEFINE_CUSTOM_RENDER_FUNCTION("copyTexImage2D", &HtmlWebGLRenderingContext::copyTexImage2D, 8)
		DEFINE_CUSTOM_RENDER_FUNCTION("copyTexSubImage2D", &HtmlWebGLRenderingContext::copyTexSubImage2D, 8)
		DEFINE_CUSTOM_FUNCTION("createBuffer", &HtmlWebGLRenderingContext::createBuffer, 0)
		DEFINE_CUSTOM_FUNCTION("createFramebuffer", &HtmlWebGLRenderingContext::createFramebuffer, 0)
		DEFINE_CUSTOM_FUNCTION("createProgram", &HtmlWebGLRenderingContext::createProgram, 0)
		DEFINE_CUSTOM_FUNCTION("createRenderbuffer", &HtmlWebGLRenderingContext::createRenderbuffer, 0)
		DEFINE_CUSTOM_FUNCTION("createShader", &HtmlWebGLRenderingContext::createShader, 1)
		DEFINE_CUSTOM_FUNCTION("createTexture", &HtmlWebGLRenderingContext::createTexture, 0)
		DEFINE_CUSTOM_FUNCTION("cullFace", &HtmlWebGLRenderingContext::cullFace, 1)
		DEFINE_CUSTOM_FUNCTION("deleteBuffer", &HtmlWebGLRenderingContext::deleteBuffer, 1)
		DEFINE_CUSTOM_FUNCTION("deleteFramebuffer", &HtmlWebGLRenderingContext::deleteFramebuffer, 1)
		DEFINE_CUSTOM_FUNCTION("deleteRenderbuffer", &HtmlWebGLRenderingContext::deleteRenderbuffer, 1)
		DEFINE_CUSTOM_FUNCTION("deleteShader", &HtmlWebGLRenderingContext::deleteShader, 1)
		DEFINE_CUSTOM_FUNCTION("depthFunc", &HtmlWebGLRenderingContext::depthFunc, 1)
		DEFINE_CUSTOM_FUNCTION("depthMask", &HtmlWebGLRenderingContext::depthMask, 1)
		DEFINE_CUSTOM_FUNCTION("depthRange", &HtmlWebGLRenderingContext::depthRange, 2)
		DEFINE_CUSTOM_FUNCTION("detachShader", &HtmlWebGLRenderingContext::detachShader, 2)
		DEFINE_CUSTOM_FUNCTION("disable", &HtmlWebGLRenderingContext::disable, 1)
		DEFINE_CUSTOM_FUNCTION("disableVertexAttribArray", &HtmlWebGLRenderingContext::disableVertexAttribArray, 1)
		DEFINE_CUSTOM_RENDER_FUNCTION("drawArrays", &HtmlWebGLRenderingContext::drawArrays, 3)
		DEFINE_CUSTOM_RENDER_FUNCTION("drawArraysInstancedANGLE", &HtmlWebGLRenderingContext::drawArraysInstancedANGLE, 4)
		DEFINE_CUSTOM_RENDER_FUNCTION("drawElements", &HtmlWebGLRenderingContext::drawElements, 4)

		DEFINE_CUSTOM_RENDER_FUNCTION("drawElementsInstancedANGLE", &HtmlWebGLRenderingContext::drawElementsInstancedANGLE, 1)
		DEFINE_CUSTOM_FUNCTION("enable", &HtmlWebGLRenderingContext::enable, 1)
		DEFINE_CUSTOM_FUNCTION("enableVertexAttribArray", &HtmlWebGLRenderingContext::enableVertexAttribArray, 1)
		DEFINE_CUSTOM_FUNCTION("framebufferRenderbuffer", &HtmlWebGLRenderingContext::framebufferRenderbuffer, 1)
		DEFINE_CUSTOM_FUNCTION("framebufferTexture2D", &HtmlWebGLRenderingContext::framebufferTexture2D, 1)
		DEFINE_CUSTOM_FUNCTION("frontFace", &HtmlWebGLRenderingContext::frontFace, 1)
		DEFINE_CUSTOM_FUNCTION("generateMipmap", &HtmlWebGLRenderingContext::generateMipmap, 1)
		DEFINE_CUSTOM_FUNCTION("getActiveAttrib", &HtmlWebGLRenderingContext::getActiveAttrib, 1)
		DEFINE_CUSTOM_FUNCTION("getActiveUniform", &HtmlWebGLRenderingContext::getActiveUniform, 1)
		DEFINE_CUSTOM_FUNCTION("getAttachedShaders", &HtmlWebGLRenderingContext::getAttachedShaders, 1)
		DEFINE_CUSTOM_FUNCTION("getAttribLocation", &HtmlWebGLRenderingContext::getAttribLocation, 1)
		DEFINE_CUSTOM_FUNCTION("getBufferParameter", &HtmlWebGLRenderingContext::getBufferParameter, 1)
		DEFINE_CUSTOM_FUNCTION("getContextAttributes", &HtmlWebGLRenderingContext::getContextAttributes, 1)
		DEFINE_CUSTOM_FUNCTION("getError", &HtmlWebGLRenderingContext::getError, 1)
		DEFINE_CUSTOM_FUNCTION("getExtension", &HtmlWebGLRenderingContext::getExtension, 1)
		DEFINE_CUSTOM_FUNCTION("getFramebufferAttachmentParameter", &HtmlWebGLRenderingContext::getFramebufferAttachmentParameter, 1)
		DEFINE_CUSTOM_FUNCTION("getParameter", &HtmlWebGLRenderingContext::getParameter, 1)
		DEFINE_CUSTOM_FUNCTION("getProgramInfoLog", &HtmlWebGLRenderingContext::getProgramInfoLog, 1)
		DEFINE_CUSTOM_FUNCTION("getProgramParameter", &HtmlWebGLRenderingContext::getProgramParameter, 1)
		DEFINE_CUSTOM_FUNCTION("getRenderbufferParameter", &HtmlWebGLRenderingContext::getRenderbufferParameter, 1)
		DEFINE_CUSTOM_FUNCTION("getShaderInfoLog", &HtmlWebGLRenderingContext::getShaderInfoLog, 1)
		DEFINE_CUSTOM_FUNCTION("getShaderParameter", &HtmlWebGLRenderingContext::getShaderParameter, 1)
		DEFINE_CUSTOM_FUNCTION("getShaderPrecisionFormat", &HtmlWebGLRenderingContext::getShaderPrecisionFormat, 1)
		DEFINE_CUSTOM_FUNCTION("getShaderSource", &HtmlWebGLRenderingContext::getShaderSource, 1)
		DEFINE_CUSTOM_FUNCTION("getSupportedExtensions", &HtmlWebGLRenderingContext::getSupportedExtensions, 1)
		DEFINE_CUSTOM_FUNCTION("getTexParameter", &HtmlWebGLRenderingContext::getTexParameter, 1)
		DEFINE_CUSTOM_FUNCTION("getUniform", &HtmlWebGLRenderingContext::getUniform, 1)
		DEFINE_CUSTOM_FUNCTION("getUniformLocation", &HtmlWebGLRenderingContext::getUniformLocation, 1)
		DEFINE_CUSTOM_FUNCTION("getVertexAttrib", &HtmlWebGLRenderingContext::getVertexAttrib, 1)
		DEFINE_CUSTOM_FUNCTION("getVertexAttribOffset", &HtmlWebGLRenderingContext::getVertexAttribOffset, 1)
		DEFINE_CUSTOM_FUNCTION("isBuffer", &HtmlWebGLRenderingContext::isBuffer, 1)
		DEFINE_CUSTOM_FUNCTION("isContextLost", &HtmlWebGLRenderingContext::isContextLost, 1)
		DEFINE_CUSTOM_FUNCTION("isEnabled", &HtmlWebGLRenderingContext::isEnabled, 1)
		DEFINE_CUSTOM_FUNCTION("isFramebuffer", &HtmlWebGLRenderingContext::isFramebuffer, 1)
		DEFINE_CUSTOM_FUNCTION("isProgram", &HtmlWebGLRenderingContext::isProgram, 1)
		DEFINE_CUSTOM_FUNCTION("isRenderbuffer", &HtmlWebGLRenderingContext::isRenderbuffer, 1)
		DEFINE_CUSTOM_FUNCTION("isShader", &HtmlWebGLRenderingContext::isShader, 1)
		DEFINE_CUSTOM_FUNCTION("isTexture", &HtmlWebGLRenderingContext::isTexture, 1)
		DEFINE_CUSTOM_FUNCTION("lineWidth", &HtmlWebGLRenderingContext::lineWidth, 1)
		DEFINE_CUSTOM_FUNCTION("linkProgram", &HtmlWebGLRenderingContext::linkProgram, 1)
		DEFINE_CUSTOM_FUNCTION("pixelStorei", &HtmlWebGLRenderingContext::pixelStorei, 1)
		DEFINE_CUSTOM_FUNCTION("polygonOffset", &HtmlWebGLRenderingContext::polygonOffset, 1)
		DEFINE_CUSTOM_RENDER_FUNCTION("readPixels", &HtmlWebGLRenderingContext::readPixels, 1)
		DEFINE_CUSTOM_FUNCTION("renderbufferStorage", &HtmlWebGLRenderingContext::renderbufferStorage, 1)
		DEFINE_CUSTOM_FUNCTION("sampleCoverage", &HtmlWebGLRenderingContext::sampleCoverage, 1)
		DEFINE_CUSTOM_FUNCTION("scissor", &HtmlWebGLRenderingContext::scissor, 1)
		DEFINE_CUSTOM_FUNCTION("shaderSource", &HtmlWebGLRenderingContext::shaderSource, 1)
		DEFINE_CUSTOM_FUNCTION("stencilFunc", &HtmlWebGLRenderingContext::stencilFunc, 1)
		DEFINE_CUSTOM_FUNCTION("stencilFuncSeparate", &HtmlWebGLRenderingContext::stencilFuncSeparate, 1)
		DEFINE_CUSTOM_FUNCTION("stencilMask", &HtmlWebGLRenderingContext::stencilMask, 1)
		DEFINE_CUSTOM_FUNCTION("stencilMaskSeparate", &HtmlWebGLRenderingContext::stencilMaskSeparate, 1)
		DEFINE_CUSTOM_FUNCTION("stencilOp", &HtmlWebGLRenderingContext::stencilOp, 1)
		DEFINE_CUSTOM_FUNCTION("stencilOpSeparate", &HtmlWebGLRenderingContext::stencilOpSeparate, 1)
		DEFINE_CUSTOM_FUNCTION("texImage2D", &HtmlWebGLRenderingContext::texImage2D, 1)
		DEFINE_CUSTOM_FUNCTION("texParameterf", &HtmlWebGLRenderingContext::texParameterf, 1)
		DEFINE_CUSTOM_FUNCTION("texParameteri", &HtmlWebGLRenderingContext::texParameteri, 1)
		DEFINE_CUSTOM_FUNCTION("texSubImage2D", &HtmlWebGLRenderingContext::texSubImage2D, 1)
		DEFINE_CUSTOM_FUNCTION("uniform1f", &HtmlWebGLRenderingContext::uniform1f, 1)
		DEFINE_CUSTOM_FUNCTION("uniform1fv", &HtmlWebGLRenderingContext::uniform1fv, 1)
		DEFINE_CUSTOM_FUNCTION("uniform1i", &HtmlWebGLRenderingContext::uniform1i, 1)
		DEFINE_CUSTOM_FUNCTION("uniform1iv", &HtmlWebGLRenderingContext::uniform1iv, 1)
		DEFINE_CUSTOM_FUNCTION("uniform2f", &HtmlWebGLRenderingContext::uniform2f, 1)
		DEFINE_CUSTOM_FUNCTION("uniform2fv", &HtmlWebGLRenderingContext::uniform2fv, 1)
		DEFINE_CUSTOM_FUNCTION("uniform2i", &HtmlWebGLRenderingContext::uniform2i, 1)
		DEFINE_CUSTOM_FUNCTION("uniform2iv", &HtmlWebGLRenderingContext::uniform2iv, 1)
		DEFINE_CUSTOM_FUNCTION("uniform3f", &HtmlWebGLRenderingContext::uniform3f, 1)
		DEFINE_CUSTOM_FUNCTION("uniform3fv", &HtmlWebGLRenderingContext::uniform3fv, 1)
		DEFINE_CUSTOM_FUNCTION("uniform3i", &HtmlWebGLRenderingContext::uniform3i, 1)
		DEFINE_CUSTOM_FUNCTION("uniform3iv", &HtmlWebGLRenderingContext::uniform3iv, 1)
		DEFINE_CUSTOM_FUNCTION("uniform4f", &HtmlWebGLRenderingContext::uniform4f, 1)
		DEFINE_CUSTOM_FUNCTION("uniform4fv", &HtmlWebGLRenderingContext::uniform4fv, 1)
		DEFINE_CUSTOM_FUNCTION("uniform4i", &HtmlWebGLRenderingContext::uniform4i, 1)
		DEFINE_CUSTOM_FUNCTION("uniform4iv", &HtmlWebGLRenderingContext::uniform4iv, 1)
		DEFINE_CUSTOM_FUNCTION("uniformMatrix3fv", &HtmlWebGLRenderingContext::uniformMatrix3fv, 1)
		DEFINE_CUSTOM_FUNCTION("uniformMatrix4fv", &HtmlWebGLRenderingContext::uniformMatrix4fv, 1)
		DEFINE_CUSTOM_FUNCTION("useProgram", &HtmlWebGLRenderingContext::useProgram, 1)
		DEFINE_CUSTOM_FUNCTION("validateProgram", &HtmlWebGLRenderingContext::validateProgram, 1)
		DEFINE_CUSTOM_FUNCTION("vertexAttribPointer", &HtmlWebGLRenderingContext::vertexAttribPointer, 1)
		DEFINE_CUSTOM_FUNCTION("viewport", &HtmlWebGLRenderingContext::viewport, 1)
		


	END_CUSTOM_FUNCTION

	BEGIN_CUSTOM_PROPERTY

	END_CUSTOM_PROPERTY



	HtmlWebGLRenderingContext();
	virtual ~HtmlWebGLRenderingContext();

	static bool InitClass(const HandleContext&context);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual void OnFinalize(FinalizeArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
	virtual bool OnFunctionCall(IFunctionCallbackArgs&args, CUSTOMFUNCTION*pFunc);

	//////////////////////////////////////////////////////////////////////////
	bool activeTexture(IFunctionCallbackArgs&args);
	bool attachShader(IFunctionCallbackArgs&args);
	bool bindAttribLocation(IFunctionCallbackArgs&args);
	bool bindBuffer(IFunctionCallbackArgs&args);
	bool bindFramebuffer(IFunctionCallbackArgs&args);
	bool bindRenderbuffer(IFunctionCallbackArgs&args);
	bool bindTexture(IFunctionCallbackArgs&args);
	bool blendColor(IFunctionCallbackArgs&args);
	bool blendEquation(IFunctionCallbackArgs&args);
	bool blendEquationSeparate(IFunctionCallbackArgs&args);
	bool blendFunc(IFunctionCallbackArgs&args);
	bool blendFuncSeparate(IFunctionCallbackArgs&args);
	bool bufferData(IFunctionCallbackArgs&args);
	bool bufferSubData(IFunctionCallbackArgs&args);
	bool checkFramebufferStatus(IFunctionCallbackArgs&args);
	bool clear(IFunctionCallbackArgs&args);
	bool clearColor(IFunctionCallbackArgs&args);
	bool clearDepth(IFunctionCallbackArgs&args);
	bool clearStencil(IFunctionCallbackArgs&args);
	bool colorMask(IFunctionCallbackArgs&args);
	bool compileShader(IFunctionCallbackArgs&args);
	bool compressedTexImage2D(IFunctionCallbackArgs&args);
	bool compressedTexSubImage2D(IFunctionCallbackArgs&args);
	bool copyTexImage2D(IFunctionCallbackArgs&args);
	bool copyTexSubImage2D(IFunctionCallbackArgs&args);
	bool createBuffer(IFunctionCallbackArgs&args);
	bool createFramebuffer(IFunctionCallbackArgs&args);
	bool createProgram(IFunctionCallbackArgs&args);
	bool createRenderbuffer(IFunctionCallbackArgs&args);
	bool createShader(IFunctionCallbackArgs&args);
	bool createTexture(IFunctionCallbackArgs&args);
	bool cullFace(IFunctionCallbackArgs&args);
	bool deleteBuffer(IFunctionCallbackArgs&args);
	bool deleteFramebuffer(IFunctionCallbackArgs&args);
	bool deleteRenderbuffer(IFunctionCallbackArgs&args);
	bool deleteShader(IFunctionCallbackArgs&args);
	bool depthFunc(IFunctionCallbackArgs&args);
	bool depthMask(IFunctionCallbackArgs&args);
	bool depthRange(IFunctionCallbackArgs&args);
	bool detachShader(IFunctionCallbackArgs&args);
	bool disable(IFunctionCallbackArgs&args);
	bool disableVertexAttribArray(IFunctionCallbackArgs&args);
	bool drawArrays(IFunctionCallbackArgs&args);
	bool drawArraysInstancedANGLE(IFunctionCallbackArgs&args);
	bool drawElements(IFunctionCallbackArgs&args);

	bool drawElementsInstancedANGLE(IFunctionCallbackArgs&args);
	bool enable(IFunctionCallbackArgs&args);
	bool enableVertexAttribArray(IFunctionCallbackArgs&args);
	bool framebufferRenderbuffer(IFunctionCallbackArgs&args);
	bool framebufferTexture2D(IFunctionCallbackArgs&args);
	bool frontFace(IFunctionCallbackArgs&args);
	bool generateMipmap(IFunctionCallbackArgs&args);
	bool getActiveAttrib(IFunctionCallbackArgs&args);
	bool getActiveUniform(IFunctionCallbackArgs&args);
	bool getAttachedShaders(IFunctionCallbackArgs&args);
	bool getAttribLocation(IFunctionCallbackArgs&args);
	bool getBufferParameter(IFunctionCallbackArgs&args);
	bool getContextAttributes(IFunctionCallbackArgs&args);
	bool getError(IFunctionCallbackArgs&args);
	bool getExtension(IFunctionCallbackArgs&args);
	bool getFramebufferAttachmentParameter(IFunctionCallbackArgs&args);
	bool getParameter(IFunctionCallbackArgs&args);
	bool getProgramInfoLog(IFunctionCallbackArgs&args);
	bool getProgramParameter(IFunctionCallbackArgs&args);
	bool getRenderbufferParameter(IFunctionCallbackArgs&args);
	bool getShaderInfoLog(IFunctionCallbackArgs&args);
	bool getShaderParameter(IFunctionCallbackArgs&args);
	bool getShaderPrecisionFormat(IFunctionCallbackArgs&args);
	bool getShaderSource(IFunctionCallbackArgs&args);
	bool getSupportedExtensions(IFunctionCallbackArgs&args);
	bool getTexParameter(IFunctionCallbackArgs&args);
	bool getUniform(IFunctionCallbackArgs&args);
	bool getUniformLocation(IFunctionCallbackArgs&args);
	bool getVertexAttrib(IFunctionCallbackArgs&args);
	bool getVertexAttribOffset(IFunctionCallbackArgs&args);
	bool isBuffer(IFunctionCallbackArgs&args);
	bool isContextLost(IFunctionCallbackArgs&args);
	bool isEnabled(IFunctionCallbackArgs&args);
	bool isFramebuffer(IFunctionCallbackArgs&args);
	bool isProgram(IFunctionCallbackArgs&args);
	bool isRenderbuffer(IFunctionCallbackArgs&args);
	bool isShader(IFunctionCallbackArgs&args);
	bool isTexture(IFunctionCallbackArgs&args);
	bool lineWidth(IFunctionCallbackArgs&args);
	bool linkProgram(IFunctionCallbackArgs&args);
	bool pixelStorei(IFunctionCallbackArgs&args);
	bool polygonOffset(IFunctionCallbackArgs&args);
	bool readPixels(IFunctionCallbackArgs&args);
	bool renderbufferStorage(IFunctionCallbackArgs&args);
	bool sampleCoverage(IFunctionCallbackArgs&args);
	bool scissor(IFunctionCallbackArgs&args);
	bool shaderSource(IFunctionCallbackArgs&args);
	bool stencilFunc(IFunctionCallbackArgs&args);
	bool stencilFuncSeparate(IFunctionCallbackArgs&args);
	bool stencilMask(IFunctionCallbackArgs&args);
	bool stencilMaskSeparate(IFunctionCallbackArgs&args);
	bool stencilOp(IFunctionCallbackArgs&args);
	bool stencilOpSeparate(IFunctionCallbackArgs&args);
	bool texImage2D(IFunctionCallbackArgs&args);
	bool texParameterf(IFunctionCallbackArgs&args);
	bool texParameteri(IFunctionCallbackArgs&args);
	bool texSubImage2D(IFunctionCallbackArgs&args);
	bool uniform1f(IFunctionCallbackArgs&args);
	bool uniform1fv(IFunctionCallbackArgs&args);
	bool uniform1i(IFunctionCallbackArgs&args);
	bool uniform1iv(IFunctionCallbackArgs&args);
	bool uniform2f(IFunctionCallbackArgs&args);
	bool uniform2fv(IFunctionCallbackArgs&args);
	bool uniform2i(IFunctionCallbackArgs&args);
	bool uniform2iv(IFunctionCallbackArgs&args);
	bool uniform3f(IFunctionCallbackArgs&args);
	bool uniform3fv(IFunctionCallbackArgs&args);
	bool uniform3i(IFunctionCallbackArgs&args);
	bool uniform3iv(IFunctionCallbackArgs&args);
	bool uniform4f(IFunctionCallbackArgs&args);
	bool uniform4fv(IFunctionCallbackArgs&args);
	bool uniform4i(IFunctionCallbackArgs&args);
	bool uniform4iv(IFunctionCallbackArgs&args);
	bool uniformMatrix3fv(IFunctionCallbackArgs&args);
	bool uniformMatrix4fv(IFunctionCallbackArgs&args);
	bool useProgram(IFunctionCallbackArgs&args);
	bool validateProgram(IFunctionCallbackArgs&args);
	bool vertexAttribPointer(IFunctionCallbackArgs&args);
	bool viewport(IFunctionCallbackArgs&args);

	enum _GLGETTYPE
	{
		GT_INT,
		GT_FLOAT,
		GT_BOOLEAN,
		GT_STRING,
	};
	ValueBase glGet(const HandleContext&context, GLenum pname, _GLGETTYPE paraType, int nPara);//paraType;
	static void AddConstants(const HandleContext&context);
};

