/*
 * Blocking queue
 *
 * Luke Fallon
 * 10 March 2017
 *
 */

#ifndef BLOCKING_QUEUE_H
#define BLOCKING_QUEUE_H

#include <atomic>
#include <condition_variable>
#include <exception>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>


#include "fifoqueue.h"
#include "lamport_lock.h"
#include "queue_exceptions.h"

using namespace std;

template<class T>
class BlockingQueue : public FIFOQueue<T>
{
  public:
    BlockingQueue<T>(int size);
    ~BlockingQueue<T>();

    bool add(T item);
    T    remove(T& result);

  private:
    T *items;

    std::atomic<int> head;
    std::atomic<int> tail;

    std::atomic<int> size;
    int capacity;

    std::mutex  headLock;
    std::mutex  tailLock;

    std::condition_variable notEmptyCond;
    std::condition_variable notFullCond;

    std::atomic<bool> queueNotEmpty;
    std::atomic<bool> queueNotFull;

    unsigned int numThreads;
};

template<class T>
BlockingQueue<T>::BlockingQueue(int tCap) :
  size(0),
  head(0),
  tail(0)
{
  items    = new T[tCap];
  capacity = tCap; 
}

template<class T>
bool BlockingQueue<T>::add(T item)
{
  bool wakeDequeuers = false;
  std::unique_lock<std::mutex> lg(tailLock);
  
  //Wait until the queue is not full 
  while(size.load() == capacity)
    notFullCond.wait(lg);

  // Insert the item
  items[tail++ % capacity] = item;

  // Increment the size
  if(size.fetch_add(1) == 0)
    wakeDequeuers = true;

  // If size was previously 0, wake deq threads
  if(wakeDequeuers)
    notEmptyCond.notify_all(); 

  lg.unlock();
}

template<class T>
T BlockingQueue<T>::remove(T& result)
{
  bool wakeEnqueuers = true;
  std::unique_lock<std::mutex> lg(headLock);
  
  //Wait until the queue is not empty
  while(size.load() == 0)
    notEmptyCond.wait(lg);

  //Get the value
  result = items[head++ % capacity];

  if(size.fetch_sub(1) == capacity)
    wakeEnqueuers = true;

  if(wakeEnqueuers)
    notFullCond.notify_all();

  // lg will unlock when we return bc of RAII
  return result;
}
#endif /* BASIC_QUEUE_H */
