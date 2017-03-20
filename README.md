## Programming Assignment 2
## Luke Fallon
## 10 March 2017

To Compile:
  cd src/
  make clean
  make

To Run:
  ./queues <queue_size> <num_ops> <queue_type> <num_threads> <prefill_queue> <enq_ratio>

NOTE:
    This implementation utilizes bit stealing to address the ABA Problem. It has been 
    tested on an Intel x86_64 system running Linux Mint. 
    Compiler, OS, and Architecture details can be found in the system-info/ directory.
    
    
## Important Source Files
    blocking_queue.h     : implementation of a bounded, lock based queue
    nonblocking_queue.h  : Implementation of a Lock-free, unbounded queue
    tbb_wrapper.h        : Lets us use the tbb queue generically
    main.cpp             : Main
    
## Proof of correctness
    
    Blocking:
        The blocking queue is based on the locking queue from Ch. 10. Because each thread must lock the head and tail nodes
        to perform an remove and add, respectively, there is no chance of interference from other threads except when head 
        and tail are the same. In this case, a std::conditional_variable is used to allow threads to block until the queue
        is in an appropriate state. In my implementation, it is possible for the BlockingQueue to reach a deadlock if :
            1) The queue is empty and every thread tries to dequeue
            2) The queue is full and every thread tries to enqueue
        
    Non-blocking:
        Also similar to the lock-free queue found in the book, however this addresses the ABA problem using bit-stealing. At
        various stages in the add and remove methods the method checks that the head and tail pointers are as expected and have
        not been updated since they last checked. By stealing 3 bits from the item pointer, we can perform a single word CAS
        operation and find if the node has been modififed since last observed.
        
## Difficulty in implementation
    
    By far the most difficult technical challenge was learning the std::atomics library and how to address the ABA problem. Beyond that,
    the memory management/pointer manipulation of C++ takes some time to understand and implement properly. I've observed the
    NonBlocking implementation to experience segmenation faults, however I hope the algorithm can be evaluated for it's validity.
    
    Due to having to prepare 2 presentation for the course this week, I also experience extreme time constraints that ultimately
    lead to me not being able to create as well tested an implementation as I desired.
    
## Known issues:

    Due to time constraints, I was unable to implement the local node storage and pre-runtime randomization described in the 
    assignment. 
    
# Performance:
    
    I was unfortunately not able to evaluate systematically the performance of the algorithms.