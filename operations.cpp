#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>

#include "operations.h"
#include "login.h"
#include "csv/csv_file.h"

void Operations::printBalance() {
  double balance = Login::user()->getBalance();
  std::cout << "Your balance is: €" << balance << std::endl;
}

void Operations::printMovements() {
  int number = Login::user()->getID();
  std::vector<int> match = IO::movements->getCol(0)->has(&number, -1);

  if(match[0] == -1) {
    std::cout << "No movements found for this user." << std::endl;
    return;
  }

  std::cout << std::setw(13) << "To" << std::setw(6) << "Amount" << std::setw(20) << "Date" << std::setw(10) << "Type" << std::endl;

  std::string to, date, type;
  double amount;

  for(int i: match) {
    CSVRow* row = IO::movements->getRow(i);

    to = row->getCell(1)->sget();
    amount = row->getCell(2)->dget();
    date = row->getCell(3)->sget();
    type = row->getCell(4)->sget();

    std::cout << std::setw(13) << to << std::setw(6) << amount << std::setw(20) << date << std::setw(10) << type << std::endl;
  }

}

void Operations::handleWithdraw() {
  double initial = Login::user()->getBalance();

  std::cout << "Input withdrawal amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = stod(input);
  if(initial - amount <= 0) {
    std::cout << "Insufficient credit. Operation cancelled." << std::endl;
    return;
  } else {
    Login::user()->addMovement(IO::TO_SELF, -amount, IO::MOVEMENT_WITHDRAW);
    Login::user()->setBalance(initial - amount);

    printBalance();
  }
}

void Operations::handleDeposit() {
  double initial = Login::user()->getBalance();
  std::string type;
  int option;
  IO::prompt(IO::OPTIONS_DEPOSIT, option);
  switch(option) {
  case 0:
    return;
  case 1:
    type = IO::MOVEMENT_DEPOSIT_CASH;
    break;
  case 2:
    std::cout << "Input cheque number: ";
    std::string cheque;
    if(IO::inputNumber(cheque, true, true)) {
      type = IO::MOVEMENT_DEPOSIT_CHEQUE + "@" + cheque;
    } else {
      std::cout << "Invalid cheque number.";
      return;
    }
    break;
  }

  std::cout << "Input deposit amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = stod(input);
  Login::user()->addMovement(IO::TO_SELF, amount, type);
  Login::user()->setBalance(initial + amount);

  printBalance();
}

void Operations::handleTransfer() {
  double initial = Login::user()->getBalance();

  std::cout << "Input beneficiary's card number: ";
  std::string beneficiary;
  if(!IO::inputNumber(beneficiary, true, true, 16)) {
    std::cout << "Invalid card number." << std::endl;
    return;
  }

  std::cout << "Input transfer amount: " << std::endl;
  std::string input;
  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = stod(input);
  if(initial - amount < 0) {
    std::cout << "Insufficient credit. Operation cancelled." << std::endl;
    return;
  } else {
    std::vector<int> match = IO::credentials->getCol(1)->has(&beneficiary, 1);

    if(match[0] != -1) {
      CSVRow* row = IO::credentials->getRow(match[0]);
      int id = row->getCell(0)->iget();

      if(id == Login::user()->getID()) {
        std::cout << "Cannot transfer money to yourself." << std::endl;
        return;
      }

      User other(id);

      Login::user()->addMovement(beneficiary, -amount, IO::MOVEMENT_TRANSFER);
      Login::user()->setBalance(initial - amount);

      std::string type = IO::MOVEMENT_TRANSFER + "@" +
        Login::user()->getCardNumber();
      other.addMovement(IO::TO_SELF, amount, type);
      other.setBalance(other.getBalance() + amount);

      printBalance();
    } else {
      std::cout << "External bank name: ";
      std::string bank;
      std::cin >> bank;

      CSVRow* transfer = new CSVRow();

      transfer->append(new CSVData<int>(Login::user()->getID()))->
        append(new CSVData<std::string>(beneficiary))->
        append(new CSVData<double>(amount))->
        append(new CSVData<std::string>(bank))->
        append(new CSVData<std::string>(IO::getDate()));

      IO::external->append(transfer);
      IO::external->save();

      std::cout << "Transaction has to be approved." << std::endl;
    }
  }
}
