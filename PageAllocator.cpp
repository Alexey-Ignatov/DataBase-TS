//
//  PageAllocator.cpp
//  DataBase TS
//
//  Created by Алексей on 07.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#include "PageAllocator.h"


PageAllocator::PageAllocator(int fileDesc, int pageSize): fileDesc(fileDesc), pageSize(pageSize), blocksCount(0){}

int PageAllocator::allocatePage(){
    blocksCount++;
    return pageSize*(blocksCount - 1);
}