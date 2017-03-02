
#include "lamport_lock.h"

LamportLock::LamportLock(int size) :
  size(size)
{
  flags.resize(size, false);
  labels.resize(size, 0);
}

void LamportLock::lock(int pid)
{
  int waiting = 0;
  int counter = 0;

  //Indicate we want the lock
  flags[pid] = true;

  //Take a number
  labels[pid] = maxLabel() + 1;

  //Wait for the lock
  do
  {
    counter++;

    //Reset flag
    waiting = 0;

    for(int i = 0; i < size; i++)
    {
      //Ignore ourself
      if(i == pid) continue;

      //Keep waiting if someone else is waiting
      //and their number is lower than ours
      if(flags[i] && (labels[i] < labels[pid]))
      {
        waiting = 1;
        break;
      }
      //if someone else is waiting with the same number,
      //defer to them if their id is smaller
      else if(flags[i] && (labels[i] == labels[pid]) && (i < pid))
      {
        waiting = 1;
        break;
      }
    }
  } while(waiting);
}

void LamportLock::unlock(int pid)
{
  // Indicate we're no longer waiting
  flags[pid] = false;
}

unsigned int LamportLock::maxLabel()
{
  unsigned int max = 0;

  for(int i = 0; i < labels.size(); i++)
  {
    max = (labels[i] > max) ? labels[i] : max;
  }
  return max;
}
