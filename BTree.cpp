//
//  BTree.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include "BTree.h"


//*****************************************************************API************************************************************************

int Cdel(struct DB *db, struct DBT *key){
    Key toDel((char*) key->data, (int)key->size);
    db->bTree->remove(toDel);
    return 0;
}


int Cget(struct DB *db, struct DBT *key, struct DBT *data){
    Key toGet((char*) key->data, (int)key->size);
    Record answer = db->bTree->search(toGet);
    
    data->data = (char *)malloc(answer.getSize());
    
    memcpy( data->data, answer.getSrc(),answer.getSize() );
    data->size = (size_t) answer.getSize();
    return 1;
}

int Cput(struct DB *db, struct DBT *key, struct DBT *data){
    Key keyPut((char*) key->data, (int)key->size);
    Record dataPut((char*) data->data, (int)data->size);
    db->bTree->insert(keyPut, dataPut);
    return 1;
}


extern "C" struct DB *dbcreate(const char *path, const struct DBC conf){
    struct DB *db = (struct DB *)malloc(sizeof(struct DB));
    int tree_t = 12;
    db->bTree =new BTree(tree_t, path,conf.db_size, conf.chunk_size );
    db->put = Cput;
    db->get = Cget;
    db->del = Cdel;
    db->close =db_close;
    return db;
}


extern "C" int db_close(struct DB *db) {
    db->close(db);
    return 0;
}

extern "C" int db_del(struct DB *db, void *key, size_t key_len) {
    struct DBT keyt = {
        .data = key,
        .size = key_len
    };
    return db->del(db, &keyt);
}

extern "C" int db_get(struct DB *db, void *key, size_t key_len, void **val, size_t *val_len) {
    struct DBT keyt = {
        .data = key,
        .size = key_len
    };
    struct DBT valt = {0, 0};
    int rc = db->get(db, &keyt, &valt);
    *val = valt.data;
    *val_len = valt.size;
    return rc;
}

extern "C" int db_put(struct DB *db, void *key, size_t key_len, void *val, size_t val_len) {
    struct DBT keyt = {
        .data = key,
        .size = key_len
    };
    struct DBT valt = {
        .data = val,
        .size = val_len
    };
    return db->put(db, &keyt, &valt);
}

//*****************************************************************BTREE************************************************************************

BTree::BTree(int _t, std::string path, int dbSize, int pageSize):  dbSize(dbSize), root(BTreeNode(_t, 0, NULL)), pageAllocator(PageAllocator(-1, pageSize, dbSize))
{
    t = _t;
    
    fileDesc = open(path.c_str(), O_RDWR | O_CREAT, 0777);
    
    lseek(fileDesc, 0, SEEK_SET);
    pageAllocator.fileDesc = fileDesc;
    root.pageAllocator = &pageAllocator;
}


void BTree::traverse()
{
    if (root.n >0) root.traverse();
}


Record BTree::search(Key k){
    return root.search(k);
}



void BTree::insert(Key k, Record block)
{
    if (root.n ==0)
    {
        root.keysValues[0] = pair<Key, Record>(k, block);
        root.leaf = true;
        root.n++;
        root.fileDesc = fileDesc;
        root.ownOffset = pageAllocator.allocatePage();
        root.writeNode(root.ownOffset);
        //cout<<"BTree::insert2"<<endl;
        
    }
    else
    {
        if (root.n == 2*t-1)
        {
            BTreeNode s = BTreeNode(t,fileDesc, &pageAllocator );
            s.leaf =false;
            s.n = 0;
            s.ownOffset = pageAllocator.allocatePage();

            s.childrenOffsets[0] = root.ownOffset;
            fflush(stdout);
            s.splitChild(0);
            
            
            int i = 0;
            if (s.keysValues[0].first < k)
                i++;
            
            BTreeNode sChild =BTreeNode(t,fileDesc, &pageAllocator );
            sChild.readNode(s.childrenOffsets[i]);
            
            sChild.insertNonFull(k, block);
            
            root = s;
            s.writeNode(s.ownOffset);
        }
        else
            root.insertNonFull(k, block);
    }
}


void BTree::remove(Key k)
{
    if (root.n ==0)
    {
        std::cout << "The tree is empty\n";
        return;
    }
    
    root.remove(k);
    
    if (root.n==0)
    {
        if (root.leaf){
            root = BTreeNode(t,fileDesc, &pageAllocator );
        }else{
            BTreeNode minChild =BTreeNode(t,fileDesc, &pageAllocator );
            minChild.readNode(root.childrenOffsets[0]);
            root = minChild;
        }
        
    }
    return;
}

