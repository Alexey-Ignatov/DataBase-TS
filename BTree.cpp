//
//  BTree.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include "BTree.h"
#include <iostream>
#include <string.h>
#include <term.h>


BTree::BTree(int _t)
{
    root = NULL;
    t = _t;
}

void BTree::traverse()
{
    if (root != NULL) root->traverse();
}





BTreeNode* BTree::search(int k){
    return (root == NULL)? NULL : root->search(k);
}



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

void BTree::remove(int k)
{
    if (!root)
    {
        std::cout << "The tree is empty\n";
        return;
    }
    
    // Call the remove function for root
    root->remove(k);
    
    // If the root node has 0 keys, make its first child as the new root
    //  if it has a child, otherwise set root as NULL
    if (root->n==0)
    {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];
        
        // Free the old root
        delete tmp;
    }
    return;
}
