/*
 * A basic implementation of a FIFO Queue using std::vector
 *
 */

#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <exception>
#include <iostream>
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

  Lock        *headLock;
  Lock        *tailLock;
  unsigned int numThreads;
};

template<class T>
BlockingQueue<T>::BlockingQueue(int tCap, int threadcount)
{
  items    = new T[tCap];
  capacity = tCap; 
  numThreads = threadcount;

  headLock = new LamportLock(numThreads);
  tailLock = new LamportLock(numThreads);

  size = 0;
  head = 0; 
  tail = 0;
}

template<class T>
bool BlockingQueue<T>::add(T item)
{
  tailLock->lock();
  try
  {
    if( (tail - head) == capacity )
    {
      std::cout << "Queue is full!" << std::endl;
      return false;
    }
    else
    {
      //    std::cout << "Enqueued: " << item << std::endl;
      items[tail++ % capacity] = item;
      return true;
    }
  }
  catch(const exception& e)
  {
    cout << e.what() << endl;
    tailLock->unlock();
  }
}

  template<class T>
T BlockingQueue<T>::remove()
{
  try
  {
    if( tail == head )
    {
      throw QueueEmptyException();
    }
    else
    {
      return items[head++ % capacity];
    }
  }
  catch(const exception& e)
  {
    cout << e.what() << endl;
  }
}
#endif /* BASIC_QUEUE_H */
