#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "user.h"

class operations{
public:
  operations(){};
  operations(user&);

  void prelievo(double);

private:
  user *USR;
};

#endif
