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
#include "string"

#include "PageAllocator.h"

#define MAXKEYSIZE 40
#define MAXRECSIZE 100

using namespace std;
// A BTree node

class Key {
public:
    int size;
    char data[MAXKEYSIZE];
    
    bool operator <(const Key& key) const;
    bool operator ==(const Key& key) const;
    bool operator >(const Key& key) const;
    Key& operator=(const Key& other);
    
    Key(const char *key, int size);
    Key(const Key& other);
    Key();
    Key(const char *keyStr);
};

class Record {
    int size;
    char data[MAXRECSIZE];
public:    
    
    Record(const char *record, int recSize);
    Record();
    
    const char *getSrc()const;
    int getSize();
    
    Record& operator=(const Record& other);
    bool empty()const;

};


class BTreeNode
{
public:
    int t;
    int n;
    bool leaf;
    
    std::vector<int >childrenOffsets;
    vector<pair<Key, Record> > keysValues;

    int fileDesc;
    int ownOffset;
    
    PageAllocator *pageAllocator;
    
    void printKeys();
    void printChOffsets();
    void printBlocks();
    
    int writeNode(int offset);
    
    BTreeNode *readNode(int offset);
    
    BTreeNode(int t,int fileDesc, PageAllocator *pageAlloc);
    
    void traverse();

    Record search(Key k);
    
    int findKey(Key k);
    
    void insertNonFull(Key k, Record block);
    
    void splitChild(int i);
    
    void remove(Key k);
    
    void removeFromLeaf(int idx);
    
    void removeFromNonLeaf(int idx);
    
    pair<Key, Record>  getPred(int idx);
    
    pair<Key, Record>  getSucc(int idx);
    
    void fill(int idx);
    
    void borrowFromPrev(int idx);
    
    void borrowFromNext(int idx);
    
    void merge(int idx);
    
    friend class BTree;
};



#endif /* defined(__DataBaseTS__BTreeNode__) */
