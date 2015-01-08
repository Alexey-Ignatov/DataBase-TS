//
//  BTreeNode.h
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#ifndef __DataBaseTS__BTreeNode__
#define __DataBaseTS__BTreeNode__

#include <stdio.h>
#include <vector>
#include <string>
#include<iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#include "PageAllocator.h"



// A BTree node
class BTreeNode
{
public:
    int t;     // Minimum degree (defines the range for number of keys)
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
    
    int *keys;  // An array of keys
    int *childrenOffsets; // An array of child pointers
    
    int blockSize;//memory block size
    std::vector<char *> blocks;//memory blocks
    
    //std::map<int, BTreeNode *> littleNodeCache;
    
    
    int fileDesc;
    int ownOffset;
    
    //TODO: добавить эти параметры в конструктор
    //TODO: или везде вектора или массивы
    PageAllocator *pageAllocator;
    
    void printKeys();
    void printChOffsets();
    void printBlocks();
    
    int writeNode(int offset);
    
    BTreeNode *readNode(int offset);
    
    BTreeNode(int t,int fileDesc, int blockSize, PageAllocator *pageAlloc);   // Constructor
    
    // A function to traverse all nodes in a subtree rooted with this node
    void traverse();
    
    // A function to search a key in subtree rooted with this node.
    char *search(int k);   // returns NULL if k is not present.
    
    // A function that returns the index of the first key that is greater
    // or equal to k
    int findKey(int k);
    
    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(int k, char *block);
    
    // A utility function to split the child y of this node. i is index
    // of y in child array C[].  The Child y must be full when this
    // function is called
    void splitChild(int i);
    
    // A wrapper function to remove the key k in subtree rooted with
    // this node.
    void remove(int k);
    
    // A function to remove the key present in idx-th position in
    // this node which is a leaf
    void removeFromLeaf(int idx);
    
    // A function to remove the key present in idx-th position in
    // this node which is a non-leaf node
    void removeFromNonLeaf(int idx);
    
    // A function to get the predecessor of the key- where the key
    // is present in the idx-th position in the node
    int getPred(int idx);
    
    // A function to get the successor of the key- where the key
    // is present in the idx-th position in the node
    int getSucc(int idx);
    
    // A function to fill up the child node present in the idx-th
    // position in the C[] array if that child has less than t-1 keys
    void fill(int idx);
    
    // A function to borrow a key from the C[idx-1]-th node and place
    // it in C[idx]th node
    void borrowFromPrev(int idx);
    
    // A function to borrow a key from the C[idx+1]-th node and place it
    // in C[idx]th node
    void borrowFromNext(int idx);
    
    // A function to merge idx-th child of the node with (idx+1)th child of
    // the node
    void merge(int idx);
    
    // Make BTree friend of this so that we can access private members of
    // this class in BTree functions
    friend class BTree;
};



#endif /* defined(__DataBaseTS__BTreeNode__) */
