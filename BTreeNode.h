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
#include <utility>
#include <string.h>
#include <algorithm>

#include "PageAllocator.h"

#define MAXKEYSIZE 40
//#define min(a, b) ((a) < (b) ? (a) : (b))
//#define max(a, b) ((a) > (b) ? (a) : (b))

using namespace std;
// A BTree node
struct Key;
class BTreeNode
{
public:
    int t;     // Minimum degree (defines the range for number of keys)
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
    
    //int *childrenOffsets; // An array of child pointers
    std::vector<int >childrenOffsets;
    vector<pair<Key, char*> > keysValues;
    
    int blockSize;//memory block size
    
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
    char *search(Key k);   // returns NULL if k is not present.
    
    // A function that returns the index of the first key that is greater
    // or equal to k
    int findKey(Key k);
    
    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(Key k, char *block);
    
    // A utility function to split the child y of this node. i is index
    // of y in child array C[].  The Child y must be full when this
    // function is called
    void splitChild(int i);
    
    // A wrapper function to remove the key k in subtree rooted with
    // this node.
    void remove(Key k);
    
    // A function to remove the key present in idx-th position in
    // this node which is a leaf
    void removeFromLeaf(int idx);
    
    // A function to remove the key present in idx-th position in
    // this node which is a non-leaf node
    void removeFromNonLeaf(int idx);
    
    // A function to get the predecessor of the key- where the key
    // is present in the idx-th position in the node
    pair<Key, char*>  getPred(int idx);
    
    // A function to get the successor of the key- where the key
    // is present in the idx-th position in the node
    pair<Key, char*>  getSucc(int idx);
    
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

int keycmp(struct Key *a, struct Key *b);


struct Key {
    
    Key(const char *key, int size): size(size){
        strcpy(data, key);
    };
    
    int size;
    char data[MAXKEYSIZE];
    
    bool operator <(const Key& key) const
    {
        int result = memcmp(data, key.data, min(size, key.size));
        if(result < 0){
            return true;
        }else{
            return false;
        }
        return (size > key.size);
    }
    
    bool operator ==(const Key& key) const
    {
        return !memcmp(data, key.data, size)  && size == key.size;
    }
    
    bool operator >(const Key& key) const
    {
        return !(  *this < key || *this == key );
        //return size >key.size;
    }
    
    Key& operator=(const Key& other){
        size = other.size;
        if(size == 0){
            return *this;
        }
        memcpy(data, other.data, other.size);
        return *this;
    }
    
    Key(const Key& other){
        if (this != &other) {
            size = other.size;
            memcpy(data, other.data, other.size);
        }
    }
    
    Key():size(0){};
    
    Key(const char *keyStr){
        size = strlen(keyStr);
        strcpy(data, keyStr);
    };
};


#endif /* defined(__DataBaseTS__BTreeNode__) */
