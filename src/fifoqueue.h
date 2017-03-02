/*
 * Interface for FIFO Queue
 *
 */

#ifndef FIFOQUEUE_H
#define FIFOQUEUE_H

template<class T>
class FIFOQueue
{
protected:
  // Head and tail members
  T  *head;
  T  *tail;
  
  // Number of elements in queue and capacity
  int numElem;
  int capacity;

public:
  FIFOQueue<T>()  {};
  ~FIFOQueue<T>() {};

  virtual bool add(T item) = 0;
  virtual T    remove()    = 0;

  // It is left to the implementing class to choose
  // an appropriate structure to implement the queue
  // as well as tail and head pointers (if necessary)
};

#endif /* FIFOQUEUE_H */
