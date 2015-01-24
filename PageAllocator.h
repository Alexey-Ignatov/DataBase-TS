//
//  PageAllocator.h
//  DataBase TS
//
//  Created by Алексей on 07.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#ifndef __DataBase_TS__PageAllocator__
#define __DataBase_TS__PageAllocator__

#include <stdio.h>
#include <bitset>
#include <unistd.h>
#include <iostream>

#define MAXBLOCKS 10000
class PageAllocator{
public:   
    int fileDesc;
    int pageSize;
    int maxBlocksMyDb;
    
    int firstBlockOffset;
    std::bitset<1000> dirtySet;
    
    PageAllocator(int fileDesc, int pageSize, int dbSize);
    PageAllocator();
    
    int allocatePage();
    void freePage(int offset);
};
#endif /* defined(__DataBase_TS__PageAllocator__) */
/*
// The main function that inserts a new key in this B-Tree
void BTree::insert(int k)
{
    // If tree is empty
    if (root == NULL)
    {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->keys[0] = k;  // Insert key
        root->n = 1;  // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2*t-1)
        {
            // Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);
            
            // Make old root as child of new root
            s->C[0] = root;
            
            // Split the old root and move 1 key to the new root
            s->splitChild(0, root);
            
            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->keys[0] < k)
            i++;
            s->C[i]->insertNonFull(k);
            
            // Change root
            root = s;
        }
        else  // If root is not full, call insertNonFull for root
        root->insertNonFull(k);
    }
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(int k)
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
        n = n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > k)
        i--;
        
        // See if the found child is full
        if (C[i+1]->n == 2*t-1)
        {
            // If the child is full, then split it
            splitChild(i+1, C[i+1]);
            
            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (keys[i+1] < k)
            i++;
        }
        C[i+1]->insertNonFull(k);
    }
}


*/


