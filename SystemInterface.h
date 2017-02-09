#pragma once
#include <string>
#include <unordered_map>
#include "stdafx.h"

//系统调用接口，用于从JS调用系统函数
class HtmlEventTarget;
enum CTRLTYPE
{
	CTRL_TEXTBOX,
};

class CTRLHANDLE
{
public:
	static std::unordered_map<HWND, CTRLHANDLE*>s_HandleMap;
	static CTRLHANDLE*FromHandle(HWND hwnd);
	CTRLHANDLE(HWND h, HtmlEventTarget*ele);
	~CTRLHANDLE();

	HWND hwnd=0;
	HFONT hFont=0;
	HtmlEventTarget*pElement = 0;
	
};



struct CTRLMSG
{
	enum MSGTYPE
	{
		MT_FOCUS,
		MT_BLUR,
		MT_INPUT,
	};
	MSGTYPE msg;
	CTRLHANDLE*hCtrl;
	int wParam;
	int lParam;
};
class SystemInterface
{
public:
	
	virtual~SystemInterface();

	virtual bool OpenUrl(const char*szUrl) = 0;
	virtual bool ShowAlert(const char*szText) = 0;
	virtual bool ShowErr(const char*szText) = 0;

	virtual CTRLHANDLE*CreateCtrl(CTRLTYPE ctrltype, HtmlEventTarget*pElement, int x, int y, int width, int height) = 0;
	virtual bool MoveCtrl(CTRLHANDLE*handle, int x, int y, int width, int height) = 0;
	virtual bool SetCtrlText(CTRLHANDLE*handle, const char*szText) = 0;
	virtual std::string GetCtrlText(CTRLHANDLE*handle) = 0;
	virtual bool ShowCtrl(CTRLHANDLE*handle, bool bShow) = 0;
	virtual bool DeleteCtrl(CTRLHANDLE*handle) = 0;
	virtual bool SetFont(CTRLHANDLE* handle, const FontInfo& finfo)=0;
	virtual bool FocusCtrl(CTRLHANDLE*handle, bool bFocus) = 0;
	virtual bool SelectText(CTRLHANDLE* handle, int start, int end)=0;

	virtual int OnCtrlMsg(CTRLMSG&msg) = 0;// control event
	virtual std::string callJSBFunction(const std::string &szFunName, const std::vector<std::string>& paraList) = 0;
};

