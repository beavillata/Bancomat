#include <iostream>
#include <string>

#include "user.h"

void User::setBalance(double balance) {
  Database::accounts->cell(1, id)->dset(balance);
}

std::string User::cardNumber() {
  return Database::credentials->cell(1, id)->sget();
}

std::string User::pin() {
  return Database::credentials->cell(2, id)->sget();
}

double User::balance() {
  return Database::accounts->cell(1, id)->dget();
}
