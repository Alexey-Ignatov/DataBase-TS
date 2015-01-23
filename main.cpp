//
//  main.cpp
//  DataBaseTS
//
//  Created by Apple on 04.01.15.
//  Copyright (c) 2015 Apple. All rights reserved.
//

#include<iostream>
#include "BTreeNode.h"
#include "BTree.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <string>

#include <stdio.h>
#include <fstream>

using namespace std;

int main()
{
    char *answerBuffer = new char[MAXRECSIZE];
    
    struct DBC config = {0, 0};
    config.chunk_size = 4*1024;
    config.db_size = 128*1024;

    struct DB* db = dbcreate("m.txt", config);
    
    size_t val_len = strlen("2B7a4364d8f42afDeF8");
    size_t key_len = strlen("Sungaisarimah 1");
    
    
    
    db_put(db, (void*)"Sungaisarimah 1", key_len, (void*)"2B7a4364d8f42afDeF9", val_len);
    db_put(db, (void*)"Sungaisarimah 1", key_len, (void*)"2B7a4364d8f42afDe10", val_len);

    db_get(db, (void*)"Sungaisarimah 1", key_len, (void **)&answerBuffer, &val_len);
    cout<<std::string(answerBuffer, val_len)<< std::endl;

    db_put(db, (void*)"Sungaisarimah 1", key_len, (void*)"2B7a4364d8f42afDe11", val_len);
    
    db_get(db, (void*)"Sungaisarimah 1", key_len, (void **)&answerBuffer, &val_len);
    cout<<std::string(answerBuffer, val_len);

    return 0;
}