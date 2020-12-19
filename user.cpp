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

bool User::prompt(std::vector<std::string> options, int* selected) {
  for(int i = 0; i < options.size(); i++) {
    std::cout << "[" << i << "] " << options[i] << " \t ";
    if(i % 3 == 2) {
      std::cout << std::endl;
    }
  }
  std::cout << "Select option: ";
  return static_cast<bool>(std::cin >> *selected);
}
