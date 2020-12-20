#include <iostream>

#include "admin.h"
#include "io.h"
#include "login.h"
#include "operations.h"

void Admin::handleOperations() {
  std::string number;

  std::cout << "Input user card number: ";
  if(!IO::inputNumber(number, true, true, 16)) {
    std::cout << "Invalid card number." << std::endl;
    return;
  }

  std::string adminNumber = Login::user()->getCardNumber();
  std::string adminPin = Login::user()->getPin();

  Login::logout();

  std::vector<int> match = IO::credentials->getCol(1)->has(&number, 1);
  int found = match[0];
  if(found != -1) {
    int id = IO::credentials->getCell(found, 0)->iget();
    // User with ID = 0 is admin
    Login::login(id);
    std::cout << std::endl;
    if(!Login::user()->isAdmin()) Operations::handle();
    else std::cout << "You are already logged in as admin" << std::endl;

    Login::login(adminNumber, adminPin);
    std::cout << "Logging back as admin..." << std::endl;
  }
}
