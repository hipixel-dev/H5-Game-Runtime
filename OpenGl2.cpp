#include "stdafx.h"
#include "OpenGl2.h"

OpenGl2 gl;
OpenGl2::OpenGl2()
{
}


OpenGl2::~OpenGl2()
{
}

void OpenGl2::ActiveTexture(GLenum texture)
{
	m_state->m_ActiveTexture.bSet = true;
	m_state->m_ActiveTexture.texture = texture;
	glActiveTexture(texture);
}

void OpenGl2::AttachShader(GLuint program, GLuint shader)
{
	glAttachShader(program, shader);
}

void OpenGl2::BindAttribLocation(GLuint program, GLuint index, const char* name)
{
	glBindAttribLocation(program, index, name);
}

void OpenGl2::BindBuffer(GLenum target, GLuint buffer)
{
	m_state->m_glBindBuffer[target] = buffer;
	glBindBuffer(target, buffer);
}

void OpenGl2::BindFramebuffer(GLenum target, GLuint framebuffer)
{
	m_state->m_framebuffer.bSet = true;
	m_state->m_framebuffer.target = target;
	m_state->m_framebuffer.buffer = framebuffer;
	glBindFramebuffer(target, framebuffer);
}

void OpenGl2::BindRenderbuffer(GLenum target, GLuint renderbuffer)
{
	m_state->m_renderbuffer.bSet = true;
	m_state->m_renderbuffer.target = target;
	m_state->m_renderbuffer.buffer = renderbuffer;
	glBindRenderbuffer(target, renderbuffer);
}

void OpenGl2::BindTexture(GLenum target, GLuint texture)
{
	m_state->m_glBindTexture[target] = texture;
	glBindTexture(target, texture);
}

void OpenGl2::BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	m_state->m_BlendColor.bSet = true;
	m_state->m_BlendColor.red = red;
	m_state->m_BlendColor.green = green;
	m_state->m_BlendColor.blue = blue;
	m_state->m_BlendColor.alpha = alpha;
	glBlendColor(red, green, blue, alpha);
}

void OpenGl2::BlendEquation(GLenum mode)
{
	glBlendEquation(mode);
}

void OpenGl2::BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha)
{
	glBlendEquationSeparate(modeRGB, modeAlpha);
}

void OpenGl2::BlendFunc(GLenum sfactor, GLenum dfactor)
{
	m_state->m_BlendFunc.bSet = true;
	m_state->m_BlendFunc.sfactor = sfactor;
	m_state->m_BlendFunc.dfactor = dfactor;
	glBlendFunc(sfactor, dfactor);
}

void OpenGl2::BlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)
{
	glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void OpenGl2::BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage)
{
	glBufferData(target, size, data, usage);
}

void OpenGl2::BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data)
{
	glBufferSubData(target, offset, size, data);
}

GLenum OpenGl2::CheckFramebufferStatus(GLenum target)
{
	return glCheckFramebufferStatus(target);
}

void OpenGl2::Clear(GLbitfield mask)
{
	glClear(mask);
}

void OpenGl2::ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
	m_state->m_ClearColor.bSet = true;
	m_state->m_ClearColor.red = red;
	m_state->m_ClearColor.green = green;
	m_state->m_ClearColor.blue = blue;
	m_state->m_ClearColor.alpha = alpha;
	glClearColor(red, green, blue, alpha);
}

void OpenGl2::ClearDepthf(GLclampf depth)
{
	m_state->m_ClearDepth.bSet = true;
	m_state->m_ClearDepth.depth = depth;
	glClearDepthf(depth);
}

void OpenGl2::ClearStencil(GLint s)
{
	m_state->m_ClearStencil.bSet = true;
	m_state->m_ClearStencil.s = s;
	glClearStencil(s);
}

void OpenGl2::ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
	m_state->m_ColorMask.bSet = true;
	m_state->m_ColorMask.red = red;
	m_state->m_ColorMask.green = green;
	m_state->m_ColorMask.blue = blue;
	m_state->m_ColorMask.alpha = alpha;
	glColorMask(red, green, blue, alpha);
}

void OpenGl2::CompileShader(GLuint shader)
{
	glCompileShader(shader);
}

void OpenGl2::CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data)
{
	glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data);
}

void OpenGl2::CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data)
{
	glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data);
}

void OpenGl2::CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)
{
	glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void OpenGl2::CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)
{
	glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

GLuint OpenGl2::CreateProgram(void)
{
	return glCreateProgram();
}

GLuint OpenGl2::CreateShader(GLenum type)
{
	return glCreateShader(type);
}

void OpenGl2::CullFace(GLenum mode)
{
	m_state->m_CullFace.bSet = true;
	m_state->m_CullFace.mode = mode;
	glCullFace(mode);
}

void OpenGl2::DeleteBuffers(GLsizei n, const GLuint* buffers)
{
	glDeleteBuffers(n, buffers);
}

void OpenGl2::DeleteFramebuffers(GLsizei n, const GLuint* framebuffers)
{
	glDeleteFramebuffers(n, framebuffers);
}

void OpenGl2::DeleteTextures(GLsizei n, const GLuint* textures)
{
	glDeleteTextures(n, textures);
}

void OpenGl2::DeleteProgram(GLuint program)
{
	glDeleteProgram(program);
}

void OpenGl2::DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers)
{
	glDeleteRenderbuffers(n, renderbuffers);
}

void OpenGl2::DeleteShader(GLuint shader)
{
	glDeleteShader(shader);
}

void OpenGl2::DetachShader(GLuint program, GLuint shader)
{
	glDetachShader(program, shader);
}

void OpenGl2::DepthFunc(GLenum func)
{
	glDepthFunc(func);
}

void OpenGl2::DepthMask(GLboolean flag)
{
	glDepthMask(flag);
}

void OpenGl2::DepthRangef(GLclampf zNear, GLclampf zFar)
{
	glDepthRangef(zNear, zFar);
}

void OpenGl2::Disable(GLenum cap)
{
	m_state->m_glEnableMap[cap] = false;
	glDisable(cap);
}

void OpenGl2::DisableVertexAttribArray(GLuint index)
{
	glDisableVertexAttribArray(index);
}

void OpenGl2::DrawArrays(GLenum mode, GLint first, GLsizei count)
{
	glDrawArrays(mode, first, count);
}

void OpenGl2::DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	glDrawElements(mode, count, type, indices);
}

void OpenGl2::Enable(GLenum cap)
{
	m_state->m_glEnableMap[cap] = true;
	glEnable(cap);
}

void OpenGl2::EnableVertexAttribArray(GLuint index)
{
	glEnableVertexAttribArray(index);
}

void OpenGl2::Finish(void)
{
	glFinish();
}

void OpenGl2::Flush(void)
{
	glFlush();
}

void OpenGl2::FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{
	glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void OpenGl2::FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{
	glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

void OpenGl2::FrontFace(GLenum mode)
{
	glFrontFace(mode);
}

void OpenGl2::GenBuffers(GLsizei n, GLuint* buffers)
{
	glGenBuffers(n, buffers);
}

void OpenGl2::GenerateMipmap(GLenum target)
{
	glGenerateMipmap(target);
}

void OpenGl2::GenFramebuffers(GLsizei n, GLuint* framebuffers)
{
	glGenFramebuffers(n, framebuffers);
}

void OpenGl2::GenRenderbuffers(GLsizei n, GLuint* renderbuffers)
{
	glGenRenderbuffers(n, renderbuffers);
}

void OpenGl2::GenTextures(GLsizei n, GLuint* textures)
{
	glGenTextures(n, textures);
}

void OpenGl2::GetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
	glGetActiveAttrib(program, index, bufsize, length, size, type, name);
}

void OpenGl2::GetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name)
{
	glGetActiveUniform(program, index, bufsize, length, size, type, name);
}

void OpenGl2::GetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders)
{
	glGetAttachedShaders(program, maxcount, count, shaders);
}

int OpenGl2::GetAttribLocation(GLuint program, const char* name)
{
	return glGetAttribLocation(program, name);
}

void OpenGl2::GetBooleanv(GLenum pname, GLboolean* params)
{
	glGetBooleanv(pname, params);
}

void OpenGl2::GetBufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
	glGetBufferParameteriv(target, pname, params);
}

GLenum OpenGl2::GetError(void)
{
	GLenum er= glGetError();
	if (er)
	{
		int dd = 3;
	}
	return er;
}

void OpenGl2::GetFloatv(GLenum pname, GLfloat* params)
{
	glGetFloatv(pname, params);
}

void OpenGl2::GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params)
{
	glGetFramebufferAttachmentParameteriv(target, attachment, pname, params);
}

void OpenGl2::GetIntegerv(GLenum pname, GLint* params)
{
	glGetIntegerv(pname, params);
}

void OpenGl2::GetProgramiv(GLuint program, GLenum pname, GLint* params)
{
	glGetProgramiv(program, pname, params);
}

void OpenGl2::GetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog)
{
	glGetProgramInfoLog(program, bufsize, length, infolog);
}

void OpenGl2::GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params)
{
	glGetRenderbufferParameteriv(target, pname, params);
}

void OpenGl2::GetShaderiv(GLuint shader, GLenum pname, GLint* params)
{
	glGetShaderiv(shader, pname, params);
}

void OpenGl2::GetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog)
{
	glGetShaderInfoLog(shader, bufsize, length, infolog);
}

void OpenGl2::GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision)
{
	glGetShaderPrecisionFormat(shadertype, precisiontype, range, precision);
}

void OpenGl2::GetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source)
{
	glGetShaderSource(shader, bufsize, length, source);
}

const GLubyte* OpenGl2::GetString(GLenum name)
{
	return glGetString(name);
}

void OpenGl2::GetTexParameterfv(GLenum target, GLenum pname, GLfloat* params)
{
	glGetTexParameterfv(target, pname, params);
}

void OpenGl2::GetTexParameteriv(GLenum target, GLenum pname, GLint* params)
{
	glGetTexParameteriv(target, pname, params);
}

void OpenGl2::GetUniformfv(GLuint program, GLint location, GLfloat* params)
{
	glGetUniformfv(program, location, params);
}

void OpenGl2::GetUniformiv(GLuint program, GLint location, GLint* params)
{
	glGetUniformiv(program, location, params);
}

int OpenGl2::GetUniformLocation(GLuint program, const char* name)
{
	return glGetUniformLocation(program, name);
}

void OpenGl2::GetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params)
{
	glGetVertexAttribfv(index, pname, params);
}

void OpenGl2::GetVertexAttribiv(GLuint index, GLenum pname, GLint* params)
{
	glGetVertexAttribiv(index, pname, params);
}

void OpenGl2::GetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer)
{
	glGetVertexAttribPointerv(index, pname, pointer);
}

void OpenGl2::Hint(GLenum target, GLenum mode)
{
	glHint(target, mode);
}

GLboolean OpenGl2::IsBuffer(GLuint buffer)
{
	return glIsBuffer(buffer);
}

GLboolean OpenGl2::IsEnabled(GLenum cap)
{
	return glIsEnabled(cap);
}

GLboolean OpenGl2::IsFramebuffer(GLuint framebuffer)
{
	return glIsFramebuffer(framebuffer);
}

GLboolean OpenGl2::IsProgram(GLuint program)
{
	return glIsProgram(program);
}

GLboolean OpenGl2::IsRenderbuffer(GLuint renderbuffer)
{
	return glIsRenderbuffer(renderbuffer);
}

GLboolean OpenGl2::IsShader(GLuint shader)
{
	return glIsShader(shader);
}

GLboolean OpenGl2::IsTexture(GLuint texture)
{
	return glIsTexture(texture);
}

void OpenGl2::LineWidth(GLfloat width)
{
	glLineWidth(width);
}

void OpenGl2::LinkProgram(GLuint program)
{
	glLinkProgram(program);
}

void OpenGl2::PixelStorei(GLenum pname, GLint param)
{
	glPixelStorei(pname, param);
}

void OpenGl2::PolygonOffset(GLfloat factor, GLfloat units)
{
	glPolygonOffset(factor, units);
}

void OpenGl2::ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels)
{
	glReadPixels(x, y, width, height, format, type, pixels);
}

void OpenGl2::ReleaseShaderCompiler(void)
{
	glReleaseShaderCompiler();
}

void OpenGl2::RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{
	glRenderbufferStorage(target, internalformat, width, height);
}

void OpenGl2::SampleCoverage(GLclampf value, GLboolean invert)
{
	glSampleCoverage(value, invert);
}

void OpenGl2::Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glScissor(x, y, width, height);
}

void OpenGl2::ShaderBinary(GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length)
{
	glShaderBinary(n, shaders, binaryformat, binary, length);
}

void OpenGl2::ShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length)
{
	glShaderSource(shader, count, string, length);
}

void OpenGl2::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	glStencilFunc(func, ref, mask);
}

void OpenGl2::StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask)
{
	glStencilFuncSeparate(face, func, ref, mask);
}

void OpenGl2::StencilMask(GLuint mask)
{
	glStencilMask(mask);
}

void OpenGl2::StencilMaskSeparate(GLenum face, GLuint mask)
{
	glStencilMaskSeparate(face, mask);
}

void OpenGl2::StencilOp(GLenum fail, GLenum zfail, GLenum zpass)
{
	glStencilOp(fail, zfail, zpass);
}

void OpenGl2::StencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass)
{
	glStencilOpSeparate(face, fail, zfail, zpass);
}

void OpenGl2::TexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels)
{
	glTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

void OpenGl2::TexParameterf(GLenum target, GLenum pname, GLfloat param)
{
	glTexParameterf(target, pname, param);
}

void OpenGl2::TexParameterfv(GLenum target, GLenum pname, const GLfloat* params)
{
	glTexParameterfv(target, pname, params);
}

void OpenGl2::TexParameteri(GLenum target, GLenum pname, GLint param)
{
	glTexParameteri(target, pname, param);
}

void OpenGl2::TexParameteriv(GLenum target, GLenum pname, const GLint* params)
{
	glTexParameteriv(target, pname, params);
}

void OpenGl2::TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels)
{
	glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

void OpenGl2::Uniform1f(GLint location, GLfloat x)
{
	glUniform1f(location, x);
}

void OpenGl2::Uniform1fv(GLint location, GLsizei count, const GLfloat* v)
{
	glUniform1fv(location, count, v);
}

void OpenGl2::Uniform1i(GLint location, GLint x)
{
	glUniform1i(location, x);
}

void OpenGl2::Uniform1iv(GLint location, GLsizei count, const GLint* v)
{
	glUniform1iv(location, count, v);
}

void OpenGl2::Uniform2f(GLint location, GLfloat x, GLfloat y)
{
	glUniform2f(location, x, y);
}

void OpenGl2::Uniform2fv(GLint location, GLsizei count, const GLfloat* v)
{
	glUniform2fv(location, count, v);
}

void OpenGl2::Uniform2i(GLint location, GLint x, GLint y)
{
	glUniform2i(location, x, y);
}

void OpenGl2::Uniform2iv(GLint location, GLsizei count, const GLint* v)
{
	glUniform2iv(location, count, v);
}

void OpenGl2::Uniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(location, x, y, z);
}

void OpenGl2::Uniform3fv(GLint location, GLsizei count, const GLfloat* v)
{
	glUniform3fv(location, count, v);
}

void OpenGl2::Uniform3i(GLint location, GLint x, GLint y, GLint z)
{
	glUniform3i(location, x, y, z);
}

void OpenGl2::Uniform3iv(GLint location, GLsizei count, const GLint* v)
{
	glUniform3iv(location, count, v);
}

void OpenGl2::Uniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(location, x, y, z, w);
}

void OpenGl2::Uniform4fv(GLint location, GLsizei count, const GLfloat* v)
{
	glUniform4fv(location, count, v);
}

void OpenGl2::Uniform4i(GLint location, GLint x, GLint y, GLint z, GLint w)
{
	glUniform4i(location, x, y, z, w);
}

void OpenGl2::Uniform4iv(GLint location, GLsizei count, const GLint* v)
{
	glUniform4iv(location, count, v);
}

void OpenGl2::UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix2fv(location, count, transpose, value);
}

void OpenGl2::UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix3fv(location, count, transpose, value);
}

void OpenGl2::UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
	glUniformMatrix4fv(location, count, transpose, value);
}

void OpenGl2::UseProgram(GLuint program)
{
	m_state->m_UseProgram.bSet = true;
	m_state->m_UseProgram.program= program;
	glUseProgram(program);
}

void OpenGl2::ValidateProgram(GLuint program)
{
	glValidateProgram(program);
}

void OpenGl2::VertexAttrib1f(GLuint indx, GLfloat x)
{
	glVertexAttrib1f(indx, x);
}

void OpenGl2::VertexAttrib1fv(GLuint indx, const GLfloat* values)
{
	glVertexAttrib1fv(indx, values);
}

void OpenGl2::VertexAttrib2f(GLuint indx, GLfloat x, GLfloat y)
{
	glVertexAttrib2f(indx, x, y);
}

void OpenGl2::VertexAttrib2fv(GLuint indx, const GLfloat* values)
{
	glVertexAttrib2fv(indx, values);
}

void OpenGl2::VertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z)
{
	glVertexAttrib3f(indx, x, y, z);
}

void OpenGl2::VertexAttrib3fv(GLuint indx, const GLfloat* values)
{
	glVertexAttrib3fv(indx, values);
}

void OpenGl2::VertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glVertexAttrib4f(indx, x, y, z, w);
}

void OpenGl2::VertexAttrib4fv(GLuint indx, const GLfloat* values)
{
	glVertexAttrib4fv(indx, values);
}

void OpenGl2::VertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
	glVertexAttribPointer(indx, size, type, normalized, stride, ptr);
}

void OpenGl2::Viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	m_state->m_Viewport.bSet = true;
	m_state->m_Viewport.x = x;
	m_state->m_Viewport.y = y;
	m_state->m_Viewport.width = width;
	m_state->m_Viewport.height = height;
	glViewport(x, y, width, height);
}

void OpenGl2::Cleanup()
{
	m_state->Cleanup();
}

OpenGl2::GLSTATE OpenGl2::g_defaultState;

void OpenGl2::GLSTATE::SetEnables()
{
	for (std::unordered_map<int, bool>::iterator it = m_glEnableMap.begin(); it != m_glEnableMap.end(); ++it)
	{
		if (it->second)
		{
			glEnable(it->first);
		}
		else
			glDisable(it->first);
	}
}






void OpenGl2::GLSTATE::SetBindBuffer()
{
	for (std::unordered_map<GLenum, GLuint>::iterator it = m_glBindBuffer.begin(); it != m_glBindBuffer.end(); ++it)
	{
		glBindBuffer(it->first, it->second);
	}
}

void OpenGl2::GLSTATE::SetBindTexture()
{
	for (std::unordered_map<GLenum, GLuint>::iterator it = m_glBindTexture.begin(); it != m_glBindTexture.end(); ++it)
	{
		glBindTexture(it->first, it->second);
	}
}


