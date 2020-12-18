#include <iostream>

#include "login.h"
#include "csv/csv_file.h"

int main(int argc, char* argv[]) {
  bool exit = false;
  while(!exit) {
    std::string cardNumber, pin;
    std::cout << "Please input your card number:" << std::endl;
    std::cin >> cardNumber;
    std::cout << "Please input your PIN:" << std::endl;
    std::cin >> pin;

    if(Login::login(cardNumber, pin)) {
      exit = true;
    };
  }
  return 0;
}
