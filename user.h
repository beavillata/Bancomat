#ifndef USER_H
#define USER_H

#include "io.h"

class User {

public:
  User(int number): id(number) {};

  void setBalance(double);

  std::string getCardNumber();
  std::string getPin();
  double getBalance();

  int getID();

private:
  int id;
};

#endif
