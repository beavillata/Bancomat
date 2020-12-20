#include <iostream>
#include <climits>

#include "io.h"
#include "login.h"
#include "operations.h"

//TODO
//tabellina carina

void operation() {
  int selected;
  while(IO::prompt(IO::OPTIONS_MAIN, selected)) {
    std::cout << std::endl;
    switch(selected) {
    case IO::OPTIONS_MAIN_LOGOUT:
      return;
    case IO::OPTIONS_MAIN_BALANCE:
      Operations::printBalance();
      break;
    case IO::OPTIONS_MAIN_MOVEMENTS:
      Operations::printMovements();
      break;
    case IO::OPTIONS_MAIN_WITHDRAW:
      Operations::handleWithdraw();
      break;
    case IO::OPTIONS_MAIN_DEPOSIT:
      Operations::handleDeposit();
      break;
    case IO::OPTIONS_MAIN_TRANSFER:
      Operations::handleTransfer();
      break;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[]) {
  bool exit = false;
  
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
