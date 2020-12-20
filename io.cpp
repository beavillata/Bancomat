#include <iomanip>
#include <climits>

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
  IO::MOVEMENT_DEPOSIT_CASH("DEPOSIT: CASH"),
  IO::MOVEMENT_DEPOSIT_CHEQUE("DEPOSIT: CHEQUE"),
  IO::MOVEMENT_WITHDRAWAL("WITHDRAWAL"),
  IO::MOVEMENT_TRANSFER("TRANSFER");

const std::string IO::CURRENCY("EUR");

const std::vector<std::string> IO::OPTIONS_MAIN = {"Logout",
  "Balance", "Movements", "Withdraw", "Deposit", "Transfer"};

const std::vector<std::string> IO::OPTIONS_DEPOSIT = {"Cancel",
  "Cash deposit", "Cheque deposit"};

int IO::prompt(std::vector<std::string> options) {
  for(int i = 0; i < options.size(); i++) {
    std::cout << std::left << "[" << i << "] " << std::setw(15) << options[i];
    if(i % 3 == 2) std::cout << std::endl;
  }
  std::cout << "Select option: ";
  std::string selected;
  if(inputNumber(selected, true, true, 1)) return stoi(selected);
  else return -1;
}

bool IO::inputNumber(std::string& ref, bool positive,
  bool integer, int digits) {
  std::cin >> ref;

  // Check that first char is a digit first, to use strtod.
  if(ref.empty() ||
    std::isspace(ref[0]) ||
    std::isalpha(ref[0])) return false;

  char* remainder;
  // Attempt to cast string to double, remainder becomes the position
  // of the non-number part in the string. If this value is zero all string
  // is number.
  double number = strtod(ref.c_str(), &remainder);
  if(*remainder > 0) return false;

  // Do we have a positive number?
  if(number < 0 && positive) return false;

  // Do we have an integer?
  int decimalPosition = ref.find(".");
  bool decimalFound = (decimalPosition != std::string::npos);
  if(integer && decimalFound) return false;

  // Does our number have the right amount of digits?
  if(!decimalFound) decimalPosition = ref.size();
  if(digits != -1 && decimalPosition != digits) return false;

  return true;
}

std::string IO::getDate() {
  time_t timer = time(NULL);
  char datec[26];
  strftime(datec, 26, "%Y/%m/%d %H:%M:%S", localtime(&timer));

  std::string date(datec);
  return date;
}