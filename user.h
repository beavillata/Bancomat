#ifndef USER_H
#define USER_H

#include "database.h"

class User {

public:
  User(int number): id(number) {};

  void setBalance(double);

  std::string cardNumber();
  std::string pin();
  double balance();

private:
  int id;

};

#endif
