#include <iostream>
#include <vector>
#include <string>

#include "login.h"

bool Login::login(std::string number, std::string pin) {
  logout(); // First, logout if already logged in
  // Look for the card number in the db
  std::vector<int> match = IO::credentials->getCol(1)->has(&number, 1);
  int found = match[0];
  if(found != -1) { // We have a match
    CSVRow* row = IO::credentials->getRow(found);
    if(row->getCell(2)->sget() == pin) {
      std::cout << "Matching credentials. User logged in." << std::endl;
      int id = row->getCell(0)->iget();
      current = new User(id);
      return true;
    }
    return false;
  }
  std::cout << "Incorrect card number or PIN." << std::endl;
  return false;
}

bool Login::logged() {
  return (current != NULL);
}

void Login::logout() {
  delete current;
  current = NULL;
}

User* Login::user() {
  return current;
}
