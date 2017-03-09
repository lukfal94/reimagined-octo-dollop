#ifndef QUEUE_ITEM_H
#define QUEUE_ITEM_H

#include <atomic>

#include "atomic_wrapper.h"

template<class T>
class QueueItem
{
public:
  // AtomicQueueItem provides an interface for easily
  // constructing and manipulating items with an atomic
  // version number. The AtomicQueueItem is only 8B and
  // is lock-free on the Intel x86_64 architecture
  typedef AtomicWrapper<QueueItem<T>*> AtomicQueueItem;

  QueueItem<T>();
  QueueItem<T>(T* value);
  ~QueueItem<T>() {};

  T*               getItem()    { return item;    }
  std::atomic<int> getCounter() { return counter; }

  AtomicQueueItem *next = 0;
  T                                 *item = 0;

  std::atomic<int> counter;
};

template<class T>
QueueItem<T>::QueueItem(T* value) :
  counter(0)
{
  item = value;
}

template<class T>
QueueItem<T>::QueueItem() :
  counter(-1)
{
  item = 0;
}

#endif /* QUEUE_ITEM_H */
