#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "user.h"

using namespace std;

void user::setNC(int* nc)
{
  numerocarta = nc;
}

void user::setPIN(int* pin)
{
  PIN = pin;
}

void user::setSALDO(double saldo)
{
  SALDO=saldo;
}

int* user::getNC()
{
  return numerocarta;
}

int* user::getPIN()
{
  return PIN;
}

double user::getSALDO()
{
  return SALDO;
}
