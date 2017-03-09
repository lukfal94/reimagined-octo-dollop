/*
 * Unbounded nonblocking Queue
 *
 */

#ifndef NON_BLOCKING_QUEUE_H
#define NON_BLOCKING_QUEUE_H

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#include "atomic_wrapper.h"
#include "fifoqueue.h"
#include "queue_exceptions.h"
#include "queue_item.h"

template<class T>
class NonBlockingQueue : public FIFOQueue<T>
{
public:
  NonBlockingQueue<T>(int size);
  ~NonBlockingQueue<T>();

  bool add(T item);
  T    remove(T& result = 0);

private:
  std::vector<QueueItem<T>::AtomicQueueItem*> items;

  QueueItem<T>::AtomicQueueItem* head;
  QueueItem<T>::AtomicQueueItem* tail;

  unsigned int size;
  unsigned int capacity;
};

template<class T>
NonBlockingQueue<T>::NonBlockingQueue(int tCap) :
  head(0),
  tail(0)
{
  items.resize(tCap);
  capacity = tCap;

  // Init queue items
  for(int i = 0; i < capacity; i ++)
  {
    items[i] = new QueueItem<T>::AtomicQueueItem();
  }

  size = 0;
}

template<class T>
bool NonBlockingQueue<T>::add(T item)
{
  QueueItem<T>::AtomicQueueItem last;
  QueueItem<T>::AtomicQueueItem next;

  try
  {
    while(true)
    {
      cout << "Non blocking added " << item << endl;
      return true;
    }
  }
  catch(const exception& e)
  {
    std::cout << e.what() << endl;
  }
}

template<class T>
T NonBlockingQueue<T>::remove(T& result)
{
  try
  {
;
  }
  catch(const exception& e)
  {
    std::cout << e.what() << endl;
  }
}
#endif /* NON_BLOCKING_QUEUE_H */
