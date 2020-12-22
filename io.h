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

  static const std::string XOR_KEY;

  static inline const int ADMIN_USER_ID = 0;

  static std::unique_ptr<CSVFile> credentials,
    accounts, movements, external;

  static const std::string TO_SELF,
    MOVEMENT_DEPOSIT_CASH,
    MOVEMENT_DEPOSIT_CHEQUE,
    MOVEMENT_WITHDRAWAL,
    MOVEMENT_TRANSFER,
    CHEQUE_ACCEPTED,
    CHEQUE_REJECTED,
    TRANSFER_ACCEPTED,
    TRANSFER_REJECTED,
    OK,
    PENDING;

  static const std::string CURRENCY;

  static const std::vector<std::string> OPTIONS_MAIN,
    OPTIONS_DEPOSIT, OPTIONS_ADMIN, OPTIONS_ADMIN_CHECK, OPTIONS_ADMIN_CHEQUE, OPTIONS_ADMIN_TRANSFER;

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
    OPTIONS_ADMIN_CHECKS = 4,
    OPTIONS_ADMIN_OPERATIONS = 5;

  static inline const int
    OPTIONS_ADMIN_CHECK_CHEQUE = 1,
    OPTIONS_ADMIN_CHECK_TRANSFER = 2;

  static inline const int
   ACCEPT_CHEQUE = 1,
   REFUTE_CHEQUE = 2;

  static inline const int
    ACCEPT_TRANSFER = 1,
    REFUTE_TRANSFER = 2;
};

#endif
