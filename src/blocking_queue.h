/*
 * Blocking queue
 *
 */

#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <exception>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>


#include "fifoqueue.h"
#include "lamport_lock.h"
#include "queue_exceptions.h"

template<class T>
class BlockingQueue : public FIFOQueue<T>
{
public:
  BlockingQueue<T>(int size, int threads = 1);
  ~BlockingQueue<T>();

  bool add(T item);
  T    remove();

private:
  T *items;

  int head;
  int tail;

  //Implement as a bounded size queue
  int size;
  int capacity;

  LamportLock headLock;
  LamportLock tailLock;
  unsigned int numThreads;
};

template<class T>
BlockingQueue<T>::BlockingQueue(int tCap, int threadCount) :
  headLock(threadCount),
  tailLock(threadCount)
{
  items    = new T[tCap];
  capacity = tCap; 
  numThreads = threadCount;

  size = 0;
  head = 0; 
  tail = 0;
}

template<class T>
bool BlockingQueue<T>::add(T item)
{
  std::lock_guard<Lock> lg(tailLock);
  try
  {
    if( (tail - head) == capacity )
    {
      throw QueueFullException();
    }
    else
    {
      std::cout << "Enqueued: " << item << std::endl;
      items[tail++ % capacity] = item;
      return true;
    }
  }
  catch(const exception& e)
  {
    std::cout << e.what() << endl;
  }
}

  template<class T>
T BlockingQueue<T>::remove()
{
  std::lock_guard<LamportLock> lg(headLock);
  try
  {
    if( tail == head )
    {
      throw QueueEmptyException();
    }
    else
    {
      std::cout << "Dequeued: " << items[head % capacity] << endl;
      return items[head++ % capacity];
    }
  }
  catch(const exception& e)
  {
    std::cout << e.what() << endl;
  }
}
#endif /* BASIC_QUEUE_H */
