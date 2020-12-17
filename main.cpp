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
  int inputNC[13] = {1,2,4,4,5,6,7,8,9,10,11,22,13};
  int inputPIN[5] = {1,2,3,4,5};

  user tizio (tizioNC,tizioPIN);

  //PROVA LOGIN TIZIO

  cout << "Tentativo 1: " << endl;
  //numero di carta sbagliato
  login prova1 (tizio.getNC(),tizio.getPIN(), inputNC, inputPIN);
  prova1.CNC();
  prova1.CPIN();

  cout << "Tentativo 2:" << endl;
  //numero di carta giusto, pin giusto
  int inputNC2[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
  login prova2 (tizio.getNC(), tizio.getPIN(), inputNC2, inputPIN);
  prova2.CNC();
  prova2.CPIN();

  cout << "Tentativo 3:" << endl;
  //numero di carta giusto, pin sbagliato
  int inputPIN2[5] = {1,2,4,4,5};
  login prova3 (tizio.getNC(), tizio.getPIN(), inputNC2, inputPIN2);
  prova3.CNC();
  prova3.CPIN();

}
