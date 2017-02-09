/*
 * SystemInterfaceIOS.h
 */

#ifndef SYSTEMINTERFACEIOS_H_
#define SYSTEMINTERFACEIOS_H_
#include"../../XiaobaiEngine/SystemInterface.h"
class SystemInterfaceIOS: public SystemInterface {
public:
	SystemInterfaceIOS();
	virtual ~SystemInterfaceIOS();
	virtual bool OpenUrl(const char*szUrl) ;
	virtual bool ShowAlert(const char*szText) ;
	virtual bool ShowErr(const char*szText);


	virtual CTRLHANDLE*CreateCtrl(CTRLTYPE ctrltype, HtmlEventTarget*pElement, int x, int y, int width, int height) ;
	virtual bool MoveCtrl(CTRLHANDLE*handle, int x, int y, int width, int height) ;
	virtual bool SetCtrlText(CTRLHANDLE*handle, const char*szText) ;
	virtual std::string GetCtrlText(CTRLHANDLE*handle) ;
	virtual bool ShowCtrl(CTRLHANDLE*handle, bool bShow) ;
	virtual bool DeleteCtrl(CTRLHANDLE*handle) ;
	virtual bool SetFont(CTRLHANDLE* handle, const FontInfo& finfo) ;
	virtual bool FocusCtrl(CTRLHANDLE*handle, bool bFocus) ;
    virtual bool SelectText(CTRLHANDLE* handle, int start, int end);
    virtual int OnCtrlMsg(CTRLMSG&msg);    
protected:
    int _devicePixelRatio;
};

#endif /* SYSTEMINTERFACEIOS_H_ */
