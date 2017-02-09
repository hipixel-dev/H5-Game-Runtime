#include "stdafx.h"
#include "SystemInterfaceWIN32.h"
#include <windows.h>
#include <shellapi.h>
#include "..\HtmlEventTarget.h"



SystemInterfaceWIN32::SystemInterfaceWIN32()
{
}


SystemInterfaceWIN32::~SystemInterfaceWIN32()
{
}

bool SystemInterfaceWIN32::OpenUrl(const char*szUrl)
{
	ShellExecuteA(0, "open", szUrl, 0, 0, SW_SHOW);
	return true;
}

bool SystemInterfaceWIN32::ShowAlert(const char*szText)
{
	MessageBoxA(0, szText, NULL, 0);
	return true;
}

bool SystemInterfaceWIN32::ShowErr(const char*szText)
{
	OutputDebugStringA("\n");
	OutputDebugStringA(szText);
	return true;
}
extern HWND g_hWnd;
CTRLHANDLE* SystemInterfaceWIN32::CreateCtrl(CTRLTYPE ctrltype,HtmlEventTarget*pElement, int x, int y, int width, int height)
{
	switch (ctrltype)
	{
	case CTRL_TEXTBOX:
	{
		HWND hwnd = CreateWindow("EDIT", NULL, WS_CHILD|WS_VISIBLE , x, y, width, height, g_hWnd, 0, 0, 0);
		CTRLHANDLE*hw = new CTRLHANDLE(hwnd, pElement);
		return hw;
	}
	default:
		return nullptr;
	}
}

bool SystemInterfaceWIN32::MoveCtrl(CTRLHANDLE*handle, int x, int y, int width, int height)
{
	return MoveWindow(handle->hwnd, x, y, width, height, true);
}

bool SystemInterfaceWIN32::SetCtrlText(CTRLHANDLE*handle, const char*szText)
{
	
	return SetWindowText(handle->hwnd, szText);
}

bool SystemInterfaceWIN32::DeleteCtrl(CTRLHANDLE*handle)
{
	DestroyWindow(handle->hwnd);
	if (handle->hFont)
	{
		DeleteObject(handle->hFont);
	}
	delete handle;
	return true;
}

std::string SystemInterfaceWIN32::GetCtrlText(CTRLHANDLE*handle)
{
	int len=GetWindowTextLength(handle->hwnd);
	std::string str;
	str.resize(len);
	GetWindowText(handle->hwnd, (char*)str.data(), len+1);

	return str;
}

bool SystemInterfaceWIN32::ShowCtrl(CTRLHANDLE*handle, bool bShow)
{
	return ShowWindow(handle->hwnd, bShow ? SW_SHOW : SW_HIDE);
}

bool SystemInterfaceWIN32::SetFont(CTRLHANDLE* handle, const FontInfo& finfo)
{
	if (handle->hFont)
	{
		::DeleteObject(handle->hFont);
	}
	handle->hFont = CreateFont(finfo.nFontSize, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, finfo.szFontName.c_str());
	if (!handle->hFont)return false;
	::SendMessage(handle->hwnd, WM_SETFONT, (WPARAM)handle->hFont, true);
	return true;
}

bool SystemInterfaceWIN32::FocusCtrl(CTRLHANDLE*handle, bool bFocus)
{
	if (bFocus)
	{
		SetFocus(handle->hwnd);
	}
	else
	{
		SetFocus(g_hWnd);
	}
	return true;
}

bool SystemInterfaceWIN32::SelectText(CTRLHANDLE* handle, int start, int end)
{
	SendMessage(handle->hwnd, EM_SETSEL, start, end);
	return true;
}

int SystemInterfaceWIN32::OnCtrlMsg(CTRLMSG&msg)
{
	JSVMAutoContext ac(JSVM::GetInstance());
	HandleContext context = JSVM::GetInstance()->GetContext();
	LocalObject objelement(context, msg.hCtrl->pElement->GetObjFromDomTree(context));
	if (objelement.IsNull())
	{
		assert(0);
		return 0;
	}
	CustomClassAutoThisObj cc(msg.hCtrl->pElement, objelement);
	switch (msg.msg)
	{
	case CTRLMSG::MT_BLUR:
	{
		msg.hCtrl->pElement->ExecEventListenerAsync(context, "blur");
		break;
	}
	case CTRLMSG::MT_FOCUS:
	{
		msg.hCtrl->pElement->ExecEventListenerAsync(context, "focus");
		break;
	}
	case CTRLMSG::MT_INPUT:
	{
		msg.hCtrl->pElement->ExecEventListenerAsync(context, "input");
		break;
	}
	
	}
	return 1;
}

std::string SystemInterfaceWIN32::callJSBFunction(const std::string &szFunName, const std::vector<std::string>& paraList)
{
	return "123";
}
