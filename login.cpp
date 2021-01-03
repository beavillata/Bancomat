#include "login.h"

bool Login::login(std::string number, std::string pin) {
  logout(); // Logout first (just in case).

  // Look for the card number in the db
  int match;
  if(IO::credentials->getCol(1)->first(number, match)) { // We have a match
    CSVRow* row = IO::credentials->getRow(match);
    // Make a temporary user with this number. Not logged in yet...
    User temp(row->getCell(0)->iget());
    // Check that this account isn't blocked...
    if(temp.getAttempts() >= 3) {
      std::cout << "Suspended account." << std::endl;
      // ...and if it is, refuse login.
      return false;
    }

    // Otherwise, check if pin matches at the same row found for the
    // credit card number.
    if(row->getCell(2)->is(pin)) {
      std::cout << "Matching credentials. User logged in." << std::endl;
      // Fetch our user's ID.
      int id = row->getCell(0)->iget();

      // And instantiate a new User object with the ID.
      current = new User(id);
      // Reset incorrect login attempts counter.
      current->setAttempts(0);
      // Now we are in!
      return true;
    } else {
      // If we have a wrong pin increase the counter by 1 and
      // scream at the user.
      int attempts = temp.getAttempts();
      temp.setAttempts(attempts + 1);
      if(attempts >= 3) {
        // If the user got the pin wrong 3 times scream even harder.
        std::cout << "Too many incorrect login attempts." << std::endl <<
        "Your account has been suspended." << std::endl << std::endl;

        return false;
      }
    }
  }
  std::cout << "Incorrect card number or PIN." << std::endl;
  return false;
}

// Shortcut, only use internally...
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
  IO::splash();
}

// Return the logged user. Might be NULL if nobody has logged in yet.
User* Login::user() {
  return current;
}
