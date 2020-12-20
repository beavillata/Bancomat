#include <iostream>
#include <climits>

#include "io.h"
#include "login.h"
#include "operations.h"

//TODO
//tabellina carina
//controllare caratteri numero carta e lunghezza
//idem pin

void operation() {
  int selected;
  while(IO::prompt(IO::OPTIONS_MAIN, &selected)) {
    std::cout << std::endl;
    switch(selected) {
    case 0:
      return;
    case 1:
      Operations::printBalance();
      break;
    case 2:
      Operations::printMovements();
      break;
    case 3:
      Operations::handleWithdraw();
      break;
    case 4:
      Operations::handleDeposit();
      break;
    case 5:
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
    std::cin >> number;
    std::cout << "Please input your PIN: ";
    std::cin >> pin;

    if(Login::login(number, pin)) {
      std::cout << std::endl;
      operation();
      std::cout << "Logging out..." << std::endl << std::endl;
      Login::logout();
    }
  }

  return 0;
}
