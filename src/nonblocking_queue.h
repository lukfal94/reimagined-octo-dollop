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
  std::vector<QueueItem<T>*> items;

  std::atomic<QueueItem<T>*> head;
  std::atomic<QueueItem<T>*> tail;

  unsigned int size;
  unsigned int capacity;
};

template<class T>
NonBlockingQueue<T>::NonBlockingQueue(int tCap) :
  head(new QueueItem<T>(0)),
  tail(new QueueItem<T>(0))
{
  // Initialize the head and tail pointer to the same null node
  QueueItem<T>* node = new QueueItem<T>(0); 
/*
  head.compare_exchange_weak(node->next.load(), node,
                            std::memory_order_release,
                            std::memory_order_relaxed);

  tail.compare_exchange_weak(node->next.load(), node,
                            std::memory_order_release,
                            std::memory_order_relaxed);
*/
  items.resize(tCap);
  capacity = tCap;

  size = 0;
}

template<class T>
bool NonBlockingQueue<T>::add(T item)
{
  try
  {
    // Create a QueueItem for the new data
    QueueItem<T>* new_item = new QueueItem<T>(item);
    while(true)
    {
      QueueItem<T>* tail_old = tail.load();
      QueueItem<T>* next     = tail_old->next;
      if(tail_old = tail.load())
      {
        // If next == null
        /*
        if(!next)
        {
          if(tail_old->next.compare_exchange_weak(next, new_item,
                std::memory_order_release,
                std::memory_order_relaxed))
          {
            tail.compare_exchange_weak(tail_old, new_item,
                std::memory_order_release,
                std::memory_order_relaxed);
          }
        }
        else
        {
          tail.compare_exchange_weak(tail_old, next,
            std::memory_order_release,
            std::memory_order_relaxed);
        }
        */
      }
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
