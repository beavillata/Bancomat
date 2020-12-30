#ifndef OPERATIONS_H
#define OPERATIONS_H

class Operations {
public:
  static void handle();
  static void printBalance();

private:
  static void printMovements();
  static void handleWithdrawal();
  static void handleDeposit();
  static void handleTransfer();
};

#endif
