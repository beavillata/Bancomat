#ifndef USER_H
#define USER_H

#include "io.h"

class User {

public:
  User(const int number): id(number) {};

  void addMovement(const std::string, const double, const std::string,
    const std::string, int uuid = 0);

  void setBalance(const double);
  void setAttempts(const int);

  const std::string getCardNumber() const;
  const std::string getPin() const;
  const double getBalance() const;
  const int getID() const;
  const int getAttempts() const;

  const bool isAdmin() const;

private:
  const CSVRow* getRow() const;
  const int id;
};

#endif
