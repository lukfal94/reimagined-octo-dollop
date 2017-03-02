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

  virtual void lock(int id = 0)   = 0;
  virtual void unlock(int id = 0) = 0;
};


#endif /* LOCK_H */
