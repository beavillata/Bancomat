#ifndef LOGIN_H
#define LOGIN_H

#include "user.h"

class login{
public:
  login(){};
  login(user&, int[], int[]);

  void CNC(); //check numero di numerocarta
  void CPIN(); //check pin

private:
  int checkNC[13];
  int checkPIN[5];
  user USR;
};

#endif