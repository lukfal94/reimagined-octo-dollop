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

template<class T>
class BlockingQueue : public FIFOQueue<T>
{
public:
  BlockingQueue<T>(int size);
  ~BlockingQueue<T>();

  bool enq(T item);
  T    deq();

private:
  T *items;

  int head;
  int tail;

  //Implement as a bounded size queue
  int size;
  int capacity;
};

template<class T>
BlockingQueue<T>::BlockingQueue(int tCap)
{
  items    = new T[tCap];
  capacity = tCap;

  size = 0;
  head = 0; 
  tail = 0;
}

template<class T>
bool BlockingQueue<T>::enq(T item)
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

template<class T>
T BlockingQueue<T>::deq()
{
  if( tail == head )
  {
    std::cout << "Queue is empty!" << std::endl;
    throw -1;
  }
  else
  {
    return items[head++ % capacity];
  }
}

template<class T>
bool BlockingQueue<T>::isEmpty()
{
  if( tail == head )
	  return true;
  else
	  return false;
}

template<class T>
void BlockingQueue<T>::printQueue()
{
  int tempHead = head;
  //while(tempHead != tail)
	  //std::cout << items[tempHead++ % capacity] << " ";
}
#endif /* BASIC_QUEUE_H */
