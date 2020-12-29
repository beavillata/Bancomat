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
  static bool inputPin(std::string&);
  static std::string getDate();
  static int getUUID();

  static const std::string XOR_KEY;

  static inline const int ADMIN_USER_ID = 270943001;

  static std::unique_ptr<CSVFile> credentials,
    accounts, movements, external;

  static const std::string TO_SELF,
    COORDINATE_SEPARATOR,
    MOVEMENT_DEPOSIT_CASH,
    MOVEMENT_DEPOSIT_CHEQUE,
    MOVEMENT_WITHDRAWAL,
    MOVEMENT_TRANSFER,
    MOVEMENT_OK,
    MOVEMENT_PENDING,
    MOVEMENT_REFUSED;

  static const std::string CURRENCY;

  static const std::vector<std::string> OPTIONS_MAIN,
    OPTIONS_DEPOSIT,
    OPTIONS_ADMIN,
    OPTIONS_MANAGE,
    OPTIONS_CHEQUE,
    OPTIONS_TRANSFER;

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
    OPTIONS_ADMIN_CHEQUE = 4,
    OPTIONS_ADMIN_TRANSFER = 5,
    OPTIONS_ADMIN_OPERATIONS = 6,
    OPTIONS_ADMIN_REACTIVATE = 7;

  static inline const int OPTIONS_CHEQUE_CANCEL = 0,
    OPTIONS_CHEQUE_ACCEPT = 1,
    OPTIONS_CHEQUE_REFUSE = 2;

  static inline const int OPTIONS_TRANSFER_CANCEL = 0,
    OPTIONS_TRANSFER_ACCEPT = 1,
    OPTIONS_TRANSFER_REFUSE = 2;

  static inline const bool ALIGN_LEFT = true,
    ALIGN_RIGHT = false;

  struct cell {
    int width;
    std::string content;
    bool align = ALIGN_LEFT;
  };

  static void printRow(std::vector<cell>, bool);

private:
  static int getObfuscated();
};

#endif
