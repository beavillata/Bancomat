#ifndef IO_H
#define IO_H

#include <memory>
#include <string>
#include <vector>

#include "csv/csv_file.h"

class IO {
public:
  static const int prompt(std::vector<std::string>);

  static const bool inputNumber(std::string&, const bool positive = false,
    const bool integer = false, const int digits = -1);
  static const bool inputPin(std::string&);

  static const std::string getDate();
  static const int getUUID();
  static void splash();

  static inline const int ADMIN_USER_ID = 270943001,
    CHEQUE_NUMBER_DIGITS = 7;

  static std::unique_ptr<CSVFile> credentials,
    accounts, movements, external;

  static const std::string XOR_KEY, CURRENCY;
  static const std::string TO_SELF,
    COORDINATE_SEPARATOR,
    MOVEMENT_DEPOSIT_CASH,
    MOVEMENT_DEPOSIT_CHEQUE,
    MOVEMENT_WITHDRAWAL,
    MOVEMENT_TRANSFER,
    MOVEMENT_OK,
    MOVEMENT_PENDING,
    MOVEMENT_REFUSED;

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
    OPTIONS_ADMIN_MANAGE = 7,
    OPTIONS_ADMIN_TERMINATE = 8;

  static inline const int OPTIONS_CHEQUE_CANCEL = 0,
    OPTIONS_CHEQUE_ACCEPT = 1,
    OPTIONS_CHEQUE_REFUSE = 2;

  static inline const int OPTIONS_TRANSFER_CANCEL = 0,
    OPTIONS_TRANSFER_ACCEPT = 1,
    OPTIONS_TRANSFER_REFUSE = 2;

  static inline const bool ALIGN_LEFT = true,
    ALIGN_RIGHT = false;

  // Out printable cell structure.
  struct cell {
    int width;
    std::string content;
    bool align = ALIGN_LEFT;
  };

  static void printRow(std::vector<cell>, const bool);

private:
  static const int getObfuscated();
};

#endif
