//
//  main.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include<iostream>
#include "BTreeNode.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <string>

#include <stdio.h>
#include <fstream>

using namespace std;

// Driver program to test above functions
int main()
{
    
    int tree_t = 2;
    int blockSize = 32;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*4 + 2*tree_t*4 + (2*tree_t-1)*blockSize;//80t - 39
    int dbSize =102400;
    
    
    char c1[100] = "horseporn!!!!!!!!!!!!!!!!!!!!100";
    char c2[100] = "horseporn!!!!!!!!!!!!!!!!!!!!200";
    char c3[100] = "horseporn!!!!!!!!!!!!!!!!!!!!300";
    char c4[100] = "horseporn!!!!!!!!!!!!!!!!!!!!400";
    char c5[100] = "horseporn!!!!!!!!!!!!!!!!!!!!500";
    char c6[100] = "horseporn!!!!!!!!!!!!!!!!!!!!150";
    char c7[100] = "horseporn!!!!!!!!!!!!!!!!!!!!110";
    char c8[100] = "horseporn!!!!!!!!!!!!!!!!!!!!120";
    int file = open("from main.txt", O_RDWR | O_CREAT);
    
    
    PageAllocator *pageAlloc = new PageAllocator(file, nodeSizeOnDisk);
    
    write(file, c1, dbSize);
    lseek(file, 0, SEEK_SET);
    
    
    int headOffset = pageAlloc->allocatePage();
    int leftOffset = pageAlloc->allocatePage();
    int midOffset = pageAlloc->allocatePage();
    int rightOffset = pageAlloc->allocatePage();
    
    cout<<headOffset<<endl;
    cout<<leftOffset<<endl;
    cout<<midOffset<<endl;
    cout<<rightOffset<<endl<<endl;

    
    BTreeNode head(tree_t, file, blockSize, pageAlloc);
    head.keys[0] = 200;
    head.keys[1] = 400;
    
    head.childrenOffsets[0] = leftOffset;
    head.childrenOffsets[1] = midOffset;
    head.childrenOffsets[2] = rightOffset;

    head.blocks[0] = c2;
    head.blocks[1] = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(headOffset);
    
    
    BTreeNode left(tree_t, file, blockSize, pageAlloc);
    left.keys[0] = 100;
    left.keys[1] = 110;
    left.keys[2] = 120;
    left.blocks[0] = c1;
    left.blocks[1] = c7;
    left.blocks[2] = c8;
    left.n = 3;
    left.leaf = true;
    left.writeNode(leftOffset);
    
    BTreeNode mid(tree_t, file, blockSize, pageAlloc);
    mid.keys[0] = 300;
    mid.blocks[0] = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(midOffset);
    
    BTreeNode right(tree_t, file, blockSize, pageAlloc);
    right.keys[0] = 500;
    right.blocks[0] = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(rightOffset);
    
    //
    //head.printBlocks();
    
    //head.insertNonFull(150, c6);

    cout << string(head.search(500), 32)<<endl<<endl;
    
    //cout<< "offset "<<lseek(file, 0, SEEK_CUR)<<endl;
    //cout<< "suggoffset "<<4*nodeSizeOnDisk<<endl;
    
    
    //cout << c.childrenOffsets[0]<<endl;
    //cout << c.childrenOffsets[1]<<endl;
    //cout << c.childrenOffsets[2]<<endl<<endl;

    //cout << string(head.blocks[0], 32)<<endl;
    //cout << string(c.blocks[1], 32)<<endl<<endl;
    //cout<< "is leaf "<<c.leaf<<endl;
    return 0;
}