#pragma once

#if defined(_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

//#include <tuple>
// 
// class OpenGl2;
// class FUNCALLINFOBASE
// {
// public:
// 
// 	FUNCALLINFOBASE()
// 	{
// 
// 	}
// 	virtual ~FUNCALLINFOBASE()
// 	{
// 
// 	}
// 	virtual void CallFunc() = 0;
// };
// 
// template <typename RET,typename... ARGS>
// class FUNCALLINFO :public FUNCALLINFOBASE...
// {
// public:
// 	static const int nArgs = sizeof...(ARGS);
// 	typedef RET(GL_APIENTRY *FUNC)(ARGS...args);
// 
// 	FUNC m_pfunc = nullptr;
// 	std::tuple<ARGS...>m_param;
// 	FUNCALLINFO(FUNC func,ARGS...args)
// 	{
// 		m_pfunc = func;
// 		m_param = std::make_tuple(args...);
// 	}
// 	~FUNCALLINFO()
// 	{
// 	}
// 
// 	virtual void CallFunc()
// 	{
// 
// 		switch (sizeof...(ARGS))
// 		{
// 		case 1:m_pfunc(std::get<0>(m_param)); break;
// 		case 2:m_pfunc(std::get<0>(m_param), std::get<1>(m_param)); break;
// 		case 3:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param)); break;
// 		case 4:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param), std::get<3>(m_param)); break;
// 		case 5:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param), std::get<3>(m_param), std::get<4>(m_param)); break;
// 		case 6:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param), std::get<3>(m_param), std::get<4>(m_param), std::get<5>(m_param)); break;
// 		case 7:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param), std::get<3>(m_param), std::get<4>(m_param), std::get<5>(m_param), std::get<6>(m_param)); break;
// 		case 8:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param), std::get<3>(m_param), std::get<4>(m_param), std::get<5>(m_param), std::get<6>(m_param), std::get<7>(m_param)); break;
// 		case 9:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param), std::get<3>(m_param), std::get<4>(m_param), std::get<5>(m_param), std::get<6>(m_param), std::get<7>(m_param), std::get<8>(m_param)); break;
// 		case 10:m_pfunc(std::get<0>(m_param), std::get<1>(m_param), std::get<2>(m_param), std::get<3>(m_param), std::get<4>(m_param), std::get<5>(m_param), std::get<6>(m_param), std::get<7>(m_param), std::get<8>(m_param), std::get<9>(m_param)); break;
// 		default: assert(0); break;
// 		}
// 		
// 
// 	}
// };
// 
// FUNCALLINFO<void, GLuint, GLuint > inf(glAttachShader,1, 2);


//封装OPENGL函数
class OpenGl2
{
public:
	struct GLSTATE
	{
		std::unordered_map<int, bool>m_glEnableMap;//调用过glEnable/glDisable的记录，对应的值true表示使用了glEnable，否则使用了glDisable，没记录表示未调用过
		void SetEnables();
		std::unordered_map<GLenum, GLuint>m_glBindBuffer;
		void SetBindBuffer();
		std::unordered_map<GLenum , GLuint>m_glBindTexture;
		void SetBindTexture();

		struct STATEBASE
		{
			bool bSet = false;
			virtual void SetState() = 0;
		};

		struct ACTIVETEXTURE :public STATEBASE
		{
			GLenum texture;
			void SetState()
			{
				if (bSet)glActiveTexture(texture);
			}
		}m_ActiveTexture;

		struct BLENDCOLOR :public STATEBASE
		{
			GLclampf red;
			GLclampf green;
			GLclampf blue;
			GLclampf alpha;
			void SetState()
			{
				if (bSet)glBlendColor(red, green, blue, alpha);
			}
		}m_BlendColor;

		struct BLENDFUNC :public STATEBASE
		{
			GLenum sfactor = 0;
			GLenum dfactor = 0;
			void SetState()
			{
				if (bSet)glBlendFunc(sfactor, dfactor);
			}
		}m_BlendFunc;

		struct FRAMEBUFFER :public STATEBASE
		{
			GLenum target;
			GLuint buffer;
			void SetState()
			{
				if (bSet)glBindFramebuffer(target, buffer);
			}
		}m_framebuffer;
		struct RENDERBUFFER :public STATEBASE
		{
			GLenum target;
			GLuint buffer;
			void SetState()
			{
				if (bSet)glBindRenderbuffer(target, buffer);
			}
		}m_renderbuffer;
		

		struct CLEARCOLOR :public STATEBASE
		{
			GLclampf red;
			GLclampf green;
			GLclampf blue;
			GLclampf alpha;
			void SetState()
			{
				if (bSet)glClearColor(red, green, blue, alpha);
			}
		}m_ClearColor;
		struct CLEARDEPTH :public STATEBASE
		{
			GLclampf depth;
			void SetState()
			{
				if (bSet)glClearDepthf(depth);
			}
		}m_ClearDepth;
		struct CLEARSTENCIL :public STATEBASE
		{
			GLint s;
			void SetState()
			{
				if (bSet)glClearStencil(s);
			}
		}m_ClearStencil;
		struct USEPROGRAM :public STATEBASE
		{
			GLuint program = 0;
			void SetState()
			{
				if (bSet)glUseProgram(program);
			}
		}m_UseProgram;
		
		struct VIEWPORT :public STATEBASE//保存本canvas最后使用的glviewport
		{
			int x = 0;
			int y = 0;
			int width = 0;
			int height = 0;
			void SetState()
			{
				if (bSet)glViewport(x, y, width, height);
			}
		}m_Viewport;
		
		struct COLORMASK :public STATEBASE
		{
			GLboolean red;
			GLboolean green;
			GLboolean blue;
			GLboolean alpha;
			void SetState()
			{
				if (bSet)glColorMask(red, green, blue, alpha);
			}
		}m_ColorMask;
		
		
		struct CULLFACE :public STATEBASE
		{
			GLenum mode;
			void SetState()
			{
				if (bSet)glCullFace(mode);
			}
			
		}m_CullFace;


		void SetAllState()
		{
			SetEnables();
			SetBindBuffer();
			SetBindTexture();
			m_ActiveTexture.SetState();
			m_BlendColor.SetState();
			m_BlendFunc.SetState();
			m_ClearColor.SetState();
			m_ClearDepth.SetState();
			m_ClearStencil.SetState();
			m_ColorMask.SetState();
			m_CullFace.SetState();
			m_framebuffer.SetState();
			m_renderbuffer.SetState();
			m_UseProgram.SetState();
			m_Viewport.SetState();
		}
		void Cleanup()
		{

			m_glBindBuffer.clear();
			m_glBindTexture.clear();
			m_glEnableMap.clear();
			m_ActiveTexture.bSet = false;
			m_BlendColor.bSet = false;
			m_BlendFunc.bSet = false;
			m_ClearColor.bSet = false;
			m_ClearDepth.bSet = false;
			m_ClearStencil.bSet = false;
			m_ColorMask.bSet = false;
			m_CullFace.bSet = false;
			m_framebuffer.bSet = false;
			m_renderbuffer.bSet = false;
			m_UseProgram.bSet = false;
			m_Viewport.bSet = false;

		}
	};
	static GLSTATE g_defaultState;//没调用过SetState的默认状态
	GLSTATE *m_state = &g_defaultState;
	void SetState(GLSTATE*state)//每个canvas都有一个state，开始调用opengl前都要先调用SetState,结束后调用SetState(nullptr)
	{
		if (m_state == state)return;

		if (!state)state = &g_defaultState;
		m_state = state;
		m_state->SetAllState();
	}


	OpenGl2();
	~OpenGl2();
	
	
	void        ActiveTexture(GLenum texture);
	void        AttachShader(GLuint program, GLuint shader);
	void        BindAttribLocation(GLuint program, GLuint index, const char* name);
	void        BindBuffer(GLenum target, GLuint buffer);
	void        BindFramebuffer(GLenum target, GLuint framebuffer);
	void        BindRenderbuffer(GLenum target, GLuint renderbuffer);
	void        BindTexture(GLenum target, GLuint texture);
	void        BlendColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	void        BlendEquation(GLenum mode);
	void        BlendEquationSeparate(GLenum modeRGB, GLenum modeAlpha);
	void        BlendFunc(GLenum sfactor, GLenum dfactor);
	void        BlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
	void        BufferData(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
	void        BufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
	GLenum      CheckFramebufferStatus(GLenum target);
	void        Clear(GLbitfield mask);
	void        ClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
	void        ClearDepthf(GLclampf depth);
	void        ClearStencil(GLint s);
	void        ColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
	void        CompileShader(GLuint shader);
	void        CompressedTexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
	void        CompressedTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
	void        CopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
	void        CopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
	GLuint      CreateProgram(void);
	GLuint      CreateShader(GLenum type);
	void        CullFace(GLenum mode);
	void        DeleteBuffers(GLsizei n, const GLuint* buffers);
	void        DeleteFramebuffers(GLsizei n, const GLuint* framebuffers);
	void        DeleteTextures(GLsizei n, const GLuint* textures);
	void        DeleteProgram(GLuint program);
	void        DeleteRenderbuffers(GLsizei n, const GLuint* renderbuffers);
	void        DeleteShader(GLuint shader);
	void        DetachShader(GLuint program, GLuint shader);
	void        DepthFunc(GLenum func);
	void        DepthMask(GLboolean flag);
	void        DepthRangef(GLclampf zNear, GLclampf zFar);
	void        Disable(GLenum cap);
	void        DisableVertexAttribArray(GLuint index);
	void        DrawArrays(GLenum mode, GLint first, GLsizei count);
	void        DrawElements(GLenum mode, GLsizei count, GLenum type, const void* indices);
	void        Enable(GLenum cap);
	void        EnableVertexAttribArray(GLuint index);
	void        Finish(void);
	void        Flush(void);
	void        FramebufferRenderbuffer(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	void        FramebufferTexture2D(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	void        FrontFace(GLenum mode);
	void        GenBuffers(GLsizei n, GLuint* buffers);
	void        GenerateMipmap(GLenum target);
	void        GenFramebuffers(GLsizei n, GLuint* framebuffers);
	void        GenRenderbuffers(GLsizei n, GLuint* renderbuffers);
	void        GenTextures(GLsizei n, GLuint* textures);
	void        GetActiveAttrib(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
	void        GetActiveUniform(GLuint program, GLuint index, GLsizei bufsize, GLsizei* length, GLint* size, GLenum* type, char* name);
	void        GetAttachedShaders(GLuint program, GLsizei maxcount, GLsizei* count, GLuint* shaders);
	int         GetAttribLocation(GLuint program, const char* name);
	void        GetBooleanv(GLenum pname, GLboolean* params);
	void        GetBufferParameteriv(GLenum target, GLenum pname, GLint* params);
	GLenum      GetError(void);
	void        GetFloatv(GLenum pname, GLfloat* params);
	void        GetFramebufferAttachmentParameteriv(GLenum target, GLenum attachment, GLenum pname, GLint* params);
	void        GetIntegerv(GLenum pname, GLint* params);
	void        GetProgramiv(GLuint program, GLenum pname, GLint* params);
	void        GetProgramInfoLog(GLuint program, GLsizei bufsize, GLsizei* length, char* infolog);
	void        GetRenderbufferParameteriv(GLenum target, GLenum pname, GLint* params);
	void        GetShaderiv(GLuint shader, GLenum pname, GLint* params);
	void        GetShaderInfoLog(GLuint shader, GLsizei bufsize, GLsizei* length, char* infolog);
	void        GetShaderPrecisionFormat(GLenum shadertype, GLenum precisiontype, GLint* range, GLint* precision);
	void        GetShaderSource(GLuint shader, GLsizei bufsize, GLsizei* length, char* source);
	const GLubyte* GetString(GLenum name);
	void         GetTexParameterfv(GLenum target, GLenum pname, GLfloat* params);
	void         GetTexParameteriv(GLenum target, GLenum pname, GLint* params);
	void         GetUniformfv(GLuint program, GLint location, GLfloat* params);
	void         GetUniformiv(GLuint program, GLint location, GLint* params);
	int          GetUniformLocation(GLuint program, const char* name);
	void         GetVertexAttribfv(GLuint index, GLenum pname, GLfloat* params);
	void         GetVertexAttribiv(GLuint index, GLenum pname, GLint* params);
	void         GetVertexAttribPointerv(GLuint index, GLenum pname, void** pointer);
	void         Hint(GLenum target, GLenum mode);
	GLboolean    IsBuffer(GLuint buffer);
	GLboolean    IsEnabled(GLenum cap);
	GLboolean    IsFramebuffer(GLuint framebuffer);
	GLboolean    IsProgram(GLuint program);
	GLboolean    IsRenderbuffer(GLuint renderbuffer);
	GLboolean    IsShader(GLuint shader);
	GLboolean    IsTexture(GLuint texture);
	void         LineWidth(GLfloat width);
	void         LinkProgram(GLuint program);
	void         PixelStorei(GLenum pname, GLint param);
	void         PolygonOffset(GLfloat factor, GLfloat units);
	void         ReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void* pixels);
	void         ReleaseShaderCompiler(void);
	void         RenderbufferStorage(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	void         SampleCoverage(GLclampf value, GLboolean invert);
	void         Scissor(GLint x, GLint y, GLsizei width, GLsizei height);
	void         ShaderBinary(GLint n, const GLuint* shaders, GLenum binaryformat, const void* binary, GLint length);
	void         ShaderSource(GLuint shader, GLsizei count, const char** string, const GLint* length);
	void         StencilFunc(GLenum func, GLint ref, GLuint mask);
	void         StencilFuncSeparate(GLenum face, GLenum func, GLint ref, GLuint mask);
	void         StencilMask(GLuint mask);
	void         StencilMaskSeparate(GLenum face, GLuint mask);
	void         StencilOp(GLenum fail, GLenum zfail, GLenum zpass);
	void         StencilOpSeparate(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
	void         TexImage2D(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
	void         TexParameterf(GLenum target, GLenum pname, GLfloat param);
	void         TexParameterfv(GLenum target, GLenum pname, const GLfloat* params);
	void         TexParameteri(GLenum target, GLenum pname, GLint param);
	void         TexParameteriv(GLenum target, GLenum pname, const GLint* params);
	void         TexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
	void         Uniform1f(GLint location, GLfloat x);
	void         Uniform1fv(GLint location, GLsizei count, const GLfloat* v);
	void         Uniform1i(GLint location, GLint x);
	void         Uniform1iv(GLint location, GLsizei count, const GLint* v);
	void         Uniform2f(GLint location, GLfloat x, GLfloat y);
	void         Uniform2fv(GLint location, GLsizei count, const GLfloat* v);
	void         Uniform2i(GLint location, GLint x, GLint y);
	void         Uniform2iv(GLint location, GLsizei count, const GLint* v);
	void         Uniform3f(GLint location, GLfloat x, GLfloat y, GLfloat z);
	void         Uniform3fv(GLint location, GLsizei count, const GLfloat* v);
	void         Uniform3i(GLint location, GLint x, GLint y, GLint z);
	void         Uniform3iv(GLint location, GLsizei count, const GLint* v);
	void         Uniform4f(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void         Uniform4fv(GLint location, GLsizei count, const GLfloat* v);
	void         Uniform4i(GLint location, GLint x, GLint y, GLint z, GLint w);
	void         Uniform4iv(GLint location, GLsizei count, const GLint* v);
	void         UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	void         UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	void         UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	void         UseProgram(GLuint program);
	void         ValidateProgram(GLuint program);
	void         VertexAttrib1f(GLuint indx, GLfloat x);
	void         VertexAttrib1fv(GLuint indx, const GLfloat* values);
	void         VertexAttrib2f(GLuint indx, GLfloat x, GLfloat y);
	void         VertexAttrib2fv(GLuint indx, const GLfloat* values);
	void         VertexAttrib3f(GLuint indx, GLfloat x, GLfloat y, GLfloat z);
	void         VertexAttrib3fv(GLuint indx, const GLfloat* values);
	void         VertexAttrib4f(GLuint indx, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void         VertexAttrib4fv(GLuint indx, const GLfloat* values);
	void         VertexAttribPointer(GLuint indx, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr);
	void         Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
	void Cleanup();
};

extern OpenGl2 gl;
