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
NonBlockingQueue<T>::NonBlockingQueue(int tCap)
{
  // Initialize the head and tail pointer to the same null node
  QueueItem<T>* node = new QueueItem<T>(0); 
  head.store(node);
  tail.store(node);

  items.resize(tCap);
  capacity = tCap;

  size = 0;
}

template<class T>
bool NonBlockingQueue<T>::add(T item)
{
  // Create a QueueItem for the new data
  QueueItem<T>* new_item = new QueueItem<T>(item);
  while(true)
  {
    std::cout << "spinning..." << std::endl;
    QueueItem<T>* tail_old = tail.load();
    QueueItem<T>* next     = tail_old->next;
    if(tail_old = tail.load())
    {
      // If next == null
      std::cout << "tail valid" << std::endl;
      if(!next)
      {
        std::cout << "next valid" << std::endl;
        if(tail_old->next.compare_exchange_weak(next, new_item,
              std::memory_order_release,
              std::memory_order_relaxed))
        {
          std::cout << "CAS succeeded" << std::endl;
          tail.compare_exchange_weak(tail_old, new_item,
              std::memory_order_release,
              std::memory_order_relaxed);
        }
      }
      else
      {
        std::cout << "next invalid" << std::endl;
        tail.compare_exchange_weak(tail_old, next,
            std::memory_order_release,
            std::memory_order_relaxed);
      }
    }
    cout << "Non blocking added " << item << endl;
    return true;
  }
}

template<class T>
T NonBlockingQueue<T>::remove(T& result)
{
  while(true)
  {
    QueueItem<T>* head_old = head.load();
    QueueItem<T>* tail_old = tail.load();
    QueueItem<T>* next     = head_old->next.load();

    if(head_old == head.load())
    {
      std::cout << "head valid" << std::endl;
      if(head_old == tail_old)
      {
        std::cout << "head == tail" << std::endl;
        // if next == null
        if(!next)
        {
          throw QueueEmptyException();
          return 0;
        }
        tail.compare_exchange_weak(tail_old, next,
          std::memory_order_release,
          std::memory_order_relaxed);
      }
      else
      {
        std::cout << "head != tail" << std::endl;
        result = next->data;
        if(head.compare_exchange_weak(head_old, next,
          std::memory_order_release,
          std::memory_order_relaxed))
        {
          std::cout << "dequeue success" << std::endl;
          return result;
        }
      }
    }
  }
}
#endif /* NON_BLOCKING_QUEUE_H */
