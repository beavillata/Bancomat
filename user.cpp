#include "user.h"

void User::setBalance(double balance) {
  int index;
  // First, seek this user in IO::accounts...
  IO::accounts->getCol(0)->first(id, index);
  // ...and set its new balance in there.
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

// Get user's UUID (!= ROW INDEX IN DATABASE)
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

// Get the user's row from IO::credentials.
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

// Add a movement to this user's account.
void User::addMovement(std::string to,
  double amount, std::string type, std::string status, int uuid) {
  // Instantiate new row to contain our movement.
  CSVRow* operation = new CSVRow();
  // If no UUID provided pick a new one.
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
