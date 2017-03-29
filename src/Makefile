CC=g++
CFLAGS = -g -I -Wall -std=c++11
LDFLAGS = -pthread -ltbb
DEPS = fifoqueue.h \
			 fast_rand.h \
			 counted_ptr.h \
			 queue_item.h \
			 nonblocking_queue.h \
			 tbb_wrapper.h \
			 blocking_queue.h

OBJ = main.o counted_ptr.o
EXC = queues

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o $(EXC) $^ $(LDFLAGS)

clean:
	rm *.o
