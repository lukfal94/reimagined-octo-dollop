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
#include "lamport_lock.h"
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

void do_work(FIFOQueue<int>* queue)
{
  srand(time(0));
  int value;
  int i = 0;
  while(numEnq-- > 0)
  {
    value = i++;
    try
    {
      queue->add(value);
    }
    catch(const exception& e)
    {
      numFailedEnq++;
      continue;
    }
    numOps++;
    numDeq++;
  }

  int result = 0;
  while(numDeq-- > 0)
  {
    try
    {
      queue->remove(result);
    }
    catch(const exception& e)
    {
      numFailedDeq++;
      std::cout << e.what() << std::endl;
      continue; 
    }
    numOps++;
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
  unsigned int fill_queue  = 0;
  unsigned int enq_ratio   = 50;
  unsigned int deq_ratio   = 50;

  // Get command line arguments
  if(argc == 1)
  {
    cout << "Usage: ./queues <queue_size> <num_ops> <queue_type> <num_threads> <prefill_queue> <enq_ratio>" << endl;
    cout << "  <queue_type> : 0=NonBlocking, 1=Blocking, 2=TBB " << endl;
    return 0;
  }

  if(argc > 1)
    queue_size = atoi(argv[1]);
  
  if(argc > 2)
    num_ops = atoi(argv[2]);

  if(argc > 3)
    queue_type = atoi(argv[3]);

  if(argc > 4)
    num_threads = atoi(argv[4]);

  if(argc> 5)
    fill_queue = atoi(argv[5]);

  if(argc > 6)
  {
    enq_ratio = atoi(argv[6]);
    deq_ratio = 100 - enq_ratio;
  }

  numEnq.store(num_ops);

  queues.push_back(new NonBlockingQueue<int>(BOUNDED_SIZE));
  queues.push_back(new BlockingQueue<int>(queue_size));
  queues.push_back(new TBBWrapper<int>());

  for(int i = 0; i < num_threads; i++)
  {
    threads.push_back(new thread(do_work, queues[queue_type]));
  }

  for(int i = 0; i < num_threads; i++)
  {
    threads[i]->join();
  }

  cout << endl << endl;
  cout << "Num ops : " << numOps.load() << endl;
  cout << "Enqueues: " << numEnq.load() << endl;
  cout << "Fails   : " << numFailedEnq.load() << endl;
  cout << "Dequeues: " << numDeq.load() << endl;
  cout << "Fails   : " << numFailedDeq.load() << endl;

  return 0;
}
