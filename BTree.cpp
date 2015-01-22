//
//  BTree.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include "BTree.h"

/*struct DB *dbcreate(const char *path, const struct DBC conf){
    struct DB *db;
    
    db->db = BTree(conf.)
}*/










BTree::BTree(int _t, std::string path, int blockSize, int dbSize, int pageSize): blockSize( blockSize), dbSize(dbSize), root(BTreeNode(_t, 0, blockSize, NULL)), pageAllocator(PageAllocator(-1, pageSize))
{
    t = _t;
    fileDesc = open(path.c_str(), O_RDWR | O_CREAT);
    write(fileDesc, &t, dbSize);
    lseek(fileDesc, 0, SEEK_SET);
    
    pageAllocator.fileDesc = fileDesc;
    
    root.pageAllocator = &pageAllocator;
}

void BTree::traverse()
{
    if (root.n >0) root.traverse();
}


char* BTree::search(Key k){
    return (root.n >0)? NULL : root.search(k);
}



// The main function that inserts a new key in this B-Tree
void BTree::insert(Key k, char *block)
{
    // If tree is empty
    if (root.n ==0)
    {
        // Allocate memory for root
        root.keysValues[0] = pair<Key, char*>(k, block);
        root.leaf = true;
        root.n++;
        root.fileDesc = fileDesc;
        root.ownOffset = pageAllocator.allocatePage();
        
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root.n == 2*t-1)
        {
            // Allocate memory for new root
            BTreeNode s = BTreeNode(t,fileDesc, blockSize, &pageAllocator );
            s.leaf =false;
            s.n = 0;
            s.ownOffset = pageAllocator.allocatePage();
            // Make old root as child of new root
            s.childrenOffsets[0] = root.ownOffset;
            //s.n = 1;
            fflush(stdout);
            //s.traverse();
            // Split the old root and move 1 key to the new root
            s.splitChild(0);
            
            
            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s.keysValues[0].first < k)
                i++;
            
            BTreeNode sChild =BTreeNode(t,fileDesc, blockSize, &pageAllocator );
            sChild.readNode(s.childrenOffsets[i]);
            
            sChild.insertNonFull(k, block);
            
            // Change root
            //s.ownOffset = root.ownOffset;
            root = s;
            s.writeNode(s.ownOffset);
            //TODO:
        }
        else  // If root is not full, call insertNonFull for root
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
    
    // Call the remove function for root
    root.remove(k);
    
    // If the root node has 0 keys, make its first child as the new root
    //  if it has a child, otherwise set root as NULL
    if (root.n==0)
    {
        if (root.leaf){
            root = BTreeNode(t,fileDesc, blockSize, &pageAllocator );
        }else{
            BTreeNode minChild =BTreeNode(t,fileDesc, blockSize, &pageAllocator );
            minChild.readNode(root.childrenOffsets[0]);
            root = minChild;
        }
        
    }
    return;
}
//TODO: решить проблему с дефолтыми конструкторами, точнее с тем, что я их просто так насоздавал
