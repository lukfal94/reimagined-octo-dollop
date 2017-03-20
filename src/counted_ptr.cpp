#include "counted_ptr.h"

template<class T>
static bool CountedPtr::set_counter(T* ptr, unsigned int value)
{
  uint64_t tmp = (uint64_t) ptr;

  if(value > max_count)
  {
    return false;
  }

  tmp >>= mask_size;
  tmp <<= mask_size;
  tmp  |= (uint64_t) value;

  ptr = tmp;

  return true;
}
