#pragma once

#include <MMSYSTEM.H>
#include <mmreg.h>
#include <msacm.H>
#pragma comment(lib,"Msacm32.lib")

// Locate a driver that supports a given format and return its ID
//寻找一个支持给定格式的驱动并返回其ID
struct FIND_DRIVER_INFO{
	HACMDRIVERID hadid;//指向HACMDRIVERID的句柄
	WORD wFormatTag;//32位无符号整型 格式标签
	WAVEFORMATEX *pwfSrc;//要匹配的格式
//	DWORD nAvgBytesPerSec;//枚举时保存上次枚举到的码率
	int npwfDestSize;//目标格式的sizeof
	std::vector<WAVEFORMATEX*>arpwfDest;//匹配的所有格式
	FIND_DRIVER_INFO()
	{
		hadid=0;
		wFormatTag=0;
		pwfSrc=0;
		npwfDestSize=0;
//		nAvgBytesPerSec=0xffffffff;
	}
	~FIND_DRIVER_INFO()
	{
		for(int i=0;i<arpwfDest.size();i++)
		{
			delete arpwfDest[i];
		}
		arpwfDest.clear();
	}
} ;//结构FIND_DRIVER_INFO包含2个元素:HACMDRIVERID hadid 和WORD wFormatTag

//使用ACM音频编码器
class  CAcmCodec
{
public:
	CAcmCodec(void);
	~CAcmCodec(void);

protected:
	WAVEFORMATEX m_wfSrcPCM;//Encode输入的PCM格式
	HACMDRIVERID m_hadid;

	WAVEFORMATEX* m_pwfMP3;//Encode输出的MP3格式(自动根据m_wfSrcPCM匹配)
	WAVEFORMATEX* m_pwfPCM;//Encode时将输入的PCM格式转成编码器可以用的PCM格式

	HACMSTREAM m_hstrPCM;
	HACMSTREAM m_hstrPCMtoMP3;

	HACMDRIVER m_had ;

	ACMSTREAMHEADER strhdr;
	ACMSTREAMHEADER strhdr2;



	// callback function for format enumeration
	//用来枚举格式的回调函数
	static BOOL CALLBACK find_format_enum(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport);//LPACMFORMATDETAILS pafd指向ACM格式详情的指针


	// callback function for driver enumeration
	//用来枚举驱动的回调函数
	static BOOL CALLBACK find_driver_enum(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport);

	// locate the first driver that supports a given format tag
	//寻找支持给定格式标签的第一个驱动

	// get a description of the first format supported for a given tag
	//获取驱动的描述 这个驱动是第一个支持给定标签的驱动。
	WAVEFORMATEX* get_driver_format(HACMDRIVERID hadid, WORD wFormatTag,WAVEFORMATEX *pwfSrc);

public:
	static HACMDRIVERID find_driver(WORD wFormatTag);

	BOOL Init(DWORD nSamplesPerSec=22050,WORD wBitsPerSample=16,WORD nChannels=2,WORD wFormatTag = WAVE_FORMAT_MPEGLAYER3);

	void Uninit();

	BOOL Encode(BYTE*pSrcData,DWORD dwSrcBytes,OUT BYTE*&pBufferOut,OUT DWORD&nOutlen);

};

//////////////////////////////////////////////////////////////////////////
class  CAcmDecodec
{
protected:
// 	WAVEFORMATEX m_wfSrcMP3;//输入的MP3格式
// 	WAVEFORMATEX m_wfDestPCM;//输出的PCM格式
	HACMDRIVERID m_hadid;
	HACMSTREAM m_hstrMP3toPCM;
	HACMDRIVER m_had ;

	ACMSTREAMHEADER strhdr;
public:
	CAcmDecodec();
	~CAcmDecodec();


	BOOL Init(WAVEFORMATEX*pSrcMP3,WAVEFORMATEX*pDestPCM,WORD wFormatTag = WAVE_FORMAT_MPEGLAYER3);

	//解码，返回成功解码的pSrcData长度
	DWORD Decode(BYTE*pSrcData,DWORD dwSrcBytes/*,DWORD dwOrigBytes*/, OUT BYTE*&pBufferOut,OUT DWORD&nOutlen);

	void Uninit();


	bool DecodeFromFile(const char*szFile, OUT BYTE*&pBufferOut, OUT DWORD&nOutlen);

};
