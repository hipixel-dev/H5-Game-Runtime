#pragma once
#include "..\SystemInterface.h"
#include "..\ImageDecoderInterface.h"

class SystemInterfaceWIN32 :
	public SystemInterface
{
public:
	SystemInterfaceWIN32();
	virtual ~SystemInterfaceWIN32();

	virtual bool OpenUrl(const char*szUrl) ;

	virtual bool ShowAlert(const char*szText);

	virtual bool ShowErr(const char*szText) ;

	virtual CTRLHANDLE* CreateCtrl(CTRLTYPE ctrltype, HtmlEventTarget*pElement, int x, int y, int width, int height) override;

	virtual bool MoveCtrl(CTRLHANDLE*handle, int x, int y, int width, int height) override;

	virtual bool SetCtrlText(CTRLHANDLE*handle, const char*szText) override;

	virtual bool DeleteCtrl(CTRLHANDLE*handle) override;

	virtual std::string GetCtrlText(CTRLHANDLE*handle) override;

	virtual bool ShowCtrl(CTRLHANDLE*handle, bool bShow) override;

	virtual bool SetFont(CTRLHANDLE* handle, const FontInfo& finfo) override;

	virtual bool FocusCtrl(CTRLHANDLE*handle, bool bFocus) override;

	virtual bool SelectText(CTRLHANDLE* handle, int start, int end) override;

	virtual int OnCtrlMsg(CTRLMSG&msg) override;

	virtual std::string callJSBFunction(const std::string &szFunName, const std::vector<std::string>& paraList) override;

};

