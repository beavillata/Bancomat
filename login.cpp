#include <iostream>
#include <string>

#include "login.h"

bool Login::login(std::string number, std::string pin) {
  logout(); // First, logout if already logged in
  // Look for the card number in the db
  int found = Database::credentials->col(1)->has(&number);
  if(found != -1) { // We have a match
    CSVRow* row = Database::credentials->row(found);
    if(row->cell(2)->sget() == pin) {
      std::cout << "Matching credentials. User logged in." << std::endl;
      int id = row->cell(0)->iget();
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
