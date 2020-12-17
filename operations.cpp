#include <math.h>
#include <iostream>
#include <stdlib.h>

#include "operations.h"
#include "user.h"

using namespace std;

operations::operations(user &u)
{
  *USR=u;
}

void operations::prelievo(double amount)
{
  USR->setSALDO(USR->getSALDO()-amount);
}
