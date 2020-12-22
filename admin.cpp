#include <iostream>
#include <cmath>
#include <iomanip>

#include "admin.h"
#include "io.h"
#include "login.h"
#include "operations.h"

void Admin::handle() {
  bool select = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_ADMIN)) {
    case IO::OPTIONS_ADMIN_LOGOUT:
      select = false;
      break;
    case IO::OPTIONS_ADMIN_SUB:
      Admin::subInSafetyBox();
      break;
    case IO::OPTIONS_ADMIN_ADD:
      Admin::addInSafetyBox();
      break;
    case IO::OPTIONS_ADMIN_BALANCE:
      Operations::printBalance();
      break;
    case IO::OPTIONS_ADMIN_CHECKS: {
      while(select) {
        switch(IO::prompt(IO::OPTIONS_ADMIN_CHECK)) {
        case IO::OPTIONS_ADMIN_LOGOUT:
          select = false;
          break;
        case IO::OPTIONS_ADMIN_CHECK_CHEQUE:
          Admin::printChequeToCheck();
          Admin::checkCheque();
          break;
        case IO::OPTIONS_ADMIN_CHECK_TRANSFER:
          Admin::printTransferToCheck();
          Admin::checkTransfer();
          break;
        }
      }
    }
      break;
    case IO::OPTIONS_ADMIN_OPERATIONS:
      Admin::handleOperations();
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    if(select) std::cout << std::endl;
  }
}

void Admin::addInSafetyBox() {
  double initial = Login::user()->getBalance();

  std::cout << "Input deposit amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  Login::user()->setBalance(initial + amount);

  Operations::printBalance();
}

void Admin::subInSafetyBox() {
  double initial = Login::user()->getBalance();

  std::cout << "Input withdrawal amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }
  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  if(initial - amount < 0) {
    std::cout << "Insufficient credit in ATM's safety box. Operation aborted." << std::endl;
    return;
  } else {
    Login::user()->setBalance(initial - amount);
  }
  Operations::printBalance();
}


void Admin::handleOperations() {
  std::string number;

  std::cout << "Input user card number: ";
  if(!IO::inputNumber(number, true, true, 16)) {
    std::cout << "Invalid card number." << std::endl;
    return;
  }

  std::string adminNumber = Login::user()->getCardNumber();
  std::string adminPin = Login::user()->getPin();

  Login::logout();

  std::vector<int> match = IO::credentials->getCol(1)->has(&number, 1);
  int found = match[0];
  if(found != -1) {
    int id = IO::credentials->getCell(found, 0)->iget();
    // User with ID = 0 is admin
    Login::login(id);
    std::cout << std::endl;
    if(!Login::user()->isAdmin()) Operations::handle();
    else std::cout << "You are already logged in as admin." << std::endl;

    Login::login(IO::ADMIN_USER_ID);
    std::cout << "Logging back as admin..." << std::endl;
  }
}
// to be fixed with new has version
void Admin::printChequeToCheck (){
  std::string key = "DEPOSIT: CHEQUE";
  std::vector<int> match = IO::movements->getCol(4)->has(&key, 1, CSVCol::HAS_END);
  if(match[0] == -1) {
    std::cout << "No cheques to be checked." << std::endl;
    return;
  }

  std::cout <<
    std::left << std::setw(16) << "Number" <<
    std::left << std::setw(25) << "User ID" <<
    std::left << std::setw(25) << "Card number" <<
    std::right << std::setw(15) << ("Amount (" + IO::CURRENCY + ")") <<
    std::setw(5) << " " <<
    std::left << "Cheque number" << std::endl << std::endl;

  CSVRow* current;
  int user;
  for(int i: match) {
    user = IO::movements->getRow(i)->getCell(0)->iget();
    current = IO::movements->getRow(i);
    std::cout << i <<
    std::left << std::setw(10) << user;
    std::cout <<
      std::left << std::left << std::setw(16) <<
      IO::credentials->getRow(user)->getCell(1)->sget() <<
      std::right << std::setw(15) <<
      current->getCell(2)->dget() << std::setw(5) << " " <<
      std::left << std::setw(25) <<
      current->getCell(4)->sget() <<
      std::endl;
  }
};

// to be fixed with new has version
void Admin::printTransferToCheck (){
  std::string key = "OK";
  std::vector<int> match = IO::external->getCol(3)->has(&key, 1, CSVCol::HAS_END);
  if(match[0] == -1) {
    std::cout << "No transfers to be checked." << std::endl;
    return;
  }

  std::cout <<
    std::left << std::setw(16) << "Number" <<
    std::left << std::setw(25) << "User ID" <<
    std::left << std::setw(25) << " Card number" <<
    std::right << std::setw(15) << ("Amount (" + IO::CURRENCY + ")") <<
    std::setw(5) << " " <<
    std::left << "Beneficiary" <<
    std:: left << "Beneficiary's bank" << std::endl << std::endl;

  CSVRow* current;
  int user;
  for(int i: match) {
    user = IO::external->getRow(i)->getCell(0)->iget();
    current = IO::external->getRow(i);
    std::cout << i <<
    std::left << std::setw(10) << user;
    std::cout <<
      std::left << std::left << std::setw(16) <<
      IO::credentials->getRow(user)->getCell(1)->sget() <<
      std::right << std::setw(15) <<
      current->getCell(2)->dget() << std::setw(5) << " " <<
      std::left << std::setw(25) <<
      current->getCell(1)->sget() << std::setw(5) << " " <<
      std::left << std::setw(25) <<
      current->getCell(3)->sget() <<
      std::endl;
  }
};

void Admin::checkCheque() {
  int selectnum;
   std::cout << "Insert the desired cheque's number" << std::endl;
   std::cin >> selectnum;

   double initial = Login::user()->getBalance();
   std::string type;
   bool select = true;
   while(select){
     switch(IO::prompt(IO::OPTIONS_ADMIN_CHEQUE)) {
      case 0:
        return;
      case 1:
        type = IO::CHEQUE_ACCEPTED;
        select = false;
        break;
      case 2:
        std::cout << "The bank has rejected the cheque. The amount will be refund to the bank.";
        type = IO::CHEQUE_REJECTED;
        // devo trovare un modo per selezionare lo user e toglierli i soldi
     }
   }
 // devo trovare un modo per andare a modificare le cose in movements

};

void Admin::checkTransfer() {
  int selectnum;
   std::cout << "Insert the desired cheque's number" << std::endl;
   std::cin >> selectnum;

   double initial = Login::user()->getBalance();
   std::string type;
   bool select = true;
   while(select){
     switch(IO::prompt(IO::OPTIONS_ADMIN_TRANSFER)) {
      case 0:
        return;
      case 1:
        type = IO::TRANSFER_ACCEPTED;
        select = false;
        break;
      case 2:
        std::cout << "The bank has rejected the transfer. The amount will be refund to the user.";
        type = IO::TRANSFER_REJECTED;
        // devo trovare un modo per selezionare lo user e ridargli i soldi
     }
   }
 // devo trovare un modo per andare a modificare le cose in movements

};
