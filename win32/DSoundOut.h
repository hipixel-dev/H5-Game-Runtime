#pragma once

#include <mmsystem.h>
#include <dsound.h>

//使用DirectSound播放PCM数据


class CDSoundOut;
class CDSoundOutDelegate
{
public:
	virtual void OnPlayEnd(CDSoundOut*pSound)=NULL;
};




class  CDSoundOut
{
public:
	CDSoundOut(void);
	~CDSoundOut(void);


// #define MAX_AUDIO_BUF 2
// #define BUFFERNOTIFYSIZE /*192000*/8192

protected:
	HWND m_hWnd;

	LPDIRECTSOUNDBUFFER8 m_pDSBuffer8 ; //buffer
	LPDIRECTSOUND8 m_pDsd ; //dsound
	LPDIRECTSOUNDNOTIFY8 m_pDSNotify ; 
	DSBPOSITIONNOTIFY m_aPosNotify;// [MAX_AUDIO_BUF];//设置通知标志的数组

	HANDLE m_event=0;// [MAX_AUDIO_BUF];
//	DWORD m_dwNextWriteOffset ;

//	BOOL m_bMute;//是否静音


//	CBuffer m_Buffer;
//	CWinThread *m_PlayCheckThread;
	
	WAVEFORMATEX m_WaveFormat;
	
public:
	DWORD m_dwBufferBytes = 0;


	BOOL Init(HWND hWnd, LPWAVEFORMATEX pWaveFormat, DWORD dwBufferBytes, BYTE*pData);
	BOOL Uninit();


	void start(unsigned long dwFrom,bool bLoop);
	void stop();
	void pause();
	void seek(unsigned long dwTo);
	unsigned long GetCurrentPosition();
	bool IsStop();
	//	void Mute(BOOL bMute);

	
	//write结束，将播放完的缓冲区填0,在播放完缓冲区后停止播放
//	void WriteEnd();

//	static UINT t_PlayCheck(LPVOID pParam);

	//将PCM数据写入准备播放
// 	int Write(unsigned char *pcm, int pcmlen);
// 
// 	int GetBufferMS();//取得还没播放的缓冲有多少毫秒
protected:
//	DWORD GetFreeBuffer();
	//写到g_pDSBuffer8
//	int WriteBuffer(BYTE*pData,int nDataLen);

};
