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
        cout<< keysValues[i].first<<endl;
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
        cout<< string(keysValues[i].second, blockSize)<<endl;
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
    vector<int> keys(2*t -1);
    for (int i = 0; i < keysValues.size(); ++i) {
        keys[i] = keysValues[i].first;
    }
    
    write(fileDesc, &keys[0], (2*t - 1)*sizeof(int));
    
    //потом потомков
    //для листев эта процедура кидает в файл треш из помяти
    write(fileDesc,childrenOffsets, 2*t*sizeof(int));
    
    
    //затем блоки
    char tmp[blockSize];
    for (int i = 0; i < 2*t - 1; ++i) {
        if (i <n) {
            write(fileDesc, keysValues[i].second, blockSize);
        }else{
            write(fileDesc, tmp, blockSize);
        }
        
    }
    
    //TODO: честно ли так поступать с вектором?
    //TODO: если размер blokcs != n - ERROR
    //TODO: может ли быть такое, что ownOffset нужен, а он пока невыставлен
    //TODO: написать в write кучу проверок
    //TOOD: освободить память из-под tmp
    return offset;
}



BTreeNode *BTreeNode::readNode(int offset){
    lseek(fileDesc, offset, SEEK_SET);
    char *blockBuffer = new char[blockSize*(2*t - 1)];
    
    
    read(fileDesc,&n, sizeof(n));
    
    read(fileDesc, &leaf, sizeof(leaf));
    
    vector<int> keys(2*t -1);
    read(fileDesc, &keys[0], (2*t - 1)*sizeof(int));
    
    for (int i = 0; i < keysValues.size(); ++i) {
        keysValues[i].first = keys[i];
    }
    
    //TODO: поменять sizeif(int) на sizeof(*key) или n
    
    read(fileDesc, childrenOffsets, 2*t*sizeof(int));
    
    read(fileDesc, blockBuffer, n*blockSize);
    for (int i = 0; i < n; ++i) {
        keysValues[i].second =blockBuffer + i*blockSize;
    }
    
    ownOffset = offset;
    
    
    return NULL;
}

BTreeNode::BTreeNode(int t, int fileDesc, int blockSize, PageAllocator *pageAlloc):blockSize(blockSize), t(t), leaf(false), n(0), fileDesc(fileDesc), pageAllocator(pageAlloc), keysValues(std::vector<pair<int, char *> >(2*t - 1))
{
    
    ownOffset = -1;
    childrenOffsets = new int[2*t];
    n = 0;
}

// A utility function that returns the index of the first key that is
// greater than or equal to k
int BTreeNode::findKey(int k)
{
    int idx=0;
    while (idx<n && keysValues[idx].first < k)
        ++idx;
    return idx;
}











// A function to remove the key k from the sub-tree rooted with this node
void BTreeNode::remove(int k)
{
    int idx = findKey(k);
    
    // The key to be removed is present in this node
    if (idx < n && keysValues[idx].first == k)
    {
        
        // If the node is a leaf node - removeFromLeaf is called
        // Otherwise, removeFromNonLeaf function is called
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {
        
        // If this node is a leaf node, then the key is not present in tree
        if (leaf)
        {
            std::cout << "The key "<< k <<" is does not exist in the tree\n";
            return;
        }
        
        // The key to be removed is present in the sub-tree rooted with this node
        // The flag indicates whether the key is present in the sub-tree rooted
        // with the last child of this node
        bool flag = idx==n;
        
        // If the child where the key is supposed to exist has less that t keys,
        // we fill that child
        BTreeNode child(t, fileDesc, blockSize, pageAllocator);
        child.readNode(childrenOffsets[idx]);
        
        if (child.n < t)
            fill(idx);
        
        // If the last child has been merged, it must have merged with the previous
        // child and so we recurse on the (idx-1)th child. Else, we recurse on the
        // (idx)th child which now has atleast t keys
        if (flag && idx > n)
            child.readNode(childrenOffsets[idx - 1]);
        else
            child.readNode(childrenOffsets[idx]);
        
        child.remove(k);
    }
    return;
}

// A function to remove the idx-th key from this node - which is a leaf node
void BTreeNode::removeFromLeaf (int idx)
{
    
    // Move all the keys after the idx-th pos one place backward
    for (int i=idx+1; i<n; ++i)
        keysValues[i-1] = keysValues[i];

    //TODO: освободить память за удаленным блоком
    // Reduce the count of keys
    n--;
    writeNode(ownOffset);
    return;
}

// A function to remove the idx-th key from this node - which is a non-leaf node
void BTreeNode::removeFromNonLeaf(int idx)
{
    
    int k = keysValues[idx].first;
    
    // If the child that precedes k (C[idx]) has atleast t keys,
    // find the predecessor 'pred' of k in the subtree rooted at
    // C[idx]. Replace k by pred. Recursively delete pred
    // in C[idx]
    BTreeNode childLess(t, fileDesc, blockSize, pageAllocator);
    childLess.readNode(childrenOffsets[idx]);
    
    BTreeNode childGrea(t, fileDesc, blockSize, pageAllocator);
    childGrea.readNode(childrenOffsets[idx + 1]);
    if (childLess.n >= t)
    {
        pair<int, char*> pred = getPred(idx);
        keysValues[idx] = pred;
        childLess.remove(pred.first);
        writeNode(ownOffset);
        childLess.writeNode(childLess.ownOffset);
    }
    
    // If the child C[idx] has less that t keys, examine C[idx+1].
    // If C[idx+1] has atleast t keys, find the successor 'succ' of k in
    // the subtree rooted at C[idx+1]
    // Replace k by succ
    // Recursively delete succ in C[idx+1]
    else if  (childGrea.n >= t)
    {
        pair<int, char*>  succ = getSucc(idx);
        keysValues[idx] = succ;
        childGrea.remove(succ.first);
        writeNode(ownOffset);
        childGrea.writeNode(childGrea.ownOffset);
    }
    
    // If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
    // into C[idx]
    // Now C[idx] contains 2t-1 keys
    // Free C[idx+1] and recursively delete k from C[idx]
    else
    {
        merge(idx);
        BTreeNode child(t, fileDesc, blockSize, pageAllocator);
        child.readNode(childrenOffsets[idx]);
        child.remove(k);
    }
    return;
}

// A function to get predecessor of keys[idx]
pair<int, char*> BTreeNode::getPred(int idx)
{
    // Keep moving to the right most node until we reach a leaf
    BTreeNode curr(t, fileDesc, blockSize, pageAllocator);
    curr.readNode(childrenOffsets[idx]);
    
    while (!curr.leaf)
        curr.readNode(curr.childrenOffsets[curr.n]);

    // Return the last key of the leaf
    return curr.keysValues[curr.n-1];
}

pair<int, char*>  BTreeNode::getSucc(int idx)
{
    
    // Keep moving the left most node starting from C[idx+1] until we reach a leaf
    BTreeNode curr(t, fileDesc, blockSize, pageAllocator);
    curr.readNode(childrenOffsets[idx + 1]);
    while (!curr.leaf)
        curr.readNode(curr.childrenOffsets[0]);
    
    // Return the first key of the leaf
    return curr.keysValues[0];
}

// A function to fill child C[idx] which has less than t-1 keys
void BTreeNode::fill(int idx)
{
    
    BTreeNode childLess(t, fileDesc, blockSize, pageAllocator);
    childLess.readNode(childrenOffsets[idx - 1]);
    
    BTreeNode childGrea(t, fileDesc, blockSize, pageAllocator);
    childGrea.readNode(childrenOffsets[idx + 1]);
    // If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
    // from that child
    if (idx!=0 && childLess.n>=t)
        borrowFromPrev(idx);
    
    // If the next child(C[idx+1]) has more than t-1 keys, borrow a key
    // from that child
    else if (idx!=n && childGrea.n>=t)
        borrowFromNext(idx);
    
    // Merge C[idx] with its sibling
    // If C[idx] is the last child, merge it with with its previous sibling
    // Otherwise merge it with its next sibling
    else
    {
        if (idx != n)
            merge(idx);
        else
            merge(idx-1);
    }
    return;
}

// A function to borrow a key from C[idx-1] and insert it
// into C[idx]
void BTreeNode::borrowFromPrev(int idx)
{
    
    //BTreeNode *child=C[idx];
    //BTreeNode *sibling=C[idx-1];
    
    BTreeNode child(t, fileDesc, blockSize, pageAllocator);
    child.readNode(childrenOffsets[idx]);
    
    BTreeNode sibling(t, fileDesc, blockSize, pageAllocator);
    sibling.readNode(childrenOffsets[idx - 1]);
    // The last key from C[idx-1] goes up to the parent and key[idx-1]
    // from parent is inserted as the first key in C[idx]. Thus, the  loses
    // sibling one key and child gains one key
    
    // Moving all key in C[idx] one step ahead
    for (int i=child.n-1; i>=0; --i)
        child.keysValues[i+1] = child.keysValues[i];
    
    // If C[idx] is not a leaf, move all its child pointers one step ahead
    if (!child.leaf)
    {
        for(int i=child.n; i>=0; --i)
            child.childrenOffsets[i+1] = child.childrenOffsets[i];
    }
    
    // Setting child's first key equal to keys[idx-1] from the current node
    child.keysValues[0] = keysValues[idx-1];
    
    // Moving sibling's last child as C[idx]'s first child
    if (!leaf)
        child.childrenOffsets[0] = sibling.childrenOffsets[sibling.n];
    
    // Moving the key from the sibling to the parent
    // This reduces the number of keys in the sibling
    keysValues[idx-1] = sibling.keysValues[sibling.n-1];
    
    child.n += 1;
    sibling.n -= 1;
    
    child.writeNode(child.ownOffset);
    sibling.writeNode(sibling.ownOffset);
    writeNode(ownOffset);
    
    return;
}

// A function to borrow a key from the C[idx+1] and place
// it in C[idx]
void BTreeNode::borrowFromNext(int idx)
{
    
    BTreeNode child(t, fileDesc, blockSize, pageAllocator);
    child.readNode(childrenOffsets[idx]);
    
    BTreeNode sibling(t, fileDesc, blockSize, pageAllocator);
    sibling.readNode(childrenOffsets[idx + 1]);
    
    // keys[idx] is inserted as the last key in C[idx]
    child.keysValues[child.n] = keysValues[idx];
    
    // Sibling's first child is inserted as the last child
    // into C[idx]
    if (!child.leaf)
        child.childrenOffsets[child.n+1] = sibling.childrenOffsets[0];
    
    //The first key from sibling is inserted into keys[idx]
    keysValues[idx] = sibling.keysValues[0];
    
    // Moving all keys in sibling one step behind
    for (int i=1; i<sibling.n; ++i)
        sibling.keysValues[i-1] = sibling.keysValues[i];
    
    // Moving the child pointers one step behind
    if (!sibling.leaf)
    {
        for(int i=1; i<=sibling.n; ++i)
            sibling.childrenOffsets[i-1] = sibling.childrenOffsets[i];
    }
    
    // Increasing and decreasing the key count of C[idx] and C[idx+1]
    // respectively
    child.n += 1;
    sibling.n -= 1;
    
    child.writeNode(child.ownOffset);
    sibling.writeNode(sibling.ownOffset);
    writeNode(ownOffset);
    
    
    return;
}

// A function to merge C[idx] with C[idx+1]
// C[idx+1] is freed after merging
void BTreeNode::merge(int idx)
{
    BTreeNode child(t, fileDesc, blockSize, pageAllocator);
    child.readNode(childrenOffsets[idx]);
    
    BTreeNode sibling(t, fileDesc, blockSize, pageAllocator);
    sibling.readNode(childrenOffsets[idx + 1]);
    
    // Pulling a key from the current node and inserting it into (t-1)th
    // position of C[idx]
    child.keysValues[t-1] = keysValues[idx];
    
    // Copying the keys from C[idx+1] to C[idx] at the end
    for (int i=0; i<sibling.n; ++i)
        child.keysValues[i+t] = sibling.keysValues[i];
    
    // Copying the child pointers from C[idx+1] to C[idx]
    if (!child.leaf)
    {
        for(int i=0; i<=sibling.n; ++i)
            child.childrenOffsets[i+t] = sibling.childrenOffsets[i];
    }
    
    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to C[idx]
    for (int i=idx+1; i<n; ++i)
        keysValues[i-1] = keysValues[i];
    
    // Moving the child pointers after (idx+1) in the current node one
    // step before
    for (int i=idx+2; i<=n; ++i)
        childrenOffsets[i-1] = childrenOffsets[i];
    
    // Updating the key count of child and the current node
    child.n += sibling.n+1;
    n--;
    
    // Freeing the memory occupied by sibling
    //delete(sibling);
    //TODO: освободить страницу, которую занимал sibling
    
    child.writeNode(child.ownOffset);
    sibling.writeNode(sibling.ownOffset);
    writeNode(ownOffset);
    
    return;
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
        while (i >= 0 && keysValues[i].first > k)
        {
            keysValues[i+1] = keysValues[i];
            i--;
        }
        
        // Insert the new key at found location
        keysValues[i+1].first = k;
        keysValues[i+1].second = block;
        n = n+1;
        writeNode(ownOffset);
        
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keysValues[i].first > k)
            i--;
        
        // See if the found child is full
        BTreeNode child(t, fileDesc, blockSize, pageAllocator);
        child.readNode(childrenOffsets[i+1]);
        if (child.n == 2*t-1)
        {
            splitChild(i+1);
            
            if (keysValues[i+1].first < k)
                //TODO: лишнее чтение с диска
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
    //TODO: лишнее чтение с диска
    y.readNode(childrenOffsets[i]);
    

    
    BTreeNode z(t, fileDesc, blockSize, pageAllocator);
    z.leaf = y.leaf;
    z.n = t - 1;
    
    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z.keysValues[j] = y.keysValues[j+t];
    
    
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
    for (int j = n-1; j >= i; j--)
        keysValues[j+1] = keysValues[j];

    // Copy the middle key of y to this node
    keysValues[i] = y.keysValues[t-1];

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
    cout <<ownOffset<<endl;
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
    while (i < n && k > keysValues[i].first)
        i++;
    
    // If the found key is equal to k, return this node
    if (keysValues[i].first == k)
        return keysValues[i].second;
    
    // If key is not found here and this is a leaf node
    if (leaf == true)
        return NULL;
    
    // Go to the appropriate child
    BTreeNode child(t, fileDesc, blockSize, pageAllocator);
    child.readNode(childrenOffsets[i]);
    return child.search(k);
}
