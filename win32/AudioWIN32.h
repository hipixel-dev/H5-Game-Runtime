#pragma once
#include "..\AudioInterface.h"
#include "DSoundOut.h"
#include "..\ArrayBuffer.h"


class AudioWIN32 :
	public AudioInterface
{
	std::string m_szFilename;
	CDSoundOut m_dsound;
//	DWORD m_wDeviceID = 0;
	bool m_isOpened = false;
	struct _AUDIOCACHE
	{
		int nRef;//引用次数
		CArrayBuffer buffer;
	};
	static std::map<std::string, _AUDIOCACHE*>s_AudioCache;//文件解码后的缓存
public:
	AudioWIN32();
	virtual ~AudioWIN32();

	virtual bool Release() ;

	virtual bool Open(const char*szFileName) ;

	virtual bool Play(unsigned long dwFrom, bool bRepeat) ;

	virtual bool Pause() ;

	virtual bool Stop() ;

	virtual bool Seek(unsigned long dwTo) ;

	virtual unsigned long GetLength() ;

	virtual unsigned long GetCurrentPosition() ;

	virtual bool Close() ;

	virtual bool isOpen() ;

	virtual bool isStop() ;

	virtual AudioInterface* Create();
	virtual void update();
	std::string GetExt(std::string filename);


};

