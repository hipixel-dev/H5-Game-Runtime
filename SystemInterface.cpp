#include "stdafx.h"
#include "SystemInterface.h"





SystemInterface::~SystemInterface()
{
}

std::unordered_map<HWND, CTRLHANDLE*> CTRLHANDLE::s_HandleMap;

CTRLHANDLE::CTRLHANDLE(HWND h, HtmlEventTarget*ele)
{
	hwnd = h;
	pElement = ele;
	assert(s_HandleMap.find(hwnd) == s_HandleMap.end());
	s_HandleMap[hwnd] = this;
}

CTRLHANDLE* CTRLHANDLE::FromHandle(HWND hwnd)
{
	std::unordered_map<HWND, CTRLHANDLE*>::iterator it = s_HandleMap.find(hwnd);
	if (it == s_HandleMap.end())return nullptr;
	return it->second;
}

CTRLHANDLE::~CTRLHANDLE()
{
	s_HandleMap.erase(hwnd);
}
