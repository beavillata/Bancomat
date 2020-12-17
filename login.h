#ifndef LOGIN_H
#define LOGIN_H

#include "user.h"

class login{
public:
  login(){};
  login(int[],int[], int[], int[]);

  void CNC(); //check numero di numerocarta
  void CPIN(); //check pin
  bool accesso(); //permette accesso

private:
  int checkNC[13];
  int checkPIN[5];
  int inputNC[13];
  int inputPIN[5];
  bool checkbool;
};

#endif
