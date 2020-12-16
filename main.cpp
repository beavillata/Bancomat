#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "user.h"
#include "login.h"


using namespace std;

int main() {
  //DATABASE
  int tizioNC[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
  int tizioPIN[5] = {1,2,3,4,5};

  //Tizio LOGIN
  int inputNC[13] = {1,2,4,4,5,6,7,8,9,10,11,12,13};
  int inputPIN[5] = {1,2,3,4,5};

  user tizio (inputNC,inputPIN);

  login prova (tizio, tizioNC, tizioPIN);

  prova.CNC();


}
