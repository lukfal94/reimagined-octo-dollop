#include <atomic>
#include <iostream>

#include "counted_ptr.h"

using namespace std;

// Initialize shift_amt
const unsigned int CountedPtr::shift_amt = (8 * sizeof(uint64_t)) - CountedPtr::mask_size;

bool CountedPtr::initialize(uint64_t& ptr)
{
  cout << "Size: " << sizeof(ptr) << endl;
  if(sizeof(ptr) < sizeof(uint64_t))
    return false;

  return set_counter((uint64_t&) ptr, 0);
}

bool CountedPtr::set_counter(uint64_t& ptr, unsigned int value)
{
  uint64_t orig = ptr;
  uint64_t tmp  = orig;

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

// We steal the LSB which is always 1. 
// Put it back!
uint64_t CountedPtr::clean_ptr(void *ptr)
{
  uint64_t clean = (uint64_t) ptr;

  clean >>= mask_size;
  //clean  |= (uint64_t);
  clean <<= mask_size;

  return clean;
}

unsigned int CountedPtr::get_counter(uint64_t ptr)
{
  uint64_t counter = ptr; 
  
  counter <<= shift_amt;
  counter >>= shift_amt;

  return (unsigned int) counter;
}
