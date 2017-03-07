#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "queue_item.h"
#include "fast_rand.h"
#include "nonblocking_queue.h"

using namespace std;

#define BOUNDED_SIZE 50

int main(int argc, char *argv[])
{
  FIFOQueue<int> *queue = new NonBlockingQueue<int>(BOUNDED_SIZE);
  return 0;
}
