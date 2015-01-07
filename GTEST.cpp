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
#include <string.h>
#include <string>

#include <stdio.h>
#include <fstream>

// my first fancy addition test
TEST (Calculator, Addition)
{
  
    
    char c1[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!10";
    char c2[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!20";
    char c3[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!30";
    char c4[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!40";
    char c5[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!50";
    
    int file = open("./ForGTests/Test1NodereadNodeWrite.txt", O_RDWR | O_CREAT);
    
    BTreeNode b(3, true, file);
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
    
    b.blocks.push_back(c1);
    b.blocks.push_back(c2);
    b.blocks.push_back(c3);
    b.blocks.push_back(c4);
    b.blocks.push_back(c5);
    
    b.n = 5;
    
    b.writeNode();
    
    BTreeNode c(3, true, file);
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
}

int main(int argc, char * argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
