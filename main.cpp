#include <iostream>
#include <climits>

#include "login.h"
#include "operations.h"

const int EXIT = 0,
  BALANCE = 1,
  MOVEMENTS = 2,
  WITHDRAW = 3,
  DEPOSIT = 4,
  TRANSFER = 5;

void operation() {
  bool done = false;
  int option;
  std::cout << "0: abort \n 1: Print Balance \n 2: Print Movements \n 3: Withdraw \n 4: Deposit \n 5: Transfer" << std::endl;
  while(std::cin >> option) {
    switch(option) {
    case EXIT:
      return;
    case BALANCE:
      Operations::printBalance();
      break;
    case MOVEMENTS:
      Operations::printMovements();
      break;
    case WITHDRAW:
      Operations::initWithdraw();
      break;
    case DEPOSIT:
      Operations::initDeposit();
      break;
    case TRANSFER:
      break;
    }
  }
}

int main(int argc, char* argv[]) {
  bool exit = false;

  while(!exit) {
    std::string number, pin;
    std::cout << "Please input your card number:" << std::endl;
    std::cin >> number;
    std::cout << "Please input your PIN:" << std::endl;
    std::cin >> pin;

    if(Login::login(number, pin)) {
      std::cout << "Welcome" << std::endl;
      Operations::printBalance();
      operation();
    }

    std::cout << "Logging out..." << std::endl;
    Login::logout();
  }

  return 0;
}
