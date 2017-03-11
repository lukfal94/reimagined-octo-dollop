CC=g++
CFLAGS = -g -I -Wall -std=c++11
LDFLAGS = -pthread -ltbb
DEPS = fifoqueue.h \
			 fast_rand.h \
			 queue_item.h \
			 lamport_lock.h \
			 lock.h \
			 nonblocking_queue.h \
			 tbb_wrapper.h \
			 atomic_wrapper.h \
			 blocking_queue.h

OBJ = main.o lamport_lock.o
EXC = queues

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o $(EXC) $^ $(LDFLAGS)

clean:
	rm *.o
