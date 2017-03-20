#ifndef COUNTED_PTR_H
#define COUNTED_PTR_H

template<class T>
class CountedPtr
{
public:
  // Architecture specific values
  static const mask_size = 3;
  static const max_count = 7;

  static bool         set_counter(T* ptr, unsigned int value);
  static uint64_t     clean_ptr() {};
  static bool         inc_counter() {};
  static unsigned int get_counter() {};
};

#endif /* COUNTED_PTR_H */
