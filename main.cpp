#include <iostream>
#include <climits>

#include "login.h"

const char EXIT = 'O';

void operation() {
  bool done = false;
  char option;
  std::cout << std::endl;
  // What do you want to do?
  while(std::cin >> option) {
    // WORK IN PROGRESS.... ITS 2 AM 
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
      std::cout << std::endl << "Current balance: "
        << Login::user()->balance() << " £" << std::endl;
      operation();
    }

    std::cout << "Logging out..." << std::endl;
    Login::logout();
  }

  return 0;
}
