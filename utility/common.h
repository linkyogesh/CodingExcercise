//
//  common.h
//
//  Created by Yogesh Mourya on 2/24/19.
//  Copyright © 2019 yogesh. All rights reserved.
//

#ifndef common_h
#define common_h

#include <iostream>
#include <iomanip>
#include <random>
#include <thread>

typedef float product_t;

enum MaxValue {
    QUEUE_BUFFER_SIZE = 5,
    SEMAPHORE_SIZE = 5,
    SHARED_MEMORY_SIZE = 1024,
};

const std::string kShmPathDefault = "./producer.cpp";
const std::string kSemPathDefault = "./consumer.cpp";

namespace utility {
    /*
     Each product is packed with incremental sequence numbered data
     */
    struct Data {
        int seqNum;
        product_t value;
    };
}

#endif /* common_h */
