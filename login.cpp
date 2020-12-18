#include <iostream>
#include <string>

#include "login.h"
#include "user.h"

bool Login::log = false;
CSVFile* Login::credentials = new CSVFile("persistent/credentials.csv");
User* Login::user = NULL;

bool Login::login(std::string cardNumber, std::string pin) {
  logout(); // First, logout if already logged in
  // Look for the card number in the db
  int row = credentials->col(1)->has(&cardNumber);
  if(row != -1) { // We have a match
    CSVRow* current = credentials->row(row); // Set user as row
    if(*current->cell(2)->sget() == pin) {
      std::cout << "Matching credentials. User logged in." << std::endl;

      int id = *current->cell(0)->iget();
      user = new User(id);
      log = true;
      return true;
    }
    user = NULL;
    return false;
  }
  std::cout << "Incorrect card number or PIN." << std::endl;
  return false;
}

CSVFile* Login::getCredentials() {
  return credentials;
}

bool Login::logged() {
  return log;
}

void Login::logout() {
  delete user;
  log = false;
}
