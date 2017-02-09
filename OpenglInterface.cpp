//
//  OpenglIterface.cpp
//  wuwanlib
//
//  Created by 陈JH on 15/8/21.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#include "OpenglInterface.h"
#include "Macros.h"

#if defined(_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#endif

OpenglInterface::OpenglInterface()
{
    
}

OpenglInterface::~OpenglInterface()
{
    
}

void OpenglInterface::allocatRenderbuffer(unsigned int target, unsigned int internalFormat, int width, int height)
{
    glRenderbufferStorage(target, internalFormat, width, height);
}