#include <iostream>
#include <iomanip>

#include "login.h"
#include "operations.h"
#include "admin.h"

//Intervenire in caso di guasti: (Controllo accesso admin)
// - Cancellare righe
// - Aggiungere righe
//Extra: creare account

//Ritirare denaro ed assegni versati:
// - Aggiunta e ritiro i soldi da account admin

void admin() {
  bool select = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_ADMIN)) {
    case IO::OPTIONS_ADMIN_LOGOUT:
      select = false;
      break;
    case IO::OPTIONS_ADMIN_SUB:
      break;
    case IO::OPTIONS_ADMIN_ADD:
      break;
    case IO::OPTIONS_ADMIN_BALANCE:
      Operations::printBalance();
      break;
    case IO::OPTIONS_ADMIN_OPERATIONS:
      Admin::handleOperations();
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    std::cout << std::endl;
  }
}

int main(int argc, char* argv[]) {
  bool exit = false;
  std::cout << std::setprecision(2) << std::fixed;

  while(!exit) {
    std::string number, pin;

    std::cout << "Please input your card number: ";
    if(!IO::inputNumber(number, true, true, 16)) {
      std::cout << "Invalid card number." << std::endl;
      continue;
    }

    std::cout << "Please input your PIN: ";
    if(!IO::inputNumber(pin, true, true, 5)) {
      std::cout << "Invalid pin." << std:: endl;
      continue;
    }

    if(Login::login(number, pin)) {
      std::cout << std::endl;
      if(Login::user()->isAdmin()) admin();
      else Operations::handle();
      std::cout << "Logging out..." << std::endl << std::endl;
      Login::logout();
    }
  }
  return 0;
}
