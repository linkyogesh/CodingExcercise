//
//  main.cpp
//  producer
//
//  Created by Yogesh Mourya on 2/20/19.
//  Copyright © 2019 yogesh. All rights reserved.
//


#include "common.h"
#include "sharedMemory.h"

using namespace std;

class Producer {
private:
    utility::SharedMemory shm;
    
    int sequence = 0;
public:
    Producer() = default;
    Producer (Producer const&) = delete;
    Producer& operator=(Producer const&) = delete;
    
    ~Producer() {
        shm.cleanup();
    }
    
    /*
     init proudcer with named path
     to generate key for sharedMemory and Semaphore
     */
    int init(const string& shmPath, const string& semPath){
        if (EXIT_SUCCESS == shm.init(shmPath, semPath)){
            return shm.attachMemory();
        }else {
            return EXIT_FAILURE;
        }
    }
    
    /*
     generate next product data after specific sleep time
     */
    void start() {
        /* generate random sleep duration between .5 to 1.5 seconds */
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> randomDelayRange(500, 1500);
        
        /* generate products (floating point numbers) between 0.0 to 1.0 */
        uniform_real_distribution<float> productRange(0.0, 1.0);
        
        while(1) {
            this_thread::sleep_for(chrono::milliseconds(randomDelayRange(gen)));
            sendNewData(productRange(gen));
        }
    }
    
    /*
     send the product data with sequence number to the queue buffer
     */
    void sendNewData(float product) {
        sequence++;
        
        //skip 20th sequence
        if((sequence % 20) == 0) {
            return;
        }
        utility::Data data = { sequence , product };
        
        if(shm.sendNextData(data)) {
            cout << data.seqNum << " " << fixed << setw( 16 ) << setprecision( 16 )
            << data.value << endl;
        } else {
            cout << "DROPPED " << sequence << "\n";
        }
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
    
    Producer producer;
    if(EXIT_SUCCESS == producer.init(shmPath, semPath)){
        producer.start();
    } else {
        cout << "error in initializing producer" << endl;
    }
    
    return 0;
}
