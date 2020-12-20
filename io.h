#ifndef IO_H
#define IO_H

#include <memory>
#include <string>
#include <vector>
#include "csv/csv_file.h"

class IO {
public:
  static int prompt(std::vector<std::string>);
  static bool inputNumber(std::string&, bool positive = false,
    bool integer = false, int digits = -1);
  static std::string getDate();

  static std::unique_ptr<CSVFile> credentials,
    accounts, movements, external;

  static const std::string TO_SELF,
    MOVEMENT_DEPOSIT_CASH,
    MOVEMENT_DEPOSIT_CHEQUE,
    MOVEMENT_WITHDRAWAL,
    MOVEMENT_TRANSFER;

  static const std::string CURRENCY;

  static const std::vector<std::string> OPTIONS_MAIN,
    OPTIONS_DEPOSIT, OPTIONS_ADMIN;

  static inline const int OPTIONS_MAIN_LOGOUT = 0,
    OPTIONS_MAIN_BALANCE = 1,
    OPTIONS_MAIN_MOVEMENTS = 2,
    OPTIONS_MAIN_WITHDRAWAL = 3,
    OPTIONS_MAIN_DEPOSIT = 4,
    OPTIONS_MAIN_TRANSFER = 5;

  static inline const int OPTIONS_DEPOSIT_CANCEL = 0,
    OPTIONS_DEPOSIT_CASH = 1,
    OPTIONS_DEPOSIT_CHEQUE = 2;

  static inline const int OPTIONS_ADMIN_LOGOUT = 0,
    OPTIONS_ADMIN_BALANCE = 1,
    OPTIONS_ADMIN_SUB = 2,
    OPTIONS_ADMIN_ADD = 3,
    OPTIONS_ADMIN_OPERATIONS = 4;
};

#endif
