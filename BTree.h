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
#include "BTreeNode.h"
class BTree
{
    BTreeNode *root; // Pointer to root node
    int t;  // Minimum degree
public:
    
    // Constructor (Initializes tree as empty)
    BTree(int _t);

    void traverse();
    
    // function to search a key in this tree
    BTreeNode* search(int k);
    
    // The main function that inserts a new key in this B-Tree
    void insert(int k);
    
    // The main function that removes a new key in thie B-Tree
    void remove(int k);
    
};

#endif /* defined(__DataBaseTS__BTree__) */
