#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

#include "login.h"

bool Login::login(std::string number, std::string pin) {
  logout(); // First, logout if already logged in
  // Look for the card number in the db
  std::vector<int> match = IO::credentials->getCol(1)->has(number, 1);
  int found = match[0];
  if(found != -1) { // We have a match
    CSVRow* row = IO::credentials->getRow(found);
    if(row->getCell(2)->is(pin)) {
      std::cout << "Matching credentials. User logged in." << std::endl;
      int id = row->getCell(0)->iget();

      current = new User(id);
      return true;
    }
  }
  std::cout << "Incorrect card number or PIN." << std::endl;
  return false;
}

void Login::login(int id) {
  logout(); // First, logout if already logged in
  current = new User(id);
}

bool Login::logged() {
  return (current != NULL);
}

void Login::logout() {
  delete current;
  current = NULL;

  system("cls||clear"); //to make it work both on windows and unix
  std::ifstream splash("splash.txt");
  std::cout << splash.rdbuf();
}

User* Login::user() {
  return current;
}
