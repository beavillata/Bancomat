#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "csv/csv_file.h"
#include "login.h"
#include "operations.h"
#include "admin.h"

//DA AGGIUSTARE:


int main(int argc, char* argv[]) {
  std::ifstream splash("splash.txt");

  bool exit = false;
  std::cout << std::setprecision(2) << std::fixed;

  std::cout << splash.rdbuf();

  while(!exit) {
    std::string number, pin;

    std::cout << "Please input your card number: ";
    if(!IO::inputNumber(number, true, true, 16)) {
      std::cout << "Invalid card number." << std::endl;
      continue;
    }

    int count;
    std::vector<int> match = IO::credentials->getCol(1)->has(number, 1);
    int found = match[0];

    if(found != -1) {
      CSVRow* row = IO::credentials->getRow(found);
      count = std::stoi(row->getCell(3)->sget());
      if(count == 3) {

      } else {
        std::cout << "Please input your PIN: ";
        if(!IO::inputPin(pin)) {
          std::cout << std::endl;
          std::cout << "Invalid pin." << std:: endl;
          continue;
        }
      }
    }

    std::stringstream ss;

    if(Login::login(number, pin)) {
      if(Login::user()->isAdmin()) Admin::handle();
      else Operations::handle();
      std::cout << "Logging out..." << std::endl;
      Login::logout();
      count = 0;
      ss << count;
      std::string str = ss.str();
      IO::credentials->getCell(found, 3)->set(str);
      IO::credentials->save();
      continue;
    } else if(count == 3) {
      std::cout << "Your account has been suspended;" << std::endl;
      continue;
    }
    count++;
    ss << count;
    std::string str = ss.str();
    IO::credentials->getCell(found, 3)->set(str);
    IO::credentials->save();
  }
  return 0;
}
