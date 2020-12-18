#include <iostream>
#include <string>

#include "user.h"
#include "login.h"
#include "csv/csv_file.h"

CSVFile* User::accounts = new CSVFile("persistent/accounts.csv");

User::User(int id) {
  CSVFile* credentials = Login::getCredentials();
  CSVRow* user = credentials->row(id);

  cardNumber = user->cell(1)->sget();
  PIN = user->cell(2)->sget();

  CSVRow* row2 = accounts->row(id);

  std::string ciao("ciao");
  row2->cell(1)->sset(&ciao);
  credentials->save();

}

void User::setBalance(double* money) {
  balance = money;
}

std::string User::getCardNumber() const {
  return *cardNumber;
}

std::string User::getPIN() const {
  return *PIN;
}

double User::getBalance() const {
  return *balance;
}

CSVFile* User::getAccounts() {
  return accounts;
}
