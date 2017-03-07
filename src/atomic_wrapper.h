#ifndef ATOMIC_WRAPPER_H
#define ATOMIC_WRAPPER_H

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

private:
  std::atomic<T> _a;
};

#endif /* ATOMIC_WRAPPER_H */
