#pragma once
class CArrayBuffer
{
public:
	float m_nGrowMul = 1.5f;
	uint8_t*m_pBuffer = nullptr;
	int m_nBufLen = 0;//数据的大小
	int m_nBufSize = 0;//实际分配的大小
	CArrayBuffer();
	~CArrayBuffer();

	void RemoveAll();


	void AddData(const void*pBuf, int nLen);
	void RemoveData(int nLen);//从前面删除
	uint8_t*GetBuffer();
	int GetLength();
	void Resize(int nLen);
	void AddString(const std::string &str);
};

