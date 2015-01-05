//
//  main.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

/* The following program performs deletion on a B-Tree. It contains functions
 specific for deletion along with all the other functions provided in the
 previous articles on B-Trees. See http://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
 for previous article.
 
 The deletion function has been compartmentalized into 8 functions for ease
 of understanding and clarity
 
 The following functions are exclusive for deletion
 In class BTreeNode:
 1) remove
 2) removeFromLeaf
 3) removeFromNonLeaf
 4) getPred
 5) getSucc
 6) borrowFromPrev
 7) borrowFromNext
 8) merge
 9) findKey
 
 In class BTree:
 1) remove
 
 The removal of a key from a B-Tree is a fairly complicated process. The program handles
 all the 6 different cases that might arise while removing a key.
 
 Testing: The code has been tested using the B-Tree provided in the CLRS book( included
 in the main function ) along with other cases.
 
 Reference: CLRS3 - Chapter 18 - (499-502)
 It is advised to read the material in CLRS before taking a look at the code. */

#include<iostream>
#include "BTree.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

#include <stdio.h>
#include <fstream>

using namespace std;

// Driver program to test above functions
int main()
{
    BTree t(3); // A B-Tree with minium degree 3
    
    t.insert(1);
    t.insert(3);
    t.insert(7);
    t.insert(10);
    t.insert(11);
    t.insert(13);
    t.insert(14);
    t.insert(15);
    t.insert(18);
    t.insert(16);
    t.insert(19);
    t.insert(24);
    t.insert(25);
    t.insert(26);
    t.insert(21);
    t.insert(4);
    t.insert(5);
    t.insert(20);
    t.insert(22);
    t.insert(2);
    t.insert(17);
    t.insert(12);
    t.insert(6);
    
    cout << "Traversal of tree constructed is\n";
    t.traverse();
    cout << endl;
    
    t.remove(6);
    cout << "Traversal of tree after removing 6\n";
    t.traverse();
    cout << endl;
    
    t.remove(13);
    cout << "Traversal of tree after removing 13\n";
    t.traverse();
    cout << endl;
    
    t.remove(7);
    cout << "Traversal of tree after removing 7\n";
    t.traverse();
    cout << endl;
    
    t.remove(4);
    cout << "Traversal of tree after removing 4\n";
    t.traverse();
    cout << endl;
    
    t.remove(2);
    cout << "Traversal of tree after removing 2\n";
    t.traverse();
    cout << endl;
    
    t.remove(16);
    cout << "Traversal of tree after removing 16\n";
    t.traverse();
    cout << endl;
    
    /*char buf[20];
    size_t nbytes;
    ssize_t bytes_written;
    int fd= open("m.txt", O_WRONLY | O_CREAT);
    strcpy(buf, "This is a test\n");
    nbytes = strlen(buf);
    
    bytes_written = write(fd, buf, nbytes);*/


    
    /*char c[100] = "horseporn!!!!!!!!!!!!!!!!!!!!!";
    int file = open("m.txt", O_WRONLY | O_CREAT);
    fflush(stdout);
    write(file,c ,15 );
    lseek(file, 5, SEEK_SET);
    write(file,c ,15 );    
    fflush(stdout);*/
    
    
    return 0;
}