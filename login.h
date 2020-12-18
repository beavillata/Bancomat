#ifndef LOGIN_H
#define LOGIN_H

#include "csv/csv_file.h"
#include "user.h"

class Login {
public:
  static bool login(std::string, std::string);
  static bool logged();
  static void logout();

  static CSVFile* getCredentials();

private:
  static bool log;
  static User* user;
  static CSVFile* credentials;
};

#endif
