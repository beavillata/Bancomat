#include <iostream>

#include "login.h"
#include "user.h"
#include "csv/csv_file.h"

int main(int argc, char* argv[]) {
  bool exit = false;
  while(!exit) {
    std::string cardNumber;
    std::string pin;
    std::cout << "Please input your card number:" << std::endl;
    std::cin >> cardNumber;
    std::cout << "Please input your PIN:" << std::endl;
    std::cin >> pin;

    Login::login(cardNumber, pin);

  }
  delete Login::getCredentials();
  delete User::getAccounts();
  return 0;
}
