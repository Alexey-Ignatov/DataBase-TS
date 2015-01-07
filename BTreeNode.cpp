//
//  BTreeNode.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include "BTreeNode.h"




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
        blocks.push_back(blockBuffer + i*blockSize);
    }
    
    ownOffset = offset;
    
    
    return NULL;
}

BTreeNode::BTreeNode(int t, int fileDesc, int blockSize):blockSize(blockSize), t(t), leaf(false), n(0), fileDesc(fileDesc)
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
        BTreeNode child(t, fileDesc, blockSize);
        child.readNode(childrenOffsets[i+1]);
        
        if (child.n == 2*t-1)
        {
            std::cout << "NOT IMPLEMENTED"<<std::endl;

            //splitChild(i+1, C[i+1]);
            
            //if (keys[i+1] < k)
            //    i++;
            
        }
        child.insertNonFull(k, block);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
/*void BTreeNode::splitChild(int i, BTreeNode *y)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;
    
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];
    
    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }
    
    // Reduce the number of keys in y
    y->n = t - 1;
    
    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];
    
    // Link the new child to this node
    C[i+1] = z;
    
    // A key of y will move to this node. Find location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];
    
    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];
    
    // Increment count of keys in this node
    n = n + 1;
    //TODO:  зактнуть на диск сбея y and z
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse()
{
    // There are n keys and n+1 children, travers through n keys
    // and first n children
    for (int i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (leaf == false)
            C[i]->traverse();
        std::cout << " " << keys[i];
    }
    
    // Print the subtree rooted with last child
    if (leaf == false)
        C[i]->traverse();
}

// Function to search key k in subtree rooted with this node
BTreeNode *BTreeNode::search(int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > keys[i])
        i++;
    
    // If the found key is equal to k, return this node
    if (keys[i] == k)
        return this;
    
    // If key is not found here and this is a leaf node
    if (leaf == true)
        return NULL;
    
    // Go to the appropriate child
    return C[i]->search(k);
}*/
