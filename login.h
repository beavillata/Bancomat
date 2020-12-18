#ifndef LOGIN_H
#define LOGIN_H

#include "csv/csv_file.h"

class Login {
public:
  static bool login(std::string, std::string);
  static bool logged();
  static void logout();

private:
  static bool log;
  static CSVFile credentials;
  static CSVRow* user;
};

#endif
