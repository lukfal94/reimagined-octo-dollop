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
  TBBWrapper<T>()  {};
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

#endif /* TBB_WRAPPER_H */
