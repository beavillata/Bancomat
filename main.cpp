#include <iostream>
#include <climits>

#include "login.h"
#include "operations.h"

//TODO
//Cheque: prima tipo poi Amount
//tabellina carina

const std::vector<std::string> mainOptions = {"Logout",
  "Balance", "Movements", "Withdraw", "Deposit", "Transfer"};

void operation() {
  int selected;
  while(User::prompt(mainOptions, &selected)) {
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
      Operations::initWithdraw();
      break;
    case 4:
      Operations::initDeposit();
      break;
    case 5:
      Operations::initTransfer();
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
    }

    std::cout << "Logging out..." << std::endl;
    Login::logout();
  }

  return 0;
}
