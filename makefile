CXX=g++
CXXFLAGS=-Wall -std=c++11
SUBFOLDER=utility
all: clean consumer producer

consumer: consumer.cpp $(SUBFOLDER)/sharedMemory.cpp $(SUBFOLDER)/sharedQueue.cpp $(SUBFOLDER)/semaphore.cpp
	$(CXX) $(CXXFLAGS) -Iutility $^ -o $@

producer: producer.cpp $(SUBFOLDER)/sharedMemory.cpp $(SUBFOLDER)/sharedQueue.cpp $(SUBFOLDER)/semaphore.cpp
	$(CXX) $(CXXFLAGS) -Iutility $^ -o $@

clean:
	rm producer consumer ||:
