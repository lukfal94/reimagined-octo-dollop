#ifndef ATOMIC_WRAPPER_H
#define ATOMIC_WRAPPER_H

#include <atomic>

#include "queue_item.h"

template<class T>
class QueueItem;


template<class T>
class AtomicWrapper
{
public:
  AtomicWrapper() : _a() {};

  AtomicWrapper(const std::atomic<T> &a) :
    _a(a.load()) {};

  AtomicWrapper(const AtomicWrapper &other) :
    _a(other._a.load()) {};

  AtomicWrapper &operator=(const AtomicWrapper &other)
  {
    _a.store(other._a.load());
  }

  bool is_lock_free() { return _a.is_lock_free(); }

  std::atomic<T> _a;
};

#endif /* ATOMIC_WRAPPER_H */
