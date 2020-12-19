#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

#include "operations.h"
#include "login.h"
#include "csv/csv_file.h"

const std::string TO_SELF("SELF"),
  MOVEMENT_DEPOSIT_CASH("CASH DEPOSIT"),
  MOVEMENT_DEPOSIT_CHEQUE("CHEQUE DEPOSIT"),
  MOVEMENT_WITHDRAW("WITHDRAW"),
  MOVEMENTS_TRANSFER("TRANSFER");

void Operations::printBalance() {
  double balance = Login::user()->getBalance();
  std::cout << "Your balance is: £" << balance << std::endl;
}

void Operations::printMovements() {
  int number = Login::user()->getID();
  std::vector<int> match = Database::movements->col(0)->has(&number, -1);

  if(match[0] == -1) {
    std::cout << "No movements found for this user." << std::endl;
    return;
  }

  std::cout << "|\tTo\t|\tAmount\t|\tDate\t\t|\tType\t\t|" << std::endl;

  std::string to, date, type;
  double amount;

  for(int i: match) {
    CSVRow* row = Database::movements->row(i);

    to = row->cell(1)->sget();
    amount = row->cell(2)->dget();
    date = row->cell(3)->sget();
    type = row->cell(4)->sget();

    std::cout << "|\t" << to << "\t|\t" << amount << "\t|\t"
      << date << "\t|\t" << type << "\t|" << std::endl;
  }

}

void Operations::addMovement(std::string to,
  double amount, std::string type, int id = Login::user()->getID()) {

  CSVRow* operation = new CSVRow();

  operation->append(new CSVData<int>(id));
  operation->append(new CSVData<std::string>(to));
  operation->append(new CSVData<double>(amount));
  operation->append(new CSVData<std::string>(getDate()));
  operation->append(new CSVData<std::string>(type));

  Database::movements->append(operation);
  Database::movements->save();
}

void Operations::initWithdraw() {
  double initial = Login::user()->getBalance();

  std::cout << "Input withdrawal amount: " << std::endl;
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
    addMovement(TO_SELF, -amount, MOVEMENT_WITHDRAW);
    Login::user()->setBalance(initial - amount);

    printBalance();
  }
}

const std::vector<std::string> depositOptions = {"Cancel",
  "Cash deposit", "Cheque deposit"};

void Operations::initDeposit() {
  double initial = Login::user()->getBalance();

  std::string type;

  int option;
  User::prompt(depositOptions, &option);
  switch(option) {
  case 0:
    return;
  case 1:
    type = MOVEMENT_DEPOSIT_CASH;
    break;
  case 2:
    std::cout << "Input cheque number: " << std::endl;
    std::string cheque;
    std::cin >> cheque;
    type = MOVEMENT_DEPOSIT_CHEQUE + "@" + cheque;
    break;
  }

  std::cout << "Input deposit amount: " << std::endl;
  double amount;
  std::cin >> amount;

  if(amount < 0) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  addMovement(TO_SELF, amount, type);
  Login::user()->setBalance(initial + amount);

  printBalance();
}

void Operations::initTransfer(){
  double initial = Login::user()->getBalance();

  std::cout << "Input beneficiary's card number: " << std::endl;
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
    std::vector<int> match = Database::credentials->
      col(1)->has(&beneficiary, 1);

    if(match[0] != -1) {
      CSVRow* row = Database::credentials->row(match[0]);
      int id = row->cell(0)->iget();

      if(id == Login::user()->getID()) {
        std::cout << "Cannot transfer money to yourself." << std::endl;
        return;
      }

      addMovement(beneficiary, -amount, MOVEMENTS_TRANSFER);
      Login::user()->setBalance(initial - amount);

      match = Database::accounts->col(0)->has(&id, 1);
      row = Database::accounts->row(match[0]);
      initial = row->cell(1)->dget();

      Database::accounts->cell(1, id)->dset(initial + amount);
      Database::accounts->save();

      addMovement(TO_SELF, amount, MOVEMENTS_TRANSFER, id);
      printBalance();
    } else {
      std::cout << "External bank name: ";
      std::string bank;
      std::cin >> bank;

      CSVRow* external = new CSVRow();

      external->append(new CSVData<int>(Login::user()->getID()));
      external->append(new CSVData<std::string>(beneficiary));
      external->append(new CSVData<double>(amount));
      external->append(new CSVData<std::string>(bank));
      external->append(new CSVData<std::string>(getDate()));

      Database::external->append(external);
      Database::external->save();

      std::cout << "Transaction has to be approved." << std::endl;
    }
  }
}

std::string Operations::getDate() {
      time_t timer = time(NULL);
      char datec[26];
      strftime(datec, 26, "%Y/%m/%d-%H:%M:%S", localtime(&timer));

      std::string date(datec);
      return date;
}
