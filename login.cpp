#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "login.h"
#include "user.h"

using namespace std;

login::login(int *cnc, int *cpin, int *icn, int *ipin)
{
  for(int i=0; i<13; i++)
  {
    checkNC[i]=cnc[i];
    inputNC[i]=icn[i];
  }
  for(int i=0; i<5; i++)
  {
    checkPIN[i]=cpin[i];
    inputPIN[i]=ipin[i];
  }
  checkbool=true;
}

//controllo se il numero di carta è giusto
void login::CNC()
{
  checkbool=true;
  int i=0;
  bool condition=true;
  while(condition==true)
  {
    if(checkNC[i]==inputNC[i]){}
    else
    {
      checkbool=false;
      condition=false;
      cout << "Numero di Carta ERRATO!" << endl;
    }
    i=i+1;
    if(i==13){condition=false;}
  }
  if(checkbool==true){cout << "Numero di Carta Corretto" << endl;}
}

void login::CPIN()
{
  int i=0;
  bool condition=true;
  if(checkbool == false){cout << "" << endl;}
  else
  {
    while(condition==true)
    {
      if(checkPIN[i]==inputPIN[i]){}
      else
      {
        checkbool=false;
        condition=false;
        cout << "PIN ERRATO!" << endl;
      }
      i=i+1;
      if(i==5){condition=false;}
    }
    if(checkbool==true){cout << "PIN Corretto" << endl;}
  }
}
//controllo se il pin è giusto
/*
void login::CPIN()
{
  if (checkbool == false){cout << "" << endl;} //Se il numero di carta è errato NON controllo il pin
  else{
    for(int i=0;i<5;i++)
    {
      if (USR->getPIN()[i] == *checkPIN[i]){}
      else {checkbool=false;}
    }
    if (checkbool == false) {cout << "PIN errato" << endl;}
    else {cout << "accesso" << endl;}
  }

}
*/
