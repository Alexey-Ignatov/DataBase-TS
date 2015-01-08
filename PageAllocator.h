//
//  PageAllocator.h
//  DataBase TS
//
//  Created by Алексей on 07.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#ifndef __DataBase_TS__PageAllocator__
#define __DataBase_TS__PageAllocator__

#include <stdio.h>

class PageAllocator{
    
    int fileDesc;
    int pageSize;
    
    int blocksCount;
    
public:
    
    PageAllocator(int fileDesc, int pageSize);
    
    int allocatePage();
};
#endif /* defined(__DataBase_TS__PageAllocator__) */
