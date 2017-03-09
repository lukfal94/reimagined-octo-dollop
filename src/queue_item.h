#ifndef QUEUE_ITEM_H
#define QUEUE_ITEM_H

#include <atomic>

template<class T>
struct QueueItem
{
  T data;
  std::atomic<QueueItem<T>*>   next;
  std::atomic<unsigned int> version;
  QueueItem() : data(0), next(0), version(0) {}
  QueueItem(const T& data) : data(data), next(0), version(0) {}
  QueueItem(const T& data, std::atomic<QueueItem<T>*> next, unsigned int vers) : 
    data(data), next(next), version(vers) {}
};

/*
template<class T>
class QueueItem
{
public:
  //Default
  QueueItem<T>() :
    next(0),
    counter(0)
  {
    item = 0;
  }

  QueueItem<T>(const QueueItem<T> &source)
  {
    next = source.next;
  }

  ~QueueItem<T>()
  {
    next->store(0);
  }

  QueueItem<T>& operator =(QueueItem<T> &source)
  {
    next = source.next;
    counter = source.counter;
    return *this;
  }

  QueueItem<T>(T* item , const QueueItem<T> &next_ptr, unsigned int version)
  {
    item = item;
    next = next_ptr;
    counter.store(version);
  }

  T*                        getItem()    { return item;    }
  std::atomic<unsigned int> getCounter() { return counter; }

  QueueItem<T>* next;
  std::atomic<unsigned int> counter;

  T* item = 0;

};
*/

#endif /* QUEUE_ITEM_H */
