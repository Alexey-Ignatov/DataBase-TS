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
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*4 + 2*tree_t*4 + (2*tree_t-1)*32;//80t - 39
    int blockSize = 32;
    
    char c1[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!10";
    char c2[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!20";
    char c3[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!30";
    char c4[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!40";
    char c5[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!50";
    char c6[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!60";
    int file = open("from main.txt", O_RDWR | O_CREAT);
    
    write(file, c1, 1024);
    lseek(file, 0, SEEK_SET);
    
    
    BTreeNode head(tree_t, file, blockSize);
    head.keys[0] = 200;
    head.keys[1] = 400;
    
    head.childrenOffsets[0] = nodeSizeOnDisk;
    head.childrenOffsets[1] = 2*nodeSizeOnDisk;
    head.childrenOffsets[2] = 3*nodeSizeOnDisk;

    head.blocks.push_back(c2);
    head.blocks.push_back(c4);
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(0);
    
    
    BTreeNode left(tree_t, file, blockSize);
    left.keys[0] = 100;
    left.blocks.push_back(c1);
    left.n = 1;
    left.leaf = true;
    left.writeNode(nodeSizeOnDisk);
    
    BTreeNode mid(tree_t, file, blockSize);
    mid.keys[0] = 300;
    mid.blocks.push_back(c3);
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(2*nodeSizeOnDisk);
    
    BTreeNode right(tree_t, file, blockSize);
    right.keys[0] = 500;
    right.blocks.push_back(c5);
    right.n = 1;
    right.leaf = true;
    right.writeNode(3*nodeSizeOnDisk);
    
    head.insertNonFull(150, c6);
    
    //cout<< "offset "<<lseek(file, 0, SEEK_CUR)<<endl;
    //cout<< "suggoffset "<<4*nodeSizeOnDisk<<endl;
    
    BTreeNode c(tree_t,  file, blockSize);
    c.readNode(1*nodeSizeOnDisk);
    
    
    cout << c.keys[0]<<endl;
    cout << c.keys[1]<<endl<<endl;
    
    //cout << c.childrenOffsets[0]<<endl;
    //cout << c.childrenOffsets[1]<<endl;
    //cout << c.childrenOffsets[2]<<endl<<endl;

    cout << string(c.blocks[0], 32)<<endl;
    cout << string(c.blocks[1], 32)<<endl<<endl;
    
    cout<< "is leaf "<<c.leaf<<endl;
    return 0;
}