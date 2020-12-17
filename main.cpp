#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "user.h"
#include "login.h"
#include "operations.h"


using namespace std;

int main() {
  //DATABASE
  int tizioNC[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
  int tizioPIN[5] = {1,2,3,4,5};
  double tizioSALDO = 1000.2;
  //std::vector<int> v; //movimenti (??)

  //Tizio LOGIN
  int inputNC[13] = {1,2,3,4,5,6,7,8,9,10,11,12,13};
  int inputPIN[5] = {1,2,3,4,5};

  user tizio (tizioNC,tizioPIN,tizioSALDO);

  login prova2 (tizio.getNC(), tizio.getPIN(), inputNC, inputPIN);
  if(prova2.accesso(prova2)==true){cout << "ACCESSO ESEGUITO" << "\n" << endl;}

  cout << tizio.getSALDO() << endl;

  operations prendo20 (tizio);
  prendo20.prelievo(20);

  cout << tizio.getSALDO() << endl;

  //PROVE LOGIN TIZIO:
/*
  cout << "Tentativo 1: " << endl;
  //numero di carta sbagliato
  int inputNC2[13] = {1,2,3,5,5,6,7,8,9,10,11,12,13};
  login prova1 (tizio.getNC2(),tizio.getPIN(), inputNC, inputPIN);
  if(prova1.accesso(prova1)==true){cout << "ACCESSO ESEGUITO" << "\n" << endl;}

  cout << "Tentativo 2:" << endl;
  //numero di carta giusto, pin giusto
  login prova2 (tizio.getNC(), tizio.getPIN(), inputNC2, inputPIN);
  if(prova2.accesso(prova2)==true){cout << "ACCESSO ESEGUITO" << "\n" << endl;}

  cout << "Tentativo 3:" << endl;
  //numero di carta giusto, pin sbagliato
  int inputPIN2[5] = {1,2,4,4,5};
  login prova3 (tizio.getNC(), tizio.getPIN(), inputNC2, inputPIN2);
  if(prova3.accesso(prova3)==true){cout << "ACCESSO ESEGUITO" << "\n" << endl;}
*/

}
