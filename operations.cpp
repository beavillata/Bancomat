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

void Operations::addMovement(std::string to,
  double amount, std::string type, int id = Login::user()->getID()) {

  CSVRow* operation = new CSVRow();

  operation->append(new CSVData<int>(id))
    ->append(new CSVData<std::string>(to))
    ->append(new CSVData<double>(amount))
    ->append(new CSVData<std::string>(getDate()))
    ->append(new CSVData<std::string>(type));

  IO::movements->append(operation);
  IO::movements->save();
}

void Operations::handleWithdraw() {
  double initial = Login::user()->getBalance();

  std::cout << "Input withdrawal amount: ";
  double amount;
  std::cin >> amount;

  if(amount < 0) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  if(initial - amount <= 0) {
    std::cout << "Insufficient credit. Operation cancelled." << std::endl;
    return;
  } else {
    addMovement(IO::TO_SELF, -amount, IO::MOVEMENT_WITHDRAW);
    Login::user()->setBalance(initial - amount);

    printBalance();
  }
}

void Operations::handleDeposit() {
  double initial = Login::user()->getBalance();
  std::string type;
  int option;
  IO::prompt(IO::OPTIONS_DEPOSIT, &option);
  switch(option) {
  case 0:
    return;
  case 1:
    type = IO::MOVEMENT_DEPOSIT_CASH;
    break;
  case 2:
    std::cout << "Input cheque number: ";
    std::string cheque;
    std::cin >> cheque;
    type = IO::MOVEMENT_DEPOSIT_CHEQUE + "@" + cheque;
    break;
  }

  std::cout << "Input deposit amount: ";
  double amount;
  std::cin >> amount;

  if(amount < 0) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  addMovement(IO::TO_SELF, amount, type);
  Login::user()->setBalance(initial + amount);

  printBalance();
}

void Operations::handleTransfer() {
  double initial = Login::user()->getBalance();

  std::cout << "Input beneficiary's card number: ";
  std::string beneficiary;
  std::cin >> beneficiary;

  std::cout << "Input transfer amount: " << std::endl;
  double amount;
  std::cin >> amount;

  if(amount < 0) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

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

      addMovement(beneficiary, -amount, IO::MOVEMENT_TRANSFER);
      Login::user()->setBalance(initial - amount);

      match = IO::accounts->getCol(0)->has(&id, 1);
      row = IO::accounts->getRow(match[0]);
      initial = row->getCell(1)->dget();

      IO::accounts->getCell(1, id)->dset(initial + amount);
      IO::accounts->save();

      addMovement(IO::TO_SELF, amount, IO::MOVEMENT_TRANSFER, id);
      printBalance();
    } else {
      std::cout << "External bank name: ";
      std::string bank;
      std::cin >> bank;

      CSVRow* transfer = new CSVRow();

      transfer->append(new CSVData<int>(Login::user()->getID()))
        ->append(new CSVData<std::string>(beneficiary))
        ->append(new CSVData<double>(amount))
        ->append(new CSVData<std::string>(bank))
        ->append(new CSVData<std::string>(getDate()));

      IO::external->append(transfer);
      IO::external->save();

      std::cout << "Transaction has to be approved." << std::endl;
    }
  }
}

std::string Operations::getDate() {
      time_t timer = time(NULL);
      char datec[26];
      strftime(datec, 26, "%Y/%m/%d %H:%M:%S", localtime(&timer));

      std::string date(datec);
      return date;
}
