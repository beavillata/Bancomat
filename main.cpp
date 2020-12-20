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
      if(Login::user()->isAdmin()) Admin::handle();
      else Operations::handle();
      std::cout << "Logging out..." << std::endl << std::endl;
      Login::logout();
    }
  }
  return 0;
}
