//
//  PageAllocator.cpp
//  DataBase TS
//
//  Created by Алексей on 07.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#include "PageAllocator.h"

//PageAllocator::PageAllocator(): fileDesc(0), blocksCount(0), pageSize(0){}


PageAllocator::PageAllocator(int fileDesc, int pageSize, int dbSize): fileDesc(fileDesc), pageSize(pageSize), maxBlocksMyDb(dbSize/pageSize){

    firstBlockOffset = sizeof(dirtySet);
    
    char buf[1024];
    for (int i = 0 ; i<dbSize; i+=1024) {
        write(fileDesc, buf, 1024);
    }
    lseek(fileDesc, 0, SEEK_SET);
    
    
    write(fileDesc, &dirtySet, sizeof(dirtySet));
    
    /*dirtySet.flip();
    for (int i = 0; i<maxBlocksMyDb ; ++i) {
        dirtySet[i] = 0;
    }*/
}


int PageAllocator::allocatePage(){
    
    for(int i= 0; i< maxBlocksMyDb; ++i){
        std::cout<<dirtySet[i]<<std::endl;
        if (!dirtySet[i]){
            dirtySet.flip(i) ;
            std::cout<<dirtySet<<std::endl;
            return firstBlockOffset + i*pageSize;
        }
    }
    //blocksCount++;
    //return pageSize*(blocksCount - 1);
    return -1;
}

void PageAllocator::freePage(int offset){
    int pageid = (offset - firstBlockOffset)/pageSize;
    dirtySet[pageid] = 0;
}