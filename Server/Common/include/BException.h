#ifndef BEXCEPTION_H
# define BEXCEPTION_H

#include <exception>
#include <string>

class BException : public std::exception
{
private:
  const char *_e;
public:
  BException(const char *e) : _e(e) {};
  virtual ~BException();
  virtual const char* what() const throw()
  { return (_e); };
};

#endif
