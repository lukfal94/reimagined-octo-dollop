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
#include "counted_ptr.h"
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
  FIFOQueue<int>* queue;
  vector<thread*> threads;
  
  // Config parameters
  unsigned int num_ops     = 50;
  unsigned int queue_type  = 0;
  unsigned int num_threads = 1;
  unsigned int enq_ratio   = 50;
  unsigned int deq_ratio   = 50;
  bool fill_queue          = false;


  // Get command line arguments
  if(argc == 1)
  {
    cout << "Usage: ./queues <num_ops> <num_threads>" << endl;
    return 0;
  }

  if(argc > 1)
    num_ops = atoi(argv[1]);

  if(argc > 2)
    num_threads = atoi(argv[2]);

  numOps.store(num_ops);

  queue = new NonBlockingQueue<int>(fill_queue);

  /*
   *
   *
   */

  struct myStruct
  {
    int member;
  };

  /* UNIT TESTING */
  myStruct *ms = new myStruct();

  ms->member = 5;

  cout << "initial: " << hex << ms << dec << ", val: " << ms->member << endl;

  CountedPtr::initialize((uint64_t&)ms);

  for(int i = 0; i < 8; i++)
  {
    std::atomic<myStruct*> prev = ms;
    CountedPtr::set_counter((uint64_t&) ms, i);

    prev.compare_exchange_weak(prev, ms, 
        std::memory_order_release, std::memory_order_relaxed);
    
    cout << " clean : " << hex << (myStruct*) CountedPtr::clean_ptr((void*) ms) << dec;
    cout << " count : " << CountedPtr::get_counter((uint64_t)ms);
    cout << " value : " << ((myStruct*) CountedPtr::clean_ptr((void*) ms))->member << endl;
  }

  return 0;
 
  /*
   *
   *
   */





  for(int i = 0; i < num_threads; i++)
  {
    threads.push_back(new thread(do_work, queue, enq_ratio));
  }

  for(int i = 0; i < num_threads; i++)
  {
    threads[i]->join();
  }

  return 0;
}
