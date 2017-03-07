/* 
 * Wrapper around the TBB queue that derives FIFOQueue
 *
 */

#ifndef TBB_WRAPPER_H
#define TBB_WRAPPER_H

#include <iostream>

#include "tbb/concurrent_queue.h"

#include "fifoqueue.h"

template<class T>
class TBBWrapper : public FIFOQueue<T>
{
public:
  TBBWrapper<T>()  {};
  ~TBBWrapper<T>() {};

  bool add(T item)        { std::cout << "TBB added " << item << std::endl; queue.push(item); }
  T    remove(T& result)  { queue.try_pop(result); }

private:
  tbb::concurrent_queue<T> queue;
};

#endif /* TBB_WRAPPER_H */
