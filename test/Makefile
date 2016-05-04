all: thread-object-destruction thread-object-sync
thread-object-destruction: thread-object-destruction.cpp
	g++ -o thread-object-destruction -std=c++11 -g thread-object-destruction.cpp -pthread
thread-object-sync: thread-object-sync.cpp
	g++ -o thread-object-sync -std=c++11 -g thread-object-sync.cpp -pthread
clean:
	rm thread-object-destruction thread-object-sync
