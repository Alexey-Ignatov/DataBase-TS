//
//  main.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include<iostream>
#include "BTreeNode.h"
#include "BTree.h"

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
    char c6[100] = "horseporn!!!!!!!!!!!!!!!!!!!!600";
    char c7[100] = "horseporn!!!!!!!!!!!!!!!!!!!!700";
    
    
    
    BTree a(tree_t, "m.txt", blockSize, dbSize);
    a.insert(100, c1);
    a.insert(200, c2);
    a.insert(300, c3);
    a.insert(400, c4);
    a.insert(500, c5);
    a.insert(600, c6);
    a.insert(700, c7);
    
    std::vector<int> testVec1(100, 1);
    std::vector<BTreeNode> testVec(4, BTreeNode(tree_t, a.fileDesc, blockSize, NULL));
    
    for (int i = 0; i < testVec.size(); ++i) {
        testVec[i].readNode(nodeSizeOnDisk*i);
    }
    
    EXPECT_EQ( std::string(a.fi, 32),std::string(c4, 32) );
}