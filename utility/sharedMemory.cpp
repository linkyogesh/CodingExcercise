//
//  sharedMemory.cpp
//
//  Created by Yogesh Mourya on 2/21/19.
//  Copyright © 2019 yogesh. All rights reserved.
//

#include "sharedMemory.h"

namespace utility {
    using namespace std;

    /*
     init shared memory
     */
    int SharedMemory::init(const string& shmPath, const string& semPath) {
        //generate unique key
        if((key = ftok(shmPath.c_str(),0)) < 0) {
            perror("error in key gen for shared memory");
            return EXIT_FAILURE;
        }
        
        //create segment
        shmid = shmget(key, sharedMemorySize, 0666 | IPC_CREAT | IPC_EXCL );
        if(shmid < 0) {
            if(errno == EEXIST) {
                shmid = shmget(key, sharedMemorySize, 0666);
            } else {
                perror("error in getting shared memory");
                return EXIT_FAILURE;
            }
        }
        return sharedQueue.init(semPath);
    }
    
    /*
     attach shared memory and save pointer of start in local memory
     */
    int SharedMemory::attachMemory() {
        startAddr = (utility::Data*) shmat(shmid, nullptr, 0);
        if (startAddr) {
            sharedQueue.setStartAddr(startAddr);
            return EXIT_SUCCESS;
        }else {
            return EXIT_SUCCESS;
        }
    }
    
    /*
     detach and remove shared memory
     */
    int SharedMemory::cleanup() {
        if(startAddr && (shmdt(startAddr) < 0)) {
            perror("detach failed");
            return EXIT_FAILURE;
        } else {
            if(shmctl(shmid,IPC_RMID,NULL) < 0){
                perror("memory removal failed");
                return EXIT_FAILURE;
            }
        }
        return sharedQueue.cleanup();
    }
    
    /*
     add data to buffer queue
     */
    bool SharedMemory::sendNextData(const Data& data) {
        return sharedQueue.enqueue(data);
    }
    
    /*
     get data from buffer queue
     */
    bool SharedMemory::getNextData(Data& data) {
        return sharedQueue.dequeue(data);
    }
}
