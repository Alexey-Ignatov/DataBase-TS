//
//  BTree.h
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#ifndef __DataBaseTS__BTree__
#define __DataBaseTS__BTree__

#include <stdio.h>
#include <string>

#include "BTreeNode.h"

class BTree
{
    
public:
    BTreeNode root; // Pointer to root node
    int t;  // Minimum degree
    
    int dbSize;
    int blockSize;
    int fileDesc;
    
    PageAllocator pageAllocator;
    

    
    // Constructor (Initializes tree as empty)
    BTree(int _t, std::string path, int blockSize, int dbSize);

    void traverse();
    
    // function to search a key in this tree
    char* search(int k);
    
    // The main function that inserts a new key in this B-Tree
    void insert(int k, char *block);
    
    // The main function that removes a new key in thie B-Tree
    void remove(int k);
    //TODO: чтобы избежать фокусов с тем, что я кручу права доступа, нужно, например, делать френд класс для тестировщика
};


#endif /* defined(__DataBaseTS__BTree__) */
