#include "stdafx.h"
#include "MadDecodec.h"
// #ifdef _DEBUG
// #pragma comment(lib,"lib/libmad_d.lib")
// #else
// #pragma comment(lib,"lib/libmad.lib")
// #endif

signed short to_short(mad_fixed_t fixed)
{
	if (fixed >= MAD_F_ONE)
		return(SHRT_MAX);
	if (fixed <= -MAD_F_ONE)
		return(-SHRT_MAX);
	fixed = fixed >> (MAD_F_FRACBITS - 15);
	return((signed short)fixed);
}

unsigned char inputBuffer[4096 + MAD_BUFFER_GUARD];



MadDecodec::MadDecodec()
{
	file = nullptr;
}


MadDecodec::~MadDecodec()
{
	if (file)
	{
		fclose(file);
		mad_synth_finish(&synth);
		mad_frame_finish(&frame);
		mad_stream_finish(&stream);
	}

}

int MadDecodec::setup(unsigned char*pBuffer,int nLen)
{
	mad_stream_buffer(&stream, pBuffer, nLen);
	char dest[100];
	mad_timer_string(timer, dest, "lu:u:u", MAD_UNITS_HOURS, MAD_UNITS_MILLISECONDS, 0);
	stream.error = (mad_error)0;
	return 0;
}
int MadDecodec::setup()
{
	size_t    remaining;
	unsigned char* read;

	
	
	if (stream.next_frame != NULL)
	{
		remaining = stream.bufend - stream.next_frame;
		memmove(inputBuffer, stream.next_frame, remaining);
		read = inputBuffer + remaining;
		readsize = 4096 - remaining;
	}
	else
		readsize = 4096,
		read = inputBuffer,
		remaining = 0;
	readsize = fread(read, 1, readsize, file);
	
	if (readsize <= 0)
	{
		return -1;
	}

	mad_stream_buffer(&stream, inputBuffer, readsize + remaining);
	char dest[100];
	mad_timer_string(timer, dest, "lu:u:u", MAD_UNITS_HOURS, MAD_UNITS_MILLISECONDS, 0);
	stream.error = (mad_error)0;
	return 0;
}

long MadDecodec::readFrame(unsigned char* pcm)
{
	if (!bFromBuffer&&stream.buffer == NULL)
	{
		if (setup() == -1)
			return -1;
	}
	while (mad_frame_decode(&frame, &stream) != 0)
	{
		if (MAD_RECOVERABLE(stream.error))
		{
			if (stream.error != MAD_ERROR_LOSTSYNC)
			{
			}
			continue;
		}
		else
			if (stream.error == MAD_ERROR_BUFLEN)
			{
				if (bFromBuffer)return -1;
				if (setup() == -1) return -1;
				continue;
			}
			else
			{
				return -1;
			}
	}
	mad_synth_frame(&synth, &frame);

	//! »ñÈ¡ÆµÂÊ
	//freq = synth.pcm.samplerate; 

	//bitrate = frame.header.bitrate;
	//channels = (frame.header.mode == MAD_MODE_SINGLE_CHANNEL) ? 1 : 2;

	mad_timer_add(&timer, frame.header.duration);
	int j = 0;

	for (int i = 0; i < synth.pcm.length; i++)
	{
		signed short    sample;
		sample = to_short(synth.pcm.samples[0][i]);
		pcm[j++] = sample & 0xff;
		pcm[j++] = sample >> 8;
		if (MAD_NCHANNELS(&frame.header) == 2)
			sample = to_short(synth.pcm.samples[1][i]);
		pcm[j++] = sample & 0xff;
		pcm[j++] = sample >> 8;
	}

	char dest[120];
	mad_timer_string(timer, dest, "%lu:lu.u", MAD_UNITS_MINUTES, MAD_UNITS_MILLISECONDS, 0);
	return j;
}

bool MadDecodec::open(const char* filename)
{
	bFromBuffer = false;
	file = fopen(filename, "rb");
	if (!file)return false;
	mad_stream_init(&stream);
	mad_frame_init(&frame);
	mad_synth_init(&synth);
	mad_timer_reset(&timer);
	return true;
}

bool MadDecodec::openBuffer(void*pBuffer, int nLen)
{
	bFromBuffer = true;
	mad_stream_init(&stream);
	mad_frame_init(&frame);
	mad_synth_init(&synth);
	mad_timer_reset(&timer);
	setup((unsigned char*)pBuffer, nLen);
	return true;
}

int MemMapFile::Read(void*pBuf, int nLen)
{
	if (!m_pBuffer)return 0;
	int nRead = nLen;
	if (m_nPosition + nLen > m_nBufferLen)
	{
		nRead = m_nBufferLen - m_nPosition;
	}
	if (nRead <= 0)return 0;
	memcpy(pBuf, m_pBuffer, nRead);
	m_nPosition += nRead;
	return nRead;
}

void MemMapFile::SetBuffer(void*pBuf, int nLen)
{
	m_pBuffer = (uint8_t*)pBuf;
	m_nBufferLen = nLen;
	m_nPosition = 0;
}
