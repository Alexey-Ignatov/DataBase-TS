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

struct DB *dbcreate(const char *path, const struct DBC conf);

extern "C" int db_close(struct DB *db);
extern "C" int db_del(const struct DB *, void *, size_t);
extern "C" int db_get(const struct DB *, void *, size_t, void **, size_t *);
extern "C" int db_put(const struct DB *, void *, size_t, void * , size_t  );


class BTree
{
    
public:
    BTreeNode root; // Pointer to root node
    int t;  // Minimum degree
    
    int dbSize;
    int fileDesc;
    
    PageAllocator pageAllocator;
    

    
    // Constructor (Initializes tree as empty)
    BTree(int _t, std::string path, int dbSize, int pageSize);

    void traverse();
    
    // function to search a key in this tree
    Record search(Key k);
    
    // The main function that inserts a new key in this B-Tree
    void insert(Key k, Record block);
    
    // The main function that removes a new key in thie B-Tree
    void remove(Key k);
    //TODO: чтобы избежать фокусов с тем, что я кручу права доступа, нужно, например, делать френд класс для тестировщика
};

extern "C" struct DB {
    BTree db;
};

struct DBC {
    size_t db_size;
    size_t chunk_size;
};


#endif /* defined(__DataBaseTS__BTree__) */
