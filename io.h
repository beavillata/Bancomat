#ifndef IO_H
#define IO_H

#include <memory>
#include <string>
#include <vector>
#include "csv/csv_file.h"

class IO {
public:
  static bool prompt(std::vector<std::string>, int*);
  
  static std::unique_ptr<CSVFile> credentials,
    accounts, movements, external;
  static const std::string TO_SELF,
    MOVEMENT_DEPOSIT_CASH,
    MOVEMENT_DEPOSIT_CHEQUE,
    MOVEMENT_WITHDRAW,
    MOVEMENT_TRANSFER;
  static const std::vector<std::string> OPTIONS_MAIN,
    OPTIONS_DEPOSIT;
};

#endif
