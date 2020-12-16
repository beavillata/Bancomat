#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "user.h"

using namespace std;

user::user(int nc[], int pin[])
{
  for(int i=0;i<13;i++)
  {numerocarta[i]=nc[i];}
  for(int i=0;i<5;i++)
  {PIN[i]=pin[i];}
}

void user::setNC(int nc[])
{
  for(int i=0;i<13;i++)
  {numerocarta[i]=nc[i];}
}

void user::setPIN(int pin[])
{
  for(int i=0;i<5;i++)
  {PIN[i]=pin[i];}
}

int* user::getNC()
{
  return numerocarta;
}

int* user::getPIN()
{
  return PIN;
}

//void punto::stampa()
//{
//  cout << "[" << TheX << "," << TheY << "," << TheZ << "]" << endl;
//}
