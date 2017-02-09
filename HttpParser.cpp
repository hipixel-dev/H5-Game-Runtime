#include "stdafx.h"
#include "HttpParser.h"


HttpParser::HttpParser()
{
}


HttpParser::~HttpParser()
{
}
void TrimLeft(char*&p)
{
	while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n')p++;
}
bool HttpParser::Parse(char*pData)
{
	m_bHaveContentLength = false;
	bool ret = true;
	pData = strdup(pData);
	do
	{

		
		int datalen = strlen(pData);
		char*ctx=nullptr;
		char*p = strtok_s(pData, "\r", &ctx);
		char*sp = strchr(p, ' ');
		assert(sp);
		if (!sp)
		{
			ret = false;
			break;
		}
		sp++;
		m_nRetCode = atoi(sp);
		p = strtok_s(nullptr, "\r", &ctx);
		while (p)
		{

			TrimLeft(p);
			if (p[0] == 0)//¿ÕÐÐ
			{
				m_DataOffset = p + 1 - pData;
				if (pData[m_DataOffset] == '\n')m_DataOffset++;
				if (m_DataOffset > datalen)
				{
					m_DataOffset = 0;
				}
			
				break;
			}
			char*pd = strchr(p, ':');
			if (pd)
			{
				char szkey[30];
				strncpy(szkey, p, pd - p);
				szkey[pd - p] = 0;
				p = pd + 1;
				TrimLeft(p);
				if (!_stricmp(szkey, "Content-Length"))
				{
					m_nContentLength = atoi(p);
					m_bHaveContentLength = true;
				}
				else if(!_stricmp(szkey, "Transfer-Encoding"))
				{
					m_bTransferEncodingChunked = true;
				}
			}
			p = strtok_s(nullptr, "\r", &ctx);
		}
		if (!m_bHaveContentLength)
		{
			m_nContentLength = datalen - m_DataOffset;
			if (m_bTransferEncodingChunked)
			{
				p = strtok_s(pData+m_DataOffset, "\r", &ctx);
				m_nContentLength = strtol(p, 0, 16);
				ctx++;
				m_DataOffset = ctx - pData;
			}
		}
	} while (false);
	free(pData);
	return ret;
}
