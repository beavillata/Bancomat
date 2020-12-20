#ifndef USER_H
#define USER_H

#include "io.h"

class User {

public:
  User(int number, bool su): id(number), admin(su) {};
  User(int number): id(number) {
    admin = false;
  };

  void addMovement(std::string, double, std::string);

  void setBalance(double);

  std::string getCardNumber();
  std::string getPin();
  double getBalance();
  int getID();
  bool isAdmin();

private:
  int id;
  bool admin;
};

#endif
