#ifndef USER_H
#define USER_H

#include "csv/csv_file.h"

class User {

public:
  User(int);

  void setBalance(double*);

  std::string getCardNumber() const;
  std::string getPIN() const;
  double getBalance() const;

  static CSVFile* getAccounts();

private:
  std::string* cardNumber;
  std::string* PIN;
  double* balance;
  int* id;

  static CSVFile* accounts;
};

#include "login.h"

#endif
