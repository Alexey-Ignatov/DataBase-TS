//
//  DiskWorker.h
//  DataBase TS
//
//  Created by Алексей on 06.01.15.
//  Copyright (c) 2015 Алексей. All rights reserved.
//

#ifndef __DataBase_TS__DiskWorker__
#define __DataBase_TS__DiskWorker__

#include <stdio.h>
#include "BTreeNode.h"

class DiskWorker{
    int first_offset;
    
    int fileDesc;
    
public:
    
    DiskWorker(int first_offset, int fileDesc);
    
    int writeNode(BTreeNode *target);
    
    BTreeNode *readNode(int offset);
    
    
};
#endif /* defined(__DataBase_TS__DiskWorker__) */
