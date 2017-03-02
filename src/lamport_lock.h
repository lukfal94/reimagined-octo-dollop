/*
 * Implementation of the Lamport Bakery Lock
 *
 */

#ifndef LAMPORT_LOCK_H
#define LAMPORT_LOCK_H

#include <vector>

#include "lock.h"

class LamportLock : public Lock
{
public:
  LamportLock() {};
  ~Lock()       {};

  void lock(int pid);
  void unlock(int pid);

private:
  int id;
  int size;

  std::vector<bool>         flags;
  std::vector<unsigned int> labels;

  unsigned int  maxLabel(); 
}


#endif
