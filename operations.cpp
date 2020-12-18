#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "operations.h"
#include "user.h"
#include "login.h"

using namespace std;

operations::operations(user &u, bool log)
{
  *USR=u;
  LOG=log;
}

void operations::prelievo(double amount)
{
  if(LOG==true)
  {
    if((USR->getSALDO()-amount)<0){cout << "Saldo Insufficente: operazione bloccata" << endl;}
    else
    {
      USR->setSALDO(USR->getSALDO()-amount);
      cout << "Sono stati prelevati: " << amount << " euro" << endl;
    }
  }
  else{cout << "accesso negato" << endl;}
}

void operations::versamento(double amount)
{
    USR->setSALDO(USR->getSALDO()+amount);
    cout << "Sono stati versati " << amount << " euro" << endl;
}
