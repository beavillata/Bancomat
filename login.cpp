#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

#include "login.h"

bool Login::login(std::string number, std::string pin) {
  logout(); // First, logout if already logged in
  // Look for the card number in the db
  int match;
  if(IO::credentials->getCol(1)->first(number, match)) { // We have a match
    CSVRow* row = IO::credentials->getRow(match);
    // Check that this account isn't blocked.
    User temp(row->getCell(0)->iget());
    if(temp.getAttempts() >= 3) {
      std::cout << "Suspended account." << std::endl;
      return false;
    }

    if(row->getCell(2)->is(pin)) {
      std::cout << "Matching credentials. User logged in." << std::endl;
      int id = row->getCell(0)->iget();

      current = new User(id);
      // Reset incorrect login attempts counter.
      current->setAttempts(0);
      return true;
    } else {
      int attempts = temp.getAttempts();
      if(attempts >= 2) {
        std::cout << "Too many incorrect login attempts." << std::endl <<
          "Your account has been suspended." << std::endl << std::endl;
      }

      temp.setAttempts(attempts + 1);
      return false;
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

  // Clear the screen, on both windows and unix.
  system("cls || clear");
  std::ifstream splash("splash.txt");
  std::cout << splash.rdbuf();
}

User* Login::user() {
  return current;
}
