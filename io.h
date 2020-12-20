#ifndef IO_H
#define IO_H

#include <memory>
#include <string>
#include <vector>
#include "csv/csv_file.h"

class IO {
public:
  static bool prompt(std::vector<std::string>, int&);
  static bool inputNumber(std::string&, bool positive = false,
    bool integer = false, int digits = -1);
  static std::string getDate();

  static std::unique_ptr<CSVFile> credentials,
    accounts, movements, external;

  static const std::string TO_SELF,
    MOVEMENT_DEPOSIT_CASH,
    MOVEMENT_DEPOSIT_CHEQUE,
    MOVEMENT_WITHDRAW,
    MOVEMENT_TRANSFER;

  static const std::vector<std::string> OPTIONS_MAIN,
    OPTIONS_DEPOSIT;

  static inline const int OPTIONS_MAIN_LOGOUT = 0,
    OPTIONS_MAIN_BALANCE = 1,
    OPTIONS_MAIN_MOVEMENTS = 2,
    OPTIONS_MAIN_WITHDRAW = 3,
    OPTIONS_MAIN_DEPOSIT = 4,
    OPTIONS_MAIN_TRANSFER = 5;

  static inline const int OPTIONS_DEPOSIT_CANCEL = 0,
    OPTIONS_DEPOSIT_CASH = 1,
    OPTIONS_DEPOSIT_CHEQUE = 2;
};

#endif
