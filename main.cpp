#include <iostream>
#include <iomanip>

#include "login.h"
#include "operations.h"

void operation() {
  bool select = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_MAIN)) {
    case IO::OPTIONS_MAIN_LOGOUT:
      select = false;
      break;
    case IO::OPTIONS_MAIN_BALANCE:
      Operations::printBalance();
      break;
    case IO::OPTIONS_MAIN_MOVEMENTS:
      Operations::printMovements();
      break;
    case IO::OPTIONS_MAIN_WITHDRAWAL:
      Operations::handleWithdrawal();
      break;
    case IO::OPTIONS_MAIN_DEPOSIT:
      Operations::handleDeposit();
      break;
    case IO::OPTIONS_MAIN_TRANSFER:
      Operations::handleTransfer();
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[]) {
  bool exit = false;
  std::cout << std::setprecision(2) << std::fixed;

  while(!exit) {
    std::string number, pin;

    std::cout << "Please input your card number: ";
    if(!IO::inputNumber(number, true, true, 16)) {
      std::cout << "Invalid card number." << std::endl;
      continue;
    }

    std::cout << "Please input your PIN: ";
    if(!IO::inputNumber(pin, true, true, 5)) {
      std::cout << "Invalid pin." << std:: endl;
      continue;
    }

    if(Login::login(number, pin)) {
      std::cout << std::endl;
      operation();
      std::cout << "Logging out..." << std::endl << std::endl;
      Login::logout();
    }
  }
  return 0;
}
