 //
//  BTreeNode.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include "BTreeNode.h"



using namespace std;


void BTreeNode::printKeys(){
    cout<< "This is my key"<<endl;
    for (int i = 0 ; i < n ; ++i) {
        cout<< keys[i]<<endl;
    }
    cout<< "ENd of ket"<<endl<<endl;
}

void BTreeNode::printChOffsets(){
    cout<< "This is my childrenOffsets"<<endl;
    for (int i = 0 ; i <= n ; ++i) {
        cout<< childrenOffsets[i]<<endl;
    }
    cout<< "ENd of childrenOffsets"<<endl<<endl;
}

void BTreeNode::printBlocks(){
    cout<< "This is my blocks"<<endl;
    for (int i = 0 ; i < n ; ++i) {
        cout<< string(blocks[i], blockSize)<<endl;
    }
    cout<< "ENd of blocks"<<endl<<endl;
}


int BTreeNode::writeNode(int offset ){

    ownOffset = offset;
    lseek(fileDesc, offset, SEEK_SET);

    //сначала пишем количество ключей
    write(fileDesc, &n, sizeof(n));
    
    write(fileDesc, &leaf, sizeof(leaf));
    

    //потом пишем ключи
    write(fileDesc, keys, (2*t - 1)*sizeof(int));
    
    //потом потомков
    //для листев эта процедура кидает в файл треш из помяти
    write(fileDesc,childrenOffsets, 2*t*sizeof(int));
    
    
    //затем блоки
    char tmp[blockSize];
    for (int i = 0; i < 2*t - 1; ++i) {
        if (i <n) {
            write(fileDesc, blocks[i], blockSize);
        }else{
            write(fileDesc, tmp, blockSize);
        }
        
    }
    
    //TODO: честно ли так поступать с вектором?
    //TODO: если размер blokcs != n - ERROR
    //TODO: может ли быть такое, что ownOffset нужен, а он пока невыставлен
    //TODO: написать в write кучу проверок
    
    return offset;
}



BTreeNode *BTreeNode::readNode(int offset){
    lseek(fileDesc, offset, SEEK_SET);
    char *blockBuffer = new char[blockSize*(2*t - 1)];
    
    
    read(fileDesc,&n, sizeof(n));
    
    read(fileDesc, &leaf, sizeof(leaf));
    
    read(fileDesc, keys, (2*t - 1)*sizeof(int));
    
    //TODO: поменять sizeif(int) на sizeof(*key) или n
    
    read(fileDesc, childrenOffsets, 2*t*sizeof(int));
    
    read(fileDesc, blockBuffer, n*blockSize);
    for (int i = 0; i < n; ++i) {
        blocks[i] =blockBuffer + i*blockSize;
    }
    
    ownOffset = offset;
    
    
    return NULL;
}

BTreeNode::BTreeNode(int t, int fileDesc, int blockSize, PageAllocator *pageAlloc):blockSize(blockSize), t(t), leaf(false), n(0), fileDesc(fileDesc), pageAllocator(pageAlloc), blocks(std::vector<char *>((2*t - 1)))
{
    
    ownOffset = -1;
    keys = new int[2*t-1];
    childrenOffsets = new int[2*t];
    
}


void BTreeNode::insertNonFull(int k, char *block)
{
    // Initialize index as index of rightmost element
    int i = n-1;
    
    // If this is a leaf node
    if (leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > k)
        {
            keys[i+1] = keys[i];
            i--;
        }
        
        // Insert the new key at found location
        keys[i+1] = k;
        blocks[i+1] = block;
        n = n+1;
        writeNode(ownOffset);
        
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > k)
            i--;
        
        // See if the found child is full
        BTreeNode child(t, fileDesc, blockSize, pageAllocator);
        child.readNode(childrenOffsets[i+1]);
        if (child.n == 2*t-1)
        {
            splitChild(i+1);
            
            if (keys[i+1] < k)
                child.readNode(childrenOffsets[i+2]);
            
        }

        child.insertNonFull(k, block);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int i)
{
    
    BTreeNode y(t, fileDesc, blockSize, pageAllocator);
    y.readNode(childrenOffsets[i]);
    

    
    BTreeNode z(t, fileDesc, blockSize, pageAllocator);
    z.leaf = y.leaf;
    z.n = t - 1;
    
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++){
        z.keys[j] = y.keys[j+t];
        z.blocks[j] = y.blocks[j+t];
    }
    
    
    // Copy the last t children of y to z
    if (!y.leaf)
    {
        for (int j = 0; j < t; j++)
            z.childrenOffsets[j] = y.childrenOffsets[j+t];
    }
    
    // Reduce the number of keys in y
    y.n = t - 1;
    
    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        childrenOffsets[j+1] = childrenOffsets[j];
    
    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--){
        keys[j+1] = keys[j];
        blocks[j+1] = blocks[j];
    }

    // Copy the middle key of y to this node
    keys[i] = y.keys[t-1];
    blocks[i] = y.blocks[t - 1];

    // Increment count of keys in this node
    n = n + 1;

    
    z.writeNode(pageAllocator->allocatePage());
    
    // Link the new child to this node
    childrenOffsets[i+1] = z.ownOffset;
    
    y.writeNode(y.ownOffset);
    writeNode(ownOffset);
    
}

// Function to traverse all nodes in a subtree rooted with this node

void BTreeNode::traverse()
{
    // There are n keys and n+1 children, travers through n keys
    // and first n children
    printKeys();
    printBlocks();
    if (leaf == false)
        for (int i = 0; i <= n; i++)
        {
            BTreeNode tmp(t, fileDesc, blockSize, pageAllocator);
            tmp.readNode(childrenOffsets[i]);
            tmp.traverse();
        }
    
}




char *BTreeNode::search(int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > keys[i])
        i++;
    
    // If the found key is equal to k, return this node
    if (keys[i] == k)
        return blocks[i];
    
    // If key is not found here and this is a leaf node
    if (leaf == true)
        return NULL;
    
    // Go to the appropriate child
    BTreeNode child(t, fileDesc, blockSize, pageAllocator);
    child.readNode(childrenOffsets[i]);
    return child.search(k);
}
