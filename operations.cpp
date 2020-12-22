#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
#include <cmath>
#include <string>

#include "operations.h"
#include "login.h"

void Operations::handle() {
  bool select = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_MAIN)) {
    case IO::OPTIONS_MAIN_LOGOUT:
      select = false;
      break;
    case IO::OPTIONS_MAIN_BALANCE:
      printBalance();
      break;
    case IO::OPTIONS_MAIN_MOVEMENTS:
      printMovements();
      break;
    case IO::OPTIONS_MAIN_WITHDRAWAL:
      handleWithdrawal();
      break;
    case IO::OPTIONS_MAIN_DEPOSIT:
      handleDeposit();
      break;
    case IO::OPTIONS_MAIN_TRANSFER:
      handleTransfer();
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    if(select) std::cout << std::endl;
  }
}

void Operations::printBalance() {
  double balance = Login::user()->getBalance();
  std::cout << "Your balance is: " << IO::CURRENCY << " " <<
    balance << std::endl;
}

void Operations::printMovements() {
  int number = Login::user()->getID();
  std::vector<int> match = IO::movements->getCol(0)->has(number);
  if(match[0] == -1) {
    std::cout << "No movements found for this user." << std::endl;
    return;
  }

  std::cout <<
    std::left << std::setw(16) << "Beneficiary" <<
    std::right << std::setw(15) << ("Amount (" + IO::CURRENCY + ")") <<
    std::setw(5) << " " <<
    std::left << std::setw(25) << "Date" <<
    std::left << "Type" << std::endl << std::endl;

  CSVRow* current;
  for(int i: match) {
    current = IO::movements->getRow(i);
    std::cout <<
      std::left << std::left << std::setw(16) <<
      current->getCell(1)->sget() <<
      std::right << std::setw(15) <<
      current->getCell(2)->dget() << std::setw(5) << " " <<
      std::left << std::setw(25) <<
      current->getCell(3)->sget() <<
      std::left <<
      current->getCell(4)->sget() << std::endl;
  }
}

void Operations::handleWithdrawal() {
  double initial = Login::user()->getBalance();

  std::cout << "Input withdrawal amount: ";
  std::string input;
  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  User bancomat(0);
  if((bancomat.getBalance() - amount) < 0) {
    std::cout << "Amount not available on this ATM." << std::endl;
    return;
  } else if(initial - amount < 0) {
    std::cout << "Insufficient credit. Operation cancelled." << std::endl;
    return;
  } else {
    Login::user()->addMovement(IO::TO_SELF, -amount,
      IO::MOVEMENT_WITHDRAWAL, IO::MOVEMENT_OK);
    Login::user()->setBalance(initial - amount);

    bancomat.setBalance(bancomat.getBalance() - amount);

    std::vector<double> dens{500.0, 200.0, 100.0, 50.0, 20.0, 10.0, 5.0};
    int size = dens.size();
    std::cout << "Dispensing ";
    for(int i = 0; i < size; i++) {
      int n = amount / dens[i];
      if(n == 0) continue;

      amount -= dens[i] * n;
      std::cout << n << " " << IO::CURRENCY << " " << dens[i];

      if(amount >= dens[size - 1]) std::cout << ", ";
    }
    std::cout << " banknotes and " << IO::CURRENCY << " " <<
      amount << " in coins." << std::endl;
    printBalance();
  }
}

void Operations::handleDeposit() {
  double initial = Login::user()->getBalance();
  std::string type, cheque, status;
  bool select = true, cash = true;
  while(select) {
    switch(IO::prompt(IO::OPTIONS_DEPOSIT)) {
    case 0:
      return;
    case 1:
      type = IO::MOVEMENT_DEPOSIT_CASH;
      status = IO::MOVEMENT_OK;
      select = false;
      break;
    case 2:
      std::cout << "Input cheque number: ";
      // Assume 7-digit code for cheque
      if(IO::inputNumber(cheque, true, true, 7)) {
        cash = false;
        type = IO::MOVEMENT_DEPOSIT_CHEQUE + " < " + cheque;
        status = IO::MOVEMENT_PENDING;
      } else {
        std::cout << "Invalid cheque number." << std::endl;
        return;
      }
      select = false;
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    if(select) std::cout << std::endl;
  }

  std::cout << "Input deposit amount: ";
  std::string input;

  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  Login::user()->addMovement(IO::TO_SELF, amount, type, status);
  Login::user()->setBalance(initial + amount);

  User bancomat(0);
  if(cash) bancomat.setBalance(bancomat.getBalance() + amount);

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

  std::cout << "Input transfer amount: ";
  std::string input;
  if(!IO::inputNumber(input, true)) {
    std::cout << "Amount must be a positive number." << std::endl;
    return;
  }

  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  if(initial - amount < 0) {
    std::cout << "Insufficient credit. Operation cancelled." << std::endl;
    return;
  } else {
    std::vector<int> match = IO::credentials->getCol(1)->has(beneficiary, 1);

    if(match[0] != -1) {
      CSVRow* row = IO::credentials->getRow(match[0]);
      int id = row->getCell(0)->iget();

      if(id == Login::user()->getID()) {
        std::cout << "Cannot transfer money to yourself." << std::endl;
        return;
      }

      User other(id);

      Login::user()->addMovement(beneficiary, -amount,
        IO::MOVEMENT_TRANSFER, IO::MOVEMENT_OK);
      Login::user()->setBalance(initial - amount);

      std::string type = IO::MOVEMENT_TRANSFER + " < " +
        Login::user()->getCardNumber();
      other.addMovement(IO::TO_SELF, amount, type, IO::MOVEMENT_OK);
      other.setBalance(other.getBalance() + amount);

      printBalance();
    } else {
      std::cout << "External bank name: ";
      std::string bank;
      std::getline(std::cin >> std::ws, bank);

      CSVRow* transfer = new CSVRow();

      transfer->append(new CSVCell(Login::user()->getID()))->
        append(new CSVCell(beneficiary))->
        append(new CSVCell(amount))->
        append(new CSVCell(bank))->
        append(new CSVCell(IO::getDate()))->
        append(new CSVCell(IO::MOVEMENT_PENDING));

      IO::external->append(transfer);
      IO::external->save();

      std::cout << "Transaction has to be approved." << std::endl;
    }
  }
}
