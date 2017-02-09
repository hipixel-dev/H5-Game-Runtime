#include "StdAfx.h"
#include "DSoundOut.h"
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dsound.lib")

#define TM_WRITEAUDIO WM_USER+332



#define SAFERELEASE(x) \
	if(NULL !=x)		\
			 {					\
	 x->Release();	\
	 x=NULL;		    \
				 }


CDSoundOut::CDSoundOut(void)
{
	m_hWnd=NULL;
//	m_PlayCheckThread=NULL;
	m_pDSBuffer8 = NULL; //buffer
	m_pDsd = NULL; //dsound
	m_pDSNotify = NULL; 
//	memset(m_aPosNotify,0,sizeof(m_aPosNotify));
//	memset(m_event,0,sizeof(m_event));
//	m_dwNextWriteOffset = 0;
//	m_bMute=FALSE;
}

CDSoundOut::~CDSoundOut(void)
{
	Uninit();
}



BOOL CDSoundOut::Init(HWND hWnd, LPWAVEFORMATEX pWaveFormat, DWORD dwBufferBytes, BYTE*pData)
{
	m_dwBufferBytes = dwBufferBytes;
	m_hWnd=hWnd;

	//初始化DirectSound
	HRESULT hr;
	if(FAILED(hr = DirectSoundCreate8(NULL,&m_pDsd,NULL)))
		return FALSE;
	if(FAILED(hr = m_pDsd->SetCooperativeLevel(m_hWnd,DSSCL_NORMAL)))
		return FALSE;

	m_WaveFormat=*pWaveFormat;

	DSBUFFERDESC dsbd;
	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes = dwBufferBytes;// MAX_AUDIO_BUF * BUFFERNOTIFYSIZE;
	dsbd.lpwfxFormat = pWaveFormat;
	LPDIRECTSOUNDBUFFER lpbuffer;
	//创建DirectSound辅助缓冲区
	if(FAILED(hr = m_pDsd->CreateSoundBuffer(&dsbd,&lpbuffer,NULL)))
		return FALSE;
	if( FAILED( hr = lpbuffer->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID*) &m_pDSBuffer8) ) )
		return FALSE; 
	lpbuffer->Release();
	//设置DirectSound通知 机制
	m_aPosNotify.dwOffset = dwBufferBytes;
	m_event = ::CreateEvent(NULL, false, false, NULL);
	m_aPosNotify.hEventNotify = m_event;
// 	for(int i =0; i< MAX_AUDIO_BUF;i++)
// 	{
// 		m_aPosNotify[i].dwOffset = i* BUFFERNOTIFYSIZE ;
// 		m_event[i]=::CreateEvent(NULL,false,false,NULL); 
// 		m_aPosNotify[i].hEventNotify = m_event[i];
// 	}

	if(FAILED(hr=m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*) &m_pDSNotify )))
		return FALSE;
	m_pDSNotify->SetNotificationPositions(1/*MAX_AUDIO_BUF*/,&m_aPosNotify);
	//		m_pDSNotify->Release();



	LPVOID lplockbuf;
	DWORD len;
	hr = m_pDSBuffer8->Lock(0, dwBufferBytes, &lplockbuf, &len, NULL, NULL, 0);
	memcpy(lplockbuf, pData, len);
	hr = m_pDSBuffer8->Unlock(lplockbuf, len, NULL, 0);
// 	m_dwNextWriteOffset += len;
// 	m_dwNextWriteOffset %= BUFFERNOTIFYSIZE*MAX_AUDIO_BUF;
//	return nDataLen;

	return TRUE;
}

BOOL CDSoundOut::Uninit()
{
	
	stop();
// 	if(m_PlayCheckThread)
// 		WaitForSingleObject(m_PlayCheckThread,-1);
	SAFERELEASE(m_pDSNotify);
	if (m_event)CloseHandle(m_event);
	m_event = 0;
// 	for(int i =0; i< MAX_AUDIO_BUF;i++)
// 	{
// 		CloseHandle(m_event[i]);
// 	}
	SAFERELEASE(m_pDSBuffer8);
	SAFERELEASE(m_pDsd);

// 	memset(m_aPosNotify,0,sizeof(m_aPosNotify));
// 	memset(m_event,0,sizeof(m_event));
//	m_dwNextWriteOffset = 0;
	m_hWnd=NULL;
	return TRUE;
}
//取得空闲缓冲区长度
// DWORD CDSoundOut::GetFreeBuffer()
// {
// 	HRESULT hr;
// 	DWORD curpos=0;//当前播放位置
// 	DWORD dwNoPlay;//缓冲区中未播放的数据
// 	hr=m_pDSBuffer8->GetCurrentPosition(&curpos,0);
// 	if(m_dwNextWriteOffset>=curpos)
// 		dwNoPlay=m_dwNextWriteOffset-curpos;
// 	else
// 		dwNoPlay=m_dwNextWriteOffset+BUFFERNOTIFYSIZE*MAX_AUDIO_BUF-curpos;
// 	int dwFreeBuf=BUFFERNOTIFYSIZE*MAX_AUDIO_BUF-dwNoPlay;//最多可填入的数据
// 	if(dwFreeBuf<0)dwFreeBuf=0;
// 	return dwFreeBuf;
// }

//写到g_pDSBuffer8
// int CDSoundOut::WriteBuffer(BYTE*pData,int nDataLen)
// {
// 	HRESULT hr;
// 	int nLenFreeTail=BUFFERNOTIFYSIZE*MAX_AUDIO_BUF-m_dwNextWriteOffset;//缓冲区后面的空间
// 	//					int nLenFreeHead;//缓冲区前面的空间
// 	LPVOID lplockbuf;
// 	DWORD len;
// 
// 
// 	DWORD dwFreeBuf=GetFreeBuffer();
// 	nDataLen=min(nDataLen,(int)dwFreeBuf);
// 	if(nDataLen==0)return 0;
// 
// 
// 	if(nDataLen<=nLenFreeTail)//数据可以填到缓冲区后面，不用loop
// 	{
// 		hr=m_pDSBuffer8->Lock(m_dwNextWriteOffset,nDataLen,&lplockbuf,&len,NULL,NULL,0);
// 		memcpy(lplockbuf,pData,len);
// 		hr=m_pDSBuffer8->Unlock(lplockbuf,len,NULL,0);
// 		m_dwNextWriteOffset+=len;
// 		m_dwNextWriteOffset%=BUFFERNOTIFYSIZE*MAX_AUDIO_BUF;
// 		return nDataLen;
// 	}
// 	else//后面的缓冲区不足，要loop
// 	{
// 		//先填后面
// 		int nWrite=0;
// 		hr=m_pDSBuffer8->Lock(m_dwNextWriteOffset,nLenFreeTail,&lplockbuf,&len,NULL,NULL,0);
// 		memcpy(lplockbuf,pData,len);
// 		hr=m_pDSBuffer8->Unlock(lplockbuf,len,NULL,0);
// 		pData+=len;
// 		m_dwNextWriteOffset=0;
// 		nDataLen-=len;
// 		nWrite+=len;
// 		if(nDataLen>0)
// 		{
// 			hr=m_pDSBuffer8->Lock(m_dwNextWriteOffset,nDataLen,&lplockbuf,&len,NULL,NULL,0);
// 			memcpy(lplockbuf,pData,len);
// 			hr=m_pDSBuffer8->Unlock(lplockbuf,len,NULL,0);
// 			m_dwNextWriteOffset=len;
// 			nWrite+=len;
// 		}
// 		return nWrite;
// 	}
// 
// }

void CDSoundOut::start(unsigned long dwFrom,bool bLoop)
{
	HRESULT hr=m_pDSBuffer8->SetCurrentPosition(dwFrom);
	hr=m_pDSBuffer8->Play(0, 0, bLoop?DSBPLAY_LOOPING:0);
//	HRESULT hr=m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,m_aPosNotify);
// 	if(m_PlayCheckThread)return;
// 	m_PlayCheckThread=AfxBeginThread((AFX_THREADPROC)t_PlayCheck,this);

}

void CDSoundOut::stop()
{
	if(!m_pDSBuffer8)return;
	m_pDSBuffer8->Stop();
	m_pDSBuffer8->SetCurrentPosition(0);

// 	if(m_PlayCheckThread)
// 	{
// 		HANDLE hthread=m_PlayCheckThread->m_hThread;
// 		DWORD tid=m_PlayCheckThread->m_nThreadID;
// 		while(1)
// 		{
// 			PostThreadMessage(tid,WM_QUIT,0,0);
// 			if(WaitForSingleObject(hthread,100)!=WAIT_TIMEOUT)
// 				break;
// 
// 		}
// 	}
// 	m_dwNextWriteOffset=0;
// 	m_Buffer.Lock();
// 	m_Buffer.RemoveAll();
// 	m_Buffer.Unlock();
}

void CDSoundOut::pause()
{
	if (!m_pDSBuffer8)return;
	m_pDSBuffer8->Stop();
}

void CDSoundOut::seek(unsigned long dwTo)
{
	if (!m_pDSBuffer8)return;
	m_pDSBuffer8->SetCurrentPosition(dwTo);
}

unsigned long CDSoundOut::GetCurrentPosition()
{
	if (!m_pDSBuffer8)return 0;
	DWORD pos=0, pos2=0;
	HRESULT hr = m_pDSBuffer8->GetCurrentPosition(&pos, &pos2);
	return pos;
}

bool CDSoundOut::IsStop()
{
	if (!m_pDSBuffer8)return true;
	DWORD status = 0;
	HRESULT hr=m_pDSBuffer8->GetStatus(&status);
	if (DSBSTATUS_PLAYING&status)return false;
	return true;
}

// void CDSoundOut::Mute(BOOL bMute)
// {
// 	m_bMute=bMute;
// }

//write结束，将播放完的缓冲区填0,在播放完缓冲区后停止播放
// void CDSoundOut::WriteEnd()
// {
// 	BYTE *pbuf=new BYTE[MAX_AUDIO_BUF * BUFFERNOTIFYSIZE];
// 	memset(pbuf,0,MAX_AUDIO_BUF * BUFFERNOTIFYSIZE);
// 	Write(pbuf,MAX_AUDIO_BUF * BUFFERNOTIFYSIZE);
// 	delete[]pbuf;
// 	stop();
// 
// }

// UINT CDSoundOut::t_PlayCheck(LPVOID pParam)
// {
// 	CDSoundOut*pThis=(CDSoundOut*)pParam;
// 	HRESULT hr;
// 	DWORD dwFillZero=0;//缓冲没数据时填了多少0,当填满时停止播放
// 	while(TRUE)
// 	{
// 		DWORD status;
// 		hr=pThis->m_pDSBuffer8->GetStatus(&status);
// 		if((status&DSBSTATUS_BUFFERLOST)||(status&DSBSTATUS_TERMINATED))
// 		{
// 			pThis->m_Buffer.RemoveAll();
// 			goto theend;
// 		}
// 		DWORD res = MsgWaitForMultipleObjects (MAX_AUDIO_BUF, pThis->m_event, FALSE, INFINITE,QS_ALLEVENTS);
// 		if((res >=WAIT_OBJECT_0)&&(res <WAIT_OBJECT_0+MAX_AUDIO_BUF)) 
// 		{
// line1:
// 			pThis->m_Buffer.Lock();
// 			BYTE*p=pThis->m_Buffer.GetData();
// 			int len=(int)pThis->m_Buffer.GetSize();
// 			if(len==0)//缓冲没数据，填0
// 			{
// 				pThis->m_Buffer.Unlock();
// 
// 				DWORD dwNoPlayLen=BUFFERNOTIFYSIZE*MAX_AUDIO_BUF-pThis->GetFreeBuffer();
// 				if(dwNoPlayLen>BUFFERNOTIFYSIZE)
// 					continue;
// 				
// 				//等待缓冲
// 				pThis->m_pDSBuffer8->Stop();
// 				MSG msg;
// 				while(1)
// 				{
// 					BOOL bQuit=GetMessage( &msg, NULL, 0, 0) ;
// 				
// 					if( msg.message == WM_QUIT )
// 					{
// 						goto theend;
// 					}
// 					else if(msg.message==TM_WRITEAUDIO)
// 					{
// 						//if(pThis->m_Buffer.GetSize()>=BUFFERNOTIFYSIZE*MAX_AUDIO_BUF*4)
// 						{
// 							goto line1;
// 						}
// 					}
// 				}
// 
// 
// 			}
// 			else
// 			{
// 
// 				{
// 					int nWrite=pThis->WriteBuffer(p,len);
// 					pThis->m_Buffer.RemoveData(nWrite);
// 					pThis->m_Buffer.Unlock();
// 				}
// 				
// 				
// 				
// 			}
// 			DWORD status;
// 			HRESULT hr=pThis->m_pDSBuffer8->GetStatus(&status);
// 			if((status&DSBSTATUS_PLAYING)==0)
// 			{
// 				pThis->m_pDSBuffer8->Play(0,0,DSBPLAY_LOOPING);
// 			}
// 		}
// 		else
// 		{
// 			MSG msg;
// 			while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
// 			{ 
// 				if( msg.message == WM_QUIT )
// 				{
// 					goto theend;
// 				}
// 				else if(msg.message==TM_WRITEAUDIO)
// 				{
// 					goto line1;
// 				}
// 			}
// 
// 		}
// 	}
// theend:
// 	pThis->m_PlayCheckThread=NULL;
// 	return 0;
// }

// int CDSoundOut::GetBufferMS()
// {
// 	DWORD ndata=BUFFERNOTIFYSIZE*MAX_AUDIO_BUF-GetFreeBuffer();
// 	m_Buffer.Lock();
// 	ndata+=(DWORD)m_Buffer.GetSize();
// 	m_Buffer.Unlock();
// 	if(ndata==0)return 0;
// 	int ret=ndata/m_WaveFormat.nChannels/(m_WaveFormat.wBitsPerSample/8);
// 	ret=ret*1000/m_WaveFormat.nSamplesPerSec;
// 	return ret;
// }
// 
// //将PCM数据写入准备播放
// int CDSoundOut::Write(unsigned char *pcm, int pcmlen)
// {
// 	//开始播放缓冲中的内容，缓冲区是个循环缓冲区
// 	if(m_bMute)return pcmlen;
// 
// 	m_Buffer.Lock();
// 	if(m_Buffer.GetSize()>BUFFERNOTIFYSIZE*MAX_AUDIO_BUF*4)
// 	{
// 		m_Buffer.SetSize(0);
// 	}
// 	m_Buffer.AddData(pcm,pcmlen);
// 	m_Buffer.Unlock();
// 
// 	
// 
// 	PostThreadMessage(m_PlayCheckThread->m_nThreadID,TM_WRITEAUDIO,0,0);
// 	return pcmlen;
// 
// 
// 
// 
// }

