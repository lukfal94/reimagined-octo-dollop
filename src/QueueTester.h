/*
 * Test class used to test various queue implemenations
 *
 */


#ifndef QUEUE_TESTER_H
#define QUEUE_TESTER_H

template<class T>
class QueueTester
{
public:
  QueueTester(FIFOQueue<T>* queue);
  ~QueueTester();

private:
  FIFOQueue<T>* queue;
}

#endif /* QUEUE_TESTER_H */
