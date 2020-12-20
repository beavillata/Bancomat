#include <iostream>
#include <string>

#include "user.h"

void User::setBalance(double balance) {
  IO::accounts->getCell(1, id)->dset(balance);
  IO::accounts->save();
}

std::string User::getCardNumber() {
  return IO::credentials->getCell(1, id)->sget();
}

std::string User::getPin() {
  return IO::credentials->getCell(2, id)->sget();
}

double User::getBalance() {
  return IO::accounts->getCell(1, id)->dget();
}

int User::getID() {
  return id;
}
