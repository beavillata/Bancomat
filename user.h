#ifndef USER_H
#define USER_H

#include "io.h"

class User {

public:
  User(int number): id(number) {};

  void addMovement(std::string, double, std::string, std::string);

  void setBalance(double);

  std::string getCardNumber();
  std::string getPin();
  double getBalance();
  int getID();
  bool isAdmin();

private:
  CSVRow* getRow();
  int id;
};

#endif
