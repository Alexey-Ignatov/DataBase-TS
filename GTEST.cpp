//
//  main.cpp
//  Tests
//
//  Created by Алексей on 05.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#include "gtest/gtest.h"
#include "BTreeNode.h"
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
    b.keys[0] = 12345;
    b.keys[1] = 12346;
    b.keys[2] = 12347;
    b.keys[3] = 12348;
    b.keys[4] = 12349;
    
    b.childrenOffsets[0] = 4320;
    b.childrenOffsets[1] = 4321;
    b.childrenOffsets[2] = 4322;
    b.childrenOffsets[3] = 4323;
    b.childrenOffsets[4] = 4324;
    b.childrenOffsets[5] = 4325;
    
    b.blocks[0] = c1;
    b.blocks[1] = c2;
    b.blocks[2] = c3;
    b.blocks[3] = c4;
    b.blocks[4] = c5;
    
    b.n = 5;
    b.leaf = true;
    
    b.writeNode(0);
    
    BTreeNode c(t, file, blockSize, NULL);
    c.readNode(0);
    
    EXPECT_EQ(c.keys[0], 12345);
    EXPECT_EQ(c.keys[1], 12346);
    EXPECT_EQ(c.keys[2], 12347);
    EXPECT_EQ(c.keys[3], 12348);
    EXPECT_EQ(c.keys[4], 12349);
    
    EXPECT_EQ(c.childrenOffsets[0], 4320);
    EXPECT_EQ(c.childrenOffsets[1], 4321);
    EXPECT_EQ(c.childrenOffsets[2], 4322);
    EXPECT_EQ(c.childrenOffsets[3], 4323);
    EXPECT_EQ(c.childrenOffsets[4], 4324);
    EXPECT_EQ(c.childrenOffsets[5], 4325);
    
    EXPECT_EQ( std::string(c.blocks[0], 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(c.blocks[1], 32),std::string(c2, 32) );
    EXPECT_EQ( std::string(c.blocks[2], 32),std::string(c3, 32) );
    EXPECT_EQ( std::string(c.blocks[3], 32),std::string(c4, 32) );
    EXPECT_EQ( std::string(c.blocks[4], 32),std::string(c5, 32) );
    
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
    b.keys[0] = 12345;
    b.keys[1] = 12346;
    b.keys[2] = 12347;

    
    b.childrenOffsets[0] = 4320;
    b.childrenOffsets[1] = 4321;
    b.childrenOffsets[2] = 4322;
    b.childrenOffsets[3] = 4323;

    
    b.blocks[0] = c1;
    b.blocks[1] = c2;
    b.blocks[2] = c3;

    
    b.n = 3;
    b.leaf = false;
    
    b.writeNode(0);
    
    BTreeNode c(t, file, blockSize, NULL);
    c.readNode(0);
    
    EXPECT_EQ(c.keys[0], 12345);
    EXPECT_EQ(c.keys[1], 12346);
    EXPECT_EQ(c.keys[2], 12347);

    
    EXPECT_EQ(c.childrenOffsets[0], 4320);
    EXPECT_EQ(c.childrenOffsets[1], 4321);
    EXPECT_EQ(c.childrenOffsets[2], 4322);
    EXPECT_EQ(c.childrenOffsets[3], 4323);

    
    EXPECT_EQ( std::string(c.blocks[0], 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(c.blocks[1], 32),std::string(c2, 32) );
    EXPECT_EQ( std::string(c.blocks[2], 32),std::string(c3, 32) );
    
    EXPECT_EQ(c.leaf,0 );


}

TEST (BTreeNode, simple_insert0)
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
    
    
    BTreeNode head(tree_t, file, blockSize, NULL);
    head.keys[0] = 200;
    head.keys[1] = 400;
    
    head.childrenOffsets[0] = nodeSizeOnDisk;
    head.childrenOffsets[1] = 2*nodeSizeOnDisk;
    head.childrenOffsets[2] = 3*nodeSizeOnDisk;
    
    head.blocks[0] = c2;
    head.blocks[1] = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(0);
    
    
    BTreeNode left(tree_t, file, blockSize, NULL);
    left.keys[0] = 100;
    left.blocks[0] = c1;
    left.n = 1;
    left.leaf = true;
    left.writeNode(nodeSizeOnDisk);
    
    BTreeNode mid(tree_t, file, blockSize, NULL);
    mid.keys[0] = 300;
    mid.blocks[0] = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(2*nodeSizeOnDisk);
    
    BTreeNode right(tree_t, file, blockSize, NULL);
    right.keys[0] = 500;
    right.blocks[0] = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(3*nodeSizeOnDisk);
    
    head.insertNonFull(150, c6);
    
    BTreeNode c(tree_t,  file, blockSize, NULL);
    c.readNode(1*nodeSizeOnDisk);
    
    EXPECT_EQ(c.keys[0], 100);
    EXPECT_EQ(c.keys[1], 150);
    
    EXPECT_EQ( std::string(c.blocks[0], 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(c.blocks[1], 32),std::string(c6, 32) );
}

TEST (BTreeNode, simple_insert1)
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
    
    
    BTreeNode head(tree_t, file, blockSize, NULL);
    head.keys[0] = 200;
    head.keys[1] = 400;
    
    head.childrenOffsets[0] = nodeSizeOnDisk;
    head.childrenOffsets[1] = 2*nodeSizeOnDisk;
    head.childrenOffsets[2] = 3*nodeSizeOnDisk;
    
    head.blocks[0] = c2;
    head.blocks[1] = c4;
    
    head.n = 2;
    head.leaf = false;
    head.writeNode(0);
    
    
    BTreeNode left(tree_t, file, blockSize, NULL);
    left.keys[0] = 100;
    left.blocks[0] = c1;
    left.n = 1;
    left.leaf = true;
    left.writeNode(nodeSizeOnDisk);
    
    BTreeNode mid(tree_t, file, blockSize, NULL);
    mid.keys[0] = 300;
    mid.blocks[0] = c3;
    mid.n = 1;
    mid.leaf = true;
    mid.writeNode(2*nodeSizeOnDisk);
    
    BTreeNode right(tree_t, file, blockSize, NULL);
    right.keys[0] = 500;
    right.blocks[0] = c5;
    right.n = 1;
    right.leaf = true;
    right.writeNode(3*nodeSizeOnDisk);
    
    head.insertNonFull(1000, c6);
    
    BTreeNode c(tree_t,  file, blockSize, NULL);
    c.readNode(3*nodeSizeOnDisk);
    
    EXPECT_EQ(c.keys[0], 500);
    EXPECT_EQ(c.keys[1], 1000);
    
    EXPECT_EQ( std::string(c.blocks[0], 32),std::string(c5, 32) );
    EXPECT_EQ( std::string(c.blocks[1], 32),std::string(c6, 32) );
}
TEST (BTreeNode, split_insert1)
{
    
    int tree_t = 2;
    int blockSize = 32;
    int nodeSizeOnDisk = 4 + 1 +(2*tree_t -1)*4 + 2*tree_t*4 + (2*tree_t-1)*blockSize;//80t - 39
    
    
    
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
    
    head.insertNonFull(150, c6);
    
    BTreeNode lastNode(tree_t, file, blockSize, pageAlloc);
    lastNode.readNode(4*nodeSizeOnDisk);
    
    
    EXPECT_EQ(head.keys[0], 110);
    EXPECT_EQ(head.keys[1], 200);
    EXPECT_EQ(head.keys[2], 400);
    
    EXPECT_EQ(left.keys[0], 100);

    EXPECT_EQ(lastNode.keys[0], 120);
    EXPECT_EQ(lastNode.keys[1], 150);
    
    
    
    
    EXPECT_EQ( std::string(head.blocks[0], 32),std::string(c7, 32) );
    EXPECT_EQ( std::string(head.blocks[1], 32),std::string(c2, 32) );
    EXPECT_EQ( std::string(head.blocks[2], 32),std::string(c4, 32) );
    
    EXPECT_EQ( std::string(left.blocks[0], 32),std::string(c1, 32) );
    
    EXPECT_EQ( std::string(lastNode.blocks[0], 32),std::string(c8, 32) );
    EXPECT_EQ( std::string(lastNode.blocks[1], 32),std::string(c6, 32) );
}

TEST (BTreeNode, search0)
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
    
    EXPECT_EQ( std::string(head.search(500), 32),std::string(c5, 32) );
    EXPECT_EQ( std::string(head.search(100), 32),std::string(c1, 32) );
    EXPECT_EQ( std::string(head.search(300), 32),std::string(c3, 32) );
    EXPECT_EQ( std::string(head.search(200), 32),std::string(c2, 32) );
}

int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
