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

atomic<unsigned int> numOps(100);

void do_work(FIFOQueue<int>* queue, unsigned int add_prob, unsigned int remove_prob)
{
  // Verify probabilities make sense
  if( add_prob + remove_prob != 100 )
  {
    cout << "Error: probabilites do not sum to 100%!" << endl;
    return;
  }

  // Wait for other threads to start

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
  unsigned int num_threads = 1;
  unsigned int queue_size  = 20;

  unsigned int add_prob    = 50;  
  unsigned int rem_prob    = 50;  

  if(argc > 1)
  {
    numOps.store(atoi(argv[1]));
  }
  else if(argc > 2)
  {
    queue_size = atoi(argv[2]);
  }
  else if(argc > 3)
  {
    num_threads = atoi(argv[3]);
  }

  FIFOQueue<int> *queue = new BlockingQueue<int>(queue_size, num_threads);
  vector<thread>  threads;

  for(int i = 0; i < num_threads; i++)
  {
    threads.push_back(thread(do_work, queue, add_prob, rem_prob));
  }

  for(int i = 0; i < num_threads; i++)
  {
    threads[i].join();
  }

  return 0;
}
