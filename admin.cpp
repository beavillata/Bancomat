#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

#include "admin.h"
#include "io.h"
#include "login.h"
#include "operations.h"

/*  This class handles the admin user interface and contains the logic
 *  that makes it all work.
 */

void Admin::handle() {
  bool select = true;
  // Main admin loop.
  while(select) {
    switch(IO::prompt(IO::OPTIONS_ADMIN)) {
    case IO::OPTIONS_ADMIN_LOGOUT:
      select = false;
      break;
    case IO::OPTIONS_ADMIN_SUB:
      // Take cash from this ATM.
      Admin::handleSub();
      break;
    case IO::OPTIONS_ADMIN_ADD:
      // Refill ATM with cash.
      Admin::handleAdd();
      break;
    case IO::OPTIONS_ADMIN_BALANCE:
      // How much cash do we have inside?
      Operations::printBalance();
      break;
    case IO::OPTIONS_ADMIN_CHEQUE: {
      // Show pending cheques.
      std::vector<int> match = Admin::printCheques();
      if(!match.empty()) Admin::handleCheque(match);
      break;
    }
    case IO::OPTIONS_ADMIN_TRANSFER: {
      // Show pending transfers.
      std::vector<int> match = Admin::printTransfers();
      if(!match.empty()) Admin::handleTransfer(match);
      break;
    }
    case IO::OPTIONS_ADMIN_OPERATIONS:
      // Allows an admin to log in as an user.
      Admin::handleOperations();
      break;
    case IO::OPTIONS_ADMIN_MANAGE:
      // Activate or block users' accounts.
      Admin::handleManage();
      break;
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
    if(select) std::cout << std::endl;
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

  // Round number to two decimal digits.
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

  // Round number to two decimal digits.
  double amount = std::ceil(stod(input) * 100.0) / 100.0;
  // And check that we actually have all that money in the ATM.
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

  int match;
  // Does the user our admin wants to log in with even exist?
  if(IO::credentials->getCol(1)->first(number, match)) {
    int id = IO::credentials->getCell(match, 0)->iget();

    Login::login(id);
    std::cout << std::endl;
    // Cannot login twice as admin...
    if(!Login::user()->isAdmin()) Operations::handle();
    else std::cout << "You are already logged in as admin." << std::endl;

    Login::login(IO::ADMIN_USER_ID);
    std::cout << "Logging back as admin..." << std::endl;
  } else {
    std::cout << "User not found." << std::endl;
  }
}

std::vector<int> Admin::printCheques() {
  // Pick out all the movements in IO::movements that have MOVEMENT_PENDING
  // as status: they will be cheques AND foreign bank transfers, but we'll
  // take care of those in a while.
  std::vector<int> match = IO::movements->getCol(5)->has(IO::MOVEMENT_PENDING);

  std::vector<int> cheques;
  CSVRow* row;
  for(int i: match) {
    row = IO::movements->getRow(i);
    std::string comment = row->getCell(4)->sget();
    // If the comment attached to this row contains the COORDINATE_SEPARATOR
    // then we know it's a cheque...
    int index = comment.find(IO::COORDINATE_SEPARATOR);
    // ...otherwise it will be a foreign bank transfer and we can ignore it.
    if(index != std::string::npos) cheques.push_back(i);
  }

  if(cheques.empty()) {
    std::cout << "No pending cheques." << std::endl;
  } else {
    // Print out a nice header for the table...
    // To do this we construct a vector of IO::cell (a struct)
    // that contains data about the width of our column, its content and
    // whether to have it right or left (default) aligned.
    std::vector<IO::cell> printable = {
      {15, "Transaction ID"},
      {2, " "},
      {25, "Beneficiary's card number"},
      {15, "Amount (" + IO::CURRENCY + ")", IO::ALIGN_RIGHT},
      {2, " "},
      {10, "Cheque number"}};
    IO::printRow(printable, true);

    CSVRow* row;
    // ...and then print out the actual data.
    for(int i: cheques) {
      row = IO::movements->getRow(i);

      User current(row->getCell(0)->iget());
      // Before printing the data we change the content of
      // our printable vector. Size and alignment will remain the same.
      // Row number.
      printable[0].content = std::to_string(i);
      // Customer (cheque beneficiary) card number.
      printable[2].content = current.getCardNumber();
      // Amount of this cheque.
      printable[3].content = row->getCell(2)->sget();
      // Cheque number.
      std::string comment = row->getCell(4)->sget();
      int index = comment.find(IO::COORDINATE_SEPARATOR);
      printable[5].content = comment.substr(
        index + IO::COORDINATE_SEPARATOR.size(), std::string::npos);

      IO::printRow(printable, false);
    }
    std::cout << std::endl;
  }
  return cheques;
}

std::vector<int> Admin::printTransfers() {
  // Pick out all the movements in IO::external that have MOVEMENT_PENDING
  // as status: they will be unchecked foreign transfers.
  std::vector<int> match = IO::external->getCol(5)->has(IO::MOVEMENT_PENDING);
  if(match.empty()) {
    std::cout << "No pending transfers." << std::endl;
  } else {
    // Print out a nice header for the table...
    std::vector<IO::cell> printable = {
      {15, "Transaction ID"},
      {2, " "},
      {22, "Sender's card number"},
      {15, "Amount (" + IO::CURRENCY + ")", IO::ALIGN_RIGHT},
      {2, " "},
      {25, "Beneficiary's card number"},
      {2, " "},
      {20, "Beneficiary's bank"}};
    IO::printRow(printable, true);

    // ...and then print out the actual data.
    for(int i: match) {
      CSVRow* row = IO::external->getRow(i);
      User current(row->getCell(0)->iget());

      // Row number.
      printable[0].content = std::to_string(i);
      // Customer (sender) card number.
      printable[2].content = current.getCardNumber();
      // Amount of this transfer.
      printable[3].content = row->getCell(2)->sget();
      // Beneficiary card number.
      printable[5].content = row->getCell(1)->sget();
      // Beneficiary (foreign) bank number.
      printable[7].content = row->getCell(3)->sget();

      IO::printRow(printable, false);
    }
    std::cout << std::endl;
  }
  return match;
}

void Admin::handleCheque(std::vector<int> match) {
  std::string transaction;
  std::cout << "Input transaction ID: ";
  // Make sure the transaction we want to approve/refuse exists.
  if(!IO::inputNumber(transaction, true, true) ||
    std::find(match.begin(), match.end(), stoi(transaction)) == match.end()) {
    std::cout << "Invalid transaction ID." << std::endl;
    return;
  }
  std::cout << std::endl;

  CSVRow* row = IO::movements->getRow(stoi(transaction));
  std::string type;
  bool select = true;
  // What do you want to do?
  while(select) {
    switch(IO::prompt(IO::OPTIONS_CHEQUE)) {
    case IO::OPTIONS_CHEQUE_CANCEL:
      return;
    case IO::OPTIONS_CHEQUE_ACCEPT: {
      type = IO::MOVEMENT_OK;
      select = false;
      std::cout << "Cheque accepted." << std::endl;
      break;
    }
    case IO::OPTIONS_CHEQUE_REFUSE: {
      type = IO::MOVEMENT_REFUSED;
      select = false;
      User beneficiary(row->getCell(0)->iget());
      double amount = row->getCell(2)->dget();
      // Reset beneficiary's account balance back to before he
      // tried to cash in the (now refused) cheque.
      beneficiary.setBalance(beneficiary.getBalance() - amount);

      std::cout << "Cheque refused. Amount refunded to the bank." << std::endl;
      break;
    }
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
  }

  // Set the transaction as approved or refused.
  row->getCell(5)->set(type);
  IO::movements->save();
}

void Admin::handleTransfer(std::vector<int> match) {
  std::string transaction;
  std::cout << "Input transaction ID: ";
  // Make sure the transaction we want to approve/refuse exists.
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
    case IO::OPTIONS_TRANSFER_CANCEL:
      return;
    case IO::OPTIONS_TRANSFER_ACCEPT: {
      type = IO::MOVEMENT_OK;
      select = false;
      std::cout << "Transfer accepted." << std::endl;
      break;
    }
    case IO::OPTIONS_TRANSFER_REFUSE: {
      type = IO::MOVEMENT_REFUSED;
      select = false;
      User sender(row->getCell(0)->iget());
      double amount = row->getCell(2)->dget();
      // Reset sender's account balance back what he had before.
      sender.setBalance(sender.getBalance() + amount);
      std::cout << "Transfer refused. Amount refunded." << std::endl;
      break;
    }
    default:
      std::cout << "Invalid option selected." << std::endl;
      break;
    }
  }

  // Set the transaction as approved or refused.
  row->getCell(5)->set(type);
  IO::external->save();

  // Use the transaction's UUID to pick the relevant one from
  // IO::movements and set the transaction's status there as well.
  int uuid = row->getCell(6)->iget();
  int index;
  if(IO::movements->getCol(6)->first(uuid, index)) {
    IO::movements->getRow(index)->getCell(5)->set(type);
    IO::movements->save();
  }
}

void Admin::handleManage() {
  std::vector<IO::cell> printable = {
    {15, "Account ID"},
    {16, "Card number", IO::ALIGN_RIGHT},
    {2, " "},
    {10, "Status"}};
  IO::printRow(printable, true);

  int i = 0;
  for(CSVRow* row: IO::credentials->getRows()) {
    // We don't want to be able to block (or unblock) the admin's account.
    // Instead, if this one gets blocked by too many incorrect login attempts,
    // throw away the whole ATM.
    if(row->getCell(0)->iget() == IO::ADMIN_USER_ID) {
      i++;
      continue;
    }

    // Account index (NOT account UUID)
    printable[0].content = std::to_string(i);
    // Account number.
    printable[1].content = row->getCell(1)->sget();
    // Account status.
    printable[3].content =
      (row->getCell(3)->iget() >= 3) ? "Disabled" : "Enabled";

    IO::printRow(printable, false);
    i++;
  }

  std::cout << std::endl << "Input account ID to alter state: ";
  std::string id;
  // Check that the account selected exists...
  if(!IO::inputNumber(id, true, true) ||
    IO::credentials->getRows().size() <= stoi(id)) {
    std::cout << "Invalid account ID." << std::endl;
    return;
  }

  // If the account is blocked (3 attempts) set the counter to zero,
  // otherwise (< 3 attempts) set the counter to 3.
  User current(IO::credentials->getCell(stoi(id), 0)->iget());
  int attempts = current.getAttempts();
  current.setAttempts((attempts >= 3) ? 0 : 3);

  std::cout << "Account successfully " <<
    (attempts >= 3 ? "enabled" : "disabled") << "." << std::endl;
}
