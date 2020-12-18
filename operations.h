#ifndef OPERATIONS_H
#define OPERATIONS_H

#include "user.h"
#include "login.h"

class operations{
public:
  operations(){};
  operations(user&, bool);

  void prelievo(double);
  void versamento(double);

private:
  user *USR;
  bool LOG;
};

#endif
