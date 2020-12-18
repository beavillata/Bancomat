#ifndef USER_H
#define USER_H

#include "login.h"

class user{
public:
  user(int* cardNumber, int* pin, double balance):
    numerocarta(cardNumber), PIN(pin), SALDO(balance) {};

  void setNC(int*);
  void setPIN(int*);
  void setSALDO(double);

  int* getNC();
  int* getPIN();
  double getSALDO();

private:
  int *numerocarta;
  int *PIN;
  double SALDO;
};

#endif
