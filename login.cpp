#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "login.h"
#include "user.h"

using namespace std;

login::login(user &u, int cnc[], int cpin[])
{
  for(int i=0;i<13;i++)
  {checkNC[i]=cnc[i];}
  for(int i=0;i<5;i++)
  {checkPIN[i]=cpin[i];}
  USR=u;
}

void login::CNC()
{
  bool check = true;
  for(int i=0;i<13;i++)
  {
    if (USR.getNC()[i] == checkNC[i]){}
    else {check=false;}
  }
  if (check == false) {cout << "numero di carta errato" << endl;}
  else {cout << "..." << endl;}
}
