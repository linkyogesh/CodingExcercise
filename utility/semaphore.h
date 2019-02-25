//
//  semaphore.h
//
//  Created by Yogesh Mourya on 2/21/19.
//  Copyright © 2019 yogesh. All rights reserved.
//

#ifndef semaphore_h
#define semaphore_h

#include "common.h"
#include <sys/sem.h>

namespace utility {
    using namespace std;
    /*
     This Semaphore class is wrapper around System V semaphore
     */
    class Semaphore
    {
    private:
        key_t key = 0;
        int semid = 0;
        sembuf buf[SEMAPHORE_SIZE];
        
        int getSemValue(int index);
    public:
        Semaphore() = default;
        ~Semaphore() = default;
        Semaphore (Semaphore const&) = delete;
        Semaphore& operator=(Semaphore const&) = delete;

        int removeSemaphore();
        int createSemaphore(const string& path);
        int lock(int index);
        int unlock(int index);
        bool isAvailableForWrite(int index);
        bool isValidForRead(int index);
    };

}
#endif /* semaphore_h */
