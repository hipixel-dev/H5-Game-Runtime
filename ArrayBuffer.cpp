#include "stdafx.h"
#include "ArrayBuffer.h"


CArrayBuffer::CArrayBuffer()
{
	m_pBuffer = nullptr;
}


CArrayBuffer::~CArrayBuffer()
{
	if (m_pBuffer)free(m_pBuffer);
}

void CArrayBuffer::RemoveAll()
{
	if (m_pBuffer)
	{
		free(m_pBuffer);
		m_pBuffer = nullptr;
		
	}
	m_nBufSize = 0;
	m_nBufLen = 0;
}
void CArrayBuffer::Resize(int nLen)
{
	if (nLen == 0)RemoveAll();
	else
	{

		if (m_pBuffer)
		{
			
			if (nLen>m_nBufSize)
			{
				m_nBufSize = nLen * m_nGrowMul;
				uint8_t*p = (uint8_t*)realloc(m_pBuffer, m_nBufSize + 1);
				assert(p);
				if (!p)return;
				m_pBuffer = p;
			}
			m_nBufLen = nLen;
			m_pBuffer[nLen] = 0;
		}
		else
		{
			m_nBufSize = nLen * m_nGrowMul;
			m_pBuffer = (uint8_t*)malloc(m_nBufSize + 1);
			m_nBufLen = nLen;
			m_pBuffer[nLen] = 0;
		}
	}
}

void CArrayBuffer::AddData(const void*pBuf, int nLen)
{
	int sizOld = m_nBufLen;
	Resize(sizOld + nLen);
	memcpy(m_pBuffer + sizOld, pBuf, nLen);

}

void CArrayBuffer::RemoveData(int nLen) //´ÓÇ°ÃæÉ¾³ý
{
	int sizOld = m_nBufLen;
	if (nLen > sizOld)
	{
		RemoveAll();
		return;
	}
	int sizNew = sizOld - nLen;
	memmove(m_pBuffer, m_pBuffer + nLen, sizNew);
	m_nBufLen = sizNew;
	m_pBuffer[m_nBufLen] = 0;
}

uint8_t* CArrayBuffer::GetBuffer()
{
	return m_pBuffer;
}

int CArrayBuffer::GetLength()
{
	return m_nBufLen;
}

void CArrayBuffer::AddString(const std::string &str)
{
	AddData(str.data(), str.length());
}
