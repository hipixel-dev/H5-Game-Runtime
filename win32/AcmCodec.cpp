#include "StdAfx.h"
#include "AcmCodec.h"
#include "..\ArrayBuffer.h"
#pragma warning(disable:4312)
#pragma warning(disable:4311)


CAcmCodec::CAcmCodec(void)
{
	memset(&m_wfSrcPCM,0,sizeof(m_wfSrcPCM));
	m_hadid=0;

	m_pwfMP3=0;
	m_pwfPCM=0;

	m_hstrPCM=0;
	m_hstrPCMtoMP3=0;
	m_had=0;
}

CAcmCodec::~CAcmCodec(void)
{
	Uninit();
}

BOOL CALLBACK CAcmCodec::find_format_enum(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport)//LPACMFORMATDETAILS pafd指向ACM格式详情的指针
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;//FIND_DRIVER_INFO类型的指针 指向驱动信息
	OutputDebugString(pafd->szFormat);
	char n[15];
	sprintf_s(n,sizeof(n), "%d", pafd->dwFormatTag);
	OutputDebugString(n);

	OutputDebugString("\n");
	if (pafd->dwFormatTag == (DWORD)pdi->wFormatTag) {//如果详情中的格式标签==驱动信息指针pdi中的格式标签
		// found it如果能找到
		if(pdi->pwfSrc)
		{
			if(pafd->pwfx->nSamplesPerSec==pdi->pwfSrc->nSamplesPerSec&&
				pafd->pwfx->nChannels==pdi->pwfSrc->nChannels)
			{
				if(pafd->dwFormatTag==WAVE_FORMAT_PCM)
				{
					if(pafd->pwfx->wBitsPerSample==pdi->pwfSrc->wBitsPerSample)
					{
						pdi->hadid = hadid;
						return FALSE; // stop enumerating停止枚举
					}
				}
				else
				{
					pdi->hadid = hadid;
					WAVEFORMATEX*pwf=(WAVEFORMATEX*)new BYTE[pdi->npwfDestSize];
					memcpy(pwf,pafd->pwfx,pdi->npwfDestSize);
					pdi->arpwfDest.push_back(pwf);
					return TRUE;
				}
			}
		}
		else
		{
			pdi->hadid = hadid;
			return FALSE; // stop enumerating停止枚举
		}
	}
	return TRUE; // continue enumerating继续枚举
}

BOOL CALLBACK CAcmCodec::find_driver_enum(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport)
{
	FIND_DRIVER_INFO* pdi = (FIND_DRIVER_INFO*) dwInstance;
	// open the driver打开驱动
	HACMDRIVER had = NULL;//HACMDRIVER类型的变量 had 指向ACM驱动的句柄
	MMRESULT mmr = acmDriverOpen(&had, hadid, 0);
	if (mmr) {
		// some error错误
		return FALSE; // stop enumerating停止枚举
	}
	// enumerate the formats it supports枚举它支持的格式
	DWORD dwSize = 0;
	mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
	if (dwSize < sizeof(WAVEFORMATEX)) dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
	WAVEFORMATEX* pwf = (WAVEFORMATEX*) new BYTE[dwSize];
	memset(pwf, 0, dwSize);
	pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
	pwf->wFormatTag = pdi->wFormatTag;
	ACMFORMATDETAILS fd;
	memset(&fd, 0, sizeof(fd));
	fd.cbStruct = sizeof(fd);
	fd.pwfx = pwf;
	fd.cbwfx = dwSize;
	fd.dwFormatTag = pdi->wFormatTag;
	mmr = acmFormatEnum(had, &fd, find_format_enum, (DWORD)(VOID*)pdi, 0);  
	delete []pwf;
	acmDriverClose(had, 0);
	if (pdi->hadid || mmr) {
		// found it or some error找到了 或 有错误
		return FALSE; // stop enumerating停止枚举
	}
	return TRUE; // continue enumeration继续枚举
}

HACMDRIVERID CAcmCodec::find_driver(WORD wFormatTag)
{
	FIND_DRIVER_INFO fdi;
	fdi.hadid = NULL;
	fdi.wFormatTag = wFormatTag;
	MMRESULT mmr = acmDriverEnum(find_driver_enum, (DWORD)(VOID*)&fdi, 0); 
	if (mmr) return NULL;
	return fdi.hadid;
}

WAVEFORMATEX* CAcmCodec::get_driver_format(HACMDRIVERID hadid, WORD wFormatTag,WAVEFORMATEX *pwfSrc)
{
	// open the driver打开驱动
	HACMDRIVER had = NULL;
	MMRESULT mmr = acmDriverOpen(&had, hadid, 0);
	if (mmr)
	{
		return NULL;
	}
	// allocate a structure for the info
	//为信息分配一个结构
	DWORD dwSize = 0;
	mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
	if (dwSize < sizeof(WAVEFORMATEX)) dwSize = sizeof(WAVEFORMATEX); // for MS-PCM
	WAVEFORMATEX* pwf = (WAVEFORMATEX*) new BYTE[dwSize];
	memset(pwf, 0, dwSize);
	pwf->cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
	pwf->wFormatTag = wFormatTag;
	ACMFORMATDETAILS fd;
	memset(&fd, 0, sizeof(fd));
	fd.cbStruct = sizeof(fd);
	fd.pwfx = pwf;
	fd.cbwfx = dwSize;
	fd.dwFormatTag = wFormatTag;
	// set up a struct to control the enumeration
	//建立一个结构用于控制枚举
	FIND_DRIVER_INFO fdi;
	fdi.hadid = NULL;
	fdi.wFormatTag = wFormatTag;
	fdi.pwfSrc=pwfSrc;
	fdi.npwfDestSize=fd.cbwfx;
	mmr = acmFormatEnum(had, &fd, find_format_enum, (DWORD)(VOID*)&fdi, 0);  
	acmDriverClose(had, 0);
	if ((fdi.hadid == NULL) || mmr)
	{
		delete []pwf;
		return NULL;
	}
	if(fdi.arpwfDest.size()>=1)
		memcpy(pwf,fdi.arpwfDest[fdi.arpwfDest.size()-1],fd.cbwfx);

	return pwf;
}

BOOL CAcmCodec::Init(DWORD nSamplesPerSec,WORD wBitsPerSample,WORD nChannels,WORD wFormatTag)
{
	memset(&m_wfSrcPCM, 0, sizeof(m_wfSrcPCM));
	m_wfSrcPCM.cbSize = 0;
	m_wfSrcPCM.wFormatTag = WAVE_FORMAT_PCM; // pcm
	m_wfSrcPCM.nChannels = nChannels; // mono单声道
	m_wfSrcPCM.nSamplesPerSec = nSamplesPerSec; // 11.025 kHz
	m_wfSrcPCM.wBitsPerSample = wBitsPerSample; // 8 bit
	m_wfSrcPCM.nBlockAlign = m_wfSrcPCM.nChannels * m_wfSrcPCM.wBitsPerSample / 8;
	m_wfSrcPCM.nAvgBytesPerSec = m_wfSrcPCM.nSamplesPerSec * m_wfSrcPCM.nBlockAlign;


	m_hadid = find_driver(wFormatTag);
	if (m_hadid == NULL) {
		printf("No driver found\n");
		return FALSE;
	}

	// show some information about the driver
	//显示这个驱动的一些信息
	ACMDRIVERDETAILS dd;
	dd.cbStruct = sizeof(dd);
	MMRESULT mmr = acmDriverDetails(m_hadid, &dd, 0);
	printf("     Short name: %s\n", dd.szShortName);
	printf("     Long name:    %s\n", dd.szLongName);
	printf("     Copyright:    %s\n", dd.szCopyright);
	printf("     Licensing:    %s\n", dd.szLicensing);
	printf("     Features:     %s\n", dd.szFeatures);


	// get the details of the format取得格式的详情
	// Note: this is just the first of one or more possible formats for the given tag
	//注意：也许有多个目标格式支持给定的格式，而这只是第一个。
	m_pwfMP3 = get_driver_format(m_hadid, wFormatTag,&m_wfSrcPCM);
	if (m_pwfMP3 == NULL) {
		printf("Error getting format info\n");
		return FALSE;
	}
	printf("Driver format: %u bits, %lu samples per second\n", m_pwfMP3->wBitsPerSample, m_pwfMP3->nSamplesPerSec);
	// get a PCM format tag the driver supports
	//取得一个驱动支持的PCM格式标签
	// Note: we just pick the first supported PCM format which might not really
	//注意：我们只是选取了第一个支持PCM格式的，也许这个不是最好的选择。
	// be the best choice.
	m_pwfPCM = get_driver_format(m_hadid, WAVE_FORMAT_PCM,&m_wfSrcPCM);
	if (m_pwfPCM == NULL) {
		printf("Error getting PCM format info\n");
		return FALSE;
	}
	printf("PCM format: %u bits, %lu samples per second\n", m_pwfPCM->wBitsPerSample, m_pwfPCM->nSamplesPerSec);


	////////////////////////////////////////////////////////////////////////////////
	// convert the source wave to the PCM format supported by the CODEC
	//转换源wave到编码器支持的PCM格式
	// we use any driver that can do the PCM to PCM conversion
	//我们使用任何可以完成PCM到PCM转换的驱动

	mmr = acmStreamOpen(&m_hstrPCM,
		NULL, // any driver任意驱动
		&m_wfSrcPCM, // source format源格式
		m_pwfPCM, // destination format目标格式
		NULL, // no filter无过滤
		NULL, // no callback无回调
		0, // instance data (not used)实例数据(未使用)
		ACM_STREAMOPENF_NONREALTIME); // flags
	if (mmr) {
		printf("Failed to open a stream to do PCM to PCM conversion\n");
		return FALSE;
	}



	///////////////////////////////////////////////////////////////////////////////////
	// convert the intermediate PCM format to the final format
	// open the driver

	mmr = acmDriverOpen(&m_had, m_hadid, 0);
	if (mmr) {
		printf("Failed to open driver\n");
		return FALSE;
	}
	// open the conversion stream
	// Note the use of the ACM_STREAMOPENF_NONREALTIME flag. Without this
	// some software compressors will report error 512 - not possible
	mmr = acmStreamOpen(&m_hstrPCMtoMP3,
		m_had, // driver handle
		m_pwfPCM, // source format
		m_pwfMP3, // destination format
		NULL, // no filter
		NULL, // no callback
		0, // instance data (not used)
		ACM_STREAMOPENF_NONREALTIME); // flags
	if (mmr) {
		printf("Failed to open a stream to do PCM to driver format conversion\n");
		return FALSE;
	}



	DWORD dwSrcBytes=100000;
	DWORD dwDst1Bytes=100000;
	DWORD dwDst2Bytes=100000;

	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = new BYTE[dwSrcBytes]; // the source data to convert
	strhdr.cbSrcLength = dwSrcBytes;
	strhdr.pbDst = new BYTE[dwDst1Bytes];
	strhdr.cbDstLength = dwDst1Bytes;
	// prep the header
	mmr = acmStreamPrepareHeader(m_hstrPCM, &strhdr, 0);


	memset(&strhdr2, 0, sizeof(strhdr2));
	strhdr2.cbStruct = sizeof(strhdr2);
	strhdr2.pbSrc = strhdr.pbDst; // the source data to convert
	strhdr2.cbSrcLength = dwDst1Bytes;
	strhdr2.pbDst = new BYTE[dwDst2Bytes];
	strhdr2.cbDstLength = dwDst2Bytes;
	// prep the header准备wav头
	mmr = acmStreamPrepareHeader(m_hstrPCMtoMP3, &strhdr2, 0);

	return TRUE;
}

void CAcmCodec::Uninit()
{
	acmStreamUnprepareHeader(m_hstrPCMtoMP3,&strhdr,0);
	acmStreamUnprepareHeader(m_hstrPCMtoMP3,&strhdr2,0);
	SAFE_DELETE_ARRAY(strhdr.pbSrc);
	SAFE_DELETE_ARRAY(strhdr.pbDst);
	SAFE_DELETE_ARRAY(strhdr2.pbDst);

	acmStreamClose(m_hstrPCM, 0);
	acmStreamClose(m_hstrPCMtoMP3,0);
	acmDriverClose(m_had,0);
	m_hstrPCM=0;
	m_hstrPCMtoMP3=0;
	m_had=0;
	delete m_pwfMP3;
	delete m_pwfPCM;
	m_pwfMP3=0;
	m_pwfPCM=0;



}

BOOL CAcmCodec::Encode(BYTE*pSrcData,DWORD dwSrcBytes,OUT BYTE*&pBufferOut,OUT DWORD&nOutlen)
{
	if(!m_had)return FALSE;


	memcpy(strhdr.pbSrc,pSrcData,dwSrcBytes);
	strhdr.cbSrcLength=dwSrcBytes;
	MMRESULT mmr = acmStreamConvert(m_hstrPCM, &strhdr, 0);
	if (mmr) {
		printf("Failed to do PCM to PCM conversion\n");
		return FALSE;
	}
	printf("Converted OK\n");



	strhdr2.cbSrcLength=strhdr.cbDstLengthUsed;
	printf("Converting to final format...\n");
	mmr = acmStreamConvert(m_hstrPCMtoMP3, &strhdr2, 0);
	if (mmr) {
		printf("Failed to do PCM to driver format conversion\n");
		return FALSE;
	}
	printf("Converted OK\n");
	//		acmStreamUnprepareHeader(m_hstrPCMtoMP3,&strhdr2,0);

	//		delete[]pDst1Data;
	pBufferOut=strhdr2.pbDst;
	nOutlen=strhdr2.cbDstLengthUsed;

	return TRUE;

}

CAcmDecodec::CAcmDecodec()
{
	m_hadid=0;
	m_hstrMP3toPCM=0;
	m_had=0;
	memset(&strhdr,0,sizeof(strhdr));
}
CAcmDecodec::~CAcmDecodec()
{
	Uninit();
}

BOOL CAcmDecodec::Init(WAVEFORMATEX*pSrcMP3,WAVEFORMATEX*pDestPCM,WORD wFormatTag)
{
	Uninit();
	//		m_wfSrcMP3=*pSrcMP3;

	m_hadid = CAcmCodec::find_driver(wFormatTag);
	if (m_hadid == NULL) {
		printf("No driver found\n");
		return FALSE;
	}

	// show some information about the driver
	//显示这个驱动的一些信息
	ACMDRIVERDETAILS dd;
	dd.cbStruct = sizeof(dd);
	MMRESULT mmr = acmDriverDetails(m_hadid, &dd, 0);
	printf("     Short name: %s\n", dd.szShortName);
	printf("     Long name:    %s\n", dd.szLongName);
	printf("     Copyright:    %s\n", dd.szCopyright);
	printf("     Licensing:    %s\n", dd.szLicensing);
	printf("     Features:     %s\n", dd.szFeatures);


	//		m_wfDestPCM=*pDestPCM;

	mmr = acmDriverOpen(&m_had, m_hadid, 0);
	if (mmr) {
		printf("Failed to open driver\n");
		return FALSE;
	}
	PMPEGLAYER3WAVEFORMAT pmp3=(PMPEGLAYER3WAVEFORMAT)pSrcMP3;
	mmr = acmStreamOpen(&m_hstrMP3toPCM,
		NULL, // any driver任意驱动
		pSrcMP3, // source format源格式
		pDestPCM, // destination format目标格式
		NULL, // no filter无过滤
		NULL, // no callback无回调
		0, // instance data (not used)实例数据(未使用)
		ACM_STREAMOPENF_NONREALTIME); // flags
	if (mmr) {
		printf("Failed to open a stream to do PCM to PCM conversion\n");
		return FALSE;
	}



	DWORD dwSrcBytes=100000;
	DWORD dwDestBytes=100000;

	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = new BYTE[dwSrcBytes]; // the source data to convert
	strhdr.cbSrcLength = dwSrcBytes;
	strhdr.pbDst = new BYTE[dwDestBytes];
	strhdr.cbDstLength = dwDestBytes;

	// prep the header
	mmr = acmStreamPrepareHeader(m_hstrMP3toPCM, &strhdr, 0);


	return TRUE;

}

DWORD CAcmDecodec::Decode(BYTE*pSrcData,DWORD dwSrcBytes/*,DWORD dwOrigBytes*/, OUT BYTE*&pBufferOut,OUT DWORD&nOutlen)
{
	if(!m_had)return FALSE;


	memcpy(strhdr.pbSrc,pSrcData,dwSrcBytes);
	strhdr.cbSrcLength=dwSrcBytes;

//	printf("Converting to intermediate PCM format...\n");
	MMRESULT mmr = acmStreamConvert(m_hstrMP3toPCM, &strhdr, ACM_STREAMCONVERTF_BLOCKALIGN);
	if (mmr) {
		printf("Failed to do MP3 to PCM conversion\n");
		return FALSE;
	}
//	printf("Converted OK\n");
	//		acmStreamUnprepareHeader(m_hstrMP3toPCM,&strhdr,0);

	pBufferOut=strhdr.pbDst;
	nOutlen=strhdr.cbDstLengthUsed;

	return strhdr.cbSrcLengthUsed;

}

void CAcmDecodec::Uninit()
{
	if(m_hstrMP3toPCM)acmStreamUnprepareHeader(m_hstrMP3toPCM,&strhdr,0);
	if(strhdr.pbSrc)delete[]strhdr.pbSrc;
	if(strhdr.pbDst)delete[]strhdr.pbDst;
	strhdr.pbSrc=NULL;
	strhdr.pbDst=NULL;

	if(m_hstrMP3toPCM)acmStreamClose(m_hstrMP3toPCM, 0);
	m_hstrMP3toPCM=NULL;
	if(m_had)acmDriverClose(m_had,0);
	m_had=NULL;



}

bool CAcmDecodec::DecodeFromFile(const char*szFile, OUT BYTE*&pBufferOut, OUT DWORD&nOutlen)
{
	FileIOInterface*fileio=JSVM::GetInstance()->GetFileIOInterface();
	FileHandle*file = fileio->Open(szFile, FileIOInterface::OP_READ);
	if (!file)return false;

	int len = fileio->GetLength(file);
	BYTE*pbuf = new BYTE[len];
	fileio->Read(file,pbuf, len);
	fileio->Close(file);
	do
	{
		BYTE*p = pbuf;
		if (p[0] == 'T'&&p[1] == 'A'&&p[2] == 'G')//ID3V1
		{
			p += 128;
		}
		else if (p[0] == 'I'&&p[1] == 'D'&&p[2] == '3')//ID3V2
		{
			p += 3;
			BYTE idver = *p;
			p += 3;
			DWORD tagsize = (p[0] << 21) | (p[1] << 14) | (p[2] << 7) | p[3];
			p += tagsize + 4;
		}
		while (p - pbuf < len&&*p != 0xff)
			p++;
		if (p - pbuf >= len)break;
		p++;

		//	ASSERT((*p>>5)==7);
		if ((*p >> 5) != 7)break;
		BYTE ver = BYTE(*p << 3) >> 6;
		//	ASSERT(ver==3);//目前只支持MPEG1 LAYER3
		if (ver != 3)break;
		BYTE layer = BYTE(*p << 5) >> 6;
		//	ASSERT(layer==1);
		if (layer != 1)break;
		BYTE crc = *p & 1;
		p++;
		BYTE bps = *p >> 4;
		BYTE freq = BYTE(*p << 4) >> 6;
		BYTE framelen = BYTE(*p << 6) >> 7;
		p++;
		BYTE track = *p >> 6;
		p += 32;

		BYTE*pout;
		DWORD outlen;

		static const int kbps[3][16] = {
			0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0,
			0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384, 0,
			0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 0,

		};
		static const int hz[] = { 44100, 48000, 32000, 0 };


		MPEGLAYER3WAVEFORMAT wfMP3 = {};
		wfMP3.wfx.wFormatTag = 85;
		wfMP3.wfx.nChannels = (track == 3) ? 1 : 2;
		wfMP3.wfx.nSamplesPerSec = hz[freq];
		wfMP3.wfx.nAvgBytesPerSec = kbps[2][bps] * 1024 / 8;
		wfMP3.wfx.nBlockAlign = 1;
		wfMP3.wfx.wBitsPerSample = 0;
		wfMP3.wfx.cbSize = 12;
		wfMP3.wID = 1;
		wfMP3.fdwFlags = 2;
		wfMP3.nBlockSize = 1;
		wfMP3.nFramesPerBlock = 1;
		wfMP3.nCodecDelay = 1393;

		WAVEFORMATEX wfPCM = {};
		wfPCM.wFormatTag = 1;
		wfPCM.nChannels = 2;
		wfPCM.nSamplesPerSec = 44100;
		wfPCM.nAvgBytesPerSec = 176400;
		wfPCM.nBlockAlign = 4;
		wfPCM.wBitsPerSample = 16;
		wfPCM.cbSize = 0;

		CAcmDecodec dec;
		if (!dec.Init((WAVEFORMATEX*)&wfMP3, &wfPCM))
			break;
	

		BYTE*outbuf;
		BOOL bSleep = FALSE;
		DWORD tim = GetTickCount();
		int nnnn = 0;
		

		CArrayBuffer pcmbuf;


		while (p - pbuf < len)
		{
			int l = len - (p - pbuf);
			if (l>3000)l = 3000;
			l = dec.Decode(p, l, outbuf, outlen);
			if (outlen <= 0)goto next;

			pcmbuf.AddData(outbuf, outlen);
			
		next:
			p += l;

		}
		pBufferOut = new BYTE[pcmbuf.GetLength()];
		memcpy(pBufferOut, pcmbuf.GetBuffer(), pcmbuf.GetLength());
		nOutlen = pcmbuf.GetLength();
		dec.Uninit();
	} while (FALSE);
	delete[]pbuf;

	return true;
}

