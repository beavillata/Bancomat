#include <iostream>
#include <string>

#include "login.h"

bool Login::log = false;
CSVRow* Login::user = NULL;
CSVFile Login::credentials("persistent/credentials.csv");

bool Login::login(std::string cardNumber, std::string pin) {
  logout(); // First, logout if already logged in
  // Look for the card number in the db
  int row = credentials.col(0)->has(&cardNumber);
  if(row != -1) { // We have a match
    user = credentials.row(row); // Set user as row
    if(user->cell(1)->sget() == pin) {
      std::cout << "Matching credentials. User logged in." << std::endl;
      log = true;
      return true;
    }
    user = NULL;
    return false;
  }
  std::cout << "Incorrect card number or PIN." << std::endl;
  return false;
}

bool Login::logged() {
  return log;
}

void Login::logout() {
  user = NULL;
  log = false;
}
