#include "stdafx.h"
#include "AudioWIN32.h"
#include <mmsystem.h>
#include <combaseapi.h>
#include <Digitalv.h>
#include "AcmCodec.h"
#include "..\MadDecodec.h"
#include "..\ArrayBuffer.h"
#include <string>
#include "..\JSVMInterface.h"
#include "..\libogg-1.3.2\include\ogg\ogg.h"
#pragma comment(lib,"winmm.lib")//导入声音的链接库  




AudioWIN32::AudioWIN32()
{
}


AudioWIN32::~AudioWIN32()
{
	Close();
}

bool AudioWIN32::Release()
{
	Close();
	delete this;
	return true;
}

const char* newGUID()
{
	static char buf[64] = { 0 };
	GUID guid;
	if (S_OK == ::CoCreateGuid(&guid))
	{
		_snprintf(buf, sizeof(buf),
			"{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]
			);
	}
	return (const char*)buf;
}
extern HWND g_hWnd;
bool AudioWIN32::Open(const char*szFileName)
{

	Close();
	bool bOK;
// 	DWORD nBufferLen=0;
// 	HttpRequestInterface*phttp = JSVM::GetInstance()->GetHttpRequestInterface();
	m_szFilename = szFileName;// JSVM::GetInstance()->GetFullPath(szFileName);
	std::string &filepath = m_szFilename;// phttp->GetFileToCachePath(m_szFilename);

	WAVEFORMATEX wfPCM = {};
	wfPCM.wFormatTag = 1;
	wfPCM.nChannels = 2;
	wfPCM.nSamplesPerSec = 44100;
	wfPCM.nAvgBytesPerSec = 176400;
	wfPCM.nBlockAlign = 4;
	wfPCM.wBitsPerSample = 16;
	wfPCM.cbSize = 0;
	bool bGetHead = false;

	_AUDIOCACHE*pcache = nullptr;
	std::map<std::string, _AUDIOCACHE*>::iterator it=s_AudioCache.find(filepath);
	if (it != s_AudioCache.end())
	{
		pcache = it->second;
		pcache->nRef++;
	}
	else
	{
		std::string&&ext = GetExt(filepath);
		if (!_stricmp(ext.c_str(), "mp3"))
		{
			MadDecodec maddec;
			if (!maddec.open(filepath.c_str()))
				return false;

			pcache = new _AUDIOCACHE;
			pcache->nRef = 1;



			long len = 0;
			unsigned char buffer[4096 * 2];
			while (true)
			{
				len = maddec.readFrame(buffer);
				if (len <= 0)break;
				if (!bGetHead)
				{
					wfPCM.nSamplesPerSec = maddec.synth.pcm.samplerate;
					wfPCM.nChannels = maddec.synth.pcm.channels;
					wfPCM.nAvgBytesPerSec = wfPCM.nSamplesPerSec*wfPCM.nChannels*wfPCM.wBitsPerSample / 8;
					wfPCM.nBlockAlign = wfPCM.wBitsPerSample / 8 * wfPCM.nChannels;
					bGetHead = true;
				}

				pcache->buffer.AddData(buffer, len);
			}
			assert(pcache->buffer.GetLength());
			if (pcache->buffer.GetLength() == 0)
			{
				delete pcache;
				return false;
			}
			s_AudioCache[filepath] = pcache;
		}
		else if (!_stricmp(ext.c_str(), "ogg"))
		{
// 			FileIOInterface *pfile=JSVM::GetInstance()->GetFileIOInterface();
// 			FileHandle*file = pfile->Open(filepath.c_str());
// 			if (!file)return false;
// 			int filelen = pfile->GetLength(file);
// 			ogg_sync_state os;
// 			ogg_sync_init(&os);
// 			ogg_page page;
// 			if (ogg_sync_pageout(&os, &page) != 1) {
// 
// 				char*buffer = ogg_sync_buffer(&os, 8192);
// 
// 				while (true)
// 				{
// 					int nRead = pfile->Read(file, buffer, filelen);
// 					int ret = ogg_sync_wrote(&os, nRead);
// 					ret = ogg_sync_pageout(&os, &page);
// 
// 				}
// 
// 
// 
// 
// 
// 				buffer = ogg_sync_buffer(&oy, 8192);
// 				bytes = fread(buffer, 1, 8192, stdin);
// 				ogg_sync_wrote(&oy, bytes);
// 			}
// 			

			return true;
		}
		else return true;
	}

	
	assert(pcache->buffer.GetLength());
	bOK = m_dsound.Init(g_hWnd, &wfPCM, pcache->buffer.GetLength(), pcache->buffer.GetBuffer());
	m_isOpened = bOK;



	return bOK;
}

bool AudioWIN32::Play(unsigned long dwFrom, bool bRepeat)
{

	if (!m_isOpened)return false;
	m_dsound.start(dwFrom,bRepeat);
	return true;
// 	MCI_PLAY_PARMS PlayParms;
// 	PlayParms.dwFrom = dwFrom;
// 	// 指定从什么地方（时间）播放
// 	MCIERROR ret;
// 	if (!bRepeat)
// 	{
// 		ret = mciSendCommand(m_wDeviceID, MCI_PLAY,
// 			MCI_FROM, (DWORD)(LPVOID)
// 			&PlayParms);
// 	}
// 	else
// 	{
// 		ret = mciSendCommand(m_wDeviceID, MCI_PLAY,
// 			MCI_DGV_PLAY_REPEAT, (DWORD)(LPVOID)
// 			&PlayParms);
// 
// 	}
// 	return ret;
}

bool AudioWIN32::Pause()
{
	if (!m_isOpened)return false;
	m_dsound.pause();

	return true;
// 	MCI_PLAY_PARMS PlayParms;
// 	return mciSendCommand(m_wDeviceID, MCI_PAUSE, 0,
// 		(DWORD)(LPVOID)&PlayParms);
}

bool AudioWIN32::Stop()
{
	if (!m_isOpened)return false;
	m_dsound.stop();
	return true;
//	return mciSendCommand(m_wDeviceID, MCI_STOP, NULL, NULL);
}

bool AudioWIN32::Seek(unsigned long dwTo)
{
	if (!m_isOpened)return false;
	m_dsound.seek(dwTo);
	return true;
// 	MCI_SEEK_PARMS SeekParms;
// 	SeekParms.dwTo = dwTo;
// 	//跳转的目标时间，时间单位为毫秒
// 	return mciSendCommand(m_wDeviceID, MCI_SEEK, MCI_TO
// 		/*| MCI_WAIT*/, (DWORD)(LPVOID)
// 		&SeekParms);
}

unsigned long AudioWIN32::GetLength()
{
	if (!m_isOpened)return 0;
	return m_dsound.m_dwBufferBytes;
// 	MCI_STATUS_PARMS StatusParms;
// 	StatusParms.dwItem = MCI_STATUS_LENGTH;
// 	mciSendCommand(m_wDeviceID, MCI_STATUS,
// 		MCI_WAIT | MCI_STATUS_ITEM,
// 		(DWORD)(LPVOID)&StatusParms);
// 	return StatusParms.dwReturn;
}

unsigned long AudioWIN32::GetCurrentPosition()
{
	if (!m_isOpened)return 0;
	return m_dsound.GetCurrentPosition();
// 	MCI_STATUS_PARMS StatusParms;
// 	StatusParms.dwItem = MCI_STATUS_POSITION;
// 	mciSendCommand(m_wDeviceID, MCI_STATUS,
// 		MCI_WAIT | MCI_STATUS_ITEM,
// 		(DWORD)(LPVOID)&StatusParms);
// 	return StatusParms.dwReturn;
}

bool AudioWIN32::Close()
{
	if (!m_isOpened)return 0;
	std::map<std::string, _AUDIOCACHE*>::iterator it=s_AudioCache.find(m_szFilename);
	if (it != s_AudioCache.end())
	{
		it->second->nRef--;
		if (it->second->nRef <= 0)
		{
			delete it->second;
			s_AudioCache.erase(it);
		}
	}
	return m_dsound.Uninit();
//	return mciSendCommand(m_wDeviceID, MCI_CLOSE, NULL, NULL);
}

bool AudioWIN32::isOpen()
{
	return m_isOpened;
}

bool AudioWIN32::isStop()
{
	if (!m_isOpened)return true;
	return m_dsound.IsStop();
// 	MCI_STATUS_PARMS StatusParms;
// 	StatusParms.dwItem = MCI_STATUS_MODE;
// 	MCIERROR err = mciSendCommand(m_wDeviceID, MCI_STATUS,
// 		/*MCI_WAIT |*/ MCI_STATUS_ITEM,
// 		(DWORD)(LPVOID)&StatusParms);
// 	if (err)return true;
// 	if (StatusParms.dwReturn != MCI_MODE_PLAY)return true;
// 	return false;
}

AudioInterface* AudioWIN32::Create()
{
	return new AudioWIN32;
}

std::string AudioWIN32::GetExt(std::string filename)
{
	int i = filename.rfind('.');
	if (i >= 0)return filename.substr(i + 1);
	else return "";
}

void AudioWIN32::update()
{

}

std::map<std::string, AudioWIN32::_AUDIOCACHE*> AudioWIN32::s_AudioCache;
