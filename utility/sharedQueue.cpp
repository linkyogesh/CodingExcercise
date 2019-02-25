//
//  sharedQueue.cpp
//  producer
//
//  Created by Sushmitha Mourya on 2/24/19.
//  Copyright © 2019 yogesh. All rights reserved.
//

#include "sharedQueue.h"
namespace utility {
    int SharedQueue::init(const std::string& path) {
        return sem.createSemaphore(path);
    }
    
    /*
     each index in buffer has associated semaphore
     which determines if memory can be overwritten.
     increase the writeIndex pointer.
     Two indexes are used in this progarm: writeIndex for enqueue
     readIndex for dequeue
     */
    bool SharedQueue::enqueue (const Data& data) {
        if(!isValidQueue)
            return false;

        if(sem.isAvailableForWrite(writeIndex)) {
            if(sem.lock(writeIndex) < 0) {
                perror("lock failed in acquiring semaphore");
                return false;
            }
            
            // write data in shared memory
            front->seqNum = data.seqNum;
            front->value = data.value;
            
            //beginning of circular buffer
            if(writeIndex == (capacity -1)) {
                front = front - (capacity -1);
            } else {
                front++;
            }
            writeIndex++;
            writeIndex = (writeIndex + capacity) % capacity;
            return true;
        } else {
            return false;
        }
    }
    
    /*
     each index in buffer has associated semaphore
     which determines if data present in memory has lock,
     if yes, then read data and unlock the memory
     */
    bool SharedQueue::dequeue(Data &data) {
        if(!isValidQueue)
            return false;

        if(sem.isValidForRead(readIndex)) {
            data.seqNum = front->seqNum;
            data.value = front->value;

            if (sem.unlock(readIndex) < 0) {
                perror("error unlocking the semaphore ");
                return false;
            }

            //beginning of circular buffer
            if(readIndex == (capacity -1)) {
                front = front - (capacity -1);
            } else {
                front++;
            }
            readIndex++;
            readIndex = (readIndex + capacity) % capacity;
            return true;
        }else {
            cout << "err: please restart the producer if consumer is restarted" << endl;
           // readIndex++;
           // readIndex = (readIndex + capacity) % capacity;
            return false;
        }
    }
    
    /*
     assign start address of shared memory to the queue buffer
     */
    void SharedQueue::setStartAddr(Data* memory) {
        if(memory) {
            isValidQueue = true;
            front = memory;
        }
    }
    
    int SharedQueue::cleanup() {
        return sem.removeSemaphore();
    }
}
