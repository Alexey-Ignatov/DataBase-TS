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
#include <stdlib.h>

#include "BTreeNode.h"

class BTree
{
    
public:
    BTreeNode root;
    int t;
    
    int dbSize;
    int fileDesc;
    
    PageAllocator pageAllocator;

    BTree(int _t, std::string path, int dbSize, int pageSize);

    void traverse();
    Record search(Key k);
    void insert(Key k, Record block);
    void remove(Key k);
    
};

extern "C" {
    int Cdel(struct DB *db, struct DBT *key);
    int Cget(struct DB *db, struct DBT *key, struct DBT *data);
    int Cput(struct DB *db, struct DBT *key, struct DBT *data);
    
    
    struct DB *dbcreate(const char *, const struct DBC);
    int db_close(struct DB *db);
    int db_del(struct DB *, void *, size_t);
    int db_get(struct DB *, void *, size_t, void **, size_t *);
    int db_put(struct DB *, void *, size_t, void * , size_t  );
    
    struct DBT {
        void  *data;
        size_t size;
    };
    
    struct DB {
        int test = 5;
        int (*close)(struct DB *db);
        int (*del)(struct DB *db, struct DBT *key);
        int (*get)(struct DB *db, struct DBT *key, struct DBT *data);
        int (*put)(struct DB *db, struct DBT *key, struct DBT *data);
        BTree *bTree;
        
    };
    struct DBC {
        size_t db_size;
        size_t chunk_size;
    };
    
}

#endif /* defined(__DataBaseTS__BTree__) */
