#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "csv/csv_file.h"
#include "login.h"
#include "operations.h"
#include "admin.h"

int main(int argc, char* argv[]) {
  std::ifstream splash("splash.txt");
  std::cout << splash.rdbuf();
  splash.close();

  std::cout << std::setprecision(2) << std::fixed;

  bool exit = false;
  while(!exit) {
    std::string number, pin;

    std::cout << "Please input your card number: ";
    if(!IO::inputNumber(number, true, true, 16)) {
      std::cout << "Invalid card number." << std::endl;
      continue;
    }

    std::cout << "Please input your PIN: ";
    if(!IO::inputPin(pin)) {
      std::cout << std::endl << "Invalid pin." << std::endl;
      continue;
    }

    if(Login::login(number, pin)) {
      // Use correct UI.
      if(Login::user()->isAdmin()) Admin::handle();
      else Operations::handle();

      std::cout << "Logging out..." << std::endl;
      Login::logout();
    }
  }
  return 0;
}
