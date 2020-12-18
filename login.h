#ifndef LOGIN_H
#define LOGIN_H

#include "csv/csv_file.h"

class Login {
public:
  static bool check(std::string, std::string, CSVFile*);
  static bool logged();

private:
  static bool log;
};

#endif
