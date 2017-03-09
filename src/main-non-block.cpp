#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "tbb/concurrent_queue.h"
#include "tbb_wrapper.h"

#include "queue_item.h"
#include "fast_rand.h"
#include "nonblocking_queue.h"

using namespace std;
using namespace tbb;

#define BOUNDED_SIZE 50

int main(int argc, char *argv[])
{
  ofstream enqFile;
  ofstream deqFile;

  enqFile.open("enq.txt");
  deqFile.open("deq.txt");

  //FIFOQueue<int> *queue = new NonBlockingQueue<int>(BOUNDED_SIZE);
  FIFOQueue<int> *tbb2  = new TBBWrapper<int>();

  vector<FIFOQueue<int>*> queues;
  queues.push_back(new NonBlockingQueue<int>(BOUNDED_SIZE));
  queues.push_back(new TBBWrapper<int>());

  concurrent_queue<int> tbb;

  for(int i = 0; i < 1000; i++)
  {
    enqFile << i % 10 << " ";
    queues[0]->add(i % 10);
//    tbb2->add(i % 10);
  }

  for(int i = 0; i < 1000; i++)
  {
    int result;
 //   tbb2->remove(result);  
    queues[0]->remove(result);
    deqFile << result << " ";
  }

  //delete queue;
  delete tbb2;

  enqFile.close();
  deqFile.close();
  return 0;
}
