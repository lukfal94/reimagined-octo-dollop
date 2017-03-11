#ifndef QUEUE_EXCEPTIONS_H
#define QUEUE_EXCEPTIONS_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>

using namespace std;

class QueueEmptyException : public runtime_error
{
public:
  QueueEmptyException() : runtime_error( "queue empty" ) {};

  virtual const char* what() const throw()
  {
    cnvt.str( "" );

    cnvt << runtime_error::what();

    return cnvt.str().c_str();
  }

private:
  static ostringstream cnvt;
};

ostringstream QueueEmptyException::cnvt;

class QueueFullException : public runtime_error
{
public:
  QueueFullException() : runtime_error( "queue full" ) {};

  virtual const char* what() const throw()
  {
    cnvt.str( "" );

    cnvt << runtime_error::what();

    return cnvt.str().c_str();
  }

private:
  static ostringstream cnvt;

};

ostringstream QueueFullException::cnvt;

class CounterOverflowException : public runtime_error
{
public:
  CounterOverflowException(int val) : runtime_error( "counter overflow" ), value(val) {};

  virtual const char* what() const throw()
  {
    cnvt.str( "" );

    cnvt << runtime_error::what() << " " << value;

    return cnvt.str().c_str();
  }

private:
  static ostringstream cnvt;
  int    value;

};

ostringstream CounterOverflowException::cnvt;

#endif /* QUEUE_EXCEPTIONS_H */
