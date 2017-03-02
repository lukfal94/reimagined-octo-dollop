/*
 * Our very own lock interface!
 */


#ifndef LOCK_H
#define LOCK_H

class Lock
{
public:
  Lock()  {};
  ~Lock() {};

  virtual void lock()   = 0;
  virtual void unlock() = 0;
};


#endif /* LOCK_H */
