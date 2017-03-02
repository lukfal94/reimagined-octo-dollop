#ifndef QUEUE_EXCEPTIONS_H
#define QUEUE_EXCEPTIONS_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <sstream>

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

/*
class QueueFullException : public runtime_error
{

};
*/
#endif /* QUEUE_EXCEPTIONS_H */
