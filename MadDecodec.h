#pragma once
#include "libmad-0.15.1b/mad.h"


//将内存当文件读写
class MemMapFile
{
public:
	uint8_t*m_pBuffer;
	int m_nBufferLen;
	int m_nPosition;
	int Read(void*pBuf, int nLen);
	void SetBuffer(void*pBuf, int nLen);
	
};

class MadDecodec
{
public:
	struct mad_stream    stream;
	struct mad_frame    frame;
	struct mad_synth    synth;
	struct mad_header   header;
	mad_timer_t            timer;
	bool bFromBuffer = false;
	FILE* file;
	long readsize;


	MadDecodec();
	~MadDecodec();
	bool open(const char* filename);
	bool openBuffer(void*pBuffer, int nLen);
	int setup();
	int setup(unsigned char*pBuffer, int nLen);
	long readFrame(unsigned char* pcm);

};

