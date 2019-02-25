//
//  main.cpp
//  consumer
//
//  Created by Yogesh Mourya on 2/20/19.
//  Copyright © 2019 yogesh. All rights reserved.
//

#include "common.h"
#include "sharedMemory.h"

using namespace std;

class Consumer {
private:
    utility::SharedMemory shm;
    
    int lastSequence = 0;
public:
    Consumer() = default;
    ~Consumer() {
        shm.cleanup();
    };
    
    Consumer (Consumer const&) = delete;
    Consumer& operator=(Consumer const&) = delete;
    
    /*
     init and attach associated shared memory
     */
    int init(const string& shmPath, const string& semPath){
        if (EXIT_SUCCESS == shm.init(shmPath, semPath)){
            return shm.attachMemory();
        }else {
            return EXIT_FAILURE;
        }
    }
    
    /*
     start requests for next data after specific sleep time
     */
    void start() {
        /* generate random sleep duration between 1.0 to 3.0 seconds */
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> randomDelayRange(1000, 3000);
        
        while(1) {
            this_thread::sleep_for(chrono::milliseconds(randomDelayRange(gen)));
            if(!readNextData())
                break;
        }
        shm.cleanup();
    }
    
    /*
     read the next sequence for the queue buffer
     */
    bool readNextData() {
        
        utility::Data data = { 0 , 0.0 };
        
        if(shm.getNextData(data)) {
            if(data.seqNum < lastSequence) {
                cout << "err: restart the producer if consumer restarted" << endl;
                return false;
            }
            //discontinuity
            if(data.seqNum != lastSequence + 1) {
                
                /*
                 fill up all discontinuity with 0.0
                 do not count the current sequence since it will get print at the end
                 */
                int missCount = data.seqNum - (lastSequence + 1);
                while (missCount > 0) {
                    missCount--;
                    lastSequence++;
                    cout << lastSequence << " " << fixed << setw( 1 ) << setprecision( 1 )
                    << 0.0 << endl;
                }
            }
            cout << data.seqNum << " " << fixed << setw( 16 ) << setprecision( 16 )
            << data.value << endl;
            lastSequence = data.seqNum;
        }
        return true;
    }
    
};

int main(int argc, const char * argv[]) {
    
    string shmPath;
    string semPath;
    
    if(argc < 3 ){
        cout << " use default shared memory and shared key path " << endl;
        shmPath = kShmPathDefault;
        semPath = kSemPathDefault;
    } else {
        semPath = string(argv[1]);
        shmPath = string(argv[2]);
    }
    /*
     init consumer with named path
     to generate key for sharedMemory and Semaphore
     */
    Consumer consumer;
    if(EXIT_SUCCESS == consumer.init(shmPath, semPath)){
        consumer.start();
    } else {
        cout << "error in initializing producer" << endl;
    }
    
    return 0;
}
