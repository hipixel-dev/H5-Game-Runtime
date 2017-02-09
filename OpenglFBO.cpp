#include "stdafx.h"
#include "OpenglFBO.h"
#include "HtmlCanvas.h"




OpenglFBO::OpenglFBO()
{
	s_fboList.push_back(this);
}


OpenglFBO::~OpenglFBO()
{
	for (std::list<OpenglFBO*>::iterator it = s_fboList.begin(); it != s_fboList.end(); ++it)
	{
		if (*it == this)
		{
			s_fboList.erase(it);
			break;
		}
	}

	EndFBO();

	Uninit();
}

bool OpenglFBO::DrawFBO(int x, int y)
{
    /*
    int devicePixelRatio = JSVM::GetInstance()->GetSystemInterface()->getDevicePixelRatio();
	int nScreenWidth = OpenglContext::GetWidth() * devicePixelRatio;
	int nScreenHeight = OpenglContext::GetHeight() * devicePixelRatio;
     */
    
    int nScreenWidth = OpenglContext::GetWidth();
    int nScreenHeight = OpenglContext::GetHeight();

// 	float fwidth = 2.0f*m_width / nScreenWidth ;
// 	float fheight = 2.0f*m_height / nScreenHeight ;
// 	float fleft = 2.0f*x / nScreenWidth-1 ;
// 	float ftop = 1 - 2.0 * y / nScreenHeight;
// 
// 
// 	GLfloat vVertices[] = { fleft , ftop,  // Position 0
// 		0.0f, 1.0f,        // TexCoord 0 
// 		fleft , ftop-fheight,   // Position 1
// 		0.0f, 0.0f,        // TexCoord 1
// 		fleft +  fwidth, ftop - fheight,  // Position 2
// 		1.0f, 0.0f,        // TexCoord 2
// 		fleft +  fwidth, ftop,   // Position 3
// 		1.0f, 1.0f         // TexCoord 3
// 	};

	GLfloat vVertices[] = { -1, 1,  // Position 0
		0.0f, 1.0f,        // TexCoord 0 
		-1, -1,   // Position 1
		0.0f, 0.0f,        // TexCoord 1
		1, -1,  // Position 2
		1.0f, 0.0f,        // TexCoord 2
		1,1,   // Position 3
		1.0f, 1.0f         // TexCoord 3
	};
	// Set the viewport

	glViewport(x, nScreenHeight - y - m_height, m_width, m_height);
	assert(!gl.GetError());


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Set the viewport
// 	glViewport(0,0,nScreenWidth,nScreenHeight);
// 	assert(!glGetError());
	// Use the program object
	glUseProgram(OpenglContext::s_TextureProgram);
	assert(!glGetError());
	// Load the vertex position
	OpenglContext::s_TextureProgram.SetVertexPosition(2, GL_FLOAT,GL_FALSE, 4 * sizeof(GLfloat), vVertices);
	assert(!glGetError());
	OpenglContext::s_TextureProgram.SetTexturePosition(2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), &vVertices[2]);
	assert(!glGetError());
	OpenglContext::s_TextureProgram.SetAlpha(1);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	assert(!glGetError());
	glBindTexture(GL_TEXTURE_2D, m_FboTexture);
	assert(!glGetError());
	// Set the sampler texture unit to 0
	OpenglContext::s_TextureProgram.SetTextureUnit(0);
	assert(!glGetError());

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	assert(!glGetError());
	glBindTexture(GL_TEXTURE_2D, 0);
	assert(!glGetError());
	glUseProgram(0);
	assert(!glGetError());
	return true;
}

bool OpenglFBO::Init(int width, int height)
{
	Uninit();
	m_width = width;
	m_height = height;
	glGenFramebuffers(1, &m_FrameBuffer);
	assert(!gl.GetError());
	if (m_FrameBuffer == 0)return false;
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
	assert(!gl.GetError());
	glGenTextures(1, &m_FboTexture);
	assert(!gl.GetError());
	glBindTexture(GL_TEXTURE_2D, m_FboTexture);
	assert(!gl.GetError());
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
	assert(!gl.GetError());
	glBindTexture(GL_TEXTURE_2D, 0);
	assert(!gl.GetError());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FboTexture, 0);
	assert(!gl.GetError());


	glGenRenderbuffers(1, &m_depthbuffer);
	assert(!gl.GetError());
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
	assert(!gl.GetError());
// 	GLint maxRenderbufferSize;
// 	glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, &maxRenderbufferSize);//得到可以的最大的rbo大小。
#define GL_DEPTH24_STENCIL8 0x88F0
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	assert(!gl.GetError());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);
	assert(!gl.GetError());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthbuffer);
	assert(!gl.GetError());

	





	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glClearColor(0, 0, 0, 0);
	assert(!gl.GetError());
	glClearDepthf(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	assert(!gl.GetError());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	assert(!gl.GetError());
	 switch( status )
	 {
	   case GL_FRAMEBUFFER_COMPLETE:
		   return true;
		   break;

	   case GL_FRAMEBUFFER_UNSUPPORTED:
		   assert(0);
		   return false;
		   break;
	}

	 
	 //////////////////////////////////////////////////////////////////////////
	 

	

	return true;
}

bool OpenglFBO::Uninit()
{
	if (!IsInited())return true;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	assert(!gl.GetError());
	glActiveTexture(GL_TEXTURE0);
	assert(!gl.GetError());
	glBindTexture(GL_TEXTURE_2D, 0);
	assert(!gl.GetError());
	if (m_FboTexture)
		glDeleteTextures(1, &m_FboTexture);
	m_FboTexture = 0;
	if (m_FrameBuffer)
		glDeleteFramebuffers(1, &m_FrameBuffer);
	m_FrameBuffer = 0;
	if (m_depthbuffer)
		glDeleteRenderbuffers(1, &m_depthbuffer);
	m_depthbuffer = 0;
	return true;
}

bool OpenglFBO::BeginFBO()
{
	assert(m_FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

 //	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
// 	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
// 	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
// 	glClear(GL_COLOR_BUFFER_BIT);
//	assert(!glGetError());
//	glViewport(0, 0, m_width, m_height);
	assert(!glGetError());
	
	return true;
}



bool OpenglFBO::EndFBO()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glBindRenderbuffer(GL_RENDERBUFFER, 0);
//	assert(!glGetError());
	return true;
}

void OpenglFBO::GetImageData(int x, int y, int width, int height, uint8_t* pData)
{//原点在左下角，要转换
	y = m_height - y - height;
	//目前是否绑定由外面手动调用
// 	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
// 	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
//	assert(!glGetError());
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	assert(!glGetError());
	glReadPixels(x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	assert(!glGetError());
// 	glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 	glBindRenderbuffer(GL_RENDERBUFFER, 0);
// 	assert(!glGetError());
	//将上下翻转
	uint32_t*p = (uint32_t*)pData;
	int halfy = height / 2;
	uint32_t*pbuf = new uint32_t[width];
	for (int y = 0; y < halfy; y++)
	{
		memcpy(pbuf, p + y*width, width * 4);
		memcpy(p + y*width, p + (height - y - 1)*width, width * 4);
		memcpy(p + (height - y - 1)*width, pbuf, width * 4);
	}
	delete[]pbuf;
}

bool OpenglFBO::IsInited()
{
	return m_FrameBuffer != 0;
}

std::list<OpenglFBO*> OpenglFBO::s_fboList;
