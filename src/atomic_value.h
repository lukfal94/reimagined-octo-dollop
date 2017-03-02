#ifndef ATOMIC_VALUE_H
#define ATOMIC_VALUE_H

#include <atomic>
#include <iomanip>
#include <iostream>
#include <typeinfo>

#define MAX_VERSION 7

using namespace std;

// Wrapper class for Atomic Queue Item
template<class T>
class AtomicValue
{
public:
  AtomicValue<T>();

  explicit AtomicValue<T> ( T const& v, int version = 0);
  explicit AtomicValue<T> ( atomic<T> const& a, int version = 0);

  AtomicValue<T> ( AtomicValue const& other, int version = 0 );
  //~AtomicValue<T>();

  // All constructors should call this to finish setup
  void init(int version = 0);

  //Override = operator
  AtomicValue& operator=(AtomicValue const& other)
  {
    item.store(other.item.load());
    return *this;
  }
  
  T    get_value() { return *item; }
  bool set_version(int n);
  int  get_version();

private:
  atomic<T*> item;
  uintptr_t  ptr;

  int        version_offset = 43; 
  uintptr_t  version_mask   = (7 << version_offset); 

};

template<class T>
AtomicValue<T>::AtomicValue() :
  item(T())
{ 
  init(0);
}

template<class T>
AtomicValue<T>::AtomicValue(T const& v, int version) :
  item(v)
{
  init(version);
}

template<class T>
AtomicValue<T>::AtomicValue(atomic<T> const& a, int version) :
  item(a.load())
{
  init(version);
}

template<class T>
AtomicValue<T>::AtomicValue(AtomicValue const& other, int version) :
  item(other.item.load())
{
  init(version);
}

template<class T>
void AtomicValue<T>::init(int version)
{
  /*
  // ptr for bit manipulation
  ptr  = (uintptr_t) item;

  // Update the version number
  //cout << "Created an AtomicValue : " << *item << endl;
  //cout << "item i has size (" << sizeof(item) << ")" << endl;
  //cout << "T has size (" << sizeof(T) << ")" << endl;
  cout << "Val: " << *item << ", item addr : " << hex << item << dec << endl;
  //cout << "ptr  addr : " << hex << ptr << endl;
  //cout << "mask value: " << hex << mask << endl;

  cout << endl << "Performing bit manipulation" << endl << endl;

  //set_version(1);
  cout << "Version: " << get_version() << endl;
  cout << "Val: " << *item << ", item addr : " << hex << item << dec << endl;

//  cout << "item addr : " << hex << item << endl;
//  cout << "item val  : " << *item << endl;
  */
}

template<class T>
bool AtomicValue<T>::set_version(int n)
{ 
  // Check version is in bounds (2^3 maximum)
  if( n > MAX_VERSION || n < 0 )
  {
    return false;
  }
  cout << "ptr : " << hex << ptr << endl;
  ptr |= (n << version_offset);
  cout << "ptr : " << hex << ptr << dec << endl;
  item = (T*) ptr;

  return true;
}

template<class T>
int AtomicValue<T>::get_version()
{
  uintptr_t tmp = ptr;
  int version = 0;

  tmp &= version_mask; 
  cout << "tmp : " << hex << tmp << dec << endl;
  tmp = tmp >> version_offset;
  cout << "tmp : " << hex << tmp << dec << endl;

  return (int) ptr;
}

#endif /* ATOMIC_VALUE_H */
