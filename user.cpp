#include <iostream>
#include <string>

#include "user.h"

void User::setBalance(double balance) {
  IO::accounts->getCell(id, 1)->set(balance);
  IO::accounts->save();
}

std::string User::getCardNumber() {
  int index = IO::credentials->getCol(0)->has(id, 1)[0];
  return IO::credentials->getCell(index, 1)->sget();
}

std::string User::getPin() {
  int index = IO::credentials->getCol(0)->has(id, 1)[0];
  return IO::credentials->getCell(index, 2)->sget();
}

double User::getBalance() {
  int index = IO::accounts->getCol(0)->has(id, 1)[0];
  return IO::accounts->getCell(id, 1)->dget();
}

int User::getID() {
  return id;
}

bool User::isAdmin() {
  return (id == IO::ADMIN_USER_ID);
}

/* ====================================================
*  We have to use the heap for CSV stuff as we want the
*  data to keep existing even after going out of scope.
*  Don't worry, there won't be memory leaks.
*  ==================================================== */

void User::addMovement(std::string to,
  double amount, std::string type, std::string status) {

  CSVRow* operation = new CSVRow();

  operation->append(new CSVCell(id))->
    append(new CSVCell(to))->
    append(new CSVCell(amount))->
    append(new CSVCell(IO::getDate()))->
    append(new CSVCell(type))->
    append(new CSVCell(status));

  IO::movements->append(operation);
  IO::movements->save();
}
