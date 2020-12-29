#include <iostream>
#include <string>

#include "user.h"

void User::setBalance(double balance) {
  int index;
  IO::accounts->getCol(0)->first(id, index);

  IO::accounts->getCell(index, 1)->set(balance);
  IO::accounts->save();
}

std::string User::getCardNumber() {
  return getRow()->getCell(1)->sget();
}

std::string User::getPin() {
  return getRow()->getCell(2)->sget();
}

double User::getBalance() {
  int index;
  IO::accounts->getCol(0)->first(id, index);

  return IO::accounts->getCell(index, 1)->dget();
}

int User::getID() {
  return id;
}

int User::getAttempts() {
  return getRow()->getCell(3)->iget();
}

void User::setAttempts(int attempts) {
  getRow()->getCell(3)->set(attempts);
  IO::credentials->save();
}

CSVRow* User::getRow() {
  int index;
  IO::credentials->getCol(0)->first(id, index);

  return IO::credentials->getRow(index);
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
  double amount, std::string type, std::string status, int uuid) {

  CSVRow* operation = new CSVRow();

  if(uuid == 0) uuid = IO::getUUID();

  operation->append(new CSVCell(id))->
    append(new CSVCell(to))->
    append(new CSVCell(amount))->
    append(new CSVCell(IO::getDate()))->
    append(new CSVCell(type))->
    append(new CSVCell(status))->
    append(new CSVCell(uuid));

  IO::movements->append(operation);
  IO::movements->save();
}
