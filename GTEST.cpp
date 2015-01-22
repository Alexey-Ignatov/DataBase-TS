//
//  main.cpp
//  Tests
//
//  Created by Алексей on 05.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#include "gtest/gtest.h"

#include "BTreeNode.h"
#include "BTree.h"

#include <vector>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include <string>

#include <stdio.h>
#include <fstream>

// my first fancy addition test

TEST (BTreeNode, write_read0)
{
  
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
    
    
    EXPECT_EQ(std::string(c.keysValues[0].first.data,c.keysValues[0].first.size ), std::string("12345", 5));
    EXPECT_EQ(std::string(c.keysValues[1].first.data,c.keysValues[1].first.size ), std::string("12346", 5));
    EXPECT_EQ(std::string(c.keysValues[2].first.data,c.keysValues[2].first.size ), std::string("12347", 5));
    EXPECT_EQ(std::string(c.keysValues[3].first.data,c.keysValues[3].first.size ), std::string("12348", 5));
    EXPECT_EQ(std::string(c.keysValues[4].first.data,c.keysValues[4].first.size ), std::string("12349", 5));

    
    EXPECT_EQ(c.childrenOffsets[0], 4320);
    EXPECT_EQ(c.childrenOffsets[1], 4321);
    EXPECT_EQ(c.childrenOffsets[2], 4322);
    EXPECT_EQ(c.childrenOffsets[3], 4323);
    EXPECT_EQ(c.childrenOffsets[4], 4324);
    EXPECT_EQ(c.childrenOffsets[5], 4325);
    
    EXPECT_EQ( std::string(c.keysValues[0].second, 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(c.keysValues[1].second, 32),std::string(c2, 32) );
    EXPECT_EQ( std::string(c.keysValues[2].second, 32),std::string(c3, 32) );
    EXPECT_EQ( std::string(c.keysValues[3].second, 32),std::string(c4, 32) );
    EXPECT_EQ( std::string(c.keysValues[4].second, 32),std::string(c5, 32) );
    
    EXPECT_EQ(c.leaf,1 );
    
}

TEST (BTreeNode, write_read1)
{
    
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

    
    b.childrenOffsets[0] = 4320;
    b.childrenOffsets[1] = 4321;
    b.childrenOffsets[2] = 4322;
    b.childrenOffsets[3] = 4323;

    
    b.keysValues[0].second = c1;
    b.keysValues[1].second = c2;
    b.keysValues[2].second = c3;

    
    b.n = 3;
    b.leaf = false;
    
    b.writeNode(0);
    
    BTreeNode c(t, file, blockSize, NULL);
    c.readNode(0);
    
    EXPECT_EQ(std::string(c.keysValues[0].first.data,c.keysValues[0].first.size ), std::string("12345", 5));
    EXPECT_EQ(std::string(c.keysValues[1].first.data,c.keysValues[1].first.size ), std::string("12346", 5));
    EXPECT_EQ(std::string(c.keysValues[2].first.data,c.keysValues[2].first.size ), std::string("12347", 5));

    
    EXPECT_EQ(c.childrenOffsets[0], 4320);
    EXPECT_EQ(c.childrenOffsets[1], 4321);
    EXPECT_EQ(c.childrenOffsets[2], 4322);
    EXPECT_EQ(c.childrenOffsets[3], 4323);

    
    EXPECT_EQ( std::string(c.keysValues[0].second, 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(c.keysValues[1].second, 32),std::string(c2, 32) );
    EXPECT_EQ( std::string(c.keysValues[2].second, 32),std::string(c3, 32) );
    
     EXPECT_EQ(c.leaf,0 );


}

TEST (BTreeNode, simple_insert0)
{
    
    int tree_t = 2;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*sizeof(Key) + 2*tree_t*4 + (2*tree_t-1)*32;//80t - 39
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
    
    
    BTreeNode head(tree_t, file, blockSize, NULL);
    head.keysValues[0].first = "200";
    head.keysValues[1].first = "400";
    
    head.childrenOffsets[0] = nodeSizeOnDisk;
    head.childrenOffsets[1] = 2*nodeSizeOnDisk;
    head.childrenOffsets[2] = 3*nodeSizeOnDisk;
    
    head.keysValues[0].second = c2;
    head.keysValues[1].second = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(0);
    
    
    BTreeNode left(tree_t, file, blockSize, NULL);
    left.keysValues[0].first = "100";
    left.keysValues[0].second = c1;
    left.n = 1;
    left.leaf = true;
    left.writeNode(nodeSizeOnDisk);
    
    BTreeNode mid(tree_t, file, blockSize, NULL);
    mid.keysValues[0].first = "300";
    mid.keysValues[0].second = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(2*nodeSizeOnDisk);
    
    BTreeNode right(tree_t, file, blockSize, NULL);
    right.keysValues[0].first = "500";
    right.keysValues[0].second = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(3*nodeSizeOnDisk);
    
    head.insertNonFull("150", c6);
    
    BTreeNode c(tree_t,  file, blockSize, NULL);
    c.readNode(1*nodeSizeOnDisk);
    
    EXPECT_EQ(std::string(c.keysValues[0].first.data,c.keysValues[0].first.size ), std::string("100", 3));
    EXPECT_EQ(std::string(c.keysValues[1].first.data,c.keysValues[1].first.size ), std::string("150", 3));
    
    EXPECT_EQ( std::string(c.keysValues[0].second, 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(c.keysValues[1].second, 32),std::string(c6, 32) );
}

TEST (BTreeNode, simple_insert1)
{
    
    int tree_t = 2;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*sizeof(Key) + 2*tree_t*4 + (2*tree_t-1)*32;//80t - 39
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
    
    
    BTreeNode head(tree_t, file, blockSize, NULL);
    head.keysValues[0].first = "200";
    head.keysValues[1].first = "400";
    
    head.childrenOffsets[0] = nodeSizeOnDisk;
    head.childrenOffsets[1] = 2*nodeSizeOnDisk;
    head.childrenOffsets[2] = 3*nodeSizeOnDisk;
    
    head.keysValues[0].second = c2;
    head.keysValues[1].second = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(0);
    
    
    BTreeNode left(tree_t, file, blockSize, NULL);
    left.keysValues[0].first = "100";
    left.keysValues[0].second = c1;
    left.n = 1;
    left.leaf = true;
    left.writeNode(nodeSizeOnDisk);
    
    BTreeNode mid(tree_t, file, blockSize, NULL);
    mid.keysValues[0].first = "300";
    mid.keysValues[0].second = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(2*nodeSizeOnDisk);
    
    BTreeNode right(tree_t, file, blockSize, NULL);
    right.keysValues[0].first = "500";
    right.keysValues[0].second = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(3*nodeSizeOnDisk);
    
    head.insertNonFull("999", c6);
    
    BTreeNode c(tree_t,  file, blockSize, NULL);
    c.readNode(3*nodeSizeOnDisk);
    
    EXPECT_EQ(std::string(c.keysValues[0].first.data,c.keysValues[0].first.size ), std::string("500", 3));
    EXPECT_EQ(std::string(c.keysValues[1].first.data,c.keysValues[1].first.size ), std::string("999", 3));
    
    EXPECT_EQ( std::string(c.keysValues[0].second, 32),std::string(c5, 32) );
    EXPECT_EQ( std::string(c.keysValues[1].second, 32),std::string(c6, 32) );
}

TEST (BTreeNode, split_insert1)
{
    
    int tree_t = 2;
    int blockSize = 32;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*sizeof(Key) + 2*tree_t*4 + (2*tree_t-1)*32;//80t - 39
    
    
    
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
    
    write(file, c1, 1024);
    lseek(file, 0, SEEK_SET);
    
    
    int headOffset = pageAlloc->allocatePage();
    int leftOffset = pageAlloc->allocatePage();
    int midOffset = pageAlloc->allocatePage();
    int rightOffset = pageAlloc->allocatePage();
    
    
    BTreeNode head(tree_t, file, blockSize, pageAlloc);
    head.keysValues[0].first = "200";
    head.keysValues[1].first = "400";
    
    head.childrenOffsets[0] = leftOffset;
    head.childrenOffsets[1] = midOffset;
    head.childrenOffsets[2] = rightOffset;
    
    head.keysValues[0].second = c2;
    head.keysValues[1].second = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(headOffset);
    
    
    BTreeNode left(tree_t, file, blockSize, pageAlloc);
    left.keysValues[0].first = "100";
    left.keysValues[1].first = "110";
    left.keysValues[2].first = "120";
    left.keysValues[0].second = c1;
    left.keysValues[1].second = c7;
    left.keysValues[2].second = c8;
    left.n = 3;
    left.leaf = true;
    left.writeNode(leftOffset);
    
    BTreeNode mid(tree_t, file, blockSize, pageAlloc);
    mid.keysValues[0].first = "300";
    mid.keysValues[0].second = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(midOffset);
    
    BTreeNode right(tree_t, file, blockSize, pageAlloc);
    right.keysValues[0].first = "500";
    right.keysValues[0].second = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(rightOffset);
    
    //
    //head.printBlocks();
    
    head.insertNonFull("150", c6);
    
    BTreeNode lastNode(tree_t, file, blockSize, pageAlloc);
    lastNode.readNode(4*nodeSizeOnDisk);
    
    

    
    EXPECT_EQ(std::string(head.keysValues[0].first.data,head.keysValues[0].first.size ), std::string("110", 3));
    EXPECT_EQ(std::string(head.keysValues[1].first.data,head.keysValues[1].first.size ), std::string("200", 3));
    EXPECT_EQ(std::string(head.keysValues[2].first.data,head.keysValues[2].first.size ), std::string("400", 3));
    
    EXPECT_EQ(std::string(left.keysValues[0].first.data,left.keysValues[0].first.size ), std::string("100", 3));
    
    EXPECT_EQ(std::string(lastNode.keysValues[0].first.data,lastNode.keysValues[0].first.size ), std::string("120", 3));
    EXPECT_EQ(std::string(lastNode.keysValues[1].first.data,lastNode.keysValues[1].first.size ), std::string("150", 3));
    

    EXPECT_EQ( std::string(head.keysValues[0].second, 32),std::string(c7, 32) );
    EXPECT_EQ( std::string(head.keysValues[1].second, 32),std::string(c2, 32) );
    EXPECT_EQ( std::string(head.keysValues[2].second, 32),std::string(c4, 32) );
    
    EXPECT_EQ( std::string(left.keysValues[0].second, 32),std::string(c1, 32) );
    
    EXPECT_EQ( std::string(lastNode.keysValues[0].second, 32),std::string(c8, 32) );
    EXPECT_EQ( std::string(lastNode.keysValues[1].second, 32),std::string(c6, 32) );
}

TEST (BTreeNode, search0)
{
    
    int tree_t = 2;
    int blockSize = 32;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*sizeof(Key) + 2*tree_t*4 + (2*tree_t-1)*32;
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
    
    
    BTreeNode head(tree_t, file, blockSize, pageAlloc);
    head.keysValues[0].first = "200";
    head.keysValues[1].first = "400";
    
    head.childrenOffsets[0] = leftOffset;
    head.childrenOffsets[1] = midOffset;
    head.childrenOffsets[2] = rightOffset;
    
    head.keysValues[0].second = c2;
    head.keysValues[1].second = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(headOffset);
    
    
    BTreeNode left(tree_t, file, blockSize, pageAlloc);
    left.keysValues[0].first = "100";
    left.keysValues[1].first = "110";
    left.keysValues[2].first = "120";
    left.keysValues[0].second = c1;
    left.keysValues[1].second = c7;
    left.keysValues[2].second = c8;
    left.n = 3;
    left.leaf = true;
    left.writeNode(leftOffset);
    
    BTreeNode mid(tree_t, file, blockSize, pageAlloc);
    mid.keysValues[0].first = "300";
    mid.keysValues[0].second = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(midOffset);
    
    BTreeNode right(tree_t, file, blockSize, pageAlloc);
    right.keysValues[0].first = "500";
    right.keysValues[0].second = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(rightOffset);
    
    EXPECT_EQ( std::string(head.search("500"), 32),std::string(c5, 32) );
    EXPECT_EQ( std::string(head.search("100"), 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(head.search("300"), 32),std::string(c3, 32) );
    EXPECT_EQ( std::string(head.search("200"), 32),std::string(c2, 32) );
}

TEST (BTreeNode, removeStillHead)
{
    
    int tree_t = 2;
    int blockSize = 32;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*sizeof(Key) + 2*tree_t*4 + (2*tree_t-1)*32;
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
    
    
    BTreeNode head(tree_t, file, blockSize, pageAlloc);
    head.keysValues[0].first = "200";
    head.keysValues[1].first = "400";
    
    head.childrenOffsets[0] = leftOffset;
    head.childrenOffsets[1] = midOffset;
    head.childrenOffsets[2] = rightOffset;
    
    head.keysValues[0].second = c2;
    head.keysValues[1].second = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(headOffset);
    
    
    BTreeNode left(tree_t, file, blockSize, pageAlloc);
    left.keysValues[0].first = "100";
    left.keysValues[1].first = "110";
    left.keysValues[2].first = "120";
    left.keysValues[0].second = c1;
    left.keysValues[1].second = c7;
    left.keysValues[2].second = c8;
    left.n = 3;
    left.leaf = true;
    left.writeNode(leftOffset);
    
    BTreeNode mid(tree_t, file, blockSize, pageAlloc);
    mid.keysValues[0].first = "300";
    mid.keysValues[0].second = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(midOffset);
    
    BTreeNode right(tree_t, file, blockSize, pageAlloc);
    right.keysValues[0].first = "500";
    right.keysValues[0].second = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(rightOffset);
    
    //
    //head.printBlocks();
    
    head.remove("100");
    head.remove("110");
    head.remove("120");
    
    left.readNode(nodeSizeOnDisk);
    right.readNode(3*nodeSizeOnDisk);
    
    EXPECT_EQ(std::string(head.keysValues[0].first.data,head.keysValues[0].first.size ), std::string("400", 3));
    
    EXPECT_EQ(std::string(left.keysValues[0].first.data,left.keysValues[0].first.size ), std::string("200", 3));
    EXPECT_EQ(std::string(left.keysValues[1].first.data,left.keysValues[1].first.size ), std::string("300", 3));

    EXPECT_EQ(std::string(right.keysValues[0].first.data,right.keysValues[0].first.size ), std::string("500", 3));
    
    
    EXPECT_EQ( std::string(head.keysValues[0].second, 32),std::string(c4, 32) );
    
    EXPECT_EQ( std::string(left.keysValues[0].second, 32),std::string(c2, 32) );
    EXPECT_EQ( std::string(left.keysValues[1].second, 32),std::string(c3, 32) );
    
    EXPECT_EQ( std::string(right.keysValues[0].second, 32),std::string(c5, 32) );

}


TEST (BTree, simple_inserts)
{
    
    int tree_t = 2;
    int blockSize = 32;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*sizeof(Key) + 2*tree_t*4 + (2*tree_t-1)*32;
    int dbSize =102400;
    
    char c1[100] = "horseporn!!!!!!!!!!!!!!!!!!!!100";
    char c2[100] = "horseporn!!!!!!!!!!!!!!!!!!!!200";
    char c3[100] = "horseporn!!!!!!!!!!!!!!!!!!!!300";
    char c4[100] = "horseporn!!!!!!!!!!!!!!!!!!!!400";
    char c5[100] = "horseporn!!!!!!!!!!!!!!!!!!!!500";
    char c6[100] = "horseporn!!!!!!!!!!!!!!!!!!!!600";
    char c7[100] = "horseporn!!!!!!!!!!!!!!!!!!!!700";
    
    
    
    BTree a(tree_t, "m.txt", blockSize, dbSize, nodeSizeOnDisk);
    a.insert("100", c1);
    a.insert("200", c2);
    a.insert("300", c3);
    a.insert("400", c4);
    a.insert("500", c5);
    a.insert("600", c6);
    a.insert("700", c7);
    
    std::vector<int> testVec1(100, 1);
    std::vector<BTreeNode> testVec(4, BTreeNode(tree_t, a.fileDesc, blockSize, NULL));
    
    for (int i = 0; i < testVec.size(); ++i) {
        testVec[i].readNode(nodeSizeOnDisk*i);
    }
    
    EXPECT_EQ( std::string(testVec[1].keysValues[0].second, blockSize),std::string(c2, blockSize) );
    EXPECT_EQ( std::string(testVec[1].keysValues[1].second, blockSize),std::string(c4, blockSize) );
    
    EXPECT_EQ( std::string(testVec[0].keysValues[0].second, blockSize),std::string(c1, blockSize) );

    EXPECT_EQ( std::string(testVec[2].keysValues[0].second, blockSize),std::string(c3, blockSize) );
    
    EXPECT_EQ( std::string(testVec[3].keysValues[0].second, blockSize),std::string(c5, blockSize) );
    EXPECT_EQ( std::string(testVec[3].keysValues[1].second, blockSize),std::string(c6, blockSize) );
    EXPECT_EQ( std::string(testVec[3].keysValues[2].second, blockSize),std::string(c7, blockSize) );
    
}

int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
