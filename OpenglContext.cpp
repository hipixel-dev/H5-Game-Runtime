#include "stdafx.h"
#include "OpenglContext.h"

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

//
///
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint loadShader ( GLenum type, const char *shaderSrc )
{
    GLuint shader;
    GLint compiled;
    
    // Create the shader object
    shader = glCreateShader ( type );
    
    if ( shader == 0 )
        return 0;
    
    // Load the shader source
    glShaderSource ( shader, 1, &shaderSrc, NULL );
    
    // Compile the shader
    glCompileShader ( shader );
    
    // Check the compile status
    glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
    
    if ( !compiled )
    {
        GLint infoLen = 0;
        
        glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
        
        if ( infoLen > 1 )
        {
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );
            
            glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
            //esLogMessage ( "Error compiling shader:\n%s\n", infoLog );
            
            free ( infoLog );
        }
        
        glDeleteShader ( shader );
        return 0;
    }
    
    return shader;
    
}


//
///
/// \brief Load a vertex and fragment shader, create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
//
GLuint loadProgram ( const char *vertShaderSrc, const char *fragShaderSrc )
{
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programObject;
    GLint linked;
    
    // Load the vertex/fragment shaders
    vertexShader = loadShader ( GL_VERTEX_SHADER, vertShaderSrc );
    if ( vertexShader == 0 )
        return 0;
    
    fragmentShader = loadShader ( GL_FRAGMENT_SHADER, fragShaderSrc );
    if ( fragmentShader == 0 )
    {
        glDeleteShader( vertexShader );
        return 0;
    }
    
    // Create the program object
    programObject = glCreateProgram ( );
    
    if ( programObject == 0 )
        return 0;
    
    glAttachShader ( programObject, vertexShader );
    glAttachShader ( programObject, fragmentShader );
    
    // Link the program
    glLinkProgram ( programObject );
    
    // Check the link status
    glGetProgramiv ( programObject, GL_LINK_STATUS, &linked );
    
    if ( !linked )
    {
        GLint infoLen = 0;
        
        glGetProgramiv ( programObject, GL_INFO_LOG_LENGTH, &infoLen );
        
        if ( infoLen > 1 )
        {
            char* infoLog = (char*)malloc (sizeof(char) * infoLen );
            
            glGetProgramInfoLog ( programObject, infoLen, NULL, infoLog );
            //esLogMessage ( "Error linking program:\n%s\n", infoLog );
            
            free ( infoLog );
        }
        
        glDeleteProgram ( programObject );
        return 0;
    }
    
    // Free up no longer needed shader resources
    glDeleteShader ( vertexShader );
    glDeleteShader ( fragmentShader );
    
    return programObject;
}


bool OpenglContext::SingleColorProgram::Init()
{
	const char vShaderStr[] =
		"attribute vec2 a_position;   \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position = vec4( a_position,1,1);; \n"
		"}                            \n";

	const char fShaderStr[] =
		"precision mediump float;                            \n"
		"uniform vec4 v_color;                            \n"
		"void main()                                         \n"
		"{                                                   \n"
		"  gl_FragColor = v_color;							\n"
		"}                                                   \n";

	m_Program = loadProgram(vShaderStr, fShaderStr);
	return m_Program != 0;
}

void OpenglContext::SingleColorProgram::Uninit()
{
	if (m_Program)glDeleteProgram(m_Program);
	m_Program = 0;
}

void OpenglContext::SingleColorProgram::SetColor(float r, float g, float b, float a)
{
	GLuint colorLoc = glGetUniformLocation(m_Program, "v_color");
	glUniform4f(colorLoc, r, g, b, a);
}
void OpenglContext::SingleColorProgram::SetVertexPosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
	GLuint positionLoc = glGetAttribLocation(OpenglContext::s_TextureProgram, "a_position");
	glVertexAttribPointer(positionLoc, size, type, normalized, stride, ptr);
	glEnableVertexAttribArray(positionLoc);
}


bool OpenglContext::TextureProgram::Init()
{
	const char vShaderStr[] =
		"attribute vec2 a_position;   \n"
		"attribute vec2 a_texCoord;   \n"
		"varying vec2 v_texCoord;     \n"
		"void main()                  \n"
		"{                            \n"
		"   gl_Position =vec4( a_position,1,1); \n"
		"   v_texCoord = a_texCoord;  \n"
		"}                            \n";

	const char fShaderStr[] =
		"precision mediump float;                            \n"
		"varying vec2 v_texCoord;                            \n"
		"uniform sampler2D s_texture;                        \n"
		"uniform float s_alpha;								 \n"
		"void main()                                         \n"
		"{                                                   \n"
		"vec4 outcolor=texture2D( s_texture, v_texCoord );\n"
		"outcolor.a*=s_alpha;								\n"
		"  gl_FragColor =outcolor;							\n"
		"}                                                   \n";
	m_Program = loadProgram(vShaderStr, fShaderStr);
	return m_Program != 0;
}

void OpenglContext::TextureProgram::Uninit()
{
	if (m_Program)glDeleteProgram(m_Program);
	m_Program = 0;
}

void OpenglContext::TextureProgram::SetMatrix(MATRIX* mat)
{
// 	GLuint matloc = glGetUniformLocation(s_TextureProgram, "s_matWVP");
// 	glUniformMatrix3fv(matloc, 1, GL_FALSE, (GLfloat*)mat);
// 	int err = glGetError();
}

void OpenglContext::TextureProgram::SetTextureUnit(GLint i)
{
	GLuint samplerLoc = glGetUniformLocation(OpenglContext::s_TextureProgram, "s_texture");
	glUniform1i(samplerLoc, 0);
}

void OpenglContext::TextureProgram::SetTexturePosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
	GLuint texCoordLoc = glGetAttribLocation(OpenglContext::s_TextureProgram, "a_texCoord");
	glVertexAttribPointer(texCoordLoc, size, type, normalized, stride, ptr);
	glEnableVertexAttribArray(texCoordLoc);
}

void OpenglContext::TextureProgram::SetVertexPosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
	GLuint positionLoc = glGetAttribLocation(OpenglContext::s_TextureProgram, "a_position");
	glVertexAttribPointer(positionLoc, size, type, normalized, stride, ptr);
	glEnableVertexAttribArray(positionLoc);
}

OpenglContext::TextureProgram::operator GLuint()
{
	return m_Program;
}

void OpenglContext::TextureProgram::SetAlpha(float fAlpha)
{
	GLuint samplerLoc = glGetUniformLocation(OpenglContext::s_TextureProgram, "s_alpha");
	glUniform1f(samplerLoc, fAlpha);
}

// DXPOINT OpenglContext::WindowPointToGlPoint(const DXPOINT &vPos)
// {
// 	DXPOINT pt;
// 	pt.x = WindowPointToGlPointX(vPos.x);
// 	pt.y = WindowPointToGlPointY(vPos.y);
// 	return pt;
// }

GLuint OpenglContext::CreateGlTexID(const ImageData&imgData)
{
	GLuint m_nTexID = 0;
	// Generate a texture object
	glGenTextures(1, &m_nTexID);
	assert(!glGetError());
	if (m_nTexID == 0)return 0;

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, m_nTexID);
	assert(!glGetError());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	assert(!glGetError());
	// Load the texture
	//opengl原点在左下角，需要翻转
	uint32_t*pdata = new uint32_t[imgData.nWidth*imgData.nHeight];
	imgData.SwapY(pdata);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgData.nWidth, imgData.nHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pdata);
	delete[]pdata;
	assert(!glGetError());

	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	assert(!glGetError());
	glBindTexture(GL_TEXTURE_2D, 0);
	assert(!glGetError());
	return m_nTexID;
}

OpenglContext::LinearGradientProgram OpenglContext::s_LinearGradientProgram;

OpenglContext::TextureProgram OpenglContext::s_TextureProgram;

OpenglContext::SingleColorProgram OpenglContext::s_SingleColorProgram;

bool OpenglContext::LinearGradientProgram::Init()
{
	const char vShaderStr[] =
		"attribute vec2 a_position;   \n"
		"uniform vec2 s_posA;			\n"
		"uniform float s_lenAB;			\n"
		"uniform float s_vpWidth;		\n"
		"uniform float s_vpHeight;		\n"
		"uniform mat3 s_rotAB;			\n"
		"varying vec2 v_texCoord;     \n"
		"vec2 GlToWin(vec2 pos)        \n"
		"{ \n"
		"	vec2 ret; \n"
		"   ret.x=(pos.x + 1.0)*s_vpWidth*0.5;		\n	"
		"   ret.y=s_vpHeight - (pos.y + 1.0)*s_vpHeight*0.5;\n"
		"	return ret;                       \n"
		"}\n"
		"vec2 WinToGl(vec2 pos) \n"
		"{\n"
		"   vec2 ret;\n"
		"   ret.x=2.0*pos.x / s_vpWidth- 1.0;\n"
		"   ret.y=-2.0*pos.y / s_vpHeight + 1.0;\n"
		"   return ret;\n"
		"}\n"
		"void main()                  \n"
		"{                            \n"
		"	vec2 pos;\n"
		"	pos=GlToWin(a_position)-s_posA;			\n"
		"	pos=(s_rotAB*vec3(pos,1)).xy;	\n"
		"	pos=vec2(pos.x/s_lenAB,0.5);	\n"
		"   gl_Position =vec4( a_position,1,1); \n"
		"   v_texCoord = pos;  \n"
		"}                            \n";

	const char fShaderStr[] =
		"precision mediump float;                            \n"
		"varying vec2 v_texCoord;                            \n"
		"uniform sampler2D s_texture;                        \n"
		"uniform float s_alpha;								 \n"
		"void main()                                         \n"
		"{                                                   \n"
		"vec2 vtexcoo=vec2(clamp(v_texCoord.x,0.0,1.0),0.5);\n"
		"vec4 outcolor=texture2D( s_texture, vtexcoo );\n"
		"outcolor.a*=s_alpha;								\n"
		"  gl_FragColor = outcolor							;\n"
		"}                                                   \n";

	m_Program = loadProgram(vShaderStr, fShaderStr);
	return m_Program != 0;
}

void OpenglContext::LinearGradientProgram::Uninit()
{
	if (m_Program)glDeleteProgram(m_Program);
	m_Program = 0;
}

void OpenglContext::LinearGradientProgram::SetTextureUnit(GLint i)
{
	GLuint samplerLoc = glGetUniformLocation(OpenglContext::s_LinearGradientProgram, "s_texture");
	glUniform1i(samplerLoc, 0);
}
void OpenglContext::LinearGradientProgram::SetVertexPosition(GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* ptr)
{
	GLuint positionLoc = glGetAttribLocation(OpenglContext::s_TextureProgram, "a_position");
	glVertexAttribPointer(positionLoc, size, type, normalized, stride, ptr);
	glEnableVertexAttribArray(positionLoc);
}


void OpenglContext::LinearGradientProgram::SetPosition(DXPOINT A, DXPOINT B,float viewportWidth,float viewportHeight)//AB是屏幕坐标系
{
	//算法：任意点的纹理坐标＝点到AB的垂线交点，这个点到A的距离相当于以A为原点将AB旋转到跟X轴平行后任意点的X坐标
	DXPOINT vecAB = B - A;
	float angle;

	
	angle = atan2f(vecAB.y , vecAB.x );
	
	float lenAB = vecAB.GetLength();
	MATRIX matrot;
	matrot.SetRotate(angle);

	GLuint loc = glGetUniformLocation(OpenglContext::s_LinearGradientProgram, "s_posA");
	glUniform2fv(loc, 1, (float*)&A);
	assert(!glGetError());

	loc = glGetUniformLocation(OpenglContext::s_LinearGradientProgram, "s_lenAB");
	glUniform1f(loc, lenAB);
	assert(!glGetError());
	loc = glGetUniformLocation(OpenglContext::s_LinearGradientProgram, "s_vpWidth");
	glUniform1f(loc, viewportWidth);
	assert(!glGetError());
	loc = glGetUniformLocation(OpenglContext::s_LinearGradientProgram, "s_vpHeight");
	glUniform1f(loc, viewportHeight);
	assert(!glGetError());
	loc = glGetUniformLocation(OpenglContext::s_LinearGradientProgram, "s_rotAB");
	glUniformMatrix3fv(loc, 1, GL_FALSE, (float*)&matrot);
	assert(!glGetError());
}

void OpenglContext::LinearGradientProgram::SetAlpha(float fAlpha)
{
	GLuint samplerLoc = glGetUniformLocation(OpenglContext::s_LinearGradientProgram, "s_alpha");
	glUniform1f(samplerLoc, fAlpha);
	assert(!glGetError());
}



bool OpenglContext::InitProgram()
{
	s_SingleColorProgram.Init();
	s_TextureProgram.Init();
	s_LinearGradientProgram.Init();
	return true;
}

bool OpenglContext::Cleanup()
{
	s_SingleColorProgram.Uninit();
	s_TextureProgram.Uninit();
	s_LinearGradientProgram.Uninit();
	return true;
}

void OpenglContext::SetViewPort(int x, int y, int width, int height)
{

	s_rcViewPort.x = x;
	s_rcViewPort.y = y;
	s_rcViewPort.width = width;
	s_rcViewPort.height = height;
	return;
	// 	for (std::list<OpenglFBO*>::iterator it = OpenglFBO::s_fboList.begin(); it != OpenglFBO::s_fboList.end(); ++it)
	// 	{
	// 		(*it)->Init(width, height);
	// 	}
}

int OpenglContext::GetX()
{
	return s_rcViewPort.x;
}

int OpenglContext::GetY()
{
	return s_rcViewPort.y;
}

int OpenglContext::GetWidth()
{
	return s_rcViewPort.width;
}

int OpenglContext::GetHeight()
{
	return s_rcViewPort.height;
}



OpenglContext::VIEWPORT OpenglContext::s_rcViewPort;
