//
//  DiskWorker.cpp
//  DataBase TS
//
//  Created by Алексей on 06.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#include "DiskWorker.h"
#include<iostream>
//#include "BTree.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <fstream>

int DiskWorker::writeNode(BTreeNode *target){
    
    
    offset = lseek(fileDesc, 0, SEEK_CUR);
    
    //пишем в конец
    //TODO: писать не в конец
    //сначала пишем ключи
    write(fileDesc, target->getKeys(), (2*target->getT() - 1)*sizeof(int));
    
    //потом потомков
    write(fileDesc, target->getChildren(), 2*target->getT()*sizeof(int));
    
    //затем блок
    write(fileDesc, &target->getBlocksVector()[0], (2*target->getT() - 1)*target->getBlockSize());
    //TODO: честно ли так поступать с вектором?
    
    
    
    return offset;
}



BTreeNode *DiskWorker::readNode(int offset){
    return NULL;
}


class BTreeNode
{
    int *keys;  // An array of keys
    int t;      // Minimum degree (defines the range for number of keys)
    BTreeNode **C; // An array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
    
    public: