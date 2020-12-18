#include <iostream>
#include <string>

#include "login.h"

bool Login::log = false;

bool Login::check(std::string cardNumber, std::string pin, CSVFile* login) {
  Login::log = false;
  int row = login->col(0)->has(&cardNumber);
  if(row != -1) {
    std::cout << "Numero di carta corretto!" << std::endl;
    if(login->row(row)->cell(1)->sget() == pin) {
      std::cout << "PIN corretto, login!" << std::endl;
      Login::log = true;
      return true;
    }
    return false;
  }
  std::cout << "Numero di carta errato!" << std::endl;
  return false;
}

bool Login::logged() {
  return Login::log;
}
