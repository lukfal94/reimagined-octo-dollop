#ifndef QUEUE_ITEM_H
#define QUEUE_ITEM_H

#include <atomic>

template<class T>
class QueueItem
{
public:
  QueueItem<T>();
  QueueItem<T>(T* value);
  ~QueueItem<T>() {};

  T*               getItem()    { return item;    }
  std::atomic<int> getCounter() { return counter; }
  std::atomic<int> getIndex()   { return index;   }

private:
  T*               item = 0;
  std::atomic<int> counter;

  std::atomic<int> index;
};

template<class T>
QueueItem<T>::QueueItem(T* value) :
  counter(0),
  index(-1)
{
  item = value;
}

template<class T>
QueueItem<T>::QueueItem() :
  counter(-1),
  index(-1)
{
  item = 0;
}

#endif /* QUEUE_ITEM_H */
