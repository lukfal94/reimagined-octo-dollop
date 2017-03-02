#include <atomic>
#include <iostream>
#include <functional>
#include <string>
#include <thread>
#include <utility>

#include "atomic_value.h"
#include "blocking_queue.h"
#include "fast_rand.h"

// TODO:
// - use floats for probability

using namespace std;

atomic<unsigned int> numOps(1000000);

//template<class T>
void do_work(FIFOQueue<int>* queue, unsigned int add_prob, unsigned int remove_prob)
{
  // Verify probabilities make sense
  if( add_prob + remove_prob != 100 )
  {
    cout << "Error: probabilites do not sum to 100%!" << endl;
    return;
  }

  // Cumulatively, all thread will perform numOps operations
  // on the queue
  while(numOps-- > 0)
  {
    if( (MWC % 100) < add_prob )
    {
      queue->add(MWC % 20);
    }
    else
    {
      queue->remove();
    }
  }
}

int main(int argc, char *argv[])
{

  unsigned int add_prob = 50;  
  unsigned int rem_prob = 50;  

  FIFOQueue<int> *queue = new BlockingQueue<int>(20);

  thread thread1(do_work, queue, add_prob, rem_prob);
  thread thread2(do_work, queue, add_prob, rem_prob);

  thread1.join();
  thread2.join();

  return 0;
}
