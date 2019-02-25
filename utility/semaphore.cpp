//
//  semaphore.cpp
//
//  Created by Yogesh Mourya on 2/21/19.
//  Copyright © 2019 yogesh. All rights reserved.
//
#include "semaphore.h"
namespace utility {

    /*
     Remove all semaphores
     */
    int Semaphore::removeSemaphore() {
        if(semctl(semid, 0, IPC_RMID, 0) < 0) {
            perror("failed to remove semaphore");
            if(errno == EIDRM) {
                cout << "semaphore already removed" << endl;
            } else {
                return EXIT_FAILURE;
            }
        }
        return EXIT_SUCCESS;
    }

    /*
     Get semid, for corresponding path.
     Create if does not exist
     */
    int Semaphore::createSemaphore(const string& path)
    {
        if((key = ftok(path.c_str(), 0)) < 0) {
            perror("key generation error");
            return EXIT_FAILURE;
        }
        
        //create semaphore for number of arrays in queue
        if((semid = semget(key, SEMAPHORE_SIZE , 0666 | IPC_CREAT)) < 0){
            if(errno == EEXIST) {
                cout << "Semaphore already exist" << endl;
                if((semid = semget(key, SEMAPHORE_SIZE , 0666 )) < 0){
                    perror("error in semget");
                    return EXIT_FAILURE;
                }
            }
            perror("error in semget");
            return EXIT_FAILURE;
        }
        return EXIT_SUCCESS;
    }

    /*
     Lock semaphore after writing in corresponding index
     */
    int Semaphore::lock(int index)
    {
        buf[index].sem_op = 1;
        buf[index].sem_flg = IPC_NOWAIT | SEM_UNDO;
        buf[index].sem_num = index;
        return semop(semid, &buf[index], 1);
    }

    /*
     unlock semaphore after reading from particular index
     */
    int Semaphore::unlock(int index)
    {
        buf[index].sem_op = -1;
        buf[index].sem_flg = IPC_NOWAIT;
        buf[index].sem_num = index;
        return semop(semid, &buf[index], 1);
    }

    /*
     get semaphore value for particular index
     */
    int Semaphore::getSemValue(int index) {
        return semctl(semid, index, GETVAL, 0);
    }

    /*
     retruns false if data is already written on the index and waiting to be read
     */
    bool Semaphore::isAvailableForWrite(int index) {
        return ((getSemValue(index) == 0));
    }

    /*
     returns false if unread data is present in particular index
     */
    bool Semaphore::isValidForRead(int index) {
        return ((getSemValue(index) == 1));
    }

};
