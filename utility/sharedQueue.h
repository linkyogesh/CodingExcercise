//
//  sharedQueue.h
//  producer
//
//  Created by Yogesh Mourya on 2/21/19.
//  Copyright © 2019 yogesh. All rights reserved.
//

#ifndef sharedQueue_h
#define sharedQueue_h
#include "common.h"
#include "semaphore.h"

namespace utility {
    class SharedQueue {
    private:
        Semaphore sem;

        Data *front = nullptr;

        //data queue max size
        int capacity = QUEUE_BUFFER_SIZE;

        bool isValidQueue = false;

        //writer index to write in circular queue
        int writeIndex = 0;

        //reader index to read in circular queue
        int readIndex = 0;
    public:
        
        SharedQueue () = default;
        ~SharedQueue () = default;
        SharedQueue (SharedQueue const&) = delete;
        SharedQueue& operator=(SharedQueue const&) = delete;

        int init(const string& path);
        bool enqueue (const Data& data);
        void setStartAddr(Data* memory);
        bool dequeue(Data &data);
        int cleanup();
    };
}

#endif /* sharedQueue_h */
