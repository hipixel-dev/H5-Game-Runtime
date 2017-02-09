//
//  SystemInterfaceIOS.mm
//  wuwanlib
//
//  Created by 陈JH on 15/8/4.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//
#import "stdafx.h"
#import "SystemInterfaceIOS.h"
#import "../../proj.ios/Application.h"

#import <stdarg.h>
#import <stdio.h>

#import <UIKit/UIAlert.h>

SystemInterfaceIOS::SystemInterfaceIOS()
: _devicePixelRatio(1)
{
    // TODO Auto-generated constructor stub
    
}

SystemInterfaceIOS::~SystemInterfaceIOS() {
    // TODO Auto-generated destructor stub
}

bool SystemInterfaceIOS::OpenUrl(const char*szUrl)
{
    return Application::getInstance()->openURL(szUrl);
    return true;
}

bool SystemInterfaceIOS::ShowAlert(const char*szText)
{
    NSString * tmpTitle = [NSString stringWithUTF8String : "Alert"];
    NSString * tmpMsg = (szText) ? [NSString stringWithUTF8String : szText] : nil;
    UIAlertView * messageBox = [[UIAlertView alloc] initWithTitle: tmpTitle
                                                          message: tmpMsg
                                                         delegate: nil
                                                cancelButtonTitle: @"OK"
                                                otherButtonTitles: nil];
    [messageBox autorelease];
    [messageBox show];

    return true;
}

bool SystemInterfaceIOS::ShowErr(const char*szText)
{
    fprintf(stdout, "%s", szText);
    fflush(stdout);
    return true;
}

CTRLHANDLE* SystemInterfaceIOS::CreateCtrl(CTRLTYPE ctrltype, HtmlEventTarget*pElement, int x, int y, int width, int height)
{
    return nullptr;
}

bool SystemInterfaceIOS::MoveCtrl(CTRLHANDLE*handle, int x, int y, int width, int height)
{
    return false;
}

bool SystemInterfaceIOS::SetCtrlText(CTRLHANDLE*handle, const char*szText)
{
    return false;
}

std::string SystemInterfaceIOS::GetCtrlText(CTRLHANDLE*handle)
{
    return "";
}

bool SystemInterfaceIOS::ShowCtrl(CTRLHANDLE*handle, bool bShow)
{
    return false;
}

bool SystemInterfaceIOS::DeleteCtrl(CTRLHANDLE*handle)
{
    return false;
}

bool SystemInterfaceIOS::SetFont(CTRLHANDLE* handle, const FontInfo& finfo)
{
    return false;
}

bool SystemInterfaceIOS::FocusCtrl(CTRLHANDLE*handle, bool bFocus)
{
    return false;
}

bool SystemInterfaceIOS::SelectText(CTRLHANDLE* handle, int start, int end)
{
    return false;
}

int SystemInterfaceIOS::OnCtrlMsg(CTRLMSG&msg)
{
    return 0;
}