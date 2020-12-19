#include <iostream>
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
  std::cout << "Your balance is: " << balance << "£" << std::endl;
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

void Operations::addOperation(std::string to,
  double amount, std::string type) {

  int n = Database::movements->rows().size();
  CSVRow* operation = new CSVRow(n);

  operation->append(new CSVData<std::string>(1, n, to));
  operation->append(new CSVData<double>(2, n, amount));
  operation->append(new CSVData<std::string>(3, n, "oggi"));
  operation->append(new CSVData<std::string>(4, n, type));

  Database::movements->append(operation);
}

void Operations::initWithdraw() {

  double initial = Login::user()->getBalance();

  std::cout << "Select withdrawal amount: " << std::endl;
  double amount;
  std::cin >> amount;
  if(initial-amount < 0){std::cout << "Insufficient credit: operation aborted..." << std::endl;}
  else{
    addOperation(TO_SELF, amount, MOVEMENT_WITHDRAW);
    Login::user()->setBalance(initial - amount);
    std::cout << "The new balance is: " << Login::user()->getBalance() << std::endl;
  }

}

void Operations::initDeposit() {
  double initial = Login::user()->getBalance();

  std::cout << "Select deposit amount: " << std::endl;
  double amount;
  std::cin >> amount;
  std::string type;
  std::cout << "0: abort \t 1: Cash Deposit \t 2: Cheque Deposit" << std::endl;
  int option;
  std::cin >> option;
  if(option == 0){return;}
  else if(option == 1){type = MOVEMENT_DEPOSIT_CASH;}
  else if(option == 2){type = MOVEMENT_DEPOSIT_CHEQUE;}
  addOperation(TO_SELF, amount, type);

  Login::user()->setBalance(initial + amount);
  std::cout << "The new balance is: " << Login::user()->getBalance() << std::endl;
}

void Operations::initTransfer(){
  double initial = Login::user()->getBalance();
  int id;
  std::cout << "Insert the Card Number for the transfer: " << std::endl;
  std::string transferCard;
  std::cin >> transferCard;
  std::cout << "Select the transfer amount: " << std::endl;
  double amount;
  std::cin >> amount;
  if(initial-amount < 0){std::cout << "Insufficient credit: operation aborted...";}
  else{
    addOperation(transferCard, amount, MOVEMENTS_TRANSFER);

    std::vector<int> match = Database::credentials->col(1)->has(&transferCard, 1);
    Login::user()->setBalance(initial - amount);
    std::cout << "The new balance is: " << Login::user()->getBalance() << std::endl;

    if(match[0]!=-1){
      CSVRow* row = Database::credentials->row(match[0]);
      id = row->cell(0)->iget();
      std::vector<int> found = Database::accounts->col(0)->has(&id, 1);
      row = Database::accounts->row(found[0]);
      initial = row->cell(1)->dget();
      Database::accounts->cell(1, id)->dset(initial+amount);
      Database::accounts->save();
      }
  }
}
