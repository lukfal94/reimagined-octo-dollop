#include <atomic>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>

#include "tbb/concurrent_queue.h"
#include "tbb_wrapper.h"

#include "blocking_queue.h"
#include "fast_rand.h"
#include "nonblocking_queue.h"
#include "queue_item.h"

using namespace std;
using namespace tbb;

#define BOUNDED_SIZE 5000

std::atomic<int> numOps(0);
std::atomic<int> numEnq(0);
std::atomic<int> numDeq(0);
std::atomic<int> numFailedDeq(0);
std::atomic<int> numFailedEnq(0);

void do_work(FIFOQueue<int>* queue, unsigned int enq_ratio)
{
  int value;

  unsigned int deq_ratio = 100 - enq_ratio;
  int result = 0;

  while(numOps-- > 0)
  {
    value = MWC % 100; // Defined in fast_rand
    if(value < enq_ratio)
    {
      try
      {
        queue->add(value);
      }
      catch(const exception& e)
      {
        numFailedEnq++;
        continue;
      }
    }
    else
    {
      try
      {
        queue->remove(result);
      }
      catch(const exception& e)
      {
        numFailedDeq++;
        continue; 
      }
    }
  }
}

int main(int argc, char *argv[])
{
  vector<FIFOQueue<int>*> queues;
  vector<thread*>         threads;
  
  // Config parameters
  unsigned int queue_size  = BOUNDED_SIZE;
  unsigned int num_ops     = 50;
  unsigned int queue_type  = 0;
  unsigned int num_threads = 1;
  unsigned int enq_ratio   = 50;
  unsigned int deq_ratio   = 50;
  bool fill_queue          = false;

  // Get command line arguments
  if(argc == 1)
  {
    cout << "Usage: ./queues <queue_type> <num_ops> <queue_size> <num_threads> <prefill_queue> <enq_ratio>" << endl;
    cout << "  <queue_type> : 0=NonBlocking, 1=Blocking, 2=TBB " << endl;
    return 0;
  }

  if(argc > 1)
    queue_type = atoi(argv[1]);

  if(argc > 2)
    num_ops = atoi(argv[2]);

  if(argc > 3)
    queue_size = atoi(argv[3]);

  if(argc > 4)
    num_threads = atoi(argv[4]);

  if(argc> 5)
    fill_queue = (bool) atoi(argv[5]);

  if(argc > 6)
  {
    enq_ratio = atoi(argv[6]);
  }

  numOps.store(num_ops);

  queues.push_back(new NonBlockingQueue<int>(queue_size, fill_queue));
  queues.push_back(new BlockingQueue<int>(queue_size, fill_queue));
  queues.push_back(new TBBWrapper<int>(queue_size, fill_queue));

  for(int i = 0; i < num_threads; i++)
  {
    threads.push_back(new thread(do_work, queues[queue_type], enq_ratio));
  }

  for(int i = 0; i < num_threads; i++)
  {
    threads[i]->join();
  }

  return 0;
}
