//
//  sharedMemory.h
//  producer
//
//  Created by Yogesh Mourya on 2/21/19.
//  Copyright © 2019 yogesh. All rights reserved.
//

#ifndef sharedMemory_h
#define sharedMemory_h

#include "common.h"
#include "sharedQueue.h"
#include <sys/shm.h>

namespace utility {
    class SharedMemory {
    private:
        key_t key = 0;
        int shmid = 0;
        int sharedMemorySize = SHARED_MEMORY_SIZE;
        utility::Data *startAddr = nullptr;
        
        SharedQueue sharedQueue;
    public:
        SharedMemory () = default;
        ~SharedMemory() = default;
        SharedMemory (SharedMemory const&) = delete;
        SharedMemory& operator=(SharedMemory const&) = delete;

        int init(const std::string& shmPath, const std::string& semPath);
        int attachMemory();
        bool sendNextData(const Data& data);
        bool getNextData(Data& data);
        int cleanup();
    };
}

#endif /* sharedMemory_h */
