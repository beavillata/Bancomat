#include "io.h"

std::unique_ptr<CSVFile>
  IO::credentials{new CSVFile("persistent/credentials.csv")};
std::unique_ptr<CSVFile>
  IO::accounts{new CSVFile("persistent/accounts.csv")};
std::unique_ptr<CSVFile>
  IO::movements{new CSVFile("persistent/movements.csv")};
std::unique_ptr<CSVFile>
  IO::external{new CSVFile("persistent/external.csv")};

const std::string IO::TO_SELF("SELF"),
  IO::MOVEMENT_DEPOSIT_CASH("CASHDEPOSIT"),
  IO::MOVEMENT_DEPOSIT_CHEQUE("CHEQUEDEPOSIT"),
  IO::MOVEMENT_WITHDRAW("WITHDRAW"),
  IO::MOVEMENT_TRANSFER("TRANSFER");

const std::vector<std::string> IO::OPTIONS_MAIN = {"Logout",
  "Balance", "Movements", "Withdraw", "Deposit", "Transfer"};

const std::vector<std::string> IO::OPTIONS_DEPOSIT = {"Cancel",
  "Cash deposit", "Cheque deposit"};


bool IO::prompt(std::vector<std::string> options, int* selected) {
  for(int i = 0; i < options.size(); i++) {
    std::cout << "[" << i << "] " << options[i] << " \t ";
    if(i % 3 == 2) {
      std::cout << std::endl;
    }
  }
  std::cout << "Select option: ";
  return static_cast<bool>(std::cin >> *selected);
}
