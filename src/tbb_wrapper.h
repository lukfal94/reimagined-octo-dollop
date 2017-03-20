/* 
 * Wrapper around the TBB queue that derives FIFOQueue
 *
 */

#ifndef TBB_WRAPPER_H
#define TBB_WRAPPER_H

#include <iostream>

#include "tbb/concurrent_queue.h"

#include "fifoqueue.h"
#include "queue_exceptions.h"

template<class T>
class TBBWrapper : public FIFOQueue<T>
{
public:
  TBBWrapper<T>(int size, bool fill);
  ~TBBWrapper<T>() {};

  bool add(T item)        { queue.push(item); }
  T    remove(T& result)  
  {
    if(queue.try_pop(result))
    {
      return result;
    }
    else
    {
      throw QueueEmptyException();
    }
  }

private:
  tbb::concurrent_queue<T> queue;
};

template<class T>
TBBWrapper<T>::TBBWrapper(int size, bool fill)
{
  for(int i; i < size; i++)
  {
    queue.push((T) 0);
  }
}

#endif /* TBB_WRAPPER_H */
