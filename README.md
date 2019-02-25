# Producer Consumer Coding Exercise

Write separate producer and consumer programs with the following behavior and structure:
1. The producer produces a “product” (random floating point number) at random intervals
between 0.5 and 1.5 seconds each. The products are numbered sequentially, and the
numbered products are printed and then passed to the consumer. Occasionally (1 out
of 20 products), a product is dropped and not passed to the consumer.
2. The consumer consumes numbered products at random intervals between 1.0 and 3.0
seconds, and writes them out to the screen. If a number is skipped in sequence, the
consumer writes outs out 0.0 for that sequence number.
3. Because the consumer consumes products at a slower rate than the producer
produces them, there may be multiple products produced before the consumer
consumes the task. Because of this, the products should be passed in a queue, and
the queue should be protected by semaphores. The queue should hold 5 products,
and if the queue is full when a product is produced, the producer should output
“DROPPED <#>”, and continue.

## Prerequisites:

gc++ compiler (preferably C++11)

## Build

This code is tested on OSX
Make and run from current directory

Make command for build: make
clean build: make clean


## How to run:

RunProducer: run executable with two valid file path (to generate unique keys for shared memory and semaphore ),
Otherwise program will take default file specified in common.h

```
./producer
Or
./producer <file1> <file2>
```
Run producer process first in one terminal and then start the consumer process in other.
RunConsumer: use same file path as used in producer

```
./consumer
Or
./consumer <file1> <file2>

```

Limitations: since both programs do not store front/rear of queue in shared memory, so if we restart only one then there is no reference point for the other. So please restart both in case of any issue.


Note:
Reference link below took me the code where System V semaphores were described first, so I started writing a wrapper around it. Posix semaphore would have been choice to guess.

## Files:

headers: path utility/
sharedMemory.h: sharedMemory utility - used by both Producer and Consumer programs
sharedQueue.h: circular queue utility - used by shared memory
semaphore.h:  semaphore utility  (wrapper on System V semaphore) - used by shared queue for synchronization before reading or writing the data
common.h: common header files, structure for product data, default file path for unique key generation

cpp files: path utility/
sharedMemory.cpp, sharedQueue.cpp, semaphore.cpp : implementations of their respective headers

producer.cpp: Producer program's entry point and implementation of Producer class: generate product data sharedMemory class to dump data
consumer.cpp: Consumer program's entry point and implementation of consumer class: reads product data from sharedMemory class


### References:
Semaphores: http://www.cs.cf.ac.uk/Dave/C/node26.html
Shared Memory: http://www.cs.cf.ac.uk/Dave/C/node27.html
System V IPC (Semaphores and Shared Memory): https://cse.yeditepe.edu.tr/~sbaydere/fall2010/cse331/files/SystemVIPC.pdf
