#include <cmath>

#include "operations.h"
#include "login.h"
#include "io.h"

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
  if(match.empty()) {
    std::cout << "No movements found for this user." << std::endl;
    return;
  }

  // Print out a nice header for the table...
  std::vector<IO::cell> printable = {
    {16, "Beneficiary"},
    {15, "Amount (" + IO::CURRENCY + ")", IO::ALIGN_RIGHT},
    {2, " "},
    {21, "Date"},
    {28, "Type"},
    {2, " "},
    {10, "Status"}};
  IO::printRow(printable, true);

  CSVRow* current;
  // ...and then print out the actual data.
  for(int i: match) {
    current = IO::movements->getRow(i);
    // Movement beneficiary (SELF if a deposit or transfer towards the customer)
    printable[0].content = current->getCell(1)->sget();
    // Money being passed around.
    printable[1].content = current->getCell(2)->sget();
    // Date of transfer.
    printable[3].content = current->getCell(3)->sget();
    // Movement type (DEPOSIT, TRANSFER)
    printable[4].content = current->getCell(4)->sget();
    // Status (APPROVED, PENDING, REFUSED)
    printable[6].content = current->getCell(5)->sget();

    IO::printRow(printable, false);
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
  User atm(IO::ADMIN_USER_ID);
  if((atm.getBalance() - amount) < 0) {
    std::cout << "Amount not available on this ATM." << std::endl;
    return;
  } else if(initial - amount < 0) {
    std::cout << "Insufficient credit. Operation cancelled." << std::endl;
    return;
  } else {
    // If all good subtract the amount from the atm account (admin)
    // and from the user's.
    Login::user()->addMovement(IO::TO_SELF, -amount,
      IO::MOVEMENT_WITHDRAWAL, IO::MOVEMENT_OK);
    Login::user()->setBalance(initial - amount);

    atm.setBalance(atm.getBalance() - amount);

    // Tell the user how many banknotes of each denomination will be dispensed.
    std::vector<double> dens{500.0, 200.0, 100.0, 50.0, 20.0, 10.0, 5.0};
    int size = dens.size();
    std::cout << "Dispensing ";
    for(int i = 0; i < size; i++) {
      int n = amount / dens[i];
      if(n == 0) continue;

      amount -= dens[i] * n;
      std::cout << n << " " << IO::CURRENCY << " " << dens[i];
      // Add a comma if this is not the last bill to be dispensed.
      if(amount >= dens[size - 1]) std::cout << ", ";
    }
    // The remainder will be coins...
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
      if(IO::inputNumber(cheque, true, true, IO::CHEQUE_NUMBER_DIGITS)) {
        cash = false;
        type = IO::MOVEMENT_DEPOSIT_CHEQUE + IO::COORDINATE_SEPARATOR + cheque;
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

  // Round to 2 decimal digits.
  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  Login::user()->addMovement(IO::TO_SELF, amount, type, status);
  Login::user()->setBalance(initial + amount);

  // If we have a cash deposit increment the ATM availability as well.
  User atm(IO::ADMIN_USER_ID);
  if(cash) atm.setBalance(atm.getBalance() + amount);

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
  }

  int match;
  // Check if beneficiary is part of our bank and is NOT the user.
  if(IO::credentials->getCol(1)->first(beneficiary, match)) {
    CSVRow* row = IO::credentials->getRow(match);
    int id = row->getCell(0)->iget();

    // Of course that's not happening.
    if(id == Login::user()->getID()) {
      std::cout << "Cannot transfer money to yourself." << std::endl;
      return;
    }

    // Temporary user (beneficiary).
    User other(id);

    // Add the movement to the sender's list and take away its money.
    Login::user()->addMovement(beneficiary, -amount,
      IO::MOVEMENT_TRANSFER, IO::MOVEMENT_OK);
    Login::user()->setBalance(initial - amount);

    // Now take care of the other user (beneficiary).
    std::string type = IO::MOVEMENT_TRANSFER + IO::COORDINATE_SEPARATOR +
      Login::user()->getCardNumber();
    other.addMovement(IO::TO_SELF, amount, type, IO::MOVEMENT_OK);
    other.setBalance(other.getBalance() + amount);

    printBalance();
  } else {
    // If the beneficiary's card number cannot be found in the system
    // assume our sender wants to transfer money to a foreign bank.
    std::cout << "External bank name: ";
    std::string bank;
    // Get rid of the leading witespace and give us the bank name.
    std::getline(std::cin >> std::ws, bank);

    // Now we are going to add an entry to IO::external and mark it for
    // approval by the admin. We are also going to add an entry to the user's
    // IO::movements as normal. The trick is that the two entries in the two
    // separate databases will share the same transaction UUID to make it
    // possible to mark them both as REFUSED or ACCEPTED at once.

    // Our shared UUID.
    int uuid = IO::getUUID();

    // This adds the entry to IO::movements, as normal.
    Login::user()->setBalance(initial - amount);
    Login::user()->addMovement(beneficiary, -amount,
      IO::MOVEMENT_TRANSFER, IO::MOVEMENT_PENDING, uuid);

    // And now we have the new entry for IO::external.
    CSVRow* transfer = new CSVRow();
    transfer->append(new CSVCell(Login::user()->getID()))->
      append(new CSVCell(beneficiary))->
      append(new CSVCell(amount))->
      append(new CSVCell(bank))->
      append(new CSVCell(IO::getDate()))->
      append(new CSVCell(IO::MOVEMENT_PENDING))->
      append(new CSVCell(uuid));

    IO::external->append(transfer);
    IO::external->save();

    std::cout << "Transaction has to be approved." << std::endl;
  }
}
