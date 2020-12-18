#include <iostream>

#include "login.h"
#include "csv/csv_file.h"

int main(int argc, char* argv[]) {

  CSVFile* credentials = new CSVFile("login.csv");
  credentials->print();

  std::string cardNumber, pin;
  std::cout << "Please input your card number:" << std::endl;
  std::cin >> cardNumber;
  std::cout << "Please input your PIN:" << std::endl;
  std::cin >> pin;

  Login::check(cardNumber, pin, credentials);

  delete credentials;
  return 0;
}
