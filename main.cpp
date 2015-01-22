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
    /*int tree_t = 2;
    int blockSize = 32;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*4 + 2*tree_t*4 + (2*tree_t-1)*blockSize;//80t - 39
    int dbSize =102400;
    
    char c1[100] = "horseporn!!!!!!!!!!!!!!!!!!!!300";
    char c2[100] = "horseporn!!!!!!!!!!!!!!!!!!!!200";
    char c3[100] = "horseporn!!!!!!!!!!!!!!!!!!!!300";
    char c4[100] = "horseporn!!!!!!!!!!!!!!!!!!!!400";
    char c5[100] = "horseporn!!!!!!!!!!!!!!!!!!!!500";
    char c6[100] = "horseporn!!!!!!!!!!!!!!!!!!!!600";
    char c7[100] = "horseporn!!!!!!!!!!!!!!!!!!!!700";
    
    
    
    BTree a(tree_t, "m.txt", blockSize, dbSize);
    a.insert(Key("100",3), c1);
    a.insert(Key("200",3), c1);
    a.insert(Key("300",3), c1);
    a.insert(Key("400",3), c1);
    /*a.insert(Key("500",3), c1);
    a.insert(Key("600",3), c1);
    a.insert(Key("700",3), c1);*/

    //a.traverse();

    
    int t = 3;
    int blockSize = 32;
    
    char c1[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!10";
    char c2[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!20";
    char c3[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!30";
    char c4[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!40";
    char c5[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!50";
    
    int file = open("./ForGTests/Test1NodereadNodeWrite.txt", O_RDWR | O_CREAT);
    
    write(file, c1, 1024);
    lseek(file, 0, SEEK_SET);
    
    BTreeNode b(t, file, blockSize, NULL);
    b.keysValues[0].first = "12345";
    b.keysValues[1].first = "12346";
    b.keysValues[2].first = "12347";
    b.keysValues[3].first = "12348";
    b.keysValues[4].first = "12349";
    
    b.childrenOffsets[0] = 4320;
    b.childrenOffsets[1] = 4321;
    b.childrenOffsets[2] = 4322;
    b.childrenOffsets[3] = 4323;
    b.childrenOffsets[4] = 4324;
    b.childrenOffsets[5] = 4325;
    
    b.keysValues[0].second = c1;
    b.keysValues[1].second = c2;
    b.keysValues[2].second = c3;
    b.keysValues[3].second = c4;
    b.keysValues[4].second = c5;
    
    b.n = 5;
    b.leaf = true;
    
    b.writeNode(0);
    
    BTreeNode c(t, file, blockSize, NULL);
    c.readNode(0);
    
    
    return 0;
}