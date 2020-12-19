#include <iostream>
#include <string>

#include "user.h"

void User::setBalance(double balance) {
  Database::accounts->cell(1, id)->dset(balance);
  Database::accounts->save();
}

std::string User::getCardNumber() {
  return Database::credentials->cell(1, id)->sget();
}

std::string User::getPin() {
  return Database::credentials->cell(2, id)->sget();
}

double User::getBalance() {
  return Database::accounts->cell(1, id)->dget();
}

int User::getID() {
  return id;
}
