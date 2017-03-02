/*
 * Implementation of the Lamport Bakery Lock
 *
 */

#ifndef LAMPORT_LOCK_H
#define LAMPORT_LOCK_H

#include <atomic>
#include <map>
#include <thread>
#include <vector>

#include "lock.h"

class LamportLock : public Lock
{
public:
  LamportLock(int numThreads);
  ~LamportLock() {};

  void lock();
  void unlock();

private:
  int size;

  std::vector<bool>         flags;
  std::vector<unsigned int> labels;

  std::atomic<int>               next_id;
  std::map<std::thread::id, int> id_map;

  unsigned int maxLabel(); 
};


#endif
