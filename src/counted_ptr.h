#ifndef COUNTED_PTR_H
#define COUNTED_PTR_H

#include <cstdint>
#include <stdlib.h>

class CountedPtr
{
public:
  // Architecture specific values
  static const unsigned int mask_size = 3;
  static const unsigned int max_count = 7;
  static const unsigned int shift_amt;

  static bool         initialize(uint64_t& ptr);
  static bool         set_counter(uint64_t& ptr, unsigned int value);
  static uint64_t     clean_ptr(void* ptr);
  static bool         inc_counter() {};
  static unsigned int get_counter(uint64_t ptr);
};

#endif /* COUNTED_PTR_H */
