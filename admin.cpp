#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

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
      Admin::handleSub();
      break;
    case IO::OPTIONS_ADMIN_ADD:
      Admin::handleAdd();
      break;
    case IO::OPTIONS_ADMIN_BALANCE:
      Operations::printBalance();
      break;
    case IO::OPTIONS_ADMIN_CHEQUE: {
      bool available = Admin::printCheques();
      if(available) Admin::handleCheque();
      break;
    }
    case IO::OPTIONS_ADMIN_TRANSFER: {
      bool available = Admin::printTransfers();
      if(available) Admin::handleTransfer();
      break;
    }
    case IO::OPTIONS_ADMIN_OPERATIONS: {
      Admin::handleOperations();
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    case IO::OPTIONS_ADMIN_REACTIVATE: {
      Admin::handleAccounts();
      break;
    }
    if(select) std::cout << std::endl;
  }
  }
}

void Admin::handleAdd() {
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

void Admin::handleSub() {
  double initial = Login::user()->getBalance();

  std::cout << "Input withdrawal amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  if(initial - amount < 0) {
    std::cout << "Insufficient availability." << std::endl;
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

  std::vector<int> match = IO::credentials->getCol(1)->has(number, 1);
  int found = match[0];
  if(found != -1) {
    int id = IO::credentials->getCell(found, 0)->iget();

    Login::login(id);
    std::cout << std::endl;
    if(!Login::user()->isAdmin()) Operations::handle();
    else std::cout << "You are already logged in as admin." << std::endl;

    Login::login(IO::ADMIN_USER_ID);
    std::cout << "Logging back as admin..." << std::endl;
  }
}

bool Admin::printCheques() {
  std::vector<int> match = IO::movements->getCol(5)->has(IO::MOVEMENT_PENDING);
  if(match[0] == -1) {
    std::cout << "No pending cheques." << std::endl;
    return false;
  }

  std::vector<IO::cell> printable = {
    {15, "Transaction ID"},
    {2, " "},
    {25, "Beneficiary's card number"},
    {15, "Amount (" + IO::CURRENCY + ")", IO::ALIGN_RIGHT},
    {2, " "},
    {10, "Cheque number"}
  };
  IO::printRow(printable, true);

  CSVRow* row;
  for(int i: match) {
    row = IO::movements->getRow(i);

    std::string comment = row->getCell(4)->sget();
    int index = comment.find(IO::COORDINATE_SEPARATOR);
    if(index == std::string::npos) continue;

    int user = row->getCell(0)->iget();
    printable[0].content = std::to_string(i);
    printable[2].content = IO::credentials->getRow(user)->getCell(1)->sget();
    printable[3].content = row->getCell(2)->sget();
    printable[5].content = comment.substr(index + 3, std::string::npos);

    IO::printRow(printable, false);
  }

  std::cout << std::endl;
  return true;
}

bool Admin::printTransfers() {
  std::vector<int> match = IO::external->getCol(5)->has(IO::MOVEMENT_PENDING);
  if(match[0] == -1) {
    std::cout << "No pending transfers." << std::endl;
    return false;
  }

  std::vector<IO::cell> printable = {
    {15, "Transaction ID"},
    {2, " "},
    {22, "Sender's card number"},
    {15, "Amount (" + IO::CURRENCY + ")", IO::ALIGN_RIGHT},
    {2, " "},
    {25, "Beneficiary's card number"},
    {2, " "},
    {20, "Beneficiary's bank"}
  };
  IO::printRow(printable, true);

  for(int i: match) {
    CSVRow* row = IO::external->getRow(i);
    int user = row->getCell(0)->iget();

    printable[0].content = std::to_string(i);
    printable[2].content = IO::credentials->getRow(user)->getCell(1)->sget();
    printable[3].content = row->getCell(2)->sget();
    printable[5].content = row->getCell(1)->sget();
    printable[7].content = row->getCell(3)->sget();

    IO::printRow(printable, false);
  }

  std::cout << std::endl;
  return true;
}

void Admin::handleCheque() {
  std::vector<int> match = IO::movements->getCol(5)->has(IO::MOVEMENT_PENDING);

  std::string transaction;
  std::cout << "Input transaction ID: ";
  if(!IO::inputNumber(transaction, true, true) ||
    std::find(match.begin(), match.end(), stoi(transaction)) == match.end()) {
    std::cout << "Invalid transaction ID." << std::endl;
    return;
  }
  std::cout << std::endl;

  CSVRow* row = IO::movements->getRow(stoi(transaction));
  std::string type;
  bool select = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_CHEQUE)) {
    case 0:
      return;
    case 1: {
      type = IO::MOVEMENT_OK;
      select = false;
      break;
    }
    case 2: {
      type = IO::MOVEMENT_REFUSED;
      select = false;
      User beneficiary(row->getCell(0)->iget());
      double amount = row->getCell(2)->dget();

      beneficiary.setBalance(beneficiary.getBalance() - amount);
      std::cout << "Cheque refused. Amount refunded to the bank." << std::endl;
      break;
    }
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
  }

  row->getCell(5)->set(type);
  IO::movements->save();
}

void Admin::handleTransfer() {
  std::vector<int> match = IO::external->getCol(5)->has(IO::MOVEMENT_PENDING);

  std::string transaction;
  std::cout << "Input transaction ID: ";
  if(!IO::inputNumber(transaction, true, true) ||
    std::find(match.begin(), match.end(), stoi(transaction)) == match.end()) {
    std::cout << "Invalid transaction ID." << std::endl;
    return;
  }
  std::cout << std::endl;

  CSVRow* row = IO::external->getRow(stoi(transaction));
  std::string type;
  bool select = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_TRANSFER)) {
    case 0:
      return;
    case 1: {
      type = IO::MOVEMENT_OK;
      select = false;
      break;
    }
    case 2: {
      type = IO::MOVEMENT_REFUSED;
      select = false;
      User sender(row->getCell(0)->iget());
      double amount = row->getCell(2)->dget();

      sender.setBalance(sender.getBalance() + amount);
      std::cout << "Transfer refused. Amount refunded to the user." << std::endl;
      break;
    }
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
  }

  row->getCell(5)->set(type);
  IO::external->save();

  std::string beneficiary = row->getCell(1)->sget();
  std::vector<int> matchmovement = IO::movements->getCol(1)->has(beneficiary);
  IO::movements->getRow(matchmovement[0])->getCell(5)->set(type);
  IO::movements->save();

}

void Admin::handleAccounts() {
  //print all the users with blocked accounts:
  std::vector<int> cardMatch = IO::credentials->getCol(3)->has("3");
  std::string user;
  if(cardMatch[0] == -1) {
    std::cout << "No suspended accounts." << std::endl;
  }
  else{
    CSVRow* row;
    std::cout << "Currently suspended accounts: \n" << std::endl;
    for(int i: cardMatch)
    {
      row = IO::credentials->getRow(i);
      user = row->getCell(1)->sget();
      std::cout << user << std::endl;
    }
    std::cout << "\nDo you want to reactivate a user? " << std::endl;
    std::cout << "[1] Reactivate    [2] Back" << std::endl;
    int option;
    std::cin >> option;
    if(option == 1) {
      std::string number;
      std::cout << "Insert the card number of the user: ";
      std::cin >> number;
      std::vector<int> match = IO::credentials->getCol(1)->has(number, 1);
      int found = match[0];
      if(found != -1) {
        CSVRow* row = IO::credentials->getRow(found);
        IO::credentials->getCell(found, 3)->set("0");
        IO::credentials->save();
        std::cout << "User reabilitated." << std::endl;
      }
    }
  }
}
