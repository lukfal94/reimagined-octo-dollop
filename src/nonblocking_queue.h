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
    //Bit-counter manipulation functions
    uint64_t      get_counter(QueueItem<T>* ptr);
    void          inc_counter(QueueItem<T>* ptr);
    bool          set_counter(QueueItem<T>* ptr, uint64_t value);
    QueueItem<T>* clean_ptr(QueueItem<T>* ptr);

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
  uint64_t      zero = 0;

  set_counter(node, zero);

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
  uint64_t      zero = 0;

  set_counter(new_item, zero);

  while(true)
  {
    QueueItem<T>* tail_old = tail.load();
    QueueItem<T>* next     = tail_old->next;
    if(tail_old = tail.load())
    {
      // If next == null
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
    }
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

    // Increment the counter to avoid ABA
    uint64_t value = get_counter(next) + 1;
    if(!set_counter(next, value))
      throw CounterOverflowException(value);

    if(head_old == tail_old)
    {
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
      result = next->data;
      if(head.compare_exchange_weak(head_old, next,
            std::memory_order_release,
            std::memory_order_relaxed))
      {
        return result;
      }
    }
  }
}

// Counter is a 3bit value in lower part of pointer
template<class T>
uint64_t NonBlockingQueue<T>::get_counter(QueueItem<T>* ptr)
{
  uint64_t counter = (uint64_t) ptr;
  uint64_t mask_size = 3;
  uint64_t shft_amt = (CHAR_BIT * sizeof(uint64_t) - 1) - mask_size;

  counter <<= shft_amt;
  counter >>= shft_amt;

  return counter;
}

template<class T>
void NonBlockingQueue<T>::inc_counter(QueueItem<T>* ptr)
{
  uint64_t tmp       = (uint64_t) ptr;
  uint64_t mask_size = 3;
  uint64_t shft_amt  = (CHAR_BIT * sizeof(uint64_t) - 1) - mask_size;

  uint64_t counter = get_counter(ptr);

  counter++;

  set_counter(ptr, counter);
}

template<class T>
bool NonBlockingQueue<T>::set_counter(QueueItem<T>* ptr, uint64_t value)
{
  uint64_t tmp       = (uint64_t) ptr;
  uint64_t mask_size = 3;
  uint64_t max_count = 7;

  // Can only represent up to 7 with 3 bit value
  if(value > max_count)
  {
    return false;
  }

  tmp >>= mask_size;
  tmp <<= mask_size;
  tmp |= value;

  ptr = (QueueItem<T>*) tmp;

  return true;
}

// Returns the pointer without the counter bits
template<class T>
QueueItem<T>* NonBlockingQueue<T>::clean_ptr(QueueItem<T>* ptr)
{
  uint64_t tmp       = (uint64_t) ptr;
  uint64_t mask_size = 3;

  tmp >>= mask_size;

  return (QueueItem<T>*) tmp;
}

#endif /* NON_BLOCKING_QUEUE_H */
