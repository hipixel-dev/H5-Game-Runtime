//
//  OpenglIterface.h
//  wuwanlib
//
//  Created by 陈JH on 15/8/21.
//  Copyright (c) 2015年 zhangfeng. All rights reserved.
//

#ifndef __wuwanlib__OpenglIterface__
#define __wuwanlib__OpenglIterface__

class OpenglInterface {
public:
    OpenglInterface();
    virtual ~OpenglInterface();
    
    virtual void allocatRenderbuffer(unsigned int target, unsigned int internalFormat, int width, int height);
    
};


#endif /* defined(__wuwanlib__OpenglIterface__) */
