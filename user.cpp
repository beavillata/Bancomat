#include <iostream>
#include <string>

#include "user.h"

void User::setBalance(double balance) {
  IO::accounts->getCell(1, id)->dset(balance);
  IO::accounts->save();
}

std::string User::getCardNumber() {
  return IO::credentials->getCell(1, id)->sget();
}

std::string User::getPin() {
  return IO::credentials->getCell(2, id)->sget();
}

double User::getBalance() {
  return IO::accounts->getCell(1, id)->dget();
}

int User::getID() {
  return id;
}

/* ====================================================
*  We have to use the heap for CSV stuff as we want the
*  data to keep existing even after going out of scope.
*  Don't worry, there won't be memory leaks.
*  ==================================================== */

void User::addMovement(std::string to,
  double amount, std::string type) {

  CSVRow* operation = new CSVRow();

  operation->append(new CSVData<int>(id))->
    append(new CSVData<std::string>(to))->
    append(new CSVData<double>(amount))->
    append(new CSVData<std::string>(IO::getDate()))->
    append(new CSVData<std::string>(type));

  IO::movements->append(operation);
  IO::movements->save();
}
