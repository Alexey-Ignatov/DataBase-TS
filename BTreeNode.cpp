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
        cout<< keysValues[i].first.data<<endl;
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
        cout<< keysValues[i].second.getSrc()<<endl;
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
    vector<Key> keys(2*t -1);
    for (int i = 0; i < keysValues.size(); ++i) {
        keys[i] = keysValues[i].first;
    }
    
    write(fileDesc, &keys[0], (2*t - 1)*sizeof(Key));
    
    //потом потомков
    //для листев эта процедура кидает в файл треш из помяти
    if (!childrenOffsets.empty()) {
        write(fileDesc,&childrenOffsets[0], 2*t*sizeof(int));
    }
    
    //затем блоки
    char tmp[sizeof(Record)];
    for (int i = 0; i < 2*t - 1; ++i) {
        if (i <n) {
            write(fileDesc, &keysValues[i].second, sizeof(Record));
        }else{
            write(fileDesc, tmp, sizeof(Record));
        }
        
    }
    return offset;
}



BTreeNode *BTreeNode::readNode(int offset){
    lseek(fileDesc, offset, SEEK_SET);
    Record *blockBuffer = new Record[2*t - 1]();
    
    
    read(fileDesc,&n, sizeof(n));
    
    read(fileDesc, &leaf, sizeof(leaf));
    
    vector<Key> keys(2*t -1);
    read(fileDesc, &keys[0], (2*t - 1)*sizeof(Key));
    
    for (int i = 0; i < keysValues.size(); ++i) {
        keysValues[i].first = keys[i];
    }
    
    if (!childrenOffsets.empty()) {
        read(fileDesc, &childrenOffsets[0], 2*t*sizeof(int));
    }
    
    
    read(fileDesc, blockBuffer, n*sizeof(Record));
    for (int i = 0; i < n; ++i) {
        Record *currRec =blockBuffer + i;
        keysValues[i].second = *currRec;
    }
    
    ownOffset = offset;
    
    
    return NULL;
}

BTreeNode::BTreeNode(int t, int fileDesc, PageAllocator *pageAlloc):t(t), leaf(false), n(0), fileDesc(fileDesc), pageAllocator(pageAlloc), keysValues(std::vector<pair<Key, Record> >(2*t - 1)), childrenOffsets(std::vector<int>(2*t, -1))
{
    
    ownOffset = -1;
    n = 0;
}


int BTreeNode::findKey(Key k)
{
    int idx=0;
    while (idx<n && keysValues[idx].first < k)
        ++idx;
    return idx;
}


void BTreeNode::remove(Key k)
{
    int idx = findKey(k);
    
    if (idx < n && keysValues[idx].first == k)
    {
        
        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {
        
        if (leaf)
        {
            std::cout << "The key "<< k.data <<" is does not exist in the tree\n";
            return;
        }
        
        bool flag = idx==n;
        
        BTreeNode child(t, fileDesc, pageAllocator);
        child.readNode(childrenOffsets[idx]);
        
        if (child.n < t)
            fill(idx);
        
        if (flag && idx > n)
            child.readNode(childrenOffsets[idx - 1]);
        else
            child.readNode(childrenOffsets[idx]);
        
        child.remove(k);
    }
    return;
}

void BTreeNode::removeFromLeaf (int idx)
{
    
    for (int i=idx+1; i<n; ++i)
        keysValues[i-1] = keysValues[i];

    //TODO: освободить память за удаленным блоком
    n--;
    writeNode(ownOffset);
    return;
}


void BTreeNode::removeFromNonLeaf(int idx)
{
    
    Key k = keysValues[idx].first;
    
    BTreeNode childLess(t, fileDesc, pageAllocator);
    childLess.readNode(childrenOffsets[idx]);
    
    BTreeNode childGrea(t, fileDesc, pageAllocator);
    childGrea.readNode(childrenOffsets[idx + 1]);
    if (childLess.n >= t)
    {
        pair<Key, Record> pred = getPred(idx);
        keysValues[idx] = pred;
        childLess.remove(pred.first);
        writeNode(ownOffset);
        childLess.writeNode(childLess.ownOffset);
    }
    
    else if  (childGrea.n >= t)
    {
        pair<Key, Record>  succ = getSucc(idx);
        keysValues[idx] = succ;
        childGrea.remove(succ.first);
        writeNode(ownOffset);
        childGrea.writeNode(childGrea.ownOffset);
    }
    
    else
    {
        merge(idx);
        BTreeNode child(t, fileDesc, pageAllocator);
        child.readNode(childrenOffsets[idx]);
        child.remove(k);
    }
    return;
}


pair<Key, Record> BTreeNode::getPred(int idx)
{
    BTreeNode curr(t, fileDesc, pageAllocator);
    curr.readNode(childrenOffsets[idx]);
    
    while (!curr.leaf)
        curr.readNode(curr.childrenOffsets[curr.n]);

    return curr.keysValues[curr.n-1];
}

pair<Key, Record>  BTreeNode::getSucc(int idx)
{
    
    BTreeNode curr(t, fileDesc, pageAllocator);
    curr.readNode(childrenOffsets[idx + 1]);
    while (!curr.leaf)
        curr.readNode(curr.childrenOffsets[0]);

    return curr.keysValues[0];
}


void BTreeNode::fill(int idx)
{
    BTreeNode childLess(t, fileDesc, pageAllocator);
    BTreeNode childGrea(t, fileDesc, pageAllocator);
    
    bool borrowed = false;
    
    if (idx!=0){
        childLess.readNode(childrenOffsets[idx - 1]);
        if (childLess.n>=t) {
            borrowFromPrev(idx);
            borrowed =true;
        }
    }
    
    if (!borrowed && idx!=n){
        childGrea.readNode(childrenOffsets[idx + 1]);
        if (childGrea.n>=t){
            borrowFromNext(idx);
            borrowed = true;
        }
    }
    
    if (!borrowed) {
        if (idx != n)
            merge(idx);
        else
            merge(idx-1);
    }
    return;

}


void BTreeNode::borrowFromPrev(int idx)
{
    
    
    BTreeNode child(t, fileDesc, pageAllocator);
    child.readNode(childrenOffsets[idx]);
    
    BTreeNode sibling(t, fileDesc, pageAllocator);
    sibling.readNode(childrenOffsets[idx - 1]);


    for (int i=child.n-1; i>=0; --i)
        child.keysValues[i+1] = child.keysValues[i];
    
    if (!child.leaf)
    {
        for(int i=child.n; i>=0; --i)
            child.childrenOffsets[i+1] = child.childrenOffsets[i];
    }
    
    child.keysValues[0] = keysValues[idx-1];
    
    if (!leaf)
        child.childrenOffsets[0] = sibling.childrenOffsets[sibling.n];
    
    keysValues[idx-1] = sibling.keysValues[sibling.n-1];
    
    child.n += 1;
    sibling.n -= 1;
    
    child.writeNode(child.ownOffset);
    sibling.writeNode(sibling.ownOffset);
    writeNode(ownOffset);
    
    return;
}

void BTreeNode::borrowFromNext(int idx)
{
    
    BTreeNode child(t, fileDesc, pageAllocator);
    child.readNode(childrenOffsets[idx]);
    
    BTreeNode sibling(t, fileDesc, pageAllocator);
    sibling.readNode(childrenOffsets[idx + 1]);
    
    child.keysValues[child.n] = keysValues[idx];
    
    if (!child.leaf)
        child.childrenOffsets[child.n+1] = sibling.childrenOffsets[0];
    
    keysValues[idx] = sibling.keysValues[0];
    
    for (int i=1; i<sibling.n; ++i)
        sibling.keysValues[i-1] = sibling.keysValues[i];
    
    if (!sibling.leaf)
    {
        for(int i=1; i<=sibling.n; ++i)
            sibling.childrenOffsets[i-1] = sibling.childrenOffsets[i];
    }
    
    child.n += 1;
    sibling.n -= 1;
    
    child.writeNode(child.ownOffset);
    sibling.writeNode(sibling.ownOffset);
    writeNode(ownOffset);
    
    
    return;
}

void BTreeNode::merge(int idx)
{
    BTreeNode child(t, fileDesc, pageAllocator);
    child.readNode(childrenOffsets[idx]);
    
    BTreeNode sibling(t, fileDesc, pageAllocator);
    sibling.readNode(childrenOffsets[idx + 1]);
    
    child.keysValues[t-1] = keysValues[idx];
    
    for (int i=0; i<sibling.n; ++i)
        child.keysValues[i+t] = sibling.keysValues[i];
    
    if (!child.leaf)
    {
        for(int i=0; i<=sibling.n; ++i)
            child.childrenOffsets[i+t] = sibling.childrenOffsets[i];
    }
    
    for (int i=idx+1; i<n; ++i)
        keysValues[i-1] = keysValues[i];
    
    for (int i=idx+2; i<=n; ++i)
        childrenOffsets[i-1] = childrenOffsets[i];
    
    child.n += sibling.n+1;
    n--;
    
    
    child.writeNode(child.ownOffset);
    sibling.writeNode(sibling.ownOffset);
    writeNode(ownOffset);
    
    return;
}

void BTreeNode::insertNonFull(Key k, Record block)
{
    int i = n-1;
    while (i >= 0 && keysValues[i].first > k)
        i--;
    
    if (keysValues[i].first == k) {
        keysValues[i].second = block;
        writeNode(ownOffset);
        return;
    }
    
    if (leaf == true)
    {
        i = n-1;
        while (i >= 0 && keysValues[i].first > k)
        {
            keysValues[i+1] = keysValues[i];
            i--;
        }

        keysValues[i+1].first = k;
        keysValues[i+1].second = block;
        n = n+1;
        writeNode(ownOffset);
        
    }
    else
    {
        BTreeNode child(t, fileDesc, pageAllocator);
        child.readNode(childrenOffsets[i+1]);
        if (child.n == 2*t-1)
        {
            splitChild(i+1);
            
            if (keysValues[i+1].first < k)
                child.readNode(childrenOffsets[i+2]);
        }

        child.insertNonFull(k, block);
    }
}


void BTreeNode::splitChild(int i)
{
    
    BTreeNode y(t, fileDesc, pageAllocator);
    y.readNode(childrenOffsets[i]);
    

    
    BTreeNode z(t, fileDesc, pageAllocator);
    z.leaf = y.leaf;
    z.n = t - 1;
    
    for (int j = 0; j < t-1; j++)
        z.keysValues[j] = y.keysValues[j+t];
    
    
    if (!y.leaf)
    {
        for (int j = 0; j < t; j++)
            z.childrenOffsets[j] = y.childrenOffsets[j+t];
    }
    
    y.n = t - 1;
    
    for (int j = n; j >= i+1; j--)
        childrenOffsets[j+1] = childrenOffsets[j];
    
    for (int j = n-1; j >= i; j--)
        keysValues[j+1] = keysValues[j];

    keysValues[i] = y.keysValues[t-1];


    n = n + 1;

    
    z.writeNode(pageAllocator->allocatePage());
    
    childrenOffsets[i+1] = z.ownOffset;
    
    y.writeNode(y.ownOffset);
    writeNode(ownOffset);
    
}


void BTreeNode::traverse()
{

    printKeys();
    cout <<ownOffset<<endl;
    printBlocks();
    if (leaf == false)
        for (int i = 0; i <= n; i++)
        {
            BTreeNode tmp(t, fileDesc, pageAllocator);
            tmp.readNode(childrenOffsets[i]);
            tmp.traverse();
        }
    
}




Record BTreeNode::search(Key k)
{
    int i = 0;
    while (i < n && k > keysValues[i].first)
        i++;
    
    if (keysValues[i].first == k)
        return keysValues[i].second;

    if (leaf == true)
        return Record();

    BTreeNode child(t, fileDesc, pageAllocator);
    child.readNode(childrenOffsets[i]);
    return child.search(k);
}














Key::Key(const char *key, int size): size(size){
    strcpy(data, key);
};

bool Key::operator <(const Key& key) const
{
    int result = memcmp(data, key.data, min(size, key.size));
    if(result < 0){
        return true;
    }else{
        return false;
    }
    return (size > key.size);
}

bool Key::operator ==(const Key& key) const
{
    return !memcmp(data, key.data, size)  && size == key.size;
}

bool Key::operator >(const Key& key) const
{
    return !(  *this < key || *this == key );
    //return size >key.size;
}

Key& Key::operator=(const Key& other){
    size = other.size;
    if(size == 0){
        return *this;
    }
    memcpy(data, other.data, other.size);
    return *this;
}

Key::Key(const Key& other){
    if (this != &other) {
        size = other.size;
        memcpy(data, other.data, other.size);
    }
}

Key::Key():size(0){};

Key::Key(const char *keyStr){
    size = strlen(keyStr);
    strcpy(data, keyStr);
};






Record::Record(const char *record, int recSize){
    size = recSize;
    if(size == 0)
    return;
    memcpy(data, record, recSize);
};

const char *Record::getSrc()const{
    return data;
}

int Record::getSize(){
    return size;
}

Record& Record::operator=(const Record& other){
    size = other.size;
    if(size == 0){
        return *this;
    }
    memcpy(data, other.data, other.size);
    return *this;
}

Record::Record(): size(0){};

bool Record::empty()const{
    return size == 0;
}

